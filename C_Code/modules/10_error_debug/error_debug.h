/**
 * @file error_debug.h
 * @brief 错误处理与调试模块头文件
 * @details 涵盖错误处理机制、断言、调试技巧、看门狗等知识点。
 *          良好的错误处理是嵌入式系统可靠性的关键保障。
 * 
 * @section main_features 主要内容
 * - 错误处理：错误码、错误信息、错误传播
 * - 断言机制：编译时断言、运行时断言
 * - 调试技巧：打印调试、条件编译、日志系统
 * - 日志系统：分级日志、格式化输出
 * - 防御性编程：参数检查、边界检查
 * - 错误恢复：重试机制、降级处理
 * - 看门狗：超时检测、系统复位
 * - 栈追踪：调用链记录
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 系统可靠性保障
 * - 故障诊断和定位
 * - 远程调试支持
 * - 看门狗防止死机
 * - 生产环境问题排查
 * 
 * @section best_practices 最佳实践
 * - 所有函数返回错误码
 * - 检查所有可能的错误
 * - 使用断言检查不变量
 * - 记录关键操作日志
 * - 实现错误恢复机制
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 生产环境中断言通常被禁用
 * @note 看门狗是嵌入式系统的重要保护机制
 */

#ifndef ERROR_DEBUG_H
#define ERROR_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 错误处理机制演示
 * @details 展示错误处理的基本方法：
 *          - 错误码定义和使用
 *          - 错误信息获取
 *          - 错误传播（返回值）
 *          - 错误上下文记录
 */
void demo_error_handling(void);

/**
 * @brief 断言使用演示
 * @details 展示断言的使用：
 *          - 编译时断言（静态断言）
 *          - 运行时断言
 *          - 断言与错误处理的区别
 *          - 断言在生产环境中的处理
 */
void demo_assert_usage(void);

/**
 * @brief 调试技巧演示
 * @details 展示常用的调试技巧：
 *          - 打印调试法
 *          - 条件编译调试
 *          - 调试宏的使用
 *          - 内存转储
 */
void demo_debug_techniques(void);

/**
 * @brief 日志系统演示
 * @details 展示日志系统的设计和使用：
 *          - 日志级别
 *          - 格式化日志
 *          - 日志缓冲
 *          - 日志过滤
 */
void demo_logging_system(void);

/**
 * @brief 防御性编程演示
 * @details 展示防御性编程技术：
 *          - 参数有效性检查
 *          - 边界检查
 *          - 空指针检查
 *          - 溢出检查
 */
void demo_defensive_programming(void);

/**
 * @brief 错误恢复演示
 * @details 展示错误恢复机制：
 *          - 重试机制
 *          - 降级处理
 *          - 默认值回退
 *          - 系统重启
 */
void demo_error_recovery(void);

/**
 * @brief 看门狗演示
 * @details 展示看门狗的使用：
 *          - 看门狗初始化
 *          - 喂狗操作
 *          - 超时检测
 *          - 看门狗复位
 * @note 看门狗是嵌入式系统的重要保护机制
 */
void demo_watchdog(void);

/**
 * @brief 栈追踪演示
 * @details 展示调用栈追踪技术：
 *          - 函数调用记录
 *          - 调用链打印
 *          - 错误定位辅助
 * @note 嵌入式系统中栈追踪能力有限
 */
void demo_stack_trace(void);

/*============================================================================*/
/*                           错误码定义                                         */
/*============================================================================*/

/**
 * @defgroup ErrorCodes 错误码定义
 * @brief 统一的错误码体系
 * @details 错误码用于函数返回值，表示操作结果。
 *          使用 Status_t 枚举类型，定义在 common_types.h 中。
 *          这里提供宏别名以保持兼容性。
 * 
 * @note 推荐使用 Status_t 枚举值（如 STATUS_OK）
 * @note ERR_xxx 宏是为了兼容旧代码而保留的别名
 * @{
 */

/** @brief 操作成功 */
#define ERR_SUCCESS             STATUS_OK

/** @brief 一般错误 */
#define ERR_GENERAL             STATUS_ERROR

/** @brief 无效参数 */
#define ERR_INVALID_PARAM       STATUS_INVALID_PARAM

/** @brief 空指针 */
#define ERR_NULL_PTR            STATUS_NULL_PTR

/** @brief 超时 */
#define ERR_TIMEOUT             STATUS_TIMEOUT

/** @brief 未找到 */
#define ERR_NOT_FOUND           STATUS_NOT_FOUND

/** @brief 忙 */
#define ERR_BUSY                STATUS_BUSY

/** @brief 内存不足 */
#define ERR_NO_MEMORY           STATUS_NO_MEM

/** @brief 溢出 */
#define ERR_BUFFER_OVERFLOW     STATUS_OVERFLOW

/** @brief 下溢 */
#define ERR_BUFFER_UNDERFLOW    STATUS_UNDERFLOW

/** @brief 超出范围（扩展定义） */
#define ERR_OUT_OF_RANGE        (-10)

/** @brief 已存在（扩展定义） */
#define ERR_ALREADY_EXISTS      (-11)

/** @brief I/O错误（扩展定义） */
#define ERR_IO_ERROR            (-12)

/** @brief 未初始化（扩展定义） */
#define ERR_NOT_INITIALIZED     (-13)

/** @brief 不支持（扩展定义） */
#define ERR_NOT_SUPPORTED       (-14)

/** @brief 权限拒绝（扩展定义） */
#define ERR_PERMISSION_DENIED   (-15)

/** @brief 数据损坏（扩展定义） */
#define ERR_DATA_CORRUPT        (-16)

/** @brief 设备错误（扩展定义） */
#define ERR_DEVICE_ERROR        (-17)

/** @brief 通信错误（扩展定义） */
#define ERR_COMM_ERROR          (-18)

/** @brief 协议错误（扩展定义） */
#define ERR_PROTOCOL_ERROR      (-19)

/** @brief 操作中止（扩展定义） */
#define ERR_ABORTED             (-20)

/** @brief 资源耗尽（扩展定义） */
#define ERR_RESOURCE_EXHAUSTED  (-21)

/** @} */

/*============================================================================*/
/*                           类型定义                                           */
/*============================================================================*/

/**
 * @brief 错误信息结构体
 * @details 用于存储完整的错误上下文信息
 */
typedef struct {
    int32_t code;           /**< 错误码 */
    const char *message;    /**< 错误消息 */
    const char *file;       /**< 发生错误的文件名 */
    int32_t line;           /**< 发生错误的行号 */
    const char *function;   /**< 发生错误的函数名 */
} ErrorInfo_t;

/**
 * @brief 看门狗状态枚举
 */
typedef enum {
    WATCHDOG_STATE_IDLE,      /**< 空闲状态 */
    WATCHDOG_STATE_RUNNING,   /**< 运行中 */
    WATCHDOG_STATE_TIMEOUT    /**< 超时 */
} WatchdogState_t;

/*============================================================================*/
/*                           错误处理函数                                       */
/*============================================================================*/

/**
 * @brief 设置错误信息
 * @details 记录错误码、消息、位置等信息
 * @param code 错误码
 * @param message 错误消息
 * @param file 文件名
 * @param line 行号
 * @param function 函数名
 */
void error_set(int32_t code, const char *message, const char *file, int32_t line, const char *function);

/**
 * @brief 获取当前错误信息
 * @return 错误信息结构体
 */
ErrorInfo_t error_get(void);

/**
 * @brief 清除错误信息
 */
void error_clear(void);

/**
 * @brief 根据错误码获取错误消息
 * @param code 错误码
 * @return 错误消息字符串
 */
const char *error_get_message(int32_t code);

/*============================================================================*/
/*                           错误处理宏                                         */
/*============================================================================*/

/**
 * @brief 设置错误宏
 * @details 自动填充文件名、行号、函数名
 * @param code 错误码
 * @param msg 错误消息
 * @code
 * ERROR_SET(STATUS_ERROR, "参数无效");
 * @endcode
 */
#define ERROR_SET(code, msg) \
    error_set((code), (msg), __FILE__, __LINE__, __func__)

/**
 * @brief 错误检查宏
 * @details 检查条件，失败时设置错误并返回
 * @param cond 条件表达式
 * @param code 错误码
 * @param msg 错误消息
 * @code
 * ERROR_CHECK(ptr != NULL, STATUS_NULL_PTR, "指针为空");
 * @endcode
 */
#define ERROR_CHECK(cond, code, msg) \
    do { \
        if (!(cond)) { \
            ERROR_SET((code), (msg)); \
            return (code); \
        } \
    } while(0)

/*============================================================================*/
/*                           断言宏定义                                         */
/*============================================================================*/

#ifdef DEBUG
/**
 * @brief 调试断言宏
 * @details 仅在DEBUG模式下有效，断言失败时打印信息并停止
 * @param cond 条件表达式
 */
#define DEBUG_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            printf("[ASSERT] %s:%d %s: %s\n", __FILE__, __LINE__, __func__, #cond); \
            while(1); \
        } \
    } while(0)
#else
/** @brief 调试断言宏（空实现） */
#define DEBUG_ASSERT(cond) ((void)0)
#endif

/*============================================================================*/
/*                           错误传播宏                                         */
/*============================================================================*/

/**
 * @brief TRY宏
 * @details 执行表达式，如果失败则传播错误
 * @param expr 返回错误码的表达式
 * @code
 * TRY(some_function());
 * @endcode
 */
#define TRY(expr) \
    do { \
        int32_t __result = (expr); \
        if (__result != STATUS_OK) { \
            return __result; \
        } \
    } while(0)

/*============================================================================*/
/*                           看门狗函数                                         */
/*============================================================================*/

/**
 * @brief 初始化看门狗
 * @param timeout_ms 超时时间（毫秒）
 */
void watchdog_init(uint32_t timeout_ms);

/**
 * @brief 喂狗
 * @details 重置看门狗计数器，必须在超时前调用
 */
void watchdog_feed(void);

/**
 * @brief 获取看门狗状态
 * @return 当前看门狗状态
 */
WatchdogState_t watchdog_get_state(void);

/**
 * @brief 模拟看门狗超时
 * @details 用于测试看门狗功能
 */
void watchdog_simulate_timeout(void);


/**
 * @brief 看门狗超时处理
 * @details 当看门狗超时时调用，执行系统复位或其他恢复操作。
 *
 * @note 在实际系统中应触发硬件复位
 * @note 可以在此记录错误日志或执行清理操作
 */
void watchdog_timeout_handler(void);

#ifdef __cplusplus
}
#endif

#endif
