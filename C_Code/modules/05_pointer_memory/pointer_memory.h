/**
 * @file pointer_memory.h
 * @brief 指针与内存管理模块头文件
 * @details 涵盖指针基础、指针运算、动态内存、内存对齐等核心知识点。
 *          指针是C语言最强大也最危险的特性，是嵌入式开发的核心技能。
 * 
 * @section main_features 主要内容
 * - 指针基础：指针声明、初始化、解引用、空指针
 * - 指针运算：指针加减、指针比较、指针差值
 * - 指针与数组：数组名与指针、指针遍历数组
 * - 指针与字符串：字符串指针、字符串操作
 * - 函数指针：函数指针声明、回调函数、函数指针数组
 * - 动态内存：malloc/calloc/realloc/free的使用和注意事项
 * - 内存对齐：对齐概念、对齐检查、结构体对齐
 * - 内存池：静态内存池的实现和应用
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 直接访问硬件寄存器
 * - 高效的内存管理
 * - 驱动程序开发
 * - RTOS任务栈管理
 * - DMA数据传输
 * 
 * @section safety_notes 安全注意事项
 * - 使用指针前必须检查是否为NULL
 * - 避免野指针和悬空指针
 * - 动态分配的内存必须及时释放
 * - 注意内存对齐问题
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @warning 指针操作错误可能导致程序崩溃或未定义行为
 * @note 嵌入式系统中通常避免使用动态内存分配
 */

#ifndef POINTER_MEMORY_H
#define POINTER_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 指针基础演示
 * @details 展示指针的基本概念：
 *          - 指针声明：int *p;
 *          - 指针初始化：p = &var;
 *          - 解引用：*p获取值
 *          - 空指针：NULL的使用
 *          - 野指针的危害
 */
void demo_pointer_basics(void);

/**
 * @brief 指针运算演示
 * @details 展示指针的算术运算：
 *          - 指针加减整数：p+1移动一个元素大小
 *          - 指针相减：得到元素个数差
 *          - 指针比较：判断位置关系
 *          - 指针与下标：p[i]等价于*(p+i)
 */
void demo_pointer_arithmetic(void);

/**
 * @brief 指针与数组演示
 * @details 展示指针与数组的密切关系：
 *          - 数组名是首元素地址
 *          - 指针遍历数组
 *          - 数组作为函数参数退化为指针
 *          - 多维数组与指针
 */
void demo_pointer_and_array(void);

/**
 * @brief 指针与字符串演示
 * @details 展示字符串的指针操作：
 *          - 字符串字面量与字符指针
 *          - 字符数组与字符串指针的区别
 *          - 字符串遍历和操作
 *          - 常见字符串函数的实现
 */
void demo_pointer_and_string(void);

/**
 * @brief 函数指针参数演示
 * @details 展示函数指针的使用：
 *          - 函数指针声明：int (*pfunc)(int, int);
 *          - 函数指针作为参数（回调函数）
 *          - 函数指针数组
 *          - 回调函数在嵌入式中的应用
 */
void demo_function_pointer_param(void);

/**
 * @brief 动态内存管理演示
 * @details 展示动态内存分配函数：
 *          - malloc: 分配内存
 *          - calloc: 分配并清零
 *          - realloc: 调整大小
 *          - free: 释放内存
 *          - 内存泄漏检测
 * @warning 嵌入式系统中应谨慎使用动态内存
 */
void demo_dynamic_memory(void);

/**
 * @brief 内存对齐演示
 * @details 展示内存对齐的概念和重要性：
 *          - 对齐原因：硬件访问效率
 *          - 对齐规则：各类型的对齐要求
 *          - 结构体对齐和填充
 *          - 使用alignof和aligned属性
 *          - 嵌入式中的对齐要求（DMA、硬件寄存器）
 */
void demo_memory_alignment(void);

/**
 * @brief 高级指针演示
 * @details 展示指针的高级用法：
 *          - 多级指针：int **pp;
 *          - 指向数组的指针：int (*p)[N];
 *          - 指向函数的指针数组
 *          - void指针的使用
 *          - const指针和指向const的指针
 */
void demo_pointer_advanced(void);

/**
 * @brief 内存池演示
 * @details 展示静态内存池的实现：
 *          - 内存池的概念和优势
 *          - 固定大小块分配
 *          - 内存池的申请和释放
 *          - 嵌入式系统中的应用场景
 * @note 内存池是嵌入式系统常用的内存管理方式
 */
void demo_memory_pool(void);

/*============================================================================*/
/*                           安全内存函数                                       */
/*============================================================================*/

/**
 * @brief 安全的内存分配函数
 * @details 封装malloc，增加NULL检查和错误处理。
 *          分配失败时输出错误信息并返回NULL。
 * 
 * @param size 要分配的字节数
 * @return 分配成功返回内存指针，失败返回NULL
 * 
 * @note 分配的内存需要调用safe_free释放
 * @code
 * void *ptr = safe_malloc(100);
 * if (ptr != NULL) {
 *     // 使用内存
 *     safe_free(&ptr);
 * }
 * @endcode
 */
void *safe_malloc(size_t size);

/**
 * @brief 安全的内存分配函数（清零）
 * @details 封装calloc，分配内存并初始化为零。
 * 
 * @param count 元素个数
 * @param size 每个元素的大小
 * @return 分配成功返回内存指针，失败返回NULL
 */
void *safe_calloc(size_t count, size_t size);

/**
 * @brief 安全的内存重分配函数
 * @details 封装realloc，调整已分配内存的大小。
 * 
 * @param ptr 原内存指针（可为NULL，此时等同于malloc）
 * @param size 新的大小
 * @return 重分配成功返回新指针，失败返回NULL（原内存不变）
 * 
 * @warning 返回的指针可能与原指针不同
 */
void *safe_realloc(void *ptr, size_t size);

/**
 * @brief 安全的内存释放函数
 * @details 封装free，释放后将指针置NULL，防止悬空指针。
 * 
 * @param ptr 指向指针的指针
 * 
 * @code
 * int *p = malloc(sizeof(int));
 * safe_free(&p);  // 释放内存并将p置为NULL
 * // 此时p == NULL
 * @endcode
 */
void safe_free(void **ptr);

/*============================================================================*/
/*                           数组操作函数                                       */
/*============================================================================*/

/**
 * @brief 创建动态数组
 * @details 动态分配指定大小的整型数组。
 * 
 * @param size 数组元素个数
 * @return 成功返回数组指针，失败返回NULL
 */
int32_t *create_array(size_t size);

/**
 * @brief 销毁动态数组
 * @details 释放动态数组内存并将指针置NULL。
 * 
 * @param arr 指向数组指针的指针
 */
void destroy_array(int32_t **arr);

/**
 * @brief 填充数组
 * @details 将数组的所有元素设置为指定值。
 * 
 * @param arr 数组指针
 * @param size 数组大小
 * @param value 填充值
 */
void fill_array(int32_t *arr, size_t size, int32_t value);

/**
 * @brief 查找数组元素
 * @details 在数组中查找指定值的元素。
 * 
 * @param arr 数组指针
 * @param size 数组大小
 * @param target 目标值
 * @return 找到返回元素指针，未找到返回NULL
 */
int32_t *find_element(int32_t *arr, size_t size, int32_t target);

/**
 * @brief 交换两个整数
 * @details 通过指针交换两个整数的值。
 * 
 * @param a 第一个整数的指针
 * @param b 第二个整数的指针
 */
void swap_int(int32_t *a, int32_t *b);

/**
 * @brief 反转数组
 * @details 原地反转数组元素的顺序。
 * 
 * @param arr 数组指针
 * @param size 数组大小
 */
void reverse_array(int32_t *arr, size_t size);

#ifdef __cplusplus
}
}
#endif

#endif
