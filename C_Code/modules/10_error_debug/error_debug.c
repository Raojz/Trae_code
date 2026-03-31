/**
 * @file error_debug.c
 * @brief 错误处理与调试模块实现
 * @details 演示C语言错误处理和调试技术，涵盖以下核心内容：
 *          - 错误码定义：统一的错误码体系
 *          - 错误信息管理：错误码到消息的映射
 *          - 错误上下文：记录错误发生的位置
 *          - 断言机制：运行时检查和调试断言
 *          - 日志系统：分级日志输出
 *          - 看门狗：系统监控和恢复
 *          - 调试输出：条件编译的调试信息
 *          - 错误处理模式：返回码、全局错误、异常模拟
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 系统可靠性：及时发现和处理错误
 * - 远程诊断：通过日志定位问题
 * - 看门狗监控：防止系统死锁
 * - 资源受限：高效轻量的错误处理
 * - 安全关键：符合功能安全标准
 * 
 * @section best_practices 最佳实践
 * - 所有函数返回错误码或状态
 * - 检查所有可能失败的调用
 * - 记录足够的错误上下文
 * - 使用断言检查不变量
 * - 区分错误和异常情况
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @warning 错误处理不当可能导致系统不稳定
 * @note 生产环境中应关闭调试断言
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "error_debug.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
/*============================================================================*/
/*                           私有变量定义                                       */
/*============================================================================*/

/** @brief 全局错误信息存储 */
static ErrorInfo_t g_last_error = {0, NULL, NULL, 0, NULL};

/** @brief 看门狗超时计数器 */
static volatile uint32_t g_watchdog_counter = 0;

/** @brief 看门狗超时阈值 */
static uint32_t g_watchdog_timeout = 1000;

/** @brief 看门狗是否启用 */
static uint8_t g_watchdog_enabled = 0;

/*============================================================================*/
/*                           错误处理函数实现                                   */
/*============================================================================*/

/**
 * @brief 设置错误信息
 * @details 记录最近一次发生的错误，包括错误码、消息、位置等信息。
 *          通过宏ERROR_SET调用，自动填充文件名、行号、函数名。
 * 
 * @param code 错误码
 * @param message 错误消息字符串
 * @param file 源文件名
 * @param line 行号
 * @param function 函数名
 * 
 * @note 只保存最近一次错误，之前的错误会被覆盖
 * @note 消息指针直接存储，不复制内容
 * 
 * @code
 * ERROR_SET(ERR_INVALID_PARAM, "参数不能为NULL");
 * @endcode
 */
void error_set(int32_t code, const char *message, const char *file, 
               int32_t line, const char *function)
{
    g_last_error.code = code;
    g_last_error.message = message;
    g_last_error.file = file;
    g_last_error.line = line;
    g_last_error.function = function;
    
    LOG_ERROR("错误 [%d] %s (%s:%d in %s())", 
              code, message ? message : "未知错误",
              file ? file : "未知文件", 
              line, 
              function ? function : "未知函数");
}

/**
 * @brief 获取最后的错误信息
 * @details 返回最近一次记录的错误信息结构体。
 * 
 * @return 错误信息结构体的指针
 * 
 * @note 返回的是内部静态变量的指针，不需要释放
 * @note 如果没有错误发生，code字段为0
 * 
 * @code
 * const ErrorInfo_t *err = error_get_last();
 * if (err->code != 0) {
 *     printf("错误: %s\n", err->message);
 * }
 * @endcode
 */
const ErrorInfo_t *error_get_last(void)
{
    return &g_last_error;
}

/**
 * @brief 获取当前错误信息（返回副本）
 * @details 返回错误信息结构体的副本。
 * 
 * @return 错误信息结构体
 * 
 * @note 返回结构体副本，可安全修改
 * @note 如果没有错误发生，code字段为0
 * 
 * @code
 * ErrorInfo_t err = error_get();
 * if (err.code != 0) {
 *     printf("错误: %s\n", err.message);
 * }
 * @endcode
 */
ErrorInfo_t error_get(void)
{
    return g_last_error;
}

/**
 * @brief 清除错误信息
 * @details 重置错误信息为初始状态。
 * 
 * @code
 * error_clear();
 * @endcode
 */
void error_clear(void)
{
    g_last_error.code = 0;
    g_last_error.message = NULL;
    g_last_error.file = NULL;
    g_last_error.line = 0;
    g_last_error.function = NULL;
}

/**
 * @brief 打印错误信息
 * @details 格式化输出最近的错误信息。
 * 
 * @code
 * if (error_get_last()->code != 0) {
 *     error_print();
 * }
 * @endcode
 */
void error_print(void)
{
    if (g_last_error.code == 0) {
        printf("没有错误\n");
        return;
    }
    
    printf("========== 错误信息 ==========\n");
    printf("错误码: %d\n", g_last_error.code);
    printf("消息: %s\n", g_last_error.message ? g_last_error.message : "无");
    printf("位置: %s:%d\n", 
           g_last_error.file ? g_last_error.file : "未知",
           g_last_error.line);
    printf("函数: %s()\n", 
           g_last_error.function ? g_last_error.function : "未知");
    printf("==============================\n");
}

/**
 * @brief 获取错误消息字符串
 * @details 根据错误码返回对应的错误消息。
 * 
 * @param code 错误码
 * @return 错误消息字符串，未知错误码返回"未知错误"
 * 
 * @code
 * const char *msg = error_get_message(ERR_INVALID_PARAM);
 * printf("错误: %s\n", msg);
 * @endcode
 */
const char *error_get_message(int32_t code)
{
    switch (code) {
        case STATUS_OK:
            return "操作成功";
        case STATUS_ERROR:
            return "一般错误";
        case STATUS_INVALID_PARAM:
            return "无效参数";
        case STATUS_NULL_PTR:
            return "空指针";
        case STATUS_TIMEOUT:
            return "超时";
        case STATUS_BUSY:
            return "忙";
        case STATUS_NO_MEM:
            return "内存不足";
        case STATUS_OVERFLOW:
            return "溢出";
        case STATUS_UNDERFLOW:
            return "下溢";
        case STATUS_NOT_FOUND:
            return "未找到";
        case ERR_OUT_OF_RANGE:
            return "超出范围";
        case ERR_ALREADY_EXISTS:
            return "已存在";
        case ERR_IO_ERROR:
            return "I/O错误";
        case ERR_NOT_INITIALIZED:
            return "未初始化";
        case ERR_NOT_SUPPORTED:
            return "不支持";
        case ERR_PERMISSION_DENIED:
            return "权限拒绝";
        case ERR_DATA_CORRUPT:
            return "数据损坏";
        case ERR_DEVICE_ERROR:
            return "设备错误";
        case ERR_COMM_ERROR:
            return "通信错误";
        case ERR_PROTOCOL_ERROR:
            return "协议错误";
        case ERR_ABORTED:
            return "操作中止";
        case ERR_RESOURCE_EXHAUSTED:
            return "资源耗尽";
        default:
            return "未知错误";
    }
}

/*============================================================================*/
/*                           断言函数实现                                       */
/*============================================================================*/

/**
 * @brief 断言失败处理函数
 * @details 当断言失败时调用，打印错误信息并可能中止程序。
 *          在调试版本中输出详细信息，发布版本中可选择处理方式。
 * 
 * @param expr 断言表达式字符串
 * @param file 源文件名
 * @param line 行号
 * @param function 函数名
 * 
 * @note 在嵌入式系统中，可能需要重启或进入安全模式
 * @note 生产环境中应考虑使用更安全的处理方式
 * 
 * @code
 * // 通过宏调用
 * ASSERT(ptr != NULL);
 * @endcode
 */
void assert_failed(const char *expr, const char *file, int32_t line, 
                   const char *function)
{
    printf("\n!!! 断言失败 !!!\n");
    printf("表达式: %s\n", expr);
    printf("位置: %s:%d\n", file ? file : "未知", line);
    printf("函数: %s()\n", function ? function : "未知");
    
#ifdef DEBUG
    printf("\n程序将中止。请检查上述信息。\n");
#endif
}

/*============================================================================*/
/*                           看门狗函数实现                                     */
/*============================================================================*/

/**
 * @brief 初始化看门狗
 * @details 配置看门狗的超时时间并启用。
 *          看门狗用于监控系统运行状态，防止死锁。
 * 
 * @param timeout_ms 超时时间（毫秒）
 * 
 * @note 必须定期调用watchdog_feed喂狗
 * @note 超时后通常会触发系统复位
 * 
 * @code
 * watchdog_init(1000);  // 1秒超时
 * @endcode
 */
void watchdog_init(uint32_t timeout_ms)
{
    g_watchdog_timeout = timeout_ms;
    g_watchdog_counter = 0;
    g_watchdog_enabled = 1;
    LOG_INFO("看门狗已初始化，超时: %u ms", timeout_ms);
}

/**
 * @brief 喂狗
 * @details 重置看门狗计数器，防止超时。
 *          应在主循环或关键任务中定期调用。
 * 
 * @code
 * while (1) {
 *     // 执行任务
 *     watchdog_feed();
 * }
 * @endcode
 */
void watchdog_feed(void)
{
    g_watchdog_counter = 0;
}

/**
 * @brief 获取看门狗状态
 * @details 返回当前看门狗的运行状态。
 * 
 * @return 看门狗状态枚举值
 * 
 * @code
 * WatchdogState_t state = watchdog_get_state();
 * if (state == WATCHDOG_STATE_TIMEOUT) {
 *     // 处理超时
 * }
 * @endcode
 */
WatchdogState_t watchdog_get_state(void)
{
    if (!g_watchdog_enabled) {
        return WATCHDOG_STATE_IDLE;
    }
    if (g_watchdog_counter >= g_watchdog_timeout) {
        return WATCHDOG_STATE_TIMEOUT;
    }
    return WATCHDOG_STATE_RUNNING;
}

/**
 * @brief 模拟看门狗超时
 * @details 用于测试看门狗功能，强制触发超时状态。
 * 
 * @code
 * watchdog_simulate_timeout();
 * TEST_ASSERT_EQUAL(WATCHDOG_STATE_TIMEOUT, watchdog_get_state());
 * @endcode
 */
void watchdog_simulate_timeout(void)
{
    g_watchdog_counter = g_watchdog_timeout;
}

/**
 * @brief 更新看门狗
 * @details 在定时器中断中调用，递增计数器并检查超时。
 *          如果超时，触发看门狗复位。
 * 
 * @note 应在固定周期的定时器中断中调用
 * @note 超时后会调用watchdog_timeout_handler
 */
void watchdog_update(void)
{
    if (!g_watchdog_enabled) {
        return;
    }
    
    g_watchdog_counter++;
    
    if (g_watchdog_counter >= g_watchdog_timeout) {
        watchdog_timeout_handler();
    }
}

/**
 * @brief 看门狗超时处理
 * @details 当看门狗超时时调用，执行系统复位或其他恢复操作。
 * 
 * @note 在实际系统中应触发硬件复位
 * @note 可以在此记录错误日志或执行清理操作
 */
void watchdog_timeout_handler(void)
{
    LOG_ERROR("看门狗超时！系统即将复位...");
    
#ifdef DEBUG
    printf("看门狗超时！按任意键继续...\n");
#endif
    
    while (1) {
        // 在实际系统中，这里应该触发硬件复位
        // 或者跳转到复位向量
    }
}

/*============================================================================*/
/*                           调试辅助函数实现                                   */
/*============================================================================*/

/**
 * @brief 打印内存转储
 * @details 以十六进制格式打印内存内容，附带ASCII表示。
 *          常用于调试内存问题、协议分析等。
 * 
 * @param data 内存起始地址
 * @param size 要打印的字节数
 * 
 * @code
 * uint8_t buffer[64];
 * debug_dump_memory(buffer, sizeof(buffer));
 * @endcode
 */
void debug_dump_memory(const uint8_t *data, size_t size)
{
    if (data == NULL || size == 0) {
        return;
    }
    
    printf("\n========== 内存转储 ==========\n");
    printf("地址: %p, 大小: %zu 字节\n\n", (const void *)data, size);
    
    for (size_t i = 0; i < size; i += 16) {
        printf("%04zX: ", i);
        
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size) {
                printf("%02X ", data[i + j]);
            } else {
                printf("   ");
            }
        }
        
        printf(" |");
        for (size_t j = 0; j < 16 && i + j < size; j++) {
            uint8_t c = data[i + j];
            printf("%c", (c >= 32 && c < 127) ? (char)c : '.');
        }
        printf("|\n");
    }
    
    printf("==============================\n");
}

/**
 * @brief 打印时间戳
 * @details 输出当前时间戳，用于日志记录。
 * 
 * @code
 * debug_print_timestamp();
 * @endcode
 */
void debug_print_timestamp(void)
{
#ifdef _WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    printf("[%04d-%02d-%02d %02d:%02d:%02d.%03d] ",
           st.wYear, st.wMonth, st.wDay,
           st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("[%04d-%02d-%02d %02d:%02d:%02d] ",
           t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
           t->tm_hour, t->tm_min, t->tm_sec);
#endif
}

/*============================================================================*/
/*                           演示函数实现                                       */
/*============================================================================*/

/**
 * @brief 错误处理演示
 * @details 展示错误处理的基本用法：
 *          - 错误码返回
 *          - ERROR_CHECK宏
 *          - 错误信息获取
 *          - 错误恢复
 */
void demo_error_handling(void)
{
    print_separator("错误处理演示");

    printf("[错误码设计]\n");
    printf("  typedef enum {\n");
    printf("      STATUS_OK       = 0,\n");
    printf("      STATUS_ERROR    = -1,\n");
    printf("      STATUS_NULL_PTR = -2,\n");
    printf("      STATUS_TIMEOUT  = -3,\n");
    printf("      ...\n");
    printf("  } Status_t;\n");

    printf("\n[错误设置与获取]\n");
    ERROR_SET(STATUS_INVALID_PARAM, "测试错误");
    
    ErrorInfo_t err = error_get();
    printf("  错误码: %d\n", err.code);
    printf("  错误信息: %s\n", err.message);
    printf("  文件: %s\n", err.file);
    printf("  行号: %d\n", err.line);
    printf("  函数: %s\n", err.function);
    
    error_clear();

    printf("\n[错误处理模式]\n");
    printf("  1. 返回值检查\n");
    printf("     Status_t result = do_something();\n");
    printf("     if (result != STATUS_OK) { 处理错误; }\n");
    printf("\n");
    printf("  2. 错误码输出参数\n");
    printf("     int32_t value = get_value(&error_code);\n");
    printf("\n");
    printf("  3. 全局错误变量(类似errno)\n");
    printf("     set_error(code, message);\n");
    printf("     ErrorInfo_t err = get_error();\n");

    printf("\n[嵌入式错误处理原则]\n");
    printf("  1. 每个可能失败的操作都要检查\n");
    printf("  2. 错误要向上传递\n");
    printf("  3. 资源要正确释放\n");
    printf("  4. 提供有意义的错误信息\n");
}

void demo_assert_usage(void)
{
    print_separator("断言使用演示");

    printf("[assert宏]\n");
    printf("  #include <assert.h>\n");
    printf("  assert(expression);\n");
    printf("  表达式为假时终止程序\n");

    printf("\n[自定义断言]\n");
    printf("  #define DEBUG_ASSERT(cond) \\\n");
    printf("      do { \\\n");
    printf("          if (!(cond)) { \\\n");
    printf("              printf(\"Assertion failed: %s\\n\", #cond); \\\n");
    printf("              while(1); \\\n");
    printf("          } \\\n");
    printf("      } while(0)\n");

    printf("\n[静态断言]\n");
    printf("  _Static_assert(sizeof(int) == 4, \"int must be 4 bytes\");\n");
    printf("  编译时检查, 不消耗运行时资源\n");

    printf("\n[断言使用场景]\n");
    printf("  适合:\n");
    printf("    - 检查前置条件(参数有效性)\n");
    printf("    - 检查后置条件(结果有效性)\n");
    printf("    - 检查不变量(数据结构完整性)\n");
    printf("  不适合:\n");
    printf("    - 处理用户输入错误\n");
    printf("    - 处理硬件故障\n");
    printf("    - 处理可预期的运行时错误\n");

    printf("\n[发布版本处理]\n");
    printf("  #ifdef DEBUG\n");
    printf("      #define ASSERT(cond) assert(cond)\n");
    printf("  #else\n");
    printf("      #define ASSERT(cond) ((void)0)\n");
    printf("  #endif\n");

    printf("\n[断言示例]\n");
    int32_t *ptr = (int32_t *)malloc(sizeof(int32_t));
    DEBUG_ASSERT(ptr != NULL);
    if (ptr != NULL) {
        *ptr = 42;
        printf("  ptr = %d\n", *ptr);
        free(ptr);
    }
}

void demo_debug_techniques(void)
{
    print_separator("调试技术演示");

    printf("[打印调试]\n");
    printf("  printf(\"变量值: %d\\n\", value);\n");
    printf("  优点: 简单直接\n");
    printf("  缺点: 影响性能, 可能改变时序\n");

    printf("\n[日志系统]\n");
    printf("  LOG_DEBUG(\"调试信息\");\n");
    printf("  LOG_INFO(\"普通信息\");\n");
    printf("  LOG_WARNING(\"警告信息\");\n");
    printf("  LOG_ERROR(\"错误信息\");\n");
    printf("  可按级别控制输出\n");

    printf("\n[条件编译调试]\n");
    printf("  #ifdef DEBUG\n");
    printf("      debug_print(\"调试信息\");\n");
    printf("  #endif\n");
    printf("  发布版本不包含调试代码\n");

    printf("\n[调试宏示例]\n");
    printf("  #define DUMP_VAR(var) \\\n");
    printf("      printf(#var \" = %%d\\n\", var)\n");
    printf("  DUMP_VAR(counter);  // 输出: counter = 10\n");

    printf("\n[性能测量]\n");
    printf("  uint32_t start = get_tick_count();\n");
    printf("  do_something();\n");
    printf("  uint32_t elapsed = get_tick_count() - start;\n");
    printf("  printf(\"耗时: %%u ms\\n\", elapsed);\n");

    printf("\n[内存调试]\n");
    printf("  - 使用内存池跟踪分配\n");
    printf("  - 添加魔术字检测越界\n");
    printf("  - 使用工具: Valgrind, AddressSanitizer\n");

    printf("\n[硬件调试]\n");
    printf("  - 示波器/逻辑分析仪\n");
    printf("  - JTAG/SWD调试器\n");
    printf("  - LED状态指示\n");
    printf("  - GPIO翻转测量时序\n");
}

void demo_logging_system(void)
{
    print_separator("日志系统演示");

    printf("[日志级别]\n");
    LOG_DEBUG("这是调试日志");
    LOG_INFO("这是信息日志");
    LOG_WARNING("这是警告日志");
    LOG_ERROR("这是错误日志");

    printf("\n[日志格式]\n");
    printf("  [时间] [级别] 消息内容\n");
    printf("  [14:30:25] [INFO] 系统启动\n");

    printf("\n[日志配置]\n");
    printf("  - 设置日志级别: log_set_level(LOG_LEVEL_INFO);\n");
    printf("  - 设置输出目标: 文件、串口、网络\n");
    printf("  - 设置格式: 时间戳、文件名、行号\n");

    printf("\n[嵌入式日志考虑]\n");
    printf("  1. RAM缓冲: 减少频繁I/O\n");
    printf("  2. 环形缓冲: 固定大小, 覆盖旧日志\n");
    printf("  3. 异步输出: 不阻塞主程序\n");
    printf("  4. 压缩存储: 节省Flash空间\n");

    printf("\n[日志示例]\n");
    log_set_level(LOG_LEVEL_INFO);
    LOG_DEBUG("这条不会输出(级别低于INFO)");
    LOG_INFO("这条会输出");
    log_set_level(LOG_LEVEL_DEBUG);

    printf("\n[性能优化]\n");
    printf("  #define LOG_DEBUG(fmt, ...) \\\n");
    printf("      do { \\\n");
    printf("          if (g_log_level <= LOG_LEVEL_DEBUG) { \\\n");
    printf("              printf(fmt, ##__VA_ARGS__); \\\n");
    printf("          } \\\n");
    printf("      } while(0)\n");
    printf("  级别检查在编译时或运行时进行\n");
}

void demo_defensive_programming(void)
{
    print_separator("防御性编程演示");

    printf("[输入验证]\n");
    printf("  Status_t process_data(const uint8_t *data, size_t len) {\n");
    printf("      if (data == NULL) return STATUS_NULL_PTR;\n");
    printf("      if (len == 0 || len > MAX_LEN) return STATUS_INVALID_PARAM;\n");
    printf("      // 处理数据\n");
    printf("      return STATUS_OK;\n");
    printf("  }\n");

    printf("\n[空指针检查]\n");
    printf("  #define RETURN_IF_NULL(ptr) \\\n");
    printf("      do { if ((ptr) == NULL) return STATUS_NULL_PTR; } while(0)\n");

    printf("\n[数组边界检查]\n");
    printf("  int32_t get_item(int32_t *arr, size_t size, size_t index) {\n");
    printf("      if (index >= size) {\n");
    printf("          LOG_ERROR(\"数组越界: %%zu >= %%zu\", index, size);\n");
    printf("          return DEFAULT_VALUE;\n");
    printf("      }\n");
    printf("      return arr[index];\n");
    printf("  }\n");

    printf("\n[资源管理]\n");
    printf("  void *ptr = malloc(size);\n");
    printf("  if (ptr == NULL) { return STATUS_NO_MEM; }\n");
    printf("  \n");
    printf("  Status_t result = do_work(ptr);\n");
    printf("  \n");
    printf("  free(ptr);  // 确保释放\n");
    printf("  return result;\n");

    printf("\n[魔术字验证]\n");
    printf("  typedef struct {\n");
    printf("      uint32_t magic;  // 初始化时设为0xDEADBEEF\n");
    printf("      // 其他成员\n");
    printf("  } MyStruct_t;\n");
    printf("  \n");
    printf("  bool is_valid(const MyStruct_t *s) {\n");
    printf("      return s != NULL && s->magic == 0xDEADBEEF;\n");
    printf("  }\n");

    printf("\n[防御性编程原则]\n");
    printf("  1. 不信任任何输入\n");
    printf("  2. 假设外部调用者会犯错\n");
    printf("  3. 尽早失败, 明确报错\n");
    printf("  4. 资源获取即初始化(RAII)\n");
}

void demo_error_recovery(void)
{
    print_separator("错误恢复演示");

    printf("[重试机制]\n");
    printf("  #define MAX_RETRIES 3\n");
    printf("  \n");
    printf("  Status_t send_with_retry(const uint8_t *data, size_t len) {\n");
    printf("      for (int i = 0; i < MAX_RETRIES; i++) {\n");
    printf("          Status_t result = send_data(data, len);\n");
    printf("          if (result == STATUS_OK) return STATUS_OK;\n");
    printf("          delay_ms(100);  // 等待后重试\n");
    printf("      }\n");
    printf("      return STATUS_ERROR;\n");
    printf("  }\n");

    printf("\n[超时机制]\n");
    printf("  Status_t wait_for_event(uint32_t timeout_ms) {\n");
    printf("      uint32_t start = get_tick_count();\n");
    printf("      while (!event_occurred()) {\n");
    printf("          if (get_tick_count() - start > timeout_ms) {\n");
    printf("              return STATUS_TIMEOUT;\n");
    printf("          }\n");
    printf("      }\n");
    printf("      return STATUS_OK;\n");
    printf("  }\n");

    printf("\n[故障安全设计]\n");
    printf("  - 默认安全状态\n");
    printf("  - 看门狗复位\n");
    printf("  - 冗余检查\n");
    printf("  - 降级运行\n");

    printf("\n[状态恢复]\n");
    printf("  void recover_from_error(void) {\n");
    printf("      // 1. 保存当前状态\n");
    printf("      // 2. 重置外设\n");
    printf("      // 3. 恢复默认配置\n");
    printf("      // 4. 重新初始化\n");
    printf("  }\n");

    printf("\n[错误处理示例]\n");
    printf("  Status_t init_system(void) {\n");
    printf("      TRY(init_clock());\n");
    printf("      TRY(init_gpio());\n");
    printf("      TRY(init_uart());\n");
    printf("      TRY(init_sensor());\n");
    printf("      return STATUS_OK;\n");
    printf("  }\n");
}

void demo_watchdog(void)
{
    print_separator("看门狗演示");

    printf("[看门狗原理]\n");
    printf("  硬件定时器, 需要定期喂狗\n");
    printf("  超时未喂狗则复位系统\n");
    printf("  防止程序跑飞或死锁\n");

    printf("\n[看门狗配置]\n");
    printf("  void wdt_init(uint32_t timeout_ms) {\n");
    printf("      // 设置超时时间\n");
    printf("      // 启动看门狗\n");
    printf("  }\n");

    printf("\n[喂狗操作]\n");
    printf("  void wdt_feed(void) {\n");
    printf("      // 重置看门狗计数器\n");
    printf("  }\n");

    printf("\n[使用示例]\n");
    printf("  int main(void) {\n");
    printf("      wdt_init(1000);  // 1秒超时\n");
    printf("      \n");
    printf("      while (1) {\n");
    printf("          wdt_feed();\n");
    printf("          do_task();\n");
    printf("          delay_ms(100);\n");
    printf("      }\n");
    printf("  }\n");

    printf("\n[模拟看门狗]\n");
    watchdog_init(1000);
    printf("  看门狗已初始化, 超时: 1000ms\n");
    printf("  当前状态: %d\n", watchdog_get_state());
    watchdog_feed();
    printf("  喂狗后状态: %d\n", watchdog_get_state());

    printf("\n[看门狗注意事项]\n");
    printf("  1. 喂狗间隔必须小于超时时间\n");
    printf("  2. 避免在中断中喂狗\n");
    printf("  3. 调试时可能需要禁用看门狗\n");
    printf("  4. 考虑窗口看门狗\n");
}

void demo_stack_trace(void)
{
    print_separator("调用栈跟踪演示");

    printf("[函数调用记录]\n");
    printf("  嵌入式系统通常无法获取完整调用栈\n");
    printf("  可以手动记录函数调用\n");

    printf("\n[简单调用跟踪]\n");
    printf("  #define TRACE_ENTER() \\\n");
    printf("      printf(\"-> %s\\n\", __func__)\n");
    printf("  \n");
    printf("  #define TRACE_EXIT() \\\n");
    printf("      printf(\"<- %s\\n\", __func__)\n");

    printf("\n[调用栈缓冲]\n");
    printf("  #define CALL_STACK_SIZE 16\n");
    printf("  \n");
    printf("  static const char *s_call_stack[CALL_STACK_SIZE];\n");
    printf("  static int s_call_stack_top = 0;\n");
    printf("  \n");
    printf("  void trace_push(const char *func) {\n");
    printf("      if (s_call_stack_top < CALL_STACK_SIZE) {\n");
    printf("          s_call_stack[s_call_stack_top++] = func;\n");
    printf("      }\n");
    printf("  }\n");
    printf("  \n");
    printf("  void trace_pop(void) {\n");
    printf("      if (s_call_stack_top > 0) {\n");
    printf("          s_call_stack_top--;\n");
    printf("      }\n");
    printf("  }\n");

    printf("\n[栈溢出检测]\n");
    printf("  方法1: 栈尾填充魔术字, 定期检查\n");
    printf("  方法2: 使用MPU保护栈区域\n");
    printf("  方法3: 编译器栈保护选项(-fstack-protector)\n");

    printf("\n[栈使用分析]\n");
    printf("  - 静态分析: 编译器选项 -fstack-usage\n");
    printf("  - 运行时: 栈填充模式检测\n");
    printf("  - 工具: Linker map文件分析\n");

    printf("\n[嵌入式栈管理]\n");
    printf("  - 合理设置栈大小\n");
    printf("  - 避免深度递归\n");
    printf("  - 避免大数组局部变量\n");
    printf("  - 使用静态或堆分配大数据\n");
}
