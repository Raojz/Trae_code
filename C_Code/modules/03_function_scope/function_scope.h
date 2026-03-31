/**
 * @file function_scope.h
 * @brief 函数与作用域模块头文件
 * @details 涵盖函数定义、参数传递、作用域、生命周期等核心知识点。
 *          函数是C语言模块化编程的基础，良好的函数设计是代码质量的关键。
 * 
 * @section main_features 主要内容
 * - 函数基础：声明、定义、调用
 * - 参数传递：值传递、指针传递
 * - 返回值：单值返回、多值返回（指针参数）
 * - 递归：递归函数的实现和注意事项
 * - 内联函数：inline关键字的使用
 * - 函数指针：函数指针声明和使用
 * - 回调函数：回调机制和应用场景
 * - 变量生命周期：自动变量、静态变量、全局变量
 * - 作用域规则：文件作用域、块作用域、函数原型作用域
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 函数是模块化设计的基础
 * - 回调函数用于中断处理、事件响应
 * - 函数指针用于驱动框架、状态机
 * - 内联函数用于性能关键代码
 * 
 * @section best_practices 最佳实践
 * - 函数应该单一职责
 * - 函数长度不宜过长（建议不超过50行）
 * - 参数个数不宜过多（建议不超过4个）
 * - 避免递归，防止栈溢出
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 嵌入式系统中应避免深度递归，可能导致栈溢出
 */

#ifndef FUNCTION_SCOPE_H
#define FUNCTION_SCOPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 函数基础演示
 * @details 展示函数的基本概念：
 *          - 函数声明（原型）
 *          - 函数定义
 *          - 函数调用
 *          - 函数返回值
 *          - 空函数和占位函数
 */
void demo_function_basics(void);

/**
 * @brief 函数参数传递演示
 * @details 展示参数传递的方式：
 *          - 值传递：复制实参值
 *          - 指针传递：传递地址，可修改原值
 *          - 数组参数：退化为指针
 *          - const参数：防止修改
 */
void demo_function_parameters(void);

/**
 * @brief 函数返回值演示
 * @details 展示函数返回值的使用：
 *          - 返回基本类型
 *          - 返回指针（注意生命周期）
 *          - 通过指针参数返回多值
 *          - 返回结构体
 */
void demo_function_return(void);

/**
 * @brief 递归函数演示
 * @details 展示递归函数的使用：
 *          - 递归的基本概念
 *          - 递归终止条件
 *          - 典型递归示例：阶乘、斐波那契
 *          - 递归的栈开销
 * @warning 嵌入式系统中应避免深度递归
 */
void demo_recursion(void);

/**
 * @brief 内联函数演示
 * @details 展示内联函数的使用：
 *          - inline关键字的作用
 *          - 内联函数与宏的区别
 *          - 适用场景：短小、频繁调用的函数
 *          - 编译器优化选项
 */
void demo_inline_function(void);

/**
 * @brief 函数指针演示
 * @details 展示函数指针的使用：
 *          - 函数指针声明：int (*pfunc)(int, int);
 *          - 函数指针赋值和调用
 *          - 函数指针数组
 *          - 函数指针作为结构体成员
 */
void demo_function_pointer(void);

/**
 * @brief 回调函数演示
 * @details 展示回调函数的使用：
 *          - 回调函数的概念
 *          - 回调函数的注册和调用
 *          - 典型应用：事件处理、排序比较
 *          - 嵌入式中的中断回调
 */
void demo_callback_function(void);

/**
 * @brief 变量生命周期演示
 * @details 展示变量的生命周期：
 *          - 自动变量：函数调用时创建，返回时销毁
 *          - 静态局部变量：程序启动时创建，程序结束时销毁
 *          - 全局变量：程序启动时创建，程序结束时销毁
 *          - 动态分配：手动管理生命周期
 */
void demo_variable_lifetime(void);

/**
 * @brief 作用域规则演示
 * @details 展示变量的作用域规则：
 *          - 文件作用域（全局变量）
 *          - 函数作用域（标签）
 *          - 块作用域（局部变量）
 *          - 函数原型作用域
 *          - 变量名隐藏（shadowing）
 */
void demo_scope_rules(void);

/*============================================================================*/
/*                           数学运算函数                                       */
/*============================================================================*/

/**
 * @brief 加法运算
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return a + b 的结果
 */
int32_t add(int32_t a, int32_t b);

/**
 * @brief 减法运算
 * @param a 被减数
 * @param b 减数
 * @return a - b 的结果
 */
int32_t subtract(int32_t a, int32_t b);

/**
 * @brief 乘法运算
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return a * b 的结果
 */
int32_t multiply(int32_t a, int32_t b);

/**
 * @brief 除法运算
 * @param a 被除数
 * @param b 除数
 * @return a / b 的结果
 * @warning 除数为0时行为未定义
 */
int32_t divide(int32_t a, int32_t b);

/*============================================================================*/
/*                           参数传递示例函数                                   */
/*============================================================================*/

/**
 * @brief 值传递交换（无法交换）
 * @details 演示值传递无法修改原值
 * @param a 第一个值（副本）
 * @param b 第二个值（副本）
 * @note 此函数无法真正交换两个变量的值
 */
void swap_by_value(int32_t a, int32_t b);

/**
 * @brief 指针传递交换
 * @details 演示指针传递可以修改原值
 * @param a 第一个变量的指针
 * @param b 第二个变量的指针
 */
void swap_by_pointer(int32_t *a, int32_t *b);

/*============================================================================*/
/*                           递归函数示例                                       */
/*============================================================================*/

/**
 * @brief 计算阶乘（递归实现）
 * @param n 非负整数
 * @return n的阶乘
 * @note n <= 1 返回 1
 * @warning 大数阶乘可能溢出
 */
int32_t factorial(int32_t n);

/**
 * @brief 计算斐波那契数（递归实现）
 * @param n 斐波那契数列的索引（从0开始）
 * @return 第n个斐波那契数
 * @note F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2)
 * @warning 递归实现效率低，n较大时建议使用迭代
 */
int32_t fibonacci(int32_t n);

/*============================================================================*/
/*                           查找算法                                           */
/*============================================================================*/

/**
 * @brief 二分查找
 * @details 在有序数组中查找目标值
 * @param arr 有序数组指针
 * @param size 数组大小
 * @param target 目标值
 * @return 找到返回索引，未找到返回-1
 * @note 数组必须已排序
 */
int32_t binary_search(const int32_t *arr, size_t size, int32_t target);

#ifdef __cplusplus
}
#endif

#endif
