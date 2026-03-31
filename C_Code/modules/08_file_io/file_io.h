/**
 * @file file_io.h
 * @brief 文件I/O操作模块头文件
 * @details 涵盖标准文件操作、二进制文件、配置文件等知识点。
 *          文件I/O是嵌入式系统数据持久化的重要手段。
 * 
 * @section main_features 主要内容
 * - 文件基础：打开、关闭、读写、错误处理
 * - 文件模式：文本模式、二进制模式、读写模式
 * - 文本文件：格式化读写、行操作
 * - 二进制文件：块读写、结构体存储
 * - 文件定位：fseek、ftell、rewind
 * - 错误处理：ferror、feof、perror
 * - 配置文件：INI格式、键值对解析
 * - 文件缓冲：缓冲区设置、刷新
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 配置参数存储
 * - 日志记录
 * - 固件升级文件读取
 * - 数据记录和导出
 * - 文件系统操作（FatFs等）
 * 
 * @section safety_notes 安全注意事项
 * - 检查文件打开是否成功
 * - 确保文件正确关闭
 * - 防止缓冲区溢出
 * - 处理读写错误
 * - 注意文件路径和权限
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 嵌入式系统中常用FatFs等轻量级文件系统
 * @note 注意文件操作的资源消耗和实时性影响
 */

#ifndef FILE_IO_H
#define FILE_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 文件基础操作演示
 * @details 展示文件的基本操作：
 *          - fopen：打开文件
 *          - fclose：关闭文件
 *          - fgetc/fputc：字符读写
 *          - fgets/fputs：字符串读写
 *          - 错误检查
 */
void demo_file_basics(void);

/**
 * @brief 文件打开模式演示
 * @details 展示不同的文件打开模式：
 *          - "r"：只读，文件必须存在
 *          - "w"：只写，文件不存在则创建
 *          - "a"：追加，文件不存在则创建
 *          - "r+"：读写，文件必须存在
 *          - "w+"：读写，清空或创建
 *          - "b"：二进制模式
 */
void demo_file_modes(void);

/**
 * @brief 文本文件操作演示
 * @details 展示文本文件的操作：
 *          - fprintf：格式化写入
 *          - fscanf：格式化读取
 *          - 行读写操作
 *          - 文本解析
 */
void demo_text_file(void);

/**
 * @brief 二进制文件操作演示
 * @details 展示二进制文件的操作：
 *          - fread：块读取
 *          - fwrite：块写入
 *          - 结构体存储
 *          - 数组存储
 */
void demo_binary_file(void);

/**
 * @brief 文件定位操作演示
 * @details 展示文件定位操作：
 *          - fseek：设置文件位置
 *          - ftell：获取当前位置
 *          - rewind：回到文件开头
 *          - 随机访问文件
 */
void demo_file_position(void);

/**
 * @brief 文件错误处理演示
 * @details 展示文件错误处理：
 *          - ferror：检查错误标志
 *          - feof：检查文件结束
 *          - perror：输出错误信息
 *          - clearerr：清除错误标志
 */
void demo_file_error(void);

/**
 * @brief 配置文件操作演示
 * @details 展示配置文件的处理：
 *          - INI格式解析
 *          - 键值对读取
 *          - 配置项修改
 *          - 配置保存
 */
void demo_config_file(void);

/**
 * @brief 文件缓冲区演示
 * @details 展示文件缓冲区的使用：
 *          - setvbuf：设置缓冲区
 *          - setbuf：简化缓冲区设置
 *          - fflush：刷新缓冲区
 *          - 缓冲区大小影响
 */
void demo_file_buffer(void);

/*============================================================================*/
/*                           文件操作函数                                       */
/*============================================================================*/

/**
 * @brief 写入文本文件
 * @details 将字符串写入文本文件，覆盖原有内容
 * @param filename 文件名
 * @param content 要写入的内容
 * @return 操作状态
 * 
 * @code
 * Status_t status = file_write_text("test.txt", "Hello, World!");
 * @endcode
 */
Status_t file_write_text(const char *filename, const char *content);

/**
 * @brief 读取文本文件
 * @details 从文本文件读取内容到缓冲区
 * @param filename 文件名
 * @param buffer 接收缓冲区
 * @param buffer_size 缓冲区大小
 * @return 操作状态
 * 
 * @note 读取的内容会以'\0'结尾
 */
Status_t file_read_text(const char *filename, char *buffer, size_t buffer_size);

/**
 * @brief 写入二进制文件
 * @details 将二进制数据写入文件
 * @param filename 文件名
 * @param data 数据指针
 * @param size 数据大小（字节）
 * @return 操作状态
 */
Status_t file_write_binary(const char *filename, const uint8_t *data, size_t size);

/**
 * @brief 读取二进制文件
 * @details 从二进制文件读取数据
 * @param filename 文件名
 * @param data 接收缓冲区
 * @param max_size 最大读取大小
 * @param actual_size 实际读取大小（输出）
 * @return 操作状态
 */
Status_t file_read_binary(const char *filename, uint8_t *data, size_t max_size, size_t *actual_size);

/**
 * @brief 追加文本到文件
 * @details 将字符串追加到文件末尾
 * @param filename 文件名
 * @param content 要追加的内容
 * @return 操作状态
 */
Status_t file_append_text(const char *filename, const char *content);

/**
 * @brief 检查文件是否存在
 * @param filename 文件名
 * @return 1表示存在，0表示不存在
 */
int32_t file_exists(const char *filename);

/**
 * @brief 获取文件大小
 * @param filename 文件名
 * @return 文件大小（字节），失败返回-1
 */
int64_t file_get_size(const char *filename);

/**
 * @brief 复制文件
 * @param src 源文件名
 * @param dest 目标文件名
 * @return 操作状态
 */
Status_t file_copy(const char *src, const char *dest);

/**
 * @brief 删除文件
 * @param filename 文件名
 * @return 操作状态
 */
Status_t file_delete(const char *filename);


#endif
