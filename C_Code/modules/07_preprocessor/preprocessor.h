/**
 * @file preprocessor.h
 * @brief 预处理指令模块头文件
 * @details 涵盖宏定义、条件编译、文件包含等预处理知识点。
 *          预处理器是C语言编译的第一阶段，掌握预处理技术对嵌入式开发至关重要。
 * 
 * @section main_features 主要内容
 * - 宏定义基础：#define、#undef、对象宏、函数宏
 * - 宏高级技巧：字符串化#、连接##、可变参数
 * - 条件编译：#if、#ifdef、#ifndef、#elif、#endif
 * - 文件包含：#include、重复包含防护
 * - 预定义宏：__FILE__、__LINE__、__DATE__等
 * - #pragma指令：编译器特定指令
 * - 编译时检查：静态断言、编译时计算
 * - 代码生成：X-Macro技术
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 硬件抽象：通过宏定义寄存器地址
 * - 平台适配：条件编译适配不同芯片
 * - 调试开关：DEBUG宏控制调试代码
 * - 代码优化：内联函数、常量折叠
 * - 配置管理：通过宏配置功能开关
 * 
 * @section best_practices 最佳实践
 * - 宏定义使用大写命名
 * - 函数宏参数必须加括号
 * - 使用do-while(0)包裹多语句宏
 * - 避免有副作用的宏参数
 * - 复杂逻辑优先使用内联函数
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @warning 宏定义不进行类型检查，需谨慎使用
 * @note 宏展开是文本替换，注意副作用
 */

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 宏定义基础演示
 * @details 展示宏定义的基本用法：
 *          - 对象宏：#define PI 3.14159
 *          - 函数宏：#define MAX(a,b) ((a)>(b)?(a):(b))
 *          - #undef取消定义
 *          - 宏的作用域
 */
void demo_macro_basics(void);

/**
 * @brief 宏高级技巧演示
 * @details 展示宏的高级用法：
 *          - 字符串化操作符#
 *          - 连接操作符##
 *          - 可变参数宏...和__VA_ARGS__
 *          - 宏的嵌套展开
 */
void demo_macro_advanced(void);

/**
 * @brief 条件编译演示
 * @details 展示条件编译的使用：
 *          - #if、#elif、#else、#endif
 *          - #ifdef、#ifndef
 *          - defined操作符
 *          - 平台适配和功能开关
 */
void demo_conditional_compile(void);

/**
 * @brief 文件包含演示
 * @details 展示文件包含的使用：
 *          - #include <...>和#include "..."
 *          - 重复包含防护（#ifndef/#define/#endif）
 *          - #pragma once
 *          - 头文件组织原则
 */
void demo_file_include(void);

/**
 * @brief 预定义宏演示
 * @details 展示编译器预定义宏：
 *          - __FILE__：当前文件名
 *          - __LINE__：当前行号
 *          - __DATE__、__TIME__：编译日期时间
 *          - __func__：当前函数名
 *          - 平台相关宏：__cplusplus、_WIN32等
 */
void demo_predefined_macros(void);

/**
 * @brief #pragma指令演示
 * @details 展示#pragma指令的使用：
 *          - #pragma message：编译时消息
 *          - #pragma warning：警告控制
 *          - #pragma pack：结构体对齐
 *          - #pragma once：单次包含
 */
void demo_pragma_directive(void);

/**
 * @brief 编译时检查演示
 * @details 展示编译时检查技术：
 *          - 静态断言（Static Assert）
 *          - 编译时计算
 *          - 类型大小检查
 *          - 数组大小检查
 */
void demo_compile_time_checks(void);

/**
 * @brief 代码生成演示
 * @details 展示宏代码生成技术：
 *          - X-Macro技术
 *          - 代码自动生成
 *          - 减少重复代码
 *          - 配置驱动开发
 */
void demo_code_generation(void);

/*============================================================================*/
/*                           版本宏定义                                         */
/*============================================================================*/

/** @brief 主版本号 */
#define VERSION_MAJOR 1

/** @brief 次版本号 */
#define VERSION_MINOR 0

/** @brief 补丁版本号 */
#define VERSION_PATCH 0

/*============================================================================*/
/*                           宏工具定义                                         */
/*============================================================================*/

/**
 * @brief 字符串化宏（内部实现）
 * @details 使用#操作符将参数转为字符串
 */
#define STRINGIFY_IMPL(x) #x

/**
 * @brief 字符串化宏
 * @details 将参数转为字符串，支持宏展开
 * @param x 要字符串化的值
 * @code
 * #define VALUE 100
 * const char *s = STRINGIFY(VALUE);  // s = "100"
 * @endcode
 */
#define STRINGIFY(x) STRINGIFY_IMPL(x)

/**
 * @brief 连接宏（内部实现）
 * @details 使用##操作符连接两个标记
 */
#define CONCAT_IMPL(a, b) a##b

/**
 * @brief 连接宏
 * @details 连接两个标记，支持宏展开
 * @param a 第一个标记
 * @param b 第二个标记
 * @code
 * #define PREFIX var
 * int CONCAT(PREFIX, _name) = 10;  // int var_name = 10;
 * @endcode
 */
#define CONCAT(a, b) CONCAT_IMPL(a, b)

/**
 * @brief 版本字符串
 * @details 自动生成的版本字符串
 */
#define VERSION_STRING STRINGIFY(VERSION_MAJOR) "." STRINGIFY(VERSION_MINOR) "." STRINGIFY(VERSION_PATCH)

/*============================================================================*/
/*                           调试宏定义                                         */
/*============================================================================*/

#ifdef DEBUG_MODE
/**
 * @brief 调试打印宏
 * @details 仅在DEBUG_MODE定义时输出调试信息
 * @param fmt 格式字符串
 * @param ... 可变参数
 */
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
/** @brief 调试打印宏（空实现） */
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

/*============================================================================*/
/*                           宏模板定义                                         */
/*============================================================================*/

/**
 * @brief MIN函数模板
 * @details 生成特定类型的MIN函数
 * @param type 数据类型
 * @code
 * MIN_IMPL(int32_t)  // 生成 int32_t MIN_int32_t(int32_t a, int32_t b)
 * @endcode
 */
#define MIN_IMPL(type) \
    static inline type MIN_##type(type a, type b) { \
        return (a < b) ? a : b; \
    }

/**
 * @brief 参数计数宏（内部实现）
 * @details 用于实现可变参数宏的参数计数
 */
#define COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, N, ...) N

/**
 * @brief 参数计数宏
 * @details 计算可变参数的个数（支持1-5个参数）
 * @param ... 可变参数
 * @return 参数个数
 * @code
 * int n = COUNT_ARGS(a, b, c);  // n = 3
 * @endcode
 */
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1, 0)

#ifdef __cplusplus
}
}
#endif

#endif
