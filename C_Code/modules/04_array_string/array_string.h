/**
 * @file array_string.h
 * @brief 数组与字符串模块头文件
 * @details 涵盖数组操作、字符串处理、多维数组、环形缓冲区等核心知识点。
 *          数组和字符串是C语言中最常用的数据结构，嵌入式开发中应用广泛。
 * 
 * @section main_features 主要内容
 * - 数组基础：声明、初始化、访问、越界问题
 * - 数组操作：查找、排序、统计
 * - 多维数组：二维数组、数组指针
 * - 字符串基础：字符串表示、结束符'\0'
 * - 字符串操作：复制、连接、比较、转换
 * - 缓冲区操作：安全函数、缓冲区溢出防护
 * - 环形缓冲区：FIFO实现、嵌入式典型应用
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
 * @warning 注意数组越界和缓冲区溢出问题
 * @note 嵌入式系统中常用环形缓冲区处理数据流
 */

#ifndef ARRAY_STRING_H
#define ARRAY_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
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
void demo_array_basics(void);

/**
 * @brief 数组操作演示
 * @details 展示数组的常见操作：
 *          - 查找最大/最小值
 *          - 数组排序（冒泡、选择）
 *          - 数组求和、平均值
 *          - 数组反转
 */
void demo_array_operations(void);

/**
 * @brief 多维数组演示
 * @details 展示多维数组的使用：
 *          - 二维数组声明和初始化
 *          - 二维数组的内存布局
 *          - 多维数组与指针
 *          - 典型应用：矩阵运算、图像处理
 */
void demo_multidimensional_array(void);

/**
 * @brief 字符串基础演示
 * @details 展示字符串的基本概念：
 *          - 字符串字面量
 *          - 字符数组与字符串
 *          - 字符串结束符'\0'
 *          - 字符串长度
 */
void demo_string_basics(void);

/**
 * @brief 字符串操作演示
 * @details 展示字符串的常见操作：
 *          - 字符串复制（安全版本）
 *          - 字符串连接
 *          - 字符串比较
 *          - 大小写转换
 */
void demo_string_operations(void);

/**
 * @brief 字符串转换演示
 * @details 展示字符串与其他类型的转换：
 *          - 字符串转整数（atoi实现）
 *          - 整数转字符串
 *          - 字符串转浮点数
 *          - 格式化字符串
 */
void demo_string_conversion(void);

/**
 * @brief 缓冲区操作演示
 * @details 展示安全的缓冲区操作：
 *          - 安全的字符串函数
 *          - 缓冲区长度检查
 *          - 缓冲区溢出防护
 *          - memset/memcpy/memmove的使用
 */
void demo_buffer_operations(void);

/**
 * @brief 环形缓冲区演示
 * @details 展示环形缓冲区的实现：
 *          - 环形缓冲区的概念
 *          - 读写指针管理
 *          - 满/空判断
 *          - 嵌入式应用：串口接收缓冲
 * @note 环形缓冲区是嵌入式系统常用的数据结构
 */
void demo_ring_buffer(void);

/*============================================================================*/
/*                           数组操作函数                                       */
/*============================================================================*/

/**
 * @brief 查找数组中的最大值索引
 * @param arr 数组指针
 * @param size 数组大小
 * @return 最大值的索引
 * @note 如果数组为空，返回0
 */
size_t array_find_max(const int32_t *arr, size_t size);

/**
 * @brief 查找数组中的最小值索引
 * @param arr 数组指针
 * @param size 数组大小
 * @return 最小值的索引
 * @note 如果数组为空，返回0
 */
size_t array_find_min(const int32_t *arr, size_t size);

/**
 * @brief 冒泡排序
 * @details 对数组进行升序排序
 * @param arr 数组指针
 * @param size 数组大小
 * @note 时间复杂度O(n^2)，适合小规模数据
 */
void array_sort_bubble(int32_t *arr, size_t size);

/**
 * @brief 选择排序
 * @details 对数组进行升序排序
 * @param arr 数组指针
 * @param size 数组大小
 * @note 时间复杂度O(n^2)，交换次数少于冒泡排序
 */
void array_sort_selection(int32_t *arr, size_t size);

/**
 * @brief 计算数组元素之和
 * @param arr 数组指针
 * @param size 数组大小
 * @return 数组元素之和
 */
int32_t array_sum(const int32_t *arr, size_t size);

/**
 * @brief 计算数组元素平均值
 * @param arr 数组指针
 * @param size 数组大小
 * @return 数组元素平均值
 */
double array_average(const int32_t *arr, size_t size);

/*============================================================================*/
/*                           字符串操作函数                                     */
/*============================================================================*/

/**
 * @brief 计算字符串长度
 * @param str 字符串指针
 * @return 字符串长度（不含结束符）
 * @note 类似于标准库strlen
 */
size_t string_length(const char *str);

/**
 * @brief 安全的字符串复制
 * @details 将src复制到dest，最多复制dest_size-1个字符，确保以'\0'结尾
 * @param dest 目标缓冲区
 * @param src 源字符串
 * @param dest_size 目标缓冲区大小
 * @return 成功返回dest，失败返回NULL
 * @note 比strcpy更安全，防止缓冲区溢出
 */
char *string_copy(char *dest, const char *src, size_t dest_size);

/**
 * @brief 安全的字符串连接
 * @details 将src连接到dest末尾，确保不超出缓冲区大小
 * @param dest 目标缓冲区
 * @param src 源字符串
 * @param dest_size 目标缓冲区大小
 * @return 成功返回dest，失败返回NULL
 */
char *string_concat(char *dest, const char *src, size_t dest_size);

/**
 * @brief 字符串比较
 * @details 比较两个字符串的大小
 * @param s1 第一个字符串
 * @param s2 第二个字符串
 * @return s1<s2返回负数，s1==s2返回0，s1>s2返回正数
 */
int32_t string_compare(const char *s1, const char *s2);

/**
 * @brief 字符串转大写
 * @details 将字符串中的所有小写字母转为大写
 * @param str 字符串指针
 * @return 返回str指针
 */
char *string_to_upper(char *str);

/**
 * @brief 字符串转小写
 * @details 将字符串中的所有大写字母转为小写
 * @param str 字符串指针
 * @return 返回str指针
 */
char *string_to_lower(char *str);

/*============================================================================*/
/*                           类型转换函数                                       */
/*============================================================================*/

/**
 * @brief 字符串转整数
 * @details 将字符串转换为整数，支持正负号
 * @param str 数字字符串
 * @return 转换后的整数
 * @note 无效字符串返回0
 */
int32_t string_to_int(const char *str);

/**
 * @brief 整数转字符串
 * @details 将整数转换为字符串
 * @param value 整数值
 * @param str 目标缓冲区
 * @param size 缓冲区大小
 * @note 缓冲区应足够大以容纳结果
 */
void int_to_string(int32_t value, char *str, size_t size);

#ifdef __cplusplus
}
#endif

#endif
