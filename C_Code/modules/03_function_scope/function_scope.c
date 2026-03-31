/**
 * @file function_scope.c
 * @brief 函数与作用域模块实现
 * @details 演示C语言函数与作用域相关知识点。
 *          本模块涵盖以下核心知识点：
 *          - 函数声明与定义
 *          - 参数传递：值传递、指针传递
 *          - 返回值：基本类型、指针、多返回值
 *          - 递归函数：阶乘、斐波那契、二分查找
 *          - 内联函数：inline关键字
 *          - 函数指针：声明、使用、数组
 *          - 回调函数：事件处理、定时器
 *          - 变量生命周期：自动、静态、动态
 *          - 作用域规则：块、文件、函数原型
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 函数是模块化设计的基础
 * - 回调函数是事件驱动编程的核心
 * - 函数指针用于驱动表和状态机
 * - 避免递归防止栈溢出
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 * @note 嵌入式环境中应避免深度递归
 */

#include "function_scope.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <stdlib.h>

/*============================================================================*/
/*                           文件内部变量                                       */
/*============================================================================*/

/** @brief 静态全局变量（仅本文件可见） */
static int32_t s_static_global = 100;

/*============================================================================*/
/*                           基本运算函数                                       */
/*============================================================================*/

/**
 * @brief 加法运算
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return 两数之和
 */
int32_t add(int32_t a, int32_t b)
{
    return a + b;
}

/**
 * @brief 减法运算
 * @param a 被减数
 * @param b 减数
 * @return 差值
 */
int32_t subtract(int32_t a, int32_t b)
{
    return a - b;
}

/**
 * @brief 乘法运算
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return 乘积
 */
int32_t multiply(int32_t a, int32_t b)
{
    return a * b;
}

/**
 * @brief 除法运算
 * @param a 被除数
 * @param b 除数
 * @return 商（除数为0时返回0）
 * @note 包含除零检查
 */
int32_t divide(int32_t a, int32_t b)
{
    if (b == 0) {
        LOG_ERROR("除零错误");
        return 0;
    }
    return a / b;
}

/*============================================================================*/
/*                           参数传递演示函数                                   */
/*============================================================================*/

/**
 * @brief 值传递交换（演示值传递的局限性）
 * @param a 第一个值
 * @param b 第二个值
 * @note 此函数无法真正交换调用者的变量
 */
void swap_by_value(int32_t a, int32_t b)
{
    int32_t temp = a;
    a = b;
    b = temp;
}

/**
 * @brief 指针传递交换
 * @param a 第一个变量的指针
 * @param b 第二个变量的指针
 * @note 通过指针可以修改调用者的变量
 */
void swap_by_pointer(int32_t *a, int32_t *b)
{
    if (a == NULL || b == NULL) {
        return;
    }
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

/*============================================================================*/
/*                           递归函数示例                                       */
/*============================================================================*/

/**
 * @brief 计算阶乘（递归实现）
 * @param n 非负整数
 * @return n的阶乘
 * @note 递归深度受栈大小限制，嵌入式环境慎用
 */
int32_t factorial(int32_t n)
{
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

/**
 * @brief 计算斐波那契数列（递归实现）
 * @param n 位置索引（从0开始）
 * @return 第n个斐波那契数
 * @note 此实现效率低，存在大量重复计算
 */
int32_t fibonacci(int32_t n)
{
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

/**
 * @brief 二分查找（迭代实现）
 * @param arr 已排序的数组
 * @param size 数组大小
 * @param target 查找目标
 * @return 目标索引，未找到返回-1
 * @note 时间复杂度O(log n)
 */
int32_t binary_search(const int32_t *arr, size_t size, int32_t target)
{
    if (arr == NULL || size == 0) {
        return -1;
    }

    int32_t left = 0;
    int32_t right = (int32_t)size - 1;

    while (left <= right) {
        int32_t mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

/*============================================================================*/
/*                           演示函数实现                                       */
/*============================================================================*/

/**
 * @brief 函数基础演示
 * @details 展示函数的声明、定义、调用等基础知识
 */
void demo_function_basics(void)
{
    print_separator("函数基础演示");

    printf("[函数声明与定义]\n");
    printf("  声明: int32_t add(int32_t a, int32_t b);  // 告诉编译器函数存在\n");
    printf("  定义: int32_t add(int32_t a, int32_t b) { return a + b; }  // 实际代码\n");

    printf("\n[函数调用]\n");
    int32_t result = add(10, 20);
    printf("  add(10, 20) = %d\n", result);

    printf("\n[函数原型的重要性]\n");
    printf("  - 确保参数类型正确\n");
    printf("  - 确保返回值类型正确\n");
    printf("  - 允许函数在声明前被调用\n");

    printf("\n[嵌入式函数设计原则]\n");
    printf("  1. 函数功能单一(单一职责原则)\n");
    printf("  2. 参数数量适中(建议不超过4个)\n");
    printf("  3. 函数长度适中(建议不超过50行)\n");
    printf("  4. 避免递归(防止栈溢出)\n");
}

/**
 * @brief 函数参数传递演示
 * @details 展示值传递和指针传递的区别
 */
void demo_function_parameters(void)
{
    print_separator("函数参数传递演示");

    printf("[值传递]\n");
    int32_t x = 10, y = 20;
    printf("  调用前: x = %d, y = %d\n", x, y);
    swap_by_value(x, y);
    printf("  swap_by_value后: x = %d, y = %d (未改变)\n", x, y);

    printf("\n[指针传递]\n");
    x = 10; y = 20;
    printf("  调用前: x = %d, y = %d\n", x, y);
    swap_by_pointer(&x, &y);
    printf("  swap_by_pointer后: x = %d, y = %d (已交换)\n", x, y);

    printf("\n[const参数]\n");
    printf("  int32_t process(const uint8_t *data, size_t len);\n");
    printf("  const确保函数不会修改传入的数据\n");

    printf("\n[数组参数]\n");
    printf("  void process_array(int32_t arr[], size_t size);\n");
    printf("  数组作为参数时退化为指针, 需要额外传递大小\n");

    printf("\n[可变参数函数]\n");
    printf("  int printf(const char *format, ...);\n");
    printf("  使用stdarg.h中的va_list、va_start、va_arg、va_end\n");
}

/**
 * @brief 函数返回值演示
 * @details 展示不同类型的返回值处理
 */
void demo_function_return(void)
{
    print_separator("函数返回值演示");

    printf("[返回基本类型]\n");
    printf("  add(5, 3) = %d\n", add(5, 3));
    printf("  divide(10, 3) = %d\n", divide(10, 3));

    printf("\n[返回指针]\n");
    printf("  注意: 不要返回局部变量的指针!\n");
    printf("  正确做法:\n");
    printf("    - 返回静态变量指针\n");
    printf("    - 返回动态分配内存指针\n");
    printf("    - 返回传入参数指针\n");

    printf("\n[多返回值模拟]\n");
    printf("  方法1: 通过指针参数返回多个值\n");
    printf("  方法2: 返回结构体\n");
    printf("  方法3: 返回位域编码值\n");

    printf("\n[错误码返回模式]\n");
    printf("  Status_t read_sensor(uint8_t *data, size_t *len);\n");
    printf("  返回状态码, 数据通过参数返回\n");
}

/**
 * @brief 递归函数演示
 * @details 展示递归函数的使用和风险
 */
void demo_recursion(void)
{
    print_separator("递归函数演示");

    printf("[阶乘计算]\n");
    for (int32_t i = 0; i <= 10; i++) {
        printf("  %d! = %d\n", i, factorial(i));
    }

    printf("\n[斐波那契数列]\n");
    printf("  前15项: ");
    for (int32_t i = 0; i < 15; i++) {
        printf("%d ", fibonacci(i));
    }
    printf("\n");

    printf("\n[二分查找(递归思想)]\n");
    int32_t arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int32_t target = 11;
    int32_t idx = binary_search(arr, ARRAY_SIZE(arr), target);
    printf("  查找 %d: 索引 = %d\n", target, idx);

    printf("\n[递归风险]\n");
    printf("  1. 栈溢出: 深度递归消耗大量栈空间\n");
    printf("  2. 效率低: 重复计算(如斐波那契)\n");
    printf("  3. 嵌入式慎用: 资源受限环境\n");

    printf("\n[递归优化]\n");
    printf("  1. 尾递归优化(编译器支持)\n");
    printf("  2. 记忆化(缓存中间结果)\n");
    printf("  3. 转换为迭代\n");
}

/**
 * @brief 内联函数演示
 * @details 展示inline关键字的使用
 * @param x 输入值
 * @return x的平方
 */
static inline int32_t square_inline(int32_t x)
{
    return x * x;
}

/**
 * @brief 内联函数演示
 * @details 展示内联函数的定义和使用
 */
void demo_inline_function(void)
{
    print_separator("内联函数演示");

    printf("[内联函数定义]\n");
    printf("  static inline int32_t square_inline(int32_t x) {\n");
    printf("      return x * x;\n");
    printf("  }\n");

    printf("\n[内联函数调用]\n");
    for (int32_t i = 1; i <= 5; i++) {
        printf("  square_inline(%d) = %d\n", i, square_inline(i));
    }

    printf("\n[内联函数特点]\n");
    printf("  - 编译时展开, 无函数调用开销\n");
    printf("  - 适合短小、频繁调用的函数\n");
    printf("  - 可能增加代码大小\n");

    printf("\n[内联 vs 宏定义]\n");
    printf("  宏: #define SQUARE(x) ((x) * (x))\n");
    printf("  内联: 有类型检查, 可调试, 无副作用\n");
    printf("  示例: SQUARE(i++) 会执行两次i++, inline不会\n");

    printf("\n[嵌入式应用]\n");
    printf("  - GPIO操作函数\n");
    printf("  - 简单计算函数\n");
    printf("  - 性能关键路径\n");
}

/**
 * @brief 函数指针演示
 * @details 展示函数指针的声明和使用
 */
void demo_function_pointer(void)
{
    print_separator("函数指针演示");

    printf("[函数指针声明]\n");
    printf("  int32_t (*operation)(int32_t, int32_t);\n");
    printf("  operation = add;  // 指向add函数\n");

    printf("\n[函数指针数组]\n");
    int32_t (*operations[])(int32_t, int32_t) = {add, subtract, multiply, divide};
    const char *op_names[] = {"+", "-", "*", "/"};

    int32_t a = 20, b = 5;
    for (int32_t i = 0; i < 4; i++) {
        printf("  %d %s %d = %d\n", a, op_names[i], b, operations[i](a, b));
    }

    printf("\n[typedef简化]\n");
    printf("  typedef int32_t (*MathOp_t)(int32_t, int32_t);\n");
    printf("  MathOp_t op = add;  // 更清晰\n");

    printf("\n[回调函数应用]\n");
    printf("  void register_callback(Callback_t cb);\n");
    printf("  事件驱动编程的核心机制\n");

    printf("\n[嵌入式应用]\n");
    printf("  - 驱动函数表\n");
    printf("  - 命令处理器\n");
    printf("  - 状态机转换表\n");
}

/*============================================================================*/
/*                           回调函数示例                                       */
/*============================================================================*/

/**
 * @brief 事件处理回调函数
 * @param param 回调参数
 */
static void event_handler(void *param)
{
    int32_t *value = (int32_t *)param;
    printf("  事件触发, 参数值: %d\n", *value);
}

/**
 * @brief 定时器回调函数
 * @param param 回调参数（未使用）
 */
static void timer_callback(void *param)
{
    UNUSED_PARAM(param);
    printf("  定时器到期回调\n");
}

/**
 * @brief 数据处理函数（带回调）
 * @param data 数据指针
 * @param len 数据长度
 * @param callback 处理完成回调
 */
static void process_data(const uint8_t *data, size_t len, void (*callback)(const uint8_t *, size_t))
{
    if (callback != NULL) {
        callback(data, len);
    }
}

/**
 * @brief 数据回调函数
 * @param data 数据指针
 * @param len 数据长度
 */
static void data_callback(const uint8_t *data, size_t len)
{
    printf("  处理数据: %zu 字节, 首字节: 0x%02X\n", len, data[0]);
}

/**
 * @brief 回调函数演示
 * @details 展示回调函数的设计模式
 */
void demo_callback_function(void)
{
    print_separator("回调函数演示");

    printf("[基本回调模式]\n");
    int32_t event_param = 42;
    event_handler(&event_param);

    printf("\n[定时器回调模拟]\n");
    timer_callback(NULL);

    printf("\n[数据处理回调]\n");
    uint8_t test_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    process_data(test_data, ARRAY_SIZE(test_data), data_callback);

    printf("\n[回调函数设计模式]\n");
    printf("  1. 定义回调函数类型: typedef void (*Callback_t)(void *param);\n");
    printf("  2. 注册回调: void register_callback(Callback_t cb);\n");
    printf("  3. 触发回调: if (callback != NULL) callback(param);\n");

    printf("\n[嵌入式回调应用]\n");
    printf("  - 中断处理完成通知\n");
    printf("  - DMA传输完成通知\n");
    printf("  - 通信协议解析完成\n");
    printf("  - 按键事件处理\n");
}

/*============================================================================*/
/*                           变量生命周期演示                                   */
/*============================================================================*/

/** @brief 静态局部变量计数器 */
static int32_t s_call_count = 0;

/**
 * @brief 递增计数器函数
 * @details 演示静态局部变量的生命周期
 */
static void increment_counter(void)
{
    s_call_count++;
}

/**
 * @brief 变量生命周期演示
 * @details 展示不同存储期变量的特点
 */
void demo_variable_lifetime(void)
{
    print_separator("变量生命周期演示");

    printf("[自动变量(栈上)]\n");
    int32_t auto_var = 10;
    printf("  auto_var = %d (函数返回后销毁)\n", auto_var);

    printf("\n[静态局部变量]\n");
    static int32_t static_local = 0;
    static_local++;
    printf("  static_local = %d (程序运行期间保持)\n", static_local);

    printf("\n[静态全局变量]\n");
    printf("  s_static_global = %d (本文件可见)\n", s_static_global);

    printf("\n[生命周期演示]\n");
    for (int32_t i = 0; i < 5; i++) {
        increment_counter();
    }
    printf("  调用increment_counter 5次, s_call_count = %d\n", s_call_count);

    printf("\n[动态分配变量]\n");
    int32_t *heap_var = (int32_t *)malloc(sizeof(int32_t));
    if (heap_var != NULL) {
        *heap_var = 100;
        printf("  heap_var = %d (手动管理生命周期)\n", *heap_var);
        free(heap_var);
        heap_var = NULL;
    }

    printf("\n[生命周期总结]\n");
    printf("  自动变量: 块作用域开始创建, 结束销毁\n");
    printf("  静态变量: 程序启动创建, 结束销毁\n");
    printf("  动态变量: malloc创建, free销毁\n");
}

/**
 * @brief 作用域规则演示
 * @details 展示不同作用域的特点
 */
void demo_scope_rules(void)
{
    print_separator("作用域规则演示");

    printf("[块作用域]\n");
    {
        int32_t block_var = 100;
        printf("  块内变量: block_var = %d\n", block_var);
    }
    printf("  块外: block_var不可访问\n");

    printf("\n[文件作用域]\n");
    printf("  全局变量: 整个文件可见\n");
    printf("  静态全局变量: 仅本文件可见\n");

    printf("\n[函数原型作用域]\n");
    printf("  函数声明中的参数名仅在该声明中有效\n");

    printf("\n[变量遮蔽]\n");
    int32_t shadow = 1;
    printf("  外层 shadow = %d\n", shadow);
    {
        int32_t shadow = 2;
        printf("  内层 shadow = %d (遮蔽外层)\n", shadow);
    }
    printf("  外层 shadow = %d (恢复)\n", shadow);

    printf("\n[作用域最佳实践]\n");
    printf("  1. 变量在最小作用域内声明\n");
    printf("  2. 避免全局变量\n");
    printf("  3. 使用static限制全局变量作用域\n");
    printf("  4. 避免变量遮蔽\n");
}
