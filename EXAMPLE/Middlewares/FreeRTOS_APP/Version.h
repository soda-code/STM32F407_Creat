#ifndef __VERSION_H
#define __VERSION_H

#include "sys.h"

// 第一层：转换宏（真正执行转换）
#define TO_STR(s) #s

// 第二层：展开宏（先展开宏内容，再传给第一层）
#define STR(s) TO_STR(s)

// 版本定义
#define VER_MAJOR    1
#define VER_MINOR    00
#define VER_PATCH    01

// 合并版本号：V1.0.00
#define APP_VERSION  "V" STR(VER_MAJOR) "." STR(VER_MINOR) "." STR(VER_PATCH)

// 合并编译时间：V2.5.3 (Build: Dec 27 2025 01:50:00)
#define FULL_VERSION APP_VERSION " (Build: " __DATE__ " " __TIME__ ")"


#endif
