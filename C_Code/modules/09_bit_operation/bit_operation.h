/**
 * @file bit_operation.h
 * @brief 位运算与嵌入式特性模块头文件
 * @details 涵盖位操作、寄存器操作、volatile关键字、大小端等嵌入式核心特性。
 *          位运算是嵌入式开发的基础技能，直接操作硬件寄存器必备。
 * 
 * @section main_features 主要内容
 * - 位运算基础：&、|、^、~、<<、>>
 * - 位操作技巧：位反转、位计数、位查找
 * - 寄存器操作：位设置、清除、切换、读取
 * - 位域操作：位掩码、位提取、位插入
 * - volatile关键字：防止编译器优化
 * - 中断上下文：中断安全编程
 * - 大小端：字节序转换
 * - 位掩码操作：标志位管理
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 硬件寄存器直接操作
 * - 外设配置（GPIO、定时器等）
 * - 协议解析（位域提取）
 * - 标志位管理
 * - 性能优化（位运算替代乘除）
 * 
 * @section best_practices 最佳实践
 * - 使用volatile修饰硬件寄存器指针
 * - 位操作使用无符号类型
 * - 注意移位操作的溢出
 * - 使用命名常量定义位掩码
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @warning 位操作必须使用无符号整数类型
 * @note volatile仅防止编译器优化，不保证原子性
 */

#ifndef BIT_OPERATION_H
#define BIT_OPERATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 位运算基础演示
 * @details 展示位运算的基本操作：
 *          - 按位与&：清零特定位、取特定位
 *          - 按位或|：设置特定位
 *          - 按位异或^：切换特定位、判断不同
 *          - 按位取反~：位反转
 *          - 左移<<：乘2^n、位对齐
 *          - 右移>>：除2^n、位提取
 */
void demo_bit_basics(void);

/**
 * @brief 位操作技巧演示
 * @details 展示常用的位操作技巧：
 *          - 判断奇偶
 *          - 交换两数（不用临时变量）
 *          - 取绝对值
 *          - 判断是否为2的幂
 *          - 快速乘除
 */
void demo_bit_tricks(void);

/**
 * @brief 寄存器操作演示
 * @details 展示硬件寄存器的位操作：
 *          - 设置位：REG |= (1 << n)
 *          - 清除位：REG &= ~(1 << n)
 *          - 切换位：REG ^= (1 << n)
 *          - 读取位：(REG >> n) & 1
 *          - 写入位：REG = (REG & ~mask) | (value << shift)
 * @note 实际嵌入式开发中寄存器指针需用volatile修饰
 */
void demo_register_operations(void);

/**
 * @brief 位操作函数演示
 * @details 展示封装的位操作函数：
 *          - 位反转
 *          - 位计数（计算1的个数）
 *          - 查找第一个/最后一个设置的位
 *          - 循环移位
 */
void demo_bit_manipulation(void);

/**
 * @brief volatile关键字演示
 * @details 展示volatile的使用场景：
 *          - 硬件寄存器访问
 *          - 中断服务程序共享变量
 *          - 多线程共享变量
 *          - 防止编译器优化
 * @note volatile不保证原子性，需要时配合锁使用
 */
void demo_volatile_usage(void);

/**
 * @brief 中断上下文演示
 * @details 展示中断编程的注意事项：
 *          - 中断服务程序的特点
 *          - 中断安全的数据访问
 *          - 中断标志位处理
 *          - 中断与主程序的通信
 * @warning 中断服务程序中避免复杂操作
 */
void demo_interrupt_context(void);

/**
 * @brief 大小端演示
 * @details 展示字节序的概念和转换：
 *          - 大端模式（Big-Endian）
 *          - 小端模式（Little-Endian）
 *          - 字节序检测
 *          - 字节序转换
 * @note ARM Cortex-M通常是小端模式
 */
void demo_endianness(void);

/**
 * @brief 位掩码操作演示
 * @details 展示位掩码的使用：
 *          - 定义位掩码常量
 *          - 多位操作
 *          - 位域提取
 *          - 标志位组合
 */
void demo_bitmask_operations(void);

/*============================================================================*/
/*                           位操作函数                                         */
/*============================================================================*/

/**
 * @brief 反转字节中的位顺序
 * @details 将字节的位顺序反转，如0b11010010变为0b01001011
 * @param value 要反转的字节值
 * @return 位反转后的值
 */
uint8_t reverse_bits(uint8_t value);

/**
 * @brief 计算值中设置的位数量
 * @details 计算二进制表示中1的个数（人口计数）
 * @param value 要计算的值
 * @return 设置的位数量
 */
uint8_t count_set_bits(uint32_t value);

/**
 * @brief 查找第一个设置的位（从低位开始）
 * @details 找到最低位的1的位置
 * @param value 要查找的值
 * @return 第一个设置位的位置（0-based），如果值为0返回0
 */
uint8_t find_first_set(uint32_t value);

/**
 * @brief 查找最后一个设置的位（从高位开始）
 * @details 找到最高位的1的位置
 * @param value 要查找的值
 * @return 最后一个设置位的位置（0-based），如果值为0返回0
 */
uint8_t find_last_set(uint32_t value);

/**
 * @brief 循环左移
 * @details 将值循环左移指定位数，移出的位从右边进入
 * @param value 要移位的值
 * @param count 移位位数
 * @return 移位后的值
 */
uint32_t rotate_left(uint32_t value, uint8_t count);

/**
 * @brief 循环右移
 * @details 将值循环右移指定位数，移出的位从左边进入
 * @param value 要移位的值
 * @param count 移位位数
 * @return 移位后的值
 */
uint32_t rotate_right(uint32_t value, uint8_t count);

/**
 * @brief 判断值是否为2的幂
 * @param value 要判断的值
 * @return true表示是2的幂，false表示不是
 */
bool is_power_of_two(uint32_t value);

/**
 * @brief 获取大于等于指定值的最小2的幂
 * @param value 输入值
 * @return 大于等于value的最小2的幂
 */
uint32_t next_power_of_two(uint32_t value);

/*============================================================================*/
/*                           寄存器操作函数                                     */
/*============================================================================*/

/**
 * @brief 设置寄存器中的位
 * @details 将寄存器中掩码对应的位设置为1
 * @param reg 寄存器指针
 * @param mask 位掩码
 * @code
 * set_bits(&GPIO->MODER, (1 << 5));  // 设置第5位
 * @endcode
 */
void set_bits(volatile uint32_t *reg, uint32_t mask);

/**
 * @brief 清除寄存器中的位
 * @details 将寄存器中掩码对应的位清零
 * @param reg 寄存器指针
 * @param mask 位掩码
 * @code
 * clear_bits(&GPIO->MODER, (1 << 5));  // 清除第5位
 * @endcode
 */
void clear_bits(volatile uint32_t *reg, uint32_t mask);

/**
 * @brief 切换寄存器中的位
 * @details 将寄存器中掩码对应的位取反
 * @param reg 寄存器指针
 * @param mask 位掩码
 */
void toggle_bits(volatile uint32_t *reg, uint32_t mask);

/**
 * @brief 读取寄存器中的位
 * @details 读取寄存器中掩码对应的位的值
 * @param reg 寄存器指针
 * @param mask 位掩码
 * @return 掩码对应的位值（已移位到低位）
 */
uint32_t read_bits(volatile uint32_t *reg, uint32_t mask);

/**
 * @brief 写入寄存器中的位
 * @details 将值写入寄存器中掩码对应的位置
 * @param reg 寄存器指针
 * @param mask 位掩码
 * @param value 要写入的值
 * @code
 * write_bits(&GPIO->MODER, 0x3 << 10, 0x2 << 10);  // 写入多位
 * @endcode
 */
void write_bits(volatile uint32_t *reg, uint32_t mask, uint32_t value);

#ifdef __cplusplus
}
}
#endif

#endif
