/**
 * @file bit_operation.c
 * @brief 位运算与嵌入式特性模块实现
 * @details 演示C语言位运算和嵌入式开发特性，涵盖以下核心内容：
 *          - 位运算基础：与(&)、或(|)、异或(^)、取反(~)
 *          - 移位操作：左移(<<)、右移(>>)
 *          - 位操作技巧：置位、清零、翻转、测试
 *          - 位域：精确控制位宽、寄存器映射
 *          - volatile关键字：防止编译器优化
 *          - 寄存器访问：直接地址操作
 *          - 大小端：字节序判断和转换
 *          - 位掩码：标志位管理
 *          - 位操作宏：常用位操作封装
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 硬件寄存器操作：配置外设、读取状态
 * - 协议解析：通信协议中的位字段
 * - 内存优化：节省存储空间
 * - 性能优化：位运算比算术运算更快
 * - 标志位管理：状态标志、错误标志
 * 
 * @section best_practices 最佳实践
 * - 使用无符号类型进行位运算
 * - 注意移位操作的边界
 * - volatile用于硬件寄存器访问
 * - 使用宏封装常用位操作
 * - 注意编译器的位域布局差异
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @warning 有符号数的右移行为依赖编译器实现
 * @note 位域在跨平台时需注意兼容性
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "bit_operation.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <string.h>

/*============================================================================*/
/*                           位操作函数实现                                     */
/*============================================================================*/

/**
 * @brief 置位（设置位为1）
 * @details 将指定位置1，其他位保持不变。
 *          使用按位或操作实现。
 * 
 * @param value 要操作的值指针，不能为NULL
 * @param bit 位索引（0-based，从最低位开始）
 * 
 * @code
 * uint8_t flags = 0x00;
 * bit_set(&flags, 3);  // flags = 0x08
 * bit_set(&flags, 0);  // flags = 0x09
 * @endcode
 */
void bit_set(uint32_t *value, uint8_t bit)
{
    if (value == NULL || bit >= 32) {
        return;
    }
    *value |= ((uint32_t)1U << bit);
}

/**
 * @brief 清零（设置位为0）
 * @details 将指定位置0，其他位保持不变。
 *          使用按位与操作实现。
 * 
 * @param value 要操作的值指针，不能为NULL
 * @param bit 位索引（0-based，从最低位开始）
 * 
 * @code
 * uint8_t flags = 0xFF;
 * bit_clear(&flags, 3);  // flags = 0xF7
 * @endcode
 */
void bit_clear(uint32_t *value, uint8_t bit)
{
    if (value == NULL || bit >= 32) {
        return;
    }
    *value &= ~((uint32_t)1U << bit);
}

/**
 * @brief 翻转位
 * @details 将指定位取反，0变1，1变0。
 *          使用按位异或操作实现。
 * 
 * @param value 要操作的值指针，不能为NULL
 * @param bit 位索引（0-based，从最低位开始）
 * 
 * @code
 * uint8_t flags = 0x0F;
 * bit_toggle(&flags, 4);  // flags = 0x1F
 * bit_toggle(&flags, 4);  // flags = 0x0F
 * @endcode
 */
void bit_toggle(uint32_t *value, uint8_t bit)
{
    if (value == NULL || bit >= 32) {
        return;
    }
    *value ^= ((uint32_t)1U << bit);
}

/**
 * @brief 测试位
 * @details 检查指定位是否为1。
 * 
 * @param value 要测试的值
 * @param bit 位索引（0-based，从最低位开始）
 * @return 1表示该位为1，0表示该位为0
 * 
 * @code
 * uint8_t flags = 0x05;
 * if (bit_test(flags, 0)) {
 *     printf("第0位为1\n");
 * }
 * @endcode
 */
uint8_t bit_test(uint32_t value, uint8_t bit)
{
    if (bit >= 32) {
        return 0;
    }
    return (uint8_t)((value & ((uint32_t)1U << bit)) != 0U);
}

/**
 * @brief 设置位域
 * @details 设置指定范围内的位值。
 *          例如：设置bit[7:4] = value。
 * 
 * @param reg 寄存器值指针，不能为NULL
 * @param start 起始位（包含）
 * @param width 位宽
 * @param value 要设置的值
 * 
 * @code
 * uint32_t reg = 0;
 * bitfield_set(&reg, 4, 4, 0x0A);  // 设置bit[7:4] = 0x0A
 * // reg = 0x000000A0
 * @endcode
 */
void bitfield_set(uint32_t *reg, uint8_t start, uint8_t width, uint32_t value)
{
    if (reg == NULL || start >= 32 || width == 0 || (start + width) > 32) {
        return;
    }
    
    uint32_t mask = ((1U << width) - 1U) << start;
    *reg = (*reg & ~mask) | ((value << start) & mask);
}

/**
 * @brief 获取位域
 * @details 获取指定范围内的位值。
 * 
 * @param reg 寄存器值
 * @param start 起始位（包含）
 * @param width 位宽
 * @return 位域值
 * 
 * @code
 * uint32_t reg = 0x000000A5;
 * uint32_t val = bitfield_get(reg, 4, 4);  // 获取bit[7:4]
 * // val = 0x0A
 * @endcode
 */
uint32_t bitfield_get(uint32_t reg, uint8_t start, uint8_t width)
{
    if (start >= 32 || width == 0 || (start + width) > 32) {
        return 0;
    }
    
    uint32_t mask = (1U << width) - 1U;
    return (reg >> start) & mask;
}

/**
 * @brief 统计置位个数
 * @details 统计二进制表示中1的个数（Population Count）。
 *          使用Brian Kernighan算法，效率较高。
 * 
 * @param value 要统计的值
 * @return 1的个数
 * 
 * @note 时间复杂度O(k)，k为置位个数
 * 
 * @code
 * uint32_t val = 0x55;  // 二进制: 01010101
 * printf("置位个数: %d\n", bit_count(val));  // 输出: 4
 * @endcode
 */
uint8_t bit_count(uint32_t value)
{
    uint8_t count = 0;
    while (value != 0U) {
        value &= (value - 1U);
        count++;
    }
    return count;
}

/**
 * @brief 反转字节中的位顺序
 * @details 将字节的位顺序反转，如0b11010010变为0b01001011。
 *          常用于通信协议和数据转换。
 * 
 * @param value 要反转的字节值
 * @return 位反转后的值
 * 
 * @code
 * uint8_t val = 0xF0;  // 二进制: 11110000
 * uint8_t rev = reverse_bits(val);  // rev = 0x0F (二进制: 00001111)
 * @endcode
 */
uint8_t reverse_bits(uint8_t value)
{
    uint8_t result = 0;
    for (uint8_t i = 0; i < 8; i++) {
        result = (uint8_t)((result << 1) | (value & 1U));
        value = (uint8_t)(value >> 1);
    }
    return result;
}

/**
 * @brief 反转位顺序（32位）
 * @details 将32位整数的位顺序反转。
 *          例如：bit[0]与bit[31]交换，bit[1]与bit[30]交换...
 * 
 * @param value 要反转的值
 * @return 反转后的值
 * 
 * @code
 * uint32_t val = 0x80000001;
 * uint32_t rev = bit_reverse(val);  // rev = 0x80000001
 * @endcode
 */
uint32_t bit_reverse(uint32_t value)
{
    uint32_t result = 0;
    for (uint8_t i = 0; i < 32; i++) {
        result <<= 1;
        result |= (value & 1U);
        value >>= 1;
    }
    return result;
}

/**
 * @brief 字节序转换（16位）
 * @details 将16位整数的大小端转换。
 *          常用于网络通信和跨平台数据交换。
 * 
 * @param value 要转换的值
 * @return 转换后的值
 * 
 * @code
 * uint16_t val = 0x1234;
 * uint16_t swapped = byte_swap16(val);  // swapped = 0x3412
 * @endcode
 */
uint16_t byte_swap16(uint16_t value)
{
    return (uint16_t)((value >> 8) | (value << 8));
}

/**
 * @brief 字节序转换（32位）
 * @details 将32位整数的大小端转换。
 *          常用于网络通信和跨平台数据交换。
 * 
 * @param value 要转换的值
 * @return 转换后的值
 * 
 * @code
 * uint32_t val = 0x12345678;
 * uint32_t swapped = byte_swap32(val);  // swapped = 0x78563412
 * @endcode
 */
uint32_t byte_swap32(uint32_t value)
{
    return ((value >> 24) & 0x000000FFU) |
           ((value >> 8)  & 0x0000FF00U) |
           ((value << 8)  & 0x00FF0000U) |
           ((value << 24) & 0xFF000000U);
}

/**
 * @brief 判断系统字节序
 * @details 检测当前系统是大端还是小端。
 * 
 * @return 0表示小端，1表示大端
 * 
 * @code
 * if (is_big_endian()) {
 *     printf("大端模式\n");
 * } else {
 *     printf("小端模式\n");
 * }
 * @endcode
 */
uint8_t is_big_endian(void)
{
    uint16_t test = 0x0001U;
    return (uint8_t)(*((uint8_t *)&test) == 0U);
}

/**
 * @brief 循环左移
 * @details 将值循环左移指定位数。
 *          移出的位会从右边重新进入。
 * 
 * @param value 要移位的值
 * @param shift 移位位数
 * @return 移位后的值
 * 
 * @code
 * uint32_t val = 0x80000001;
 * uint32_t result = rotate_left(val, 1);  // result = 0x00000003
 * @endcode
 */
uint32_t rotate_left(uint32_t value, uint8_t shift)
{
    shift &= 31U;
    if (shift == 0U) {
        return value;
    }
    return (value << shift) | (value >> (32U - shift));
}

/**
 * @brief 循环右移
 * @details 将值循环右移指定位数。
 *          移出的位会从左边重新进入。
 * 
 * @param value 要移位的值
 * @param shift 移位位数
 * @return 移位后的值
 * 
 * @code
 * uint32_t val = 0x00000003;
 * uint32_t result = rotate_right(val, 1);  // result = 0x80000001
 * @endcode
 */
uint32_t rotate_right(uint32_t value, uint8_t shift)
{
    shift &= 31U;
    if (shift == 0U) {
        return value;
    }
    return (value >> shift) | (value << (32U - shift));
}

/**
 * @brief 打印二进制表示
 * @details 将整数以二进制格式打印输出。
 *          每隔4位添加分隔符，便于阅读。
 * 
 * @param value 要打印的值
 * @param bits 打印位数（通常为8、16、32）
 * 
 * @code
 * print_binary(0xA5, 8);
 * // 输出: 1010_0101
 * @endcode
 */
void print_binary(uint32_t value, uint8_t bits)
{
    if (bits == 0 || bits > 32) {
        bits = 32;
    }
    
    for (int8_t i = (int8_t)(bits - 1); i >= 0; i--) {
        printf("%d", (int)((value >> i) & 1U));
        if ((i > 0) && ((i % 4) == 0)) {
            printf("_");
        }
    }
}

/*============================================================================*/
/*                           演示函数实现                                       */
/*============================================================================*/

/**
 * @brief 计算值中设置的位数量
 * @details 计算二进制表示中1的个数（人口计数/汉明重量）。
 *          使用Brian Kernighan算法，效率较高。
 * 
 * @param value 要计算的值
 * @return 设置的位数量
 * 
 * @code
 * uint8_t count = count_set_bits(0xF0);  // count = 4
 * @endcode
 */
uint8_t count_set_bits(uint32_t value)
{
    uint8_t count = 0;
    while (value != 0U) {
        value &= (value - 1U);
        count++;
    }
    return count;
}

/**
 * @brief 查找第一个设置的位（从低位开始）
 * @details 找到最低位的1的位置。
 * 
 * @param value 要查找的值
 * @return 第一个设置位的位置（1-based），如果值为0返回0
 * 
 * @code
 * uint8_t pos = find_first_set(0x10);  // pos = 5 (bit 4)
 * @endcode
 */
uint8_t find_first_set(uint32_t value)
{
    if (value == 0U) {
        return 0;
    }
    
    uint8_t pos = 1;
    while ((value & 1U) == 0U) {
        value >>= 1;
        pos++;
    }
    return pos;
}

/**
 * @brief 查找最后一个设置的位（从高位开始）
 * @details 找到最高位的1的位置。
 * 
 * @param value 要查找的值
 * @return 最后一个设置位的位置（1-based），如果值为0返回0
 * 
 * @code
 * uint8_t pos = find_last_set(0x10);  // pos = 5 (bit 4)
 * @endcode
 */
uint8_t find_last_set(uint32_t value)
{
    if (value == 0U) {
        return 0;
    }
    
    uint8_t pos = 0;
    while (value != 0U) {
        value >>= 1;
        pos++;
    }
    return pos;
}

/**
 * @brief 判断值是否为2的幂
 * @details 检查值是否为2的整数次幂。
 *          2的幂的二进制表示只有一个1。
 * 
 * @param value 要判断的值
 * @return true表示是2的幂，false表示不是
 * 
 * @code
 * bool result = is_power_of_two(16);  // result = true
 * bool result2 = is_power_of_two(15); // result2 = false
 * @endcode
 */
bool is_power_of_two(uint32_t value)
{
    return (value != 0U) && ((value & (value - 1U)) == 0U);
}

/**
 * @brief 获取大于等于指定值的最小2的幂
 * @details 计算不小于value的最小2的幂。
 *          常用于内存对齐、缓冲区大小计算。
 * 
 * @param value 输入值
 * @return 大于等于value的最小2的幂
 * 
 * @code
 * uint32_t result = next_power_of_two(5);   // result = 8
 * uint32_t result2 = next_power_of_two(16); // result2 = 16
 * @endcode
 */
uint32_t next_power_of_two(uint32_t value)
{
    if (value == 0U) {
        return 1U;
    }
    
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value++;
    
    return value;
}

/**
 * @brief 设置寄存器中的位
 * @details 将寄存器中掩码对应的位设置为1。
 *          使用volatile确保编译器不优化寄存器访问。
 * 
 * @param reg 寄存器指针
 * @param mask 位掩码
 * 
 * @code
 * set_bits(&GPIO->MODER, (1 << 5));  // 设置第5位
 * @endcode
 */
void set_bits(volatile uint32_t *reg, uint32_t mask)
{
    if (reg != NULL) {
        *reg |= mask;
    }
}

/**
 * @brief 清除寄存器中的位
 * @details 将寄存器中掩码对应的位清零。
 * 
 * @param reg 寄存器指针
 * @param mask 位掩码
 * 
 * @code
 * clear_bits(&GPIO->MODER, (1 << 5));  // 清除第5位
 * @endcode
 */
void clear_bits(volatile uint32_t *reg, uint32_t mask)
{
    if (reg != NULL) {
        *reg &= ~mask;
    }
}

/**
 * @brief 切换寄存器中的位
 * @details 将寄存器中掩码对应的位取反。
 * 
 * @param reg 寄存器指针
 * @param mask 位掩码
 */
void toggle_bits(volatile uint32_t *reg, uint32_t mask)
{
    if (reg != NULL) {
        *reg ^= mask;
    }
}

/**
 * @brief 读取寄存器中的位
 * @details 读取寄存器中掩码对应的位的值。
 * 
 * @param reg 寄存器指针
 * @param mask 位掩码
 * @return 掩码对应的位值
 */
uint32_t read_bits(volatile uint32_t *reg, uint32_t mask)
{
    if (reg == NULL) {
        return 0U;
    }
    return *reg & mask;
}

/**
 * @brief 写入寄存器中的位
 * @details 将值写入寄存器中掩码对应的位置。
 *          先清除掩码位，再写入新值。
 * 
 * @param reg 寄存器指针
 * @param mask 位掩码
 * @param value 要写入的值
 * 
 * @code
 * write_bits(&GPIO->MODER, 0x3 << 10, 0x2 << 10);  // 写入多位
 * @endcode
 */
void write_bits(volatile uint32_t *reg, uint32_t mask, uint32_t value)
{
    if (reg != NULL) {
        *reg = (*reg & ~mask) | (value & mask);
    }
}

/**
 * @brief 位运算基础演示
 * @details 展示位运算的基本概念：
 *          - 按位与(&)：清零、取位
 *          - 按位或(|)：置位
 *          - 按位异或(^)：翻转、比较
 *          - 按位取反(~)：反转
 *          - 左移(<<)：乘以2的n次方
 *          - 右移(>>)：除以2的n次方
 */
void demo_bit_basics(void)
{
    print_separator("位运算基础演示");

    printf("[位运算符]\n");
    uint8_t a = 0b11001010;
    uint8_t b = 0b10110101;

    printf("  a = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", BYTE_TO_BINARY(a), a);
    printf("  b = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", BYTE_TO_BINARY(b), b);

    printf("\n  与 (&):  a & b  = 0b" BYTE_TO_BINARY_FMT "\n", BYTE_TO_BINARY(a & b));
    printf("  或 (|):  a | b  = 0b" BYTE_TO_BINARY_FMT "\n", BYTE_TO_BINARY(a | b));
    printf("  异或(^): a ^ b  = 0b" BYTE_TO_BINARY_FMT "\n", BYTE_TO_BINARY(a ^ b));
    printf("  取反(~): ~a     = 0b" BYTE_TO_BINARY_FMT "\n", BYTE_TO_BINARY((uint8_t)(~a)));
    printf("  左移(<<): a << 2 = 0b" BYTE_TO_BINARY_FMT "\n", BYTE_TO_BINARY((uint8_t)(a << 2)));
    printf("  右移(>>): a >> 2 = 0b" BYTE_TO_BINARY_FMT "\n", BYTE_TO_BINARY((uint8_t)(a >> 2)));

    printf("\n[移位运算规则]\n");
    printf("  左移: 低位补0, 相当于乘以2^n\n");
    printf("  右移: 高位补符号位(有符号)或0(无符号)\n");
    printf("  移位量 >= 类型位数时, 行为未定义\n");

    printf("\n[位运算优先级]\n");
    printf("  位运算优先级低于比较运算符!\n");
    printf("  错误: if (flags & 0x01 == 0)  // 先算 ==\n");
    printf("  正确: if ((flags & 0x01) == 0) // 加括号\n");
}

void demo_bit_tricks(void)
{
    print_separator("位运算技巧演示");

    printf("[判断奇偶]\n");
    for (int32_t i = 0; i <= 5; i++) {
        printf("  %d 是%s数\n", i, (i & 1) ? "奇" : "偶");
    }

    printf("\n[交换两数(无临时变量)]\n");
    int32_t x = 10, y = 20;
    printf("  交换前: x=%d, y=%d\n", x, y);
    x ^= y;
    y ^= x;
    x ^= y;
    printf("  交换后: x=%d, y=%d\n", x, y);

    printf("\n[计算置位数量]\n");
    uint32_t val = 0xDEADBEEF;
    printf("  0x%08X 有 %d 个置位\n", val, count_set_bits(val));

    printf("\n[判断2的幂]\n");
    uint32_t test_vals[] = {0, 1, 2, 3, 4, 8, 16, 31, 64};
    for (size_t i = 0; i < ARRAY_SIZE(test_vals); i++) {
        printf("  %u 是%s2的幂\n", test_vals[i], 
               is_power_of_two(test_vals[i]) ? "" : "非");
    }

    printf("\n[下一个2的幂]\n");
    uint32_t np2_vals[] = {0, 1, 5, 17, 100, 1000};
    for (size_t i = 0; i < ARRAY_SIZE(np2_vals); i++) {
        printf("  %u -> %u\n", np2_vals[i], next_power_of_two(np2_vals[i]));
    }

    printf("\n[位反转]\n");
    uint8_t orig = 0b10110100;
    printf("  0b" BYTE_TO_BINARY_FMT " 反转后: 0b" BYTE_TO_BINARY_FMT "\n", 
           BYTE_TO_BINARY(orig), BYTE_TO_BINARY(reverse_bits(orig)));

    printf("\n[循环移位]\n");
    uint32_t rot_val = 0x12345678;
    printf("  原值: 0x%08X\n", rot_val);
    printf("  循环左移4位: 0x%08X\n", rotate_left(rot_val, 4));
    printf("  循环右移4位: 0x%08X\n", rotate_right(rot_val, 4));

    printf("\n[查找位位置]\n");
    uint32_t ffs_val = 0x00001000;
    printf("  0x%08X: 首个置位位置=%d, 最后置位位置=%d\n", 
           ffs_val, find_first_set(ffs_val), find_last_set(ffs_val));
}

void demo_register_operations(void)
{
    print_separator("寄存器操作演示");

    printf("[模拟寄存器定义]\n");
    printf("  #define REG_BASE    0x40000000\n");
    printf("  #define GPIOA_BASE  (REG_BASE + 0x0000)\n");
    printf("  #define GPIOA_MODER (*(volatile uint32_t*)(GPIOA_BASE + 0x00))\n");
    printf("  #define GPIOA_ODR   (*(volatile uint32_t*)(GPIOA_BASE + 0x14))\n");

    printf("\n[位操作宏]\n");
    uint32_t simulated_reg = 0x00000000;
    
    printf("  初始值: 0x%08X\n", simulated_reg);
    
    set_bits(&simulated_reg, 0x00000001U);
    printf("  设置位0: 0x%08X\n", simulated_reg);
    
    set_bits(&simulated_reg, 0x00000004U);
    printf("  设置位2: 0x%08X\n", simulated_reg);
    
    clear_bits(&simulated_reg, 0x00000001U);
    printf("  清除位0: 0x%08X\n", simulated_reg);
    
    toggle_bits(&simulated_reg, 0x00000004U);
    printf("  翻转位2: 0x%08X\n", simulated_reg);

    printf("\n[位域操作]\n");
    simulated_reg = 0x00000000;
    write_bits(&simulated_reg, 0x00000003U, 0x00000002U);
    printf("  写入位[1:0]=2: 0x%08X\n", simulated_reg);
    uint32_t field = read_bits(&simulated_reg, 0x00000003U);
    printf("  读取位[1:0]: %u\n", field);

    printf("\n[常用寄存器操作模式]\n");
    printf("  读-修改-写: val = REG; val |= BIT; REG = val;\n");
    printf("  直接写: REG = value;\n");
    printf("  原子操作: 某些MCU支持位带操作\n");

    printf("\n[嵌入式寄存器访问原则]\n");
    printf("  1. 使用volatile防止优化\n");
    printf("  2. 使用正确的数据宽度\n");
    printf("  3. 注意访问对齐\n");
    printf("  4. 遵循硬件时序要求\n");
}

void demo_bit_manipulation(void)
{
    print_separator("位操作实战演示");

    printf("[GPIO模式配置示例]\n");
    typedef enum {
        GPIO_MODE_INPUT = 0,
        GPIO_MODE_OUTPUT = 1,
        GPIO_MODE_AF = 2,
        GPIO_MODE_ANALOG = 3
    } GpioMode_t;

    uint32_t moder = 0x00000000;
    printf("  初始MODER: 0x%08X\n", moder);

    int32_t pin = 5;
    GpioMode_t mode = GPIO_MODE_OUTPUT;
    
    uint32_t mode_mask = 0x3U << (pin * 2);
    uint32_t mode_value = (uint32_t)mode << (pin * 2);
    moder = (moder & ~mode_mask) | mode_value;
    printf("  设置PIN%d为输出模式: 0x%08X\n", pin, moder);

    pin = 10;
    mode = GPIO_MODE_AF;
    mode_mask = 0x3U << (pin * 2);
    mode_value = (uint32_t)mode << (pin * 2);
    moder = (moder & ~mode_mask) | mode_value;
    printf("  设置PIN%d为复用模式: 0x%08X\n", pin, moder);

    printf("\n[状态标志检查]\n");
    uint32_t status_reg = 0x00000015;
    printf("  状态寄存器: 0x%08X\n", status_reg);
    printf("  位0 (RXNE): %s\n", (status_reg & 0x01U) ? "置位" : "清除");
    printf("  位2 (TXE): %s\n", (status_reg & 0x04U) ? "置位" : "清除");
    printf("  位4 (IDLE): %s\n", (status_reg & 0x10U) ? "置位" : "清除");

    printf("\n[数据打包]\n");
    uint8_t byte0 = 0x12;
    uint8_t byte1 = 0x34;
    uint8_t byte2 = 0x56;
    uint8_t byte3 = 0x78;
    uint32_t packed = ((uint32_t)byte3 << 24) | 
                      ((uint32_t)byte2 << 16) | 
                      ((uint32_t)byte1 << 8) | 
                      (uint32_t)byte0;
    printf("  打包: 0x%02X %02X %02X %02X -> 0x%08X\n", 
           byte3, byte2, byte1, byte0, packed);

    printf("\n[数据解包]\n");
    printf("  解包: 0x%08X -> 0x%02X 0x%02X 0x%02X 0x%02X\n", 
           packed,
           (uint8_t)((packed >> 24) & 0xFF),
           (uint8_t)((packed >> 16) & 0xFF),
           (uint8_t)((packed >> 8) & 0xFF),
           (uint8_t)(packed & 0xFF));

    printf("\n[CRC计算示例]\n");
    uint8_t crc_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t crc = calc_crc8(crc_data, ARRAY_SIZE(crc_data));
    printf("  数据CRC-8: 0x%02X\n", crc);
}

void demo_volatile_usage(void)
{
    print_separator("volatile关键字演示");

    printf("[volatile的作用]\n");
    printf("  告诉编译器该变量可能被意外修改\n");
    printf("  防止编译器优化相关代码\n");

    printf("\n[需要volatile的场景]\n");
    printf("  1. 硬件寄存器访问\n");
    printf("     volatile uint32_t *reg = (volatile uint32_t *)0x40000000;\n");
    printf("  2. 中断服务程序与主程序共享的变量\n");
    printf("     volatile bool g_flag = false;\n");
    printf("  3. 多线程共享变量\n");
    printf("     volatile int g_counter;\n");
    printf("  4. setjmp/longjmp涉及的变量\n");

    printf("\n[优化示例]\n");
    printf("  无volatile:\n");
    printf("    while (*reg & FLAG) { }  // 可能被优化为死循环或空循环\n");
    printf("  有volatile:\n");
    printf("    while (*(volatile uint32_t*)reg & FLAG) { }  // 每次都读取\n");

    printf("\n[volatile指针]\n");
    printf("  volatile int *p;      // 指向volatile数据的指针\n");
    printf("  int *volatile p;      // volatile指针,指向普通数据\n");
    printf("  volatile int *volatile p; // 两者都是volatile\n");

    printf("\n[注意事项]\n");
    printf("  volatile不保证原子性\n");
    printf("  多线程同步需要使用锁或原子操作\n");
    printf("  过度使用可能影响性能\n");

    printf("\n[实际示例]\n");
    volatile uint32_t simulated_hw_reg = 0;
    simulated_hw_reg = 0x01;
    printf("  写入寄存器: 0x%08X\n", simulated_hw_reg);
    uint32_t read_val = simulated_hw_reg;
    printf("  读取寄存器: 0x%08X\n", read_val);
}

void demo_interrupt_context(void)
{
    print_separator("中断上下文演示");

    printf("[中断服务程序(ISR)规则]\n");
    printf("  1. 尽可能短小快速\n");
    printf("  2. 不要使用阻塞函数\n");
    printf("  3. 不要使用malloc/free\n");
    printf("  4. 使用volatile声明共享变量\n");
    printf("  5. 使用适当的同步机制\n");

    printf("\n[ISR示例框架]\n");
    printf("  volatile bool g_rx_complete = false;\n");
    printf("  volatile uint8_t g_rx_buffer[256];\n");
    printf("  volatile uint16_t g_rx_count = 0;\n");
    printf("\n");
    printf("  void USART1_IRQHandler(void) {\n");
    printf("      if (USART1->SR & USART_SR_RXNE) {\n");
    printf("          uint8_t data = (uint8_t)USART1->DR;\n");
    printf("          if (g_rx_count < sizeof(g_rx_buffer)) {\n");
    printf("              g_rx_buffer[g_rx_count++] = data;\n");
    printf("          }\n");
    printf("      }\n");
    printf("      if (USART1->SR & USART_SR_IDLE) {\n");
    printf("          g_rx_complete = true;\n");
    printf("      }\n");
    printf("  }\n");

    printf("\n[主循环处理]\n");
    printf("  while (1) {\n");
    printf("      if (g_rx_complete) {\n");
    printf("          process_data((uint8_t*)g_rx_buffer, g_rx_count);\n");
    printf("          g_rx_count = 0;\n");
    printf("          g_rx_complete = false;\n");
    printf("      }\n");
    printf("  }\n");

    printf("\n[原子操作考虑]\n");
    printf("  单字节读写通常是原子的\n");
    printf("  多字节数据需要关中断保护\n");
    printf("  或使用编译器内置原子操作\n");

    printf("\n[中断优先级]\n");
    printf("  高优先级ISR可打断低优先级ISR\n");
    printf("  合理设置优先级避免优先级反转\n");
}

void demo_endianness(void)
{
    print_separator("字节序演示");

    printf("[大端序(Big-Endian)]\n");
    printf("  高位字节存储在低地址\n");
    printf("  网络字节序使用大端\n");
    printf("  0x12345678 存储为: 12 34 56 78\n");

    printf("\n[小端序(Little-Endian)]\n");
    printf("  低位字节存储在低地址\n");
    printf("  x86, ARM默认使用小端\n");
    printf("  0x12345678 存储为: 78 56 34 12\n");

    printf("\n[检测本机字节序]\n");
    uint32_t test_val = 0x12345678;
    uint8_t *bytes = (uint8_t *)&test_val;
    printf("  0x12345678 在内存中: %02X %02X %02X %02X\n", 
           bytes[0], bytes[1], bytes[2], bytes[3]);
    if (bytes[0] == 0x78) {
        printf("  本机是小端序\n");
    } else {
        printf("  本机是大端序\n");
    }

    printf("\n[字节序转换]\n");
    uint16_t host16 = 0x1234;
    uint32_t host32 = 0x12345678;
    
    uint16_t net16 = ((host16 & 0xFF00) >> 8) | ((host16 & 0x00FF) << 8);
    uint32_t net32 = ((host32 & 0xFF000000) >> 24) |
                     ((host32 & 0x00FF0000) >> 8) |
                     ((host32 & 0x0000FF00) << 8) |
                     ((host32 & 0x000000FF) << 24);
    
    printf("  htons(0x%04X) = 0x%04X\n", host16, net16);
    printf("  htonl(0x%08X) = 0x%08X\n", host32, net32);

    printf("\n[嵌入式通信注意]\n");
    printf("  不同设备间通信需统一字节序\n");
    printf("  使用网络字节序(大端)作为标准\n");
    printf("  结构体打包时注意字节对齐\n");
}

void demo_bitmask_operations(void)
{
    print_separator("位掩码操作演示");

    printf("[定义位掩码]\n");
    printf("  #define BIT(x)      (1U << (x))\n");
    printf("  #define MASK(h, l)  ((BIT((h)-(l)+1)-1) << (l))\n");

    printf("\n[常用掩码操作]\n");
#define BIT(x) (1U << (x))
#define MASK(h, l) (((1U << ((h) - (l) + 1)) - 1) << (l))

    uint32_t reg = 0x12345678;
    printf("  原始值: 0x%08X\n", reg);
    
    printf("\n  提取位[7:0]: 0x%02X\n", (reg >> 0) & 0xFF);
    printf("  提取位[15:8]: 0x%02X\n", (reg >> 8) & 0xFF);
    printf("  提取位[23:16]: 0x%02X\n", (reg >> 16) & 0xFF);
    printf("  提取位[31:24]: 0x%02X\n", (reg >> 24) & 0xFF);

    printf("\n[掩码应用]\n");
    uint32_t cfg = 0x00000000;
    
    cfg |= (0x5 << 0);
    printf("  设置位[3:0]=5: 0x%08X\n", cfg);
    
    cfg |= (0x3 << 4);
    printf("  设置位[5:4]=3: 0x%08X\n", cfg);
    
    cfg &= ~(0xF << 0);
    cfg |= (0xA << 0);
    printf("  修改位[3:0]=10: 0x%08X\n", cfg);

    printf("\n[权限掩码示例]\n");
    typedef enum {
        PERM_READ    = BIT(0),
        PERM_WRITE   = BIT(1),
        PERM_EXECUTE = BIT(2),
        PERM_ADMIN   = BIT(3)
    } Permission_t;

    uint32_t user_perm = PERM_READ | PERM_WRITE;
    printf("  用户权限: 0x%02X (读+写)\n", user_perm);
    printf("  可读: %s\n", (user_perm & PERM_READ) ? "是" : "否");
    printf("  可执行: %s\n", (user_perm & PERM_EXECUTE) ? "是" : "否");
    printf("  可管理: %s\n", (user_perm & PERM_ADMIN) ? "是" : "否");

    user_perm |= PERM_EXECUTE;
    printf("  添加执行权限后: 0x%02X\n", user_perm);
    
    user_perm &= ~PERM_WRITE;
    printf("  移除写权限后: 0x%02X\n", user_perm);
}
