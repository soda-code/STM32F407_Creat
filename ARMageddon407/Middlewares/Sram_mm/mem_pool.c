#include "mem_pool.h"

#include <string.h>
#include <stdio.h>

#ifdef OS_ENABLE
#include "FreeRTOS.h"
#include "semphr.h"
#endif

/*=========================================================
 *
 * CONFIG
 *
 *=========================================================*/

#define ALIGN_SIZE             4U
#define ALIGN_UP(x, align)     (((uintptr_t)(x) + ((uintptr_t)(align) - 1)) & ~((uintptr_t)(align) - 1))
#define MEM_MAGIC              0x5AA55AA5U
#define MEM_TAIL_MAGIC         0xDEADBEEFU

#define MIN_SPLIT_SIZE         32U

/// 内存块结构体大小
typedef struct mem_block
{
    uint32_t magic;
    uint32_t size;
    uint8_t allocated;
    struct mem_block* prev;
    struct mem_block* next;

} mem_block_t;



static uint8_t* g_pool_start = NULL;
static uint32_t g_pool_size = 0;
static mem_block_t* g_block_list = NULL;
static bool g_initialized = false;

#ifdef OS_ENABLE
    // 内存池互斥锁定义
    static SemaphoreHandle_t g_mem_mutex = NULL;
#endif



#ifdef OS_ENABLE

#define MEM_LOCK() \
    xSemaphoreTake(g_mem_mutex, portMAX_DELAY)

#define MEM_UNLOCK() \
    xSemaphoreGive(g_mem_mutex)

#else

#define MEM_LOCK()
#define MEM_UNLOCK()

#endif

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 获取内存块尾部指针
 * @param[in ]: block: 内存块指针
 * @param[out]: null
 * @return  :uint32_t* 尾部指针
 * ==============================================================================*/
static uint32_t* get_tail(mem_block_t* block)
{
    return (uint32_t*)((uint8_t*)block + block->size - sizeof(uint32_t));
}
/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 写入内存块尾部魔数
 * @param[in ]: block: 内存块指针
 * @param[out]: null
 * @return  :null
 * ==============================================================================*/
static void write_tail(mem_block_t* block)
{
    *get_tail(block) = MEM_TAIL_MAGIC;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 检查内存块尾部魔数是否正确
 * @param[in ]: block: 内存块指针
 * @param[out]: null
 * @return  :bool 是否正确
 * ==============================================================================*/

static bool check_tail(mem_block_t* block)
{
    return (*get_tail(block) == MEM_TAIL_MAGIC);
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 验证用户指针是否合法
 * @param[in ]: ptr: 用户指针
 * @param[out]: null
 * @return  :bool 是否合法
 * ==============================================================================*/
static bool is_valid_ptr(void* ptr)
{
    if (ptr == NULL)
    {
        return false;
    }
    uint8_t* p = (uint8_t*)ptr;
    if (p < g_pool_start + sizeof(mem_block_t))
    {
        return false;
    }
    if (p >= g_pool_start + g_pool_size)
    {
        return false;
    }
    return true;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 用户指针转换为内存块指针
 * @param[in ]: ptr: 用户指针
 * @param[out]: null
 * @return  :mem_block_t* 内存块指针
 * ==============================================================================*/
static mem_block_t* ptr_to_block(void* ptr)
{
    return (mem_block_t*)((uint8_t*)ptr - sizeof(mem_block_t));
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 分割内存块
 * @param[in ]: block: 当前内存块指针
 * @param[in ]: size: 分割后内存块大小
 * @param[out]: null
 * @return  :null
 * ==============================================================================*/
static void split_block(mem_block_t* block, uint32_t size)
{
    uint32_t remain = block->size - size;
    if (remain < (sizeof(mem_block_t) + MIN_SPLIT_SIZE))
    {
        return;
    }
    mem_block_t* new_block = (mem_block_t*)((uint8_t*)block + size);

    new_block->magic = MEM_MAGIC;
    new_block->size = remain;
    new_block->allocated = 0;
    new_block->prev = block;
    new_block->next = block->next;
    if (block->next)
    {
        block->next->prev = new_block;
    }
    block->next = new_block;
    block->size = size;
    write_tail(new_block);
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 合并下一个内存块
 * @param[in ]: block: 当前内存块指针
 * @param[out]: null
 * @return  :mem_block_t* 合并后的内存块指针
 * ==============================================================================*/
static void merge_next(mem_block_t* block)
{
    if (block == NULL)
    {
        return;
    }
    mem_block_t* next = block->next;
    if (next == NULL)
    {
        return;
    }
    if (next->allocated)
    {
        return;
    }
    if (((uint8_t*)block + block->size) != (uint8_t*)next)
    {
        return;
    }
    block->size += next->size;
    block->next = next->next;
    if (next->next)
    {
        next->next->prev = block;
    }
    write_tail(block);
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 合并前一个内存块
 * @param[in ]: block: 当前内存块指针
 * @param[out]: null
 * @return  :mem_block_t* 合并后的内存块指针
 * ==============================================================================*/

static mem_block_t* merge_prev(mem_block_t* block)
{
    if (block == NULL)
    {
        return block;
    }
    mem_block_t* prev = block->prev;
    if (prev == NULL)
    {
        return block;
    }
    if (prev->allocated)
    {
        return block;
    }
    if (((uint8_t*)prev + prev->size) != (uint8_t*)block)
    {
        return block;
    }
    prev->size += block->size;
    prev->next = block->next;
    if (block->next)
    {
        block->next->prev = prev;
    }
    write_tail(prev);
    return prev;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 初始化内存池
 * @ param[in ]: start: 内存池起始地址
 *               size: 内存池大小
 *               clear: 是否清零内存池
 * @return  :void
 *==============================================================================*/

void mem_pool_init(void* addr, uint32_t size, bool clear)
{
#ifdef OS_ENABLE
    if (g_mem_mutex == NULL)
    {
        g_mem_mutex = xSemaphoreCreateMutex();
    }
#endif
    if (g_initialized == true)
    {
        return;
    }
    if (addr == NULL || size < (sizeof(mem_block_t) + sizeof(uint32_t))) // 最小内存池大小必须能容纳一个块头和一个块尾
    {
        return;
    }

    uintptr_t aligned = ALIGN_UP((uintptr_t)addr, ALIGN_SIZE);
    uint32_t offset = aligned - (uintptr_t)addr;
    g_pool_start = (uint8_t*)aligned;
    if (offset > size) // 如果对齐后的地址超出原始内存范围，无法使用内存池
    {
        g_initialized = true; // 标记为已初始化但不可用
        return;
    }

    g_pool_size = ALIGN_UP(size - offset, ALIGN_SIZE);// 调整大小以适应对齐后的起始地址
    if (clear)
    {
        memset(g_pool_start, 0, g_pool_size);
    }
    g_block_list = (mem_block_t*)g_pool_start;
    g_block_list->magic = MEM_MAGIC;
    g_block_list->size = g_pool_size;
    g_block_list->allocated = 0;
    g_block_list->prev = NULL;
    g_block_list->next = NULL;
    write_tail(g_block_list);
    g_initialized = true;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 分配申请的内存
 * @ param[in ]: size: 需要分配的内存大小
 * @return  :void*
 *==============================================================================*/  

void* mem_pool_alloc(uint32_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    MEM_LOCK();
    size = ALIGN_UP(size + sizeof(mem_block_t) + sizeof(uint32_t), ALIGN_SIZE);
    mem_block_t* curr = g_block_list;
    while (curr)
    {
        if (!curr->allocated && curr->size >= size)
        {
            split_block(curr, size);
            curr->allocated = 1;
            curr->magic = MEM_MAGIC;
            write_tail(curr);
            void* user_ptr = (uint8_t*)curr + sizeof(mem_block_t);
#ifdef MEM_DEBUG
            printf("[ALLOC] %p size=%lu\n", user_ptr, size);
#endif
            MEM_UNLOCK();
            return user_ptr;
        }
        curr = curr->next;
    }
    MEM_UNLOCK();
    return NULL;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 释放申请的内存
 * @ param[in ]: ptr: 需要释放的内存地址
 * @return  :void
 *==============================================================================*/

void mem_pool_free(void* ptr)
{
    MEM_LOCK();
    if (!is_valid_ptr(ptr))
    {
        MEM_UNLOCK();
        return;
    }

    mem_block_t* block = ptr_to_block(ptr);
    if (block->magic != MEM_MAGIC)
    {
        MEM_UNLOCK();
        return;
    }

    if (!block->allocated)
    {
        MEM_UNLOCK();
        return;
    }

    if (!check_tail(block))
    {
#ifdef MEM_DEBUG
        printf("[ERROR] memory overflow detected!\n");
#endif
    }

    uint32_t user_size = block->size - sizeof(mem_block_t) - sizeof(uint32_t);
    memset(ptr, 0xFF, user_size);
    block->allocated = 0;
    merge_next(block);
    block = merge_prev(block);
    merge_next(block);
#ifdef MEM_DEBUG
    printf("[FREE ] %p\n", ptr);
#endif

    MEM_UNLOCK();
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 分配并清零内存
 * @ param[in]: n: 元素个数
 *              size: 每个元素的大小
 * @ param[out]: null
 * @return  :void*
 *==============================================================================*/
void* mem_pool_calloc(uint32_t n, uint32_t size)
{
    uint32_t total = n * size;
    void* ptr = mem_pool_alloc(total);
    if (ptr)
    {
        memset(ptr, 0, total);
    }

    return ptr;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 重新分配内存
 * @ param[in ]: ptr: 需要重新分配的内存地址
 *               new_size: 新的内存大小
 * @return  :void*
 *==============================================================================*/

void* mem_pool_realloc(void* ptr, uint32_t new_size)
{
    if (ptr == NULL)
    {
        return mem_pool_alloc(new_size);
    }

    if (new_size == 0)
    {
        mem_pool_free(ptr);
        return NULL;
    }
    mem_block_t* block = ptr_to_block(ptr);
    uint32_t old_size = block->size - sizeof(mem_block_t) - sizeof(uint32_t);
    if (old_size >= new_size) // 如果新大小小于等于旧大小，直接返回原指针
    {
        return ptr;
    }
    void* new_ptr = mem_pool_alloc(new_size);
    if (new_ptr == NULL)
    {
        return NULL;
    }
    memcpy(new_ptr, ptr, old_size);
    mem_pool_free(ptr);
    return new_ptr;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 获取内存池中总的可用内存大小
 * @param[in ]: null
 * @param[out]: null
 * @return  :uint32_t 可用内存总大小
 *==============================================================================*/  
uint32_t mem_pool_total_free(void)
{
    MEM_LOCK();
    uint32_t total = 0;
    mem_block_t* curr = g_block_list;
    while (curr)
    {
        if (!curr->allocated)
        {
            total += curr->size - sizeof(mem_block_t) - sizeof(uint32_t);
        }
        curr = curr->next;
    }
    MEM_UNLOCK();
    return total;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 获取内存池中最大的可用内存块大小
 * @param[in ]: null
 * @param[out]: null
 * @return  :uint32_t 最大可用内存块大小
 *==============================================================================*/

uint32_t mem_pool_max_free(void)
{
    MEM_LOCK();
    uint32_t max = 0;
    mem_block_t* curr = g_block_list;
    while (curr)
    {
        if (!curr->allocated)
        {
            uint32_t usable = curr->size - sizeof(mem_block_t) - sizeof(uint32_t);
            if (usable > max)
            {
                max = usable;
            }
        }
        curr = curr->next;
    }
    MEM_UNLOCK();
    return max;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 获取内存池中已使用的内存大小
 * @param[in ]: null
 * @param[out]: null
 * @return  :uint32_t 已使用内存大小
 *==============================================================================*/

uint32_t mem_pool_used(void)
{
    MEM_LOCK();

    uint32_t used = 0;

    mem_block_t* curr = g_block_list;

    while (curr)
    {
        if (curr->allocated)
        {
            used += curr->size;
        }

        curr = curr->next;
    }

    MEM_UNLOCK();

    return used;
}

/*==============================================================================
 * @ author : 18895
 * @ date   : 2026-06-01
 * @ brief  : 打印内存池状态
 * @param[in ]: null
 * @param[out]: null
 * @return  :void
 *==============================================================================*/

void mem_pool_dump(void)
{
    MEM_LOCK();
#ifdef MEM_DEBUG
    printf("\n=========== MEM POOL DUMP ===========\n");
#endif
    mem_block_t* curr = g_block_list;

    while (curr)
    {
        #ifdef MEM_DEBUG
            printf("BLOCK %p | size=%lu | used=%u\n", curr, curr->size, curr->allocated);
        #endif
        curr = curr->next;
    }
    #ifdef MEM_DEBUG
     printf("=====================================\n");
    #endif
    MEM_UNLOCK();
}