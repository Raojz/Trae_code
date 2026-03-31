/**
 * @file pointer_memory.c
 * @brief 指针与内存管理模块实现
 * @details 演示C语言指针与内存管理相关知识点，涵盖以下核心内容：
 *          - 指针基础：声明、初始化、解引用、空指针
 *          - 指针运算：加减、比较、差值
 *          - 指针与数组：数组名与指针、指针遍历数组
 *          - 指针与字符串：字符串指针、字符串操作
 *          - 函数指针：函数指针声明、回调函数、函数指针数组
 *          - 动态内存：malloc/calloc/realloc/free的使用和注意事项
 *          - 内存对齐：对齐概念、对齐检查、结构体对齐
 *          - 内存池：静态内存池的实现和应用
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
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "pointer_memory.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*============================================================================*/
/*                           私有函数定义                                       */
/*============================================================================*/

/**
 * @brief 加法运算（内部使用）
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return 两数之和
 */
static int32_t op_add(int32_t a, int32_t b) { return a + b; }

/**
 * @brief 减法运算（内部使用）
 * @param a 被减数
 * @param b 减数
 * @return 差值
 */
static int32_t op_sub(int32_t a, int32_t b) { return a - b; }

/**
 * @brief 乘法运算（内部使用）
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return 乘积
 */
static int32_t op_mul(int32_t a, int32_t b) { return a * b; }

/**
 * @brief 除法运算（内部使用）
 * @param a 被除数
 * @param b 除数
 * @return 商
 */
static int32_t op_div(int32_t a, int32_t b) { return a / b; }

/*============================================================================*/
/*                           安全内存函数实现                                   */
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
 * @note 如果size为0，返回NULL
 * 
 * @code
 * void *ptr = safe_malloc(100);
 * if (ptr != NULL) {
 *     // 使用内存
 *     safe_free(&ptr);
 * }
 * @endcode
 */
void *safe_malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }
    
    void *ptr = malloc(size);
    if (ptr == NULL) {
        LOG_ERROR("内存分配失败: %zu 字节", size);
    }
    return ptr;
}

/**
 * @brief 安全的内存分配函数（清零）
 * @details 封装calloc，分配内存并初始化为零。
 * 
 * @param count 元素个数
 * @param size 每个元素的大小
 * @return 分配成功返回内存指针，失败返回NULL
 * 
 * @note 分配的内存自动初始化为0
 * @note 分配的内存需要调用safe_free释放
 * 
 * @code
 * int32_t *arr = safe_calloc(10, sizeof(int32_t));
 * // arr[0]到arr[9]都是0
 * @endcode
 */
void *safe_calloc(size_t count, size_t size)
{
    if (count == 0 || size == 0) {
        return NULL;
    }
    
    void *ptr = calloc(count, size);
    if (ptr == NULL) {
        LOG_ERROR("内存分配失败: %zu x %zu", count, size);
    }
    return ptr;
}

/**
 * @brief 安全的内存重分配函数
 * @details 封装realloc，调整已分配内存的大小。
 *          如果新大小大于原大小，新增部分内容不确定。
 * 
 * @param ptr 原内存指针（可为NULL，此时等同于malloc）
 * @param size 新的大小（字节）
 * @return 重分配成功返回新指针，失败返回NULL（原内存不变）
 * 
 * @warning 返回的指针可能与原指针不同
 * @note 如果size为0，等同于free并返回NULL
 * 
 * @code
 * int32_t *arr = safe_malloc(10 * sizeof(int32_t));
 * arr = safe_realloc(arr, 20 * sizeof(int32_t));  // 扩展到20个元素
 * @endcode
 */
void *safe_realloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL && size > 0) {
        LOG_ERROR("内存重新分配失败: %zu 字节", size);
    }
    return new_ptr;
}

/**
 * @brief 安全的内存释放函数
 * @details 封装free，释放后将指针置NULL，防止悬空指针。
 *          这是防止内存泄漏和悬空指针的重要手段。
 * 
 * @param ptr 指向指针的指针
 * 
 * @note 释放后自动将指针置为NULL
 * @note 如果ptr或*ptr为NULL，不执行任何操作
 * 
 * @code
 * int *p = malloc(sizeof(int));
 * safe_free(&p);  // 释放内存并将p置为NULL
 * // 此时 p == NULL
 * @endcode
 */
void safe_free(void **ptr)
{
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}

/*============================================================================*/
/*                           数组操作函数实现                                   */
/*============================================================================*/

/**
 * @brief 创建动态数组
 * @details 动态分配指定大小的整型数组，并初始化为0。
 * 
 * @param size 数组元素个数
 * @return 成功返回数组指针，失败返回NULL
 * 
 * @note 使用destroy_array释放内存
 * @note 数组元素初始化为0
 * 
 * @code
 * int32_t *arr = create_array(100);
 * if (arr != NULL) {
 *     // 使用数组
 *     destroy_array(&arr);
 * }
 * @endcode
 */
int32_t *create_array(size_t size)
{
    if (size == 0) {
        return NULL;
    }
    return (int32_t *)safe_calloc(size, sizeof(int32_t));
}

/**
 * @brief 销毁动态数组
 * @details 释放动态数组内存并将指针置NULL。
 * 
 * @param arr 指向数组指针的指针
 * 
 * @code
 * int32_t *arr = create_array(100);
 * destroy_array(&arr);  // arr 变为 NULL
 * @endcode
 */
void destroy_array(int32_t **arr)
{
    safe_free((void **)arr);
}

/**
 * @brief 填充数组
 * @details 将数组的所有元素设置为指定值。
 * 
 * @param arr 数组指针
 * @param size 数组大小
 * @param value 填充值
 * 
 * @code
 * int32_t arr[10];
 * fill_array(arr, 10, 42);  // 所有元素设为42
 * @endcode
 */
void fill_array(int32_t *arr, size_t size, int32_t value)
{
    if (arr == NULL || size == 0) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        arr[i] = value;
    }
}

/**
 * @brief 查找数组元素
 * @details 在数组中查找指定值的元素，返回其指针。
 * 
 * @param arr 数组指针
 * @param size 数组大小
 * @param target 目标值
 * @return 找到返回元素指针，未找到返回NULL
 * 
 * @note 如果有多个相同值，返回第一个找到的
 * 
 * @code
 * int32_t arr[] = {1, 2, 3, 4, 5};
 * int32_t *found = find_element(arr, 5, 3);
 * if (found != NULL) {
 *     printf("找到: %d\n", *found);
 * }
 * @endcode
 */
int32_t *find_element(int32_t *arr, size_t size, int32_t target)
{
    if (arr == NULL || size == 0) {
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == target) {
            return &arr[i];
        }
    }
    return NULL;
}

/**
 * @brief 交换两个整数
 * @details 通过指针交换两个整数的值。
 * 
 * @param a 第一个整数的指针
 * @param b 第二个整数的指针
 * 
 * @code
 * int32_t x = 10, y = 20;
 * swap_int(&x, &y);
 * // x = 20, y = 10
 * @endcode
 */
void swap_int(int32_t *a, int32_t *b)
{
    if (a == NULL || b == NULL) {
        return;
    }
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief 反转数组
 * @details 原地反转数组元素的顺序。
 * 
 * @param arr 数组指针
 * @param size 数组大小
 * 
 * @code
 * int32_t arr[] = {1, 2, 3, 4, 5};
 * reverse_array(arr, 5);
 * // arr = {5, 4, 3, 2, 1}
 * @endcode
 */
void reverse_array(int32_t *arr, size_t size)
{
    if (arr == NULL || size < 2) {
        return;
    }
    int32_t *start = arr;
    int32_t *end = arr + size - 1;
    while (start < end) {
        swap_int(start, end);
        start++;
        end--;
    }
}

/*============================================================================*/
/*                           演示函数实现                                       */
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
void demo_pointer_basics(void)
{
    print_separator("指针基础演示");

    printf("[指针声明与初始化]\n");
    int32_t value = 42;
    int32_t *ptr = &value;
    
    printf("  int32_t value = 42;\n");
    printf("  int32_t *ptr = &value;\n");
    printf("  &value = %p (变量地址)\n", (void *)&value);
    printf("  ptr = %p (指针值)\n", (void *)ptr);
    printf("  *ptr = %d (解引用)\n", *ptr);
    printf("  sizeof(ptr) = %zu 字节 (指针大小)\n", sizeof(ptr));

    printf("\n[空指针与野指针]\n");
    int32_t *null_ptr = NULL;
    printf("  NULL指针: %p\n", (void *)null_ptr);
    printf("  使用前必须检查: if (ptr != NULL) { ... }\n");
    printf("  野指针: 未初始化或指向已释放内存的指针\n");
    printf("  危险: 解引用野指针导致未定义行为\n");

    printf("\n[void指针]\n");
    void *void_ptr = &value;
    printf("  void *void_ptr = &value;\n");
    printf("  void_ptr = %p\n", void_ptr);
    printf("  使用前需类型转换: *(int32_t *)void_ptr = %d\n", 
           *(int32_t *)void_ptr);

    printf("\n[指针类型的重要性]\n");
    int32_t arr[] = {0x12345678, 0x9ABCDEF0};
    int32_t *int_ptr = arr;
    char *char_ptr = (char *)arr;
    
    printf("  int32_t数组: 0x%08X, 0x%08X\n", arr[0], arr[1]);
    printf("  *int_ptr = 0x%08X (读取4字节)\n", *int_ptr);
    printf("  *char_ptr = 0x%02X (读取1字节)\n", (uint8_t)*char_ptr);
    printf("  指针类型决定解引用时读取的字节数\n");

    printf("\n[const与指针]\n");
    int32_t x = 10, y = 20;
    const int32_t *ptr_to_const = &x;
    int32_t *const const_ptr = &x;
    int32_t const *ptr_to_const2 = &x;
    
    printf("  const int32_t *p: 指向常量的指针 (不能通过p修改值)\n");
    printf("  int32_t *const p: 常量指针 (p本身不可修改)\n");
    printf("  const int32_t *const p: 都不可修改\n");
    
    ptr_to_const = &y;
    printf("  ptr_to_const = &y; // 合法\n");
    *const_ptr = 30;
    printf("  *const_ptr = 30; // 合法, x = %d\n", x);
}

void demo_pointer_arithmetic(void)
{
    print_separator("指针运算演示");

    printf("[指针加减整数]\n");
    int32_t arr[] = {10, 20, 30, 40, 50};
    int32_t *ptr = arr;
    
    printf("  数组: ");
    for (int32_t i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    printf("  ptr = arr; *ptr = %d\n", *ptr);
    printf("  ptr + 2 = %p, *(ptr + 2) = %d\n", (void *)(ptr + 2), *(ptr + 2));
    printf("  ptr += 3; *ptr = %d\n", *(ptr += 3));
    printf("  ptr - 1 = %p, *(ptr - 1) = %d\n", (void *)(ptr - 1), *(ptr - 1));

    printf("\n[指针减法(计算距离)]\n");
    int32_t *start = arr;
    int32_t *end = arr + 4;
    printf("  end - start = %td (元素个数)\n", end - start);
    printf("  (char *)end - (char *)start = %td (字节数)\n", 
           (char *)end - (char *)start);

    printf("\n[指针比较]\n");
    printf("  start < end: %s\n", (start < end) ? "true" : "false");
    printf("  start == arr: %s\n", (start == arr) ? "true" : "false");
    printf("  指针比较仅在同一数组内有意义\n");

    printf("\n[不同类型的指针运算]\n");
    double d_arr[] = {1.1, 2.2, 3.3, 4.4};
    double *d_ptr = d_arr;
    
    printf("  double数组首地址: %p\n", (void *)d_arr);
    printf("  d_ptr + 1: %p (偏移sizeof(double)=%zu字节)\n", 
           (void *)(d_ptr + 1), sizeof(double));

    printf("\n[指针运算规则]\n");
    printf("  ptr + n 实际偏移 n * sizeof(*ptr) 字节\n");
    printf("  仅对数组元素有效, 其他情况可能越界\n");
}

void demo_pointer_and_array(void)
{
    print_separator("指针与数组演示");

    printf("[数组名与指针]\n");
    int32_t arr[] = {1, 2, 3, 4, 5};
    
    printf("  arr: 数组首元素地址 %p\n", (void *)arr);
    printf("  &arr[0]: 首元素地址 %p\n", (void *)&arr[0]);
    printf("  &arr: 整个数组地址 %p (类型: int32_t (*)[5])\n", (void *)&arr);
    printf("  arr + 1: %p\n", (void *)(arr + 1));
    printf("  &arr + 1: %p (跳过整个数组)\n", (void *)(&arr + 1));

    printf("\n[下标与指针等价性]\n");
    int32_t *ptr = arr;
    printf("  arr[i] 等价于 *(arr + i)\n");
    printf("  ptr[i] 等价于 *(ptr + i)\n");
    printf("  arr[2] = %d, *(arr + 2) = %d\n", arr[2], *(arr + 2));
    printf("  ptr[3] = %d, *(ptr + 3) = %d\n", ptr[3], *(ptr + 3));
    printf("  2[arr] = %d (合法但不推荐)\n", 2[arr]);

    printf("\n[数组作为函数参数]\n");
    printf("  void func(int32_t arr[]) 等价于 void func(int32_t *arr)\n");
    printf("  数组退化为指针, 需要额外传递大小\n");

    printf("\n[指针数组]\n");
    int32_t a = 1, b = 2, c = 3;
    int32_t *ptr_arr[] = {&a, &b, &c};
    printf("  int32_t *ptr_arr[] = {&a, &b, &c};\n");
    printf("  *ptr_arr[0] = %d, *ptr_arr[1] = %d, *ptr_arr[2] = %d\n",
           *ptr_arr[0], *ptr_arr[1], *ptr_arr[2]);

    printf("\n[数组指针]\n");
    int32_t (*arr_ptr)[5] = &arr;
    printf("  int32_t (*arr_ptr)[5] = &arr;\n");
    printf("  (*arr_ptr)[0] = %d\n", (*arr_ptr)[0]);
    printf("  用于处理多维数组\n");
}

void demo_pointer_and_string(void)
{
    print_separator("指针与字符串演示");

    printf("[字符指针与字符串]\n");
    char str_arr[] = "Hello";
    const char *str_ptr = "World";
    
    printf("  char str_arr[] = \"Hello\";\n");
    printf("  const char *str_ptr = \"World\";\n");
    printf("  str_arr: 可修改, 存储在栈/全局区\n");
    printf("  str_ptr: 指向只读区, 不可修改\n");

    printf("\n[字符串遍历]\n");
    const char *s = "Embedded";
    printf("  遍历 \"%s\":\n", s);
    while (*s != '\0') {
        printf("    %c (地址: %p)\n", *s, (void *)s);
        s++;
    }

    printf("\n[字符串复制实现]\n");
    char dest[20];
    const char *src = "Copy Me";
    char *d = dest;
    while ((*d++ = *src++) != '\0') {
        ;
    }
    printf("  复制结果: \"%s\"\n", dest);

    printf("\n[字符串数组]\n");
    const char *weekdays[] = {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };
    printf("  星期名称:\n");
    for (int32_t i = 0; i < 7; i++) {
        printf("    [%d] %s\n", i, weekdays[i]);
    }

    printf("\n[命令行参数]\n");
    printf("  int main(int argc, char *argv[])\n");
    printf("  argc: 参数个数\n");
    printf("  argv: 参数字符串数组\n");
    printf("  argv[0]: 程序名\n");
}

void demo_function_pointer_param(void)
{
    print_separator("指针作为函数参数演示");

    printf("[修改调用者变量]\n");
    int32_t x = 10, y = 20;
    printf("  交换前: x = %d, y = %d\n", x, y);
    swap_int(&x, &y);
    printf("  交换后: x = %d, y = %d\n", x, y);

    printf("\n[返回多个值]\n");
    int32_t arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
    size_t size = ARRAY_SIZE(arr);
    int32_t sum = 0;
    int32_t max_val = arr[0];
    int32_t min_val = arr[0];
    
    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
        if (arr[i] > max_val) max_val = arr[i];
        if (arr[i] < min_val) min_val = arr[i];
    }
    
    printf("  数组统计: sum=%d, max=%d, min=%d\n", sum, max_val, min_val);
    printf("  通过指针参数可以返回多个结果\n");

    printf("\n[避免大对象拷贝]\n");
    printf("  大结构体作为参数时, 传递指针避免拷贝\n");
    printf("  void process(const LargeStruct_t *data);\n");

    printf("\n[回调函数指针]\n");
    printf("  typedef void (*Callback_t)(int32_t result);\n");
    printf("  void async_operation(Callback_t callback);\n");
}

void demo_dynamic_memory(void)
{
    print_separator("动态内存管理演示");

    printf("[malloc - 分配未初始化内存]\n");
    int32_t *arr1 = (int32_t *)safe_malloc(5 * sizeof(int32_t));
    if (arr1 != NULL) {
        printf("  分配成功: %p, 大小: %zu 字节\n", (void *)arr1, 5 * sizeof(int32_t));
        for (int32_t i = 0; i < 5; i++) {
            arr1[i] = i * 10;
        }
        printf("  初始化后: ");
        for (int32_t i = 0; i < 5; i++) {
            printf("%d ", arr1[i]);
        }
        printf("\n");
        safe_free((void **)&arr1);
    }

    printf("\n[calloc - 分配并清零]\n");
    int32_t *arr2 = (int32_t *)safe_calloc(5, sizeof(int32_t));
    if (arr2 != NULL) {
        printf("  分配并清零: ");
        for (int32_t i = 0; i < 5; i++) {
            printf("%d ", arr2[i]);
        }
        printf("\n");
        safe_free((void **)&arr2);
    }

    printf("\n[realloc - 调整大小]\n");
    int32_t *arr3 = (int32_t *)safe_malloc(3 * sizeof(int32_t));
    if (arr3 != NULL) {
        for (int32_t i = 0; i < 3; i++) {
            arr3[i] = i + 1;
        }
        printf("  原始(3个): ");
        for (int32_t i = 0; i < 3; i++) {
            printf("%d ", arr3[i]);
        }
        printf("\n");
        
        int32_t *arr3_new = (int32_t *)safe_realloc(arr3, 6 * sizeof(int32_t));
        if (arr3_new != NULL) {
            arr3 = arr3_new;
            for (int32_t i = 3; i < 6; i++) {
                arr3[i] = i + 1;
            }
            printf("  扩展后(6个): ");
            for (int32_t i = 0; i < 6; i++) {
                printf("%d ", arr3[i]);
            }
            printf("\n");
        }
        safe_free((void **)&arr3);
    }

    printf("\n[内存泄漏检测]\n");
    printf("  常见原因:\n");
    printf("    1. 分配后忘记释放\n");
    printf("    2. 丢失指针(覆盖指针变量)\n");
    printf("    3. 异常路径未释放\n");
    printf("  建议: 分配和释放成对出现, 使用SAFE_FREE宏\n");

    printf("\n[嵌入式动态内存考虑]\n");
    printf("  - 内存碎片问题\n");
    printf("  - 分配失败处理\n");
    printf("  - 优先使用静态分配\n");
    printf("  - 使用内存池技术\n");
}

void demo_memory_alignment(void)
{
    print_separator("内存对齐演示");

    printf("[自然对齐]\n");
    printf("  char: 1字节对齐\n");
    printf("  short: 2字节对齐\n");
    printf("  int/float: 4字节对齐\n");
    printf("  double/指针(64位): 8字节对齐\n");

    printf("\n[结构体对齐]\n");
    struct Unaligned {
        char a;
        int b;
        char c;
    };
    struct Reordered {
        int b;
        char a;
        char c;
    };
    
    printf("  struct Unaligned { char a; int b; char c; }\n");
    printf("    大小: %zu 字节 (含填充)\n", sizeof(struct Unaligned));
    printf("    a偏移: %zu, b偏移: %zu, c偏移: %zu\n",
           offsetof(struct Unaligned, a),
           offsetof(struct Unaligned, b),
           offsetof(struct Unaligned, c));
    
    printf("  struct Reordered { int b; char a; char c; }\n");
    printf("    大小: %zu 字节 (优化后)\n", sizeof(struct Reordered));

    printf("\n[对齐属性]\n");
#ifdef _MSC_VER
    __declspec(align(16)) struct Aligned16 {
        int x;
    };
#else
    struct __attribute__((aligned(16))) Aligned16 {
        int x;
    };
#endif
    printf("  aligned(16): %zu 字节对齐\n", 
           sizeof(struct Aligned16));

    printf("\n[紧凑属性]\n");
#ifdef _MSC_VER
#pragma pack(push, 1)
    struct Packed {
        char a;
        int b;
        char c;
    };
#pragma pack(pop)
#else
    struct __attribute__((packed)) Packed {
        char a;
        int b;
        char c;
    };
#endif
    printf("  packed属性: %zu 字节 (无填充)\n", 
           sizeof(struct Packed));
    printf("  注意: 可能影响访问效率, 某些架构不支持非对齐访问\n");

    printf("\n[对齐检查宏]\n");
    struct Unaligned test;
    printf("  IS_ALIGNED(&test, 4): %s\n", 
           IS_ALIGNED(&test, 4) ? "true" : "false");
    printf("  IS_ALIGNED(&test.b, 4): %s\n", 
           IS_ALIGNED(&test.b, 4) ? "true" : "false");
}

void demo_pointer_advanced(void)
{
    print_separator("高级指针用法演示");

    printf("[多级指针]\n");
    int32_t value = 42;
    int32_t *ptr1 = &value;
    int32_t **ptr2 = &ptr1;
    int32_t ***ptr3 = &ptr2;
    
    printf("  value = %d\n", value);
    printf("  *ptr1 = %d\n", *ptr1);
    printf("  **ptr2 = %d\n", **ptr2);
    printf("  ***ptr3 = %d\n", ***ptr3);
    printf("  用途: 动态二维数组、修改指针本身\n");

    printf("\n[函数指针数组]\n");
    int32_t (*operations[])(int32_t, int32_t) = {
        op_add, op_sub, op_mul, op_div
    };
    const char *op_names[] = {"+", "-", "*", "/"};
    
    int32_t a = 20, b = 4;
    for (int32_t i = 0; i < 4; i++) {
        printf("  %d %s %d = %d\n", a, op_names[i], b, operations[i](a, b));
    }

    printf("\n[指向函数指针的指针]\n");
    int32_t (*op)(int32_t, int32_t) = operations[0];
    int32_t (**op_ptr)(int32_t, int32_t) = &op;
    printf("  (**op_ptr)(5, 3) = %d\n", (**op_ptr)(5, 3));

    printf("\n[restrict关键字]\n");
    printf("  void copy(int *restrict dest, const int *restrict src, size_t n);\n");
    printf("  restrict承诺指针是访问该对象的唯一方式\n");
    printf("  允许编译器优化, 违反承诺是未定义行为\n");

    printf("\n[volatile关键字]\n");
    printf("  volatile uint32_t *reg = (volatile uint32_t *)0x40000000;\n");
    printf("  防止编译器优化对硬件寄存器的访问\n");
    printf("  每次访问都从内存读取, 不使用缓存值\n");
}

#define POOL_SIZE 10
#define BLOCK_SIZE 32

typedef struct {
    uint8_t data[BLOCK_SIZE];
    bool in_use;
} MemoryBlock_t;

typedef struct {
    MemoryBlock_t blocks[POOL_SIZE];
    size_t allocated_count;
} MemoryPool_t;

static MemoryPool_t g_memory_pool = {0};

static void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_SIZE; i++) {
        if (!g_memory_pool.blocks[i].in_use) {
            g_memory_pool.blocks[i].in_use = true;
            g_memory_pool.allocated_count++;
            return g_memory_pool.blocks[i].data;
        }
    }
    return NULL;
}

static void pool_free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }
    
    for (size_t i = 0; i < POOL_SIZE; i++) {
        if (g_memory_pool.blocks[i].data == ptr) {
            g_memory_pool.blocks[i].in_use = false;
            g_memory_pool.allocated_count--;
            return;
        }
    }
}

static size_t pool_available(void)
{
    return POOL_SIZE - g_memory_pool.allocated_count;
}

void demo_memory_pool(void)
{
    print_separator("内存池演示");

    printf("[内存池原理]\n");
    printf("  - 预分配固定大小的内存块\n");
    printf("  - 分配/释放时间复杂度O(1)\n");
    printf("  - 无内存碎片\n");
    printf("  - 适合嵌入式实时系统\n");

    printf("\n[内存池操作]\n");
    printf("  池大小: %d 块, 每块 %d 字节\n", POOL_SIZE, BLOCK_SIZE);
    printf("  可用块: %zu\n", pool_available());

    void *block1 = pool_alloc();
    void *block2 = pool_alloc();
    void *block3 = pool_alloc();
    
    printf("  分配3块后, 可用: %zu\n", pool_available());
    
    pool_free(block2);
    printf("  释放block2后, 可用: %zu\n", pool_available());
    
    void *block4 = pool_alloc();
    printf("  再分配1块后, 可用: %zu\n", pool_available());
    
    pool_free(block1);
    pool_free(block3);
    pool_free(block4);
    printf("  全部释放后, 可用: %zu\n", pool_available());

    printf("\n[嵌入式内存管理策略]\n");
    printf("  1. 静态分配: 编译时确定, 最可靠\n");
    printf("  2. 内存池: 固定大小块, 无碎片\n");
    printf("  3. 堆分配: 灵活但有碎片风险\n");
    printf("  4. 栈分配: 函数内临时使用\n");
}
