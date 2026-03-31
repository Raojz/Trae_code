/**
 * @file array_string.c
 * @brief 数组与字符串模块实现
 * @details 演示C语言数组与字符串操作，涵盖以下核心知识点：
 *          - 数组声明、初始化、访问、越界问题
 *          - 数组操作：查找、排序、统计
 *          - 多维数组：二维数组、内存布局
 *          - 字符串基础：字符串表示、结束符'\0'
 *          - 字符串操作：复制、连接、比较、转换
 *          - 缓冲区操作：安全函数、缓冲区溢出防护
 *          - 环形缓冲区：FIFO实现、嵌入式典型应用
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 数据采集和缓存
 * - 串口通信数据缓冲
 * - 协议解析和打包
 * - 配置参数存储
 * - 日志信息处理
 * 
 * @section safety_notes 安全注意事项
 * - 数组越界是常见的bug来源
 * - 字符串必须有结束符'\0'
 * - 缓冲区溢出可能导致程序崩溃或安全漏洞
 * - 使用安全的字符串函数（如strncpy替代strcpy）
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 * @note 所有函数都进行了参数有效性检查
 */

#include "array_string.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*============================================================================*/
/*                           数组操作函数实现                                   */
/*============================================================================*/

/**
 * @brief 查找数组中的最大值索引
 * @details 遍历数组，找到最大值所在的索引位置。
 *          如果有多个相同的最大值，返回第一个出现的索引。
 * 
 * @param arr 数组指针，不能为NULL
 * @param size 数组元素个数，必须大于0
 * @return 最大值的索引（0-based）
 * 
 * @note 如果数组为空或指针为NULL，返回0
 * @note 时间复杂度O(n)
 * 
 * @code
 * int32_t data[] = {3, 1, 4, 1, 5, 9, 2, 6};
 * size_t max_idx = array_find_max(data, ARRAY_SIZE(data));
 * printf("最大值: data[%zu] = %d\n", max_idx, data[max_idx]);
 * @endcode
 */
size_t array_find_max(const int32_t *arr, size_t size)
{
    if (arr == NULL || size == 0) {
        return 0;
    }

    size_t max_idx = 0;
    for (size_t i = 1; i < size; i++) {
        if (arr[i] > arr[max_idx]) {
            max_idx = i;
        }
    }
    return max_idx;
}

/**
 * @brief 查找数组中的最小值索引
 * @details 遍历数组，找到最小值所在的索引位置。
 *          如果有多个相同的最小值，返回第一个出现的索引。
 * 
 * @param arr 数组指针，不能为NULL
 * @param size 数组元素个数，必须大于0
 * @return 最小值的索引（0-based）
 * 
 * @note 如果数组为空或指针为NULL，返回0
 * @note 时间复杂度O(n)
 * 
 * @code
 * int32_t data[] = {3, 1, 4, 1, 5, 9, 2, 6};
 * size_t min_idx = array_find_min(data, ARRAY_SIZE(data));
 * printf("最小值: data[%zu] = %d\n", min_idx, data[min_idx]);
 * @endcode
 */
size_t array_find_min(const int32_t *arr, size_t size)
{
    if (arr == NULL || size == 0) {
        return 0;
    }

    size_t min_idx = 0;
    for (size_t i = 1; i < size; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
    }
    return min_idx;
}

/**
 * @brief 冒泡排序
 * @details 对数组进行升序排序。冒泡排序通过重复遍历数组，
 *          比较相邻元素并交换，将最大元素"冒泡"到末尾。
 *          优化版本：如果某次遍历没有交换，说明已排序完成。
 * 
 * @param arr 数组指针，不能为NULL
 * @param size 数组元素个数
 * 
 * @note 时间复杂度：最好O(n)，平均O(n^2)，最坏O(n^2)
 * @note 空间复杂度：O(1)，原地排序
 * @note 稳定排序：相等元素的相对顺序不变
 * @note 适合小规模数据或基本有序的数据
 * 
 * @code
 * int32_t data[] = {5, 2, 8, 1, 9};
 * array_sort_bubble(data, ARRAY_SIZE(data));
 * // data = {1, 2, 5, 8, 9}
 * @endcode
 */
void array_sort_bubble(int32_t *arr, size_t size)
{
    if (arr == NULL || size < 2) {
        return;
    }

    for (size_t i = 0; i < size - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int32_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

/**
 * @brief 选择排序
 * @details 对数组进行升序排序。选择排序每次从未排序部分
 *          选择最小元素，放到已排序部分的末尾。
 * 
 * @param arr 数组指针，不能为NULL
 * @param size 数组元素个数
 * 
 * @note 时间复杂度：O(n^2)
 * @note 空间复杂度：O(1)，原地排序
 * @note 不稳定排序：相等元素可能改变相对顺序
 * @note 交换次数少于冒泡排序
 * 
 * @code
 * int32_t data[] = {5, 2, 8, 1, 9};
 * array_sort_selection(data, ARRAY_SIZE(data));
 * // data = {1, 2, 5, 8, 9}
 * @endcode
 */
void array_sort_selection(int32_t *arr, size_t size)
{
    if (arr == NULL || size < 2) {
        return;
    }

    for (size_t i = 0; i < size - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < size; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int32_t temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

/**
 * @brief 计算数组元素之和
 * @details 遍历数组，累加所有元素的值。
 * 
 * @param arr 数组指针，不能为NULL
 * @param size 数组元素个数
 * @return 数组元素之和
 * 
 * @note 如果数组为空或指针为NULL，返回0
 * @note 注意整数溢出风险
 * 
 * @code
 * int32_t data[] = {1, 2, 3, 4, 5};
 * int32_t sum = array_sum(data, ARRAY_SIZE(data));
 * printf("总和: %d\n", sum);  // 输出: 总和: 15
 * @endcode
 */
int32_t array_sum(const int32_t *arr, size_t size)
{
    if (arr == NULL || size == 0) {
        return 0;
    }

    int32_t sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

/**
 * @brief 计算数组元素平均值
 * @details 计算数组所有元素的平均值。
 * 
 * @param arr 数组指针，不能为NULL
 * @param size 数组元素个数
 * @return 数组元素平均值（double类型）
 * 
 * @note 如果数组为空或指针为NULL，返回0.0
 * @note 返回double类型以保持精度
 * 
 * @code
 * int32_t data[] = {1, 2, 3, 4, 5};
 * double avg = array_average(data, ARRAY_SIZE(data));
 * printf("平均值: %.2f\n", avg);  // 输出: 平均值: 3.00
 * @endcode
 */
double array_average(const int32_t *arr, size_t size)
{
    if (arr == NULL || size == 0) {
        return 0.0;
    }

    int32_t sum = array_sum(arr, size);
    return (double)sum / (double)size;
}

/*============================================================================*/
/*                           字符串操作函数实现                                 */
/*============================================================================*/

/**
 * @brief 计算字符串长度
 * @details 计算字符串的长度（不含结束符'\0'）。
 *          类似于标准库strlen函数。
 * 
 * @param str 字符串指针，不能为NULL
 * @return 字符串长度（字节数）
 * 
 * @note 如果指针为NULL，返回0
 * @note 不包含结束符'\0'
 * 
 * @code
 * size_t len = string_length("Hello");
 * printf("长度: %zu\n", len);  // 输出: 长度: 5
 * @endcode
 */
size_t string_length(const char *str)
{
    if (str == NULL) {
        return 0;
    }

    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

/**
 * @brief 安全的字符串复制
 * @details 将src复制到dest，最多复制dest_size-1个字符，
 *          确保结果字符串以'\0'结尾。
 *          这是strcpy的安全版本，防止缓冲区溢出。
 * 
 * @param dest 目标缓冲区，不能为NULL
 * @param src 源字符串，不能为NULL
 * @param dest_size 目标缓冲区大小（字节）
 * @return 成功返回dest指针，失败返回dest（可能内容不变）
 * 
 * @note 比strcpy更安全，总是保证'\0'结尾
 * @note 如果src长度 >= dest_size，只复制dest_size-1个字符
 * 
 * @code
 * char buf[10];
 * string_copy(buf, "Hello", sizeof(buf));
 * printf("%s\n", buf);  // 输出: Hello
 * @endcode
 */
char *string_copy(char *dest, const char *src, size_t dest_size)
{
    if (dest == NULL || src == NULL || dest_size == 0) {
        return dest;
    }

    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';

    return dest;
}

/**
 * @brief 安全的字符串连接
 * @details 将src连接到dest末尾，确保不超出缓冲区大小。
 *          这是strcat的安全版本，防止缓冲区溢出。
 * 
 * @param dest 目标缓冲区（已有字符串），不能为NULL
 * @param src 要追加的源字符串，不能为NULL
 * @param dest_size 目标缓冲区总大小（字节）
 * @return 成功返回dest指针，失败返回dest
 * 
 * @note 总是保证结果以'\0'结尾
 * @note dest_size是缓冲区总大小，不是剩余空间
 * 
 * @code
 * char buf[20] = "Hello";
 * string_concat(buf, " World", sizeof(buf));
 * printf("%s\n", buf);  // 输出: Hello World
 * @endcode
 */
char *string_concat(char *dest, const char *src, size_t dest_size)
{
    if (dest == NULL || src == NULL || dest_size == 0) {
        return dest;
    }

    size_t dest_len = string_length(dest);
    size_t i = 0;

    while (dest_len + i < dest_size - 1 && src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';

    return dest;
}

/**
 * @brief 字符串比较
 * @details 比较两个字符串的大小（按字典序）。
 *          类似于标准库strcmp函数。
 * 
 * @param s1 第一个字符串
 * @param s2 第二个字符串
 * @return <0表示s1<s2，=0表示s1==s2，>0表示s1>s2
 * 
 * @note NULL指针被视为小于任何非NULL字符串
 * @note 两个NULL指针被视为相等
 * 
 * @code
 * int32_t result = string_compare("abc", "abd");
 * // result < 0，因为 "abc" < "abd"
 * @endcode
 */
int32_t string_compare(const char *s1, const char *s2)
{
    if (s1 == NULL && s2 == NULL) {
        return 0;
    }
    if (s1 == NULL) {
        return -1;
    }
    if (s2 == NULL) {
        return 1;
    }

    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }

    return (int32_t)((uint8_t)*s1 - (uint8_t)*s2);
}

/**
 * @brief 字符串转大写
 * @details 将字符串中的所有小写字母转为大写。
 *          原地修改字符串。
 * 
 * @param str 字符串指针，不能为NULL
 * @return 返回str指针
 * 
 * @code
 * char str[] = "Hello";
 * string_to_upper(str);
 * printf("%s\n", str);  // 输出: HELLO
 * @endcode
 */
char *string_to_upper(char *str)
{
    if (str == NULL) {
        return NULL;
    }

    char *p = str;
    while (*p != '\0') {
        *p = (char)toupper((int)*p);
        p++;
    }
    return str;
}

/**
 * @brief 字符串转小写
 * @details 将字符串中的所有大写字母转为小写。
 *          原地修改字符串。
 * 
 * @param str 字符串指针，不能为NULL
 * @return 返回str指针
 * 
 * @code
 * char str[] = "HELLO";
 * string_to_lower(str);
 * printf("%s\n", str);  // 输出: hello
 * @endcode
 */
char *string_to_lower(char *str)
{
    if (str == NULL) {
        return NULL;
    }

    char *p = str;
    while (*p != '\0') {
        *p = (char)tolower((int)*p);
        p++;
    }
    return str;
}

/**
 * @brief 字符串转整数
 * @details 将字符串转换为整数。支持：
 *          - 前导空白字符（空格、制表符）
 *          - 正负号（+/-）
 *          - 十进制数字
 *          类似于标准库atoi函数。
 * 
 * @param str 数字字符串，不能为NULL
 * @return 转换后的整数值
 * 
 * @note 无效字符串返回0
 * @note 遇到非数字字符停止转换
 * @note 不检查溢出
 * 
 * @code
 * int32_t val = string_to_int("-12345");
 * printf("%d\n", val);  // 输出: -12345
 * @endcode
 */
int32_t string_to_int(const char *str)
{
    if (str == NULL) {
        return 0;
    }

    while (*str == ' ' || *str == '\t') {
        str++;
    }

    int32_t sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    int32_t result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    return sign * result;
}

/**
 * @brief 整数转字符串
 * @details 将整数转换为字符串表示。
 *          支持正数、负数和零。
 * 
 * @param value 要转换的整数值
 * @param str 目标缓冲区，不能为NULL
 * @param size 缓冲区大小（字节）
 * 
 * @note 缓冲区应足够大以容纳结果
 * @note 结果字符串以'\0'结尾
 * 
 * @code
 * char buf[20];
 * int_to_string(-12345, buf, sizeof(buf));
 * printf("%s\n", buf);  // 输出: -12345
 * @endcode
 */
void int_to_string(int32_t value, char *str, size_t size)
{
    if (str == NULL || size == 0) {
        return;
    }

    char buffer[32];
    int32_t idx = 0;
    bool negative = false;

    if (value < 0) {
        negative = true;
        value = -value;
    }

    if (value == 0) {
        buffer[idx++] = '0';
    } else {
        while (value > 0) {
            buffer[idx++] = (char)('0' + (value % 10));
            value /= 10;
        }
    }

    size_t out_idx = 0;
    if (negative && out_idx < size - 1) {
        str[out_idx++] = '-';
    }

    while (idx > 0 && out_idx < size - 1) {
        str[out_idx++] = buffer[--idx];
    }
    str[out_idx] = '\0';
}

/*============================================================================*/
/*                           演示函数实现                                       */
/*============================================================================*/

/**
 * @brief 数组基础演示
 * @details 展示数组的基本概念：
 *          - 数组声明和初始化
 *          - 数组元素的访问
 *          - 数组的大小和长度
 *          - 数组越界问题
 *          - 数组与指针的关系
 */
void demo_array_basics(void)
{
    print_separator("数组基础演示");

    printf("[数组声明与初始化]\n");
    int32_t arr1[5] = {1, 2, 3, 4, 5};
    int32_t arr2[] = {10, 20, 30};
    int32_t arr3[5] = {0};
    int32_t arr4[10] = {[0] = 1, [5] = 6, [9] = 10};

    printf("  arr1: ");
    for (size_t i = 0; i < ARRAY_SIZE(arr1); i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");

    printf("  arr2: ");
    for (size_t i = 0; i < ARRAY_SIZE(arr2); i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");

    printf("  arr3 (全零初始化): ");
    for (size_t i = 0; i < ARRAY_SIZE(arr3); i++) {
        printf("%d ", arr3[i]);
    }
    printf("\n");

    printf("  arr4 (指定初始化): ");
    for (size_t i = 0; i < ARRAY_SIZE(arr4); i++) {
        printf("%d ", arr4[i]);
    }
    printf("\n");

    printf("\n[数组大小计算]\n");
    printf("  sizeof(arr1) = %zu 字节\n", sizeof(arr1));
    printf("  sizeof(arr1[0]) = %zu 字节\n", sizeof(arr1[0]));
    printf("  元素个数 = %zu\n", ARRAY_SIZE(arr1));

    printf("\n[数组越界风险]\n");
    printf("  C语言不检查数组边界, 访问越界会导致未定义行为\n");
    printf("  建议: 使用ARRAY_SIZE宏或显式传递大小\n");

    printf("\n[数组与指针关系]\n");
    printf("  arr1 == &arr1[0]: %s\n", (arr1 == &arr1[0]) ? "true" : "false");
    printf("  *arr1 = %d (等价于 arr1[0])\n", *arr1);
    printf("  *(arr1 + 2) = %d (等价于 arr1[2])\n", *(arr1 + 2));
}

void demo_array_operations(void)
{
    print_separator("数组操作演示");

    int32_t arr[] = {64, 34, 25, 12, 22, 11, 90, 45};
    size_t size = ARRAY_SIZE(arr);

    printf("[原始数组]\n  ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("\n[查找最大/最小值]\n");
    size_t max_idx = array_find_max(arr, size);
    size_t min_idx = array_find_min(arr, size);
    printf("  最大值: arr[%zu] = %d\n", max_idx, arr[max_idx]);
    printf("  最小值: arr[%zu] = %d\n", min_idx, arr[min_idx]);

    printf("\n[求和与平均]\n");
    printf("  总和: %d\n", array_sum(arr, size));
    printf("  平均: %.2f\n", array_average(arr, size));

    printf("\n[冒泡排序]\n");
    int32_t arr_bubble[8];
    memcpy(arr_bubble, arr, sizeof(arr));
    array_sort_bubble(arr_bubble, size);
    printf("  排序后: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr_bubble[i]);
    }
    printf("\n");

    printf("\n[选择排序]\n");
    int32_t arr_selection[8];
    memcpy(arr_selection, arr, sizeof(arr));
    array_sort_selection(arr_selection, size);
    printf("  排序后: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr_selection[i]);
    }
    printf("\n");

    printf("\n[数组反转]\n");
    int32_t arr_reverse[8];
    memcpy(arr_reverse, arr, sizeof(arr));
    for (size_t i = 0; i < size / 2; i++) {
        int32_t temp = arr_reverse[i];
        arr_reverse[i] = arr_reverse[size - 1 - i];
        arr_reverse[size - 1 - i] = temp;
    }
    printf("  反转后: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr_reverse[i]);
    }
    printf("\n");
}

void demo_multidimensional_array(void)
{
    print_separator("多维数组演示");

    printf("[二维数组声明与初始化]\n");
    int32_t matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    printf("  矩阵 (3x4):\n");
    for (int32_t i = 0; i < 3; i++) {
        printf("    ");
        for (int32_t j = 0; j < 4; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n[内存布局]\n");
    printf("  二维数组按行优先存储\n");
    printf("  matrix[0][0]地址: %p\n", (void *)&matrix[0][0]);
    printf("  matrix[0][1]地址: %p (差值: %td)\n", 
           (void *)&matrix[0][1], 
           (char *)&matrix[0][1] - (char *)&matrix[0][0]);
    printf("  matrix[1][0]地址: %p (差值: %td)\n", 
           (void *)&matrix[1][0], 
           (char *)&matrix[1][0] - (char *)&matrix[0][0]);

    printf("\n[矩阵运算示例]\n");
    int32_t sum = 0;
    int32_t row_sums[3] = {0};
    int32_t col_sums[4] = {0};

    for (int32_t i = 0; i < 3; i++) {
        for (int32_t j = 0; j < 4; j++) {
            sum += matrix[i][j];
            row_sums[i] += matrix[i][j];
            col_sums[j] += matrix[i][j];
        }
    }

    printf("  总和: %d\n", sum);
    printf("  行和: ");
    for (int32_t i = 0; i < 3; i++) {
        printf("%d ", row_sums[i]);
    }
    printf("\n  列和: ");
    for (int32_t j = 0; j < 4; j++) {
        printf("%d ", col_sums[j]);
    }
    printf("\n");

    printf("\n[三维数组]\n");
    int32_t cube[2][2][2] = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
    printf("  cube[2][2][2]:\n");
    for (int32_t i = 0; i < 2; i++) {
        printf("    层 %d:\n", i);
        for (int32_t j = 0; j < 2; j++) {
            printf("      ");
            for (int32_t k = 0; k < 2; k++) {
                printf("%d ", cube[i][j][k]);
            }
            printf("\n");
        }
    }
}

void demo_string_basics(void)
{
    print_separator("字符串基础演示");

    printf("[字符串声明与初始化]\n");
    char str1[] = "Hello";
    const char *str2 = "World";
    char str3[10] = {'H', 'i', '\0'};
    char str4[20] = {0};

    printf("  str1[] = \"Hello\": 长度=%zu, 大小=%zu\n", string_length(str1), sizeof(str1));
    printf("  str2 = \"World\": 长度=%zu\n", string_length(str2));
    printf("  str3[10] = {'H','i','\\0'}: \"%s\"\n", str3);
    printf("  str4[20] = {0}: \"%s\" (空字符串)\n", str4);

    printf("\n[字符串与字符数组区别]\n");
    printf("  字符串: 以'\\0'结尾的字符数组\n");
    printf("  \"Hello\"实际占用6字节: H e l l o \\0\n");

    printf("\n[字符串字面量]\n");
    printf("  存储在只读区域, 不可修改\n");
    printf("  char *s = \"Hello\";  // s指向只读内存\n");
    printf("  char s[] = \"Hello\"; // s是可修改的副本\n");

    printf("\n[转义字符]\n");
    printf("  \\n: 换行 (ASCII %d)\n", '\n');
    printf("  \\t: 制表 (ASCII %d)\n", '\t');
    printf("  \\r: 回车 (ASCII %d)\n", '\r');
    printf("  \\\\: 反斜杠 (ASCII %d)\n", '\\');
    printf("  \\\": 双引号 (ASCII %d)\n", '"');
    printf("  \\0: 空字符 (ASCII %d)\n", '\0');
}

void demo_string_operations(void)
{
    print_separator("字符串操作演示");

    printf("[字符串长度]\n");
    char str[] = "Hello, World!";
    printf("  \"%s\" 长度: %zu\n", str, string_length(str));

    printf("\n[字符串复制]\n");
    char dest[50];
    string_copy(dest, "Hello", sizeof(dest));
    printf("  复制后: \"%s\"\n", dest);

    printf("\n[字符串连接]\n");
    string_concat(dest, ", World!", sizeof(dest));
    printf("  连接后: \"%s\"\n", dest);

    printf("\n[字符串比较]\n");
    printf("  比较 \"abc\" 和 \"abc\": %d\n", string_compare("abc", "abc"));
    printf("  比较 \"abc\" 和 \"abd\": %d\n", string_compare("abc", "abd"));
    printf("  比较 \"abc\" 和 \"abb\": %d\n", string_compare("abc", "abb"));

    printf("\n[大小写转换]\n");
    char upper[] = "HELLO";
    char lower[] = "world";
    printf("  \"%s\" -> \"%s\"\n", lower, string_to_upper(lower));
    printf("  \"%s\" -> \"%s\"\n", upper, string_to_lower(upper));

    printf("\n[标准库函数]\n");
    char lib_buf[50];
    strncpy(lib_buf, "Standard Library", sizeof(lib_buf) - 1);
    lib_buf[sizeof(lib_buf) - 1] = '\0';
    printf("  strncpy: \"%s\"\n", lib_buf);
    printf("  strlen: %zu\n", strlen(lib_buf));
    printf("  strcmp(\"abc\", \"abd\"): %d\n", strcmp("abc", "abd"));
    printf("  strchr(\"hello\", 'l'): \"%s\"\n", strchr("hello", 'l'));
    printf("  strstr(\"hello world\", \"wor\"): \"%s\"\n", strstr("hello world", "wor"));
}

void demo_string_conversion(void)
{
    print_separator("字符串转换演示");

    printf("[字符串转整数]\n");
    const char *num_strs[] = {"12345", "-6789", "  42", "+100"};
    for (size_t i = 0; i < ARRAY_SIZE(num_strs); i++) {
        int32_t value = string_to_int(num_strs[i]);
        printf("  \"%s\" -> %d\n", num_strs[i], value);
    }

    printf("\n[整数转字符串]\n");
    int32_t numbers[] = {12345, -6789, 0, 42};
    char buffer[20];
    for (size_t i = 0; i < ARRAY_SIZE(numbers); i++) {
        int_to_string(numbers[i], buffer, sizeof(buffer));
        printf("  %d -> \"%s\"\n", numbers[i], buffer);
    }

    printf("\n[标准库转换函数]\n");
    printf("  atoi(\"12345\"): %d\n", atoi("12345"));
    printf("  atol(\"1234567890\"): %ld\n", atol("1234567890"));
    printf("  atof(\"3.14159\"): %f\n", atof("3.14159"));
    printf("  strtol(\"FF\", NULL, 16): %ld (十六进制)\n", strtol("FF", NULL, 16));
    printf("  strtoul(\"1010\", NULL, 2): %lu (二进制)\n", strtoul("1010", NULL, 2));

    printf("\n[格式化字符串]\n");
    char fmt_buf[100];
    int32_t int_val = 42;
    float float_val = 3.14159f;
    snprintf(fmt_buf, sizeof(fmt_buf), "int=%d, float=%.2f, hex=0x%02X", 
             int_val, float_val, int_val);
    printf("  snprintf: \"%s\"\n", fmt_buf);
}

void demo_buffer_operations(void)
{
    print_separator("缓冲区操作演示");

    printf("[内存设置]\n");
    uint8_t buffer[16];
    memset(buffer, 0xAA, sizeof(buffer));
    printf("  memset(0xAA): ");
    for (size_t i = 0; i < sizeof(buffer); i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    printf("\n[内存复制]\n");
    uint8_t src[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t dest[5];
    memcpy(dest, src, sizeof(src));
    printf("  memcpy: ");
    for (size_t i = 0; i < sizeof(dest); i++) {
        printf("%02X ", dest[i]);
    }
    printf("\n");

    printf("\n[内存移动]\n");
    uint8_t overlap[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    printf("  移动前: ");
    for (size_t i = 0; i < sizeof(overlap); i++) {
        printf("%d ", overlap[i]);
    }
    printf("\n");
    memmove(overlap, overlap + 3, 7);
    printf("  memmove(overlap, overlap+3, 7): ");
    for (size_t i = 0; i < sizeof(overlap); i++) {
        printf("%d ", overlap[i]);
    }
    printf("\n");

    printf("\n[内存比较]\n");
    uint8_t buf1[] = {0x01, 0x02, 0x03};
    uint8_t buf2[] = {0x01, 0x02, 0x04};
    printf("  memcmp(buf1, buf2, 3): %d\n", memcmp(buf1, buf2, 3));

    printf("\n[嵌入式应用]\n");
    printf("  - 数据包构造与解析\n");
    printf("  - 寄存器配置\n");
    printf("  - DMA缓冲区管理\n");
    printf("  - 通信协议实现\n");
}

#define RING_BUFFER_SIZE 16

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    size_t head;
    size_t tail;
    size_t count;
} RingBuffer_t;

static void ring_buffer_init(RingBuffer_t *rb)
{
    if (rb == NULL) {
        return;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    memset(rb->buffer, 0, RING_BUFFER_SIZE);
}

static bool ring_buffer_write(RingBuffer_t *rb, uint8_t data)
{
    if (rb == NULL || rb->count >= RING_BUFFER_SIZE) {
        return false;
    }
    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
    rb->count++;
    return true;
}

static bool ring_buffer_read(RingBuffer_t *rb, uint8_t *data)
{
    if (rb == NULL || data == NULL || rb->count == 0) {
        return false;
    }
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
    rb->count--;
    return true;
}

static size_t ring_buffer_available(const RingBuffer_t *rb)
{
    if (rb == NULL) {
        return 0;
    }
    return rb->count;
}

void demo_ring_buffer(void)
{
    print_separator("环形缓冲区演示");

    printf("[环形缓冲区原理]\n");
    printf("  - 固定大小的FIFO缓冲区\n");
    printf("  - 头指针指向写入位置\n");
    printf("  - 尾指针指向读取位置\n");
    printf("  - 循环利用空间, 无需移动数据\n");

    printf("\n[环形缓冲区操作]\n");
    RingBuffer_t rb;
    ring_buffer_init(&rb);

    printf("  写入数据: 1, 2, 3, 4, 5\n");
    for (uint8_t i = 1; i <= 5; i++) {
        ring_buffer_write(&rb, i);
    }
    printf("  可用数据: %zu 字节\n", ring_buffer_available(&rb));

    printf("  读取数据: ");
    uint8_t data;
    while (ring_buffer_read(&rb, &data)) {
        printf("%d ", data);
    }
    printf("\n");
    printf("  可用数据: %zu 字节\n", ring_buffer_available(&rb));

    printf("\n  写入更多数据: 10-20\n");
    for (uint8_t i = 10; i <= 20; i++) {
        if (!ring_buffer_write(&rb, i)) {
            printf("  缓冲区满, 停止写入\n");
            break;
        }
    }
    printf("  可用数据: %zu 字节\n", ring_buffer_available(&rb));

    printf("\n[嵌入式应用]\n");
    printf("  - 串口接收缓冲\n");
    printf("  - 数据采集缓存\n");
    printf("  - 音频/视频流缓冲\n");
    printf("  - 事件队列\n");
}
