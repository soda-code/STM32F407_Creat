#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void mem_pool_init(void* addr, uint32_t size, bool clear); // 初始化内存池
void* mem_pool_alloc(uint32_t size);    // 分配内存
void mem_pool_free(void* ptr);          // 释放内存
void* mem_pool_calloc(uint32_t n, uint32_t size);       // 分配并清零内存
void* mem_pool_realloc(void* ptr, uint32_t new_size);   // 重新分配内存
uint32_t mem_pool_total_free(void); // 获取内存池中总的可用内存大小
uint32_t mem_pool_max_free(void);   // 获取内存池中最大的连续可用内存块大小
uint32_t mem_pool_used(void);       // 获取内存池中已使用的内存大小
void mem_pool_dump(void);           // 打印内存池状态

#ifdef __cplusplus
}
#endif

#endif