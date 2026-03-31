/**
 * @file file_io.c
 * @brief 文件I/O操作模块实现
 * @details 演示C语言文件操作相关知识点，涵盖以下核心内容：
 *          - 文件基础：打开、关闭、读写、错误处理
 *          - 文件模式：文本模式、二进制模式、读写模式
 *          - 文本文件：格式化读写、行操作
 *          - 二进制文件：块读写、结构体存储
 *          - 文件定位：fseek、ftell、rewind
 *          - 错误处理：ferror、feof、perror
 *          - 配置文件：INI格式、键值对解析
 *          - 文件缓冲：缓冲区设置、刷新
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
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "file_io.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*============================================================================*/
/*                           文件操作函数实现                                   */
/*============================================================================*/

/**
 * @brief 写入文本文件
 * @details 将字符串写入文本文件，覆盖原有内容。
 *          以写入模式("w")打开文件，如果文件不存在则创建。
 * 
 * @param filename 文件名，不能为NULL
 * @param content 要写入的内容，不能为NULL
 * @return 操作状态：成功返回STATUS_OK，失败返回错误码
 * 
 * @note 会覆盖文件原有内容
 * @note 写入失败会记录错误日志
 * 
 * @code
 * Status_t status = file_write_text("test.txt", "Hello, World!");
 * if (status != STATUS_OK) {
 *     printf("写入失败\n");
 * }
 * @endcode
 */
Status_t file_write_text(const char *filename, const char *content)
{
    if (filename == NULL || content == NULL) {
        return STATUS_NULL_PTR;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        LOG_ERROR("无法打开文件: %s (%s)", filename, strerror(errno));
        return STATUS_ERROR;
    }

    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, fp);
    
    fclose(fp);

    if (written != len) {
        LOG_ERROR("写入不完整: 期望 %zu, 实际 %zu", len, written);
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

/**
 * @brief 读取文本文件
 * @details 从文本文件读取内容到缓冲区。
 *          以读取模式("r")打开文件。
 * 
 * @param filename 文件名，不能为NULL
 * @param buffer 接收缓冲区，不能为NULL
 * @param buffer_size 缓冲区大小（字节）
 * @return 操作状态：成功返回STATUS_OK，失败返回错误码
 * 
 * @note 读取的内容会以'\0'结尾
 * @note 如果文件内容超过缓冲区大小，会被截断
 * 
 * @code
 * char buffer[1024];
 * Status_t status = file_read_text("test.txt", buffer, sizeof(buffer));
 * if (status == STATUS_OK) {
 *     printf("内容: %s\n", buffer);
 * }
 * @endcode
 */
Status_t file_read_text(const char *filename, char *buffer, size_t buffer_size)
{
    if (filename == NULL || buffer == NULL || buffer_size == 0) {
        return STATUS_NULL_PTR;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        LOG_ERROR("无法打开文件: %s (%s)", filename, strerror(errno));
        return STATUS_ERROR;
    }

    size_t read_count = fread(buffer, 1, buffer_size - 1, fp);
    buffer[read_count] = '\0';

    fclose(fp);
    return STATUS_OK;
}

/**
 * @brief 写入二进制文件
 * @details 将二进制数据写入文件。
 *          以二进制写入模式("wb")打开文件。
 * 
 * @param filename 文件名，不能为NULL
 * @param data 数据指针，不能为NULL
 * @param size 数据大小（字节）
 * @return 操作状态：成功返回STATUS_OK，失败返回错误码
 * 
 * @code
 * uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
 * Status_t status = file_write_binary("data.bin", data, sizeof(data));
 * @endcode
 */
Status_t file_write_binary(const char *filename, const uint8_t *data, size_t size)
{
    if (filename == NULL || data == NULL) {
        return STATUS_NULL_PTR;
    }

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        LOG_ERROR("无法打开文件: %s (%s)", filename, strerror(errno));
        return STATUS_ERROR;
    }

    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);

    if (written != size) {
        LOG_ERROR("写入不完整: 期望 %zu, 实际 %zu", size, written);
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

/**
 * @brief 读取二进制文件
 * @details 从二进制文件读取数据到缓冲区。
 *          以二进制读取模式("rb")打开文件。
 * 
 * @param filename 文件名，不能为NULL
 * @param data 接收缓冲区，不能为NULL
 * @param max_size 最大读取大小（字节）
 * @param actual_size 实际读取大小（输出），不能为NULL
 * @return 操作状态：成功返回STATUS_OK，失败返回错误码
 * 
 * @code
 * uint8_t buffer[1024];
 * size_t actual;
 * Status_t status = file_read_binary("data.bin", buffer, sizeof(buffer), &actual);
 * if (status == STATUS_OK) {
 *     printf("读取了 %zu 字节\n", actual);
 * }
 * @endcode
 */
Status_t file_read_binary(const char *filename, uint8_t *data, size_t max_size, size_t *actual_size)
{
    if (filename == NULL || data == NULL || actual_size == NULL) {
        return STATUS_NULL_PTR;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        LOG_ERROR("无法打开文件: %s (%s)", filename, strerror(errno));
        return STATUS_ERROR;
    }

    *actual_size = fread(data, 1, max_size, fp);
    fclose(fp);

    return STATUS_OK;
}

/**
 * @brief 追加文本到文件
 * @details 将字符串追加到文件末尾。
 *          以追加模式("a")打开文件，如果文件不存在则创建。
 * 
 * @param filename 文件名，不能为NULL
 * @param content 要追加的内容，不能为NULL
 * @return 操作状态：成功返回STATUS_OK，失败返回错误码
 * 
 * @code
 * file_append_text("log.txt", "新的一行日志\n");
 * @endcode
 */
Status_t file_append_text(const char *filename, const char *content)
{
    if (filename == NULL || content == NULL) {
        return STATUS_NULL_PTR;
    }

    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        LOG_ERROR("无法打开文件: %s (%s)", filename, strerror(errno));
        return STATUS_ERROR;
    }

    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, fp);
    
    fclose(fp);

    if (written != len) {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

/**
 * @brief 检查文件是否存在
 * @details 尝试以读取模式打开文件来判断文件是否存在。
 * 
 * @param filename 文件名，不能为NULL
 * @return 1表示存在，0表示不存在
 * 
 * @code
 * if (file_exists("config.ini")) {
 *     printf("配置文件存在\n");
 * }
 * @endcode
 */
int32_t file_exists(const char *filename)
{
    if (filename == NULL) {
        return 0;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 0;
    }
    fclose(fp);
    return 1;
}

/**
 * @brief 获取文件大小
 * @details 通过定位到文件末尾获取文件大小。
 * 
 * @param filename 文件名，不能为NULL
 * @return 文件大小（字节），失败返回-1
 * 
 * @code
 * int64_t size = file_get_size("data.bin");
 * if (size >= 0) {
 *     printf("文件大小: %lld 字节\n", size);
 * }
 * @endcode
 */
int64_t file_get_size(const char *filename)
{
    if (filename == NULL) {
        return -1;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }

    long size = ftell(fp);
    fclose(fp);

    return (int64_t)size;
}

/**
 * @brief 复制文件
 * @details 将源文件内容复制到目标文件。
 *          使用缓冲区分块复制，适合大文件。
 * 
 * @param src 源文件名，不能为NULL
 * @param dest 目标文件名，不能为NULL
 * @return 操作状态：成功返回STATUS_OK，失败返回错误码
 * 
 * @code
 * Status_t status = file_copy("source.txt", "dest.txt");
 * if (status != STATUS_OK) {
 *     printf("复制失败\n");
 * }
 * @endcode
 */
Status_t file_copy(const char *src, const char *dest)
{
    if (src == NULL || dest == NULL) {
        return STATUS_NULL_PTR;
    }

    FILE *fp_src = fopen(src, "rb");
    if (fp_src == NULL) {
        LOG_ERROR("无法打开源文件: %s", src);
        return STATUS_ERROR;
    }

    FILE *fp_dest = fopen(dest, "wb");
    if (fp_dest == NULL) {
        LOG_ERROR("无法创建目标文件: %s", dest);
        fclose(fp_src);
        return STATUS_ERROR;
    }

    uint8_t buffer[4096];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp_src)) > 0) {
        if (fwrite(buffer, 1, bytes_read, fp_dest) != bytes_read) {
            fclose(fp_src);
            fclose(fp_dest);
            return STATUS_ERROR;
        }
    }

    fclose(fp_src);
    fclose(fp_dest);
    return STATUS_OK;
}

/**
 * @brief 删除文件
 * @details 删除指定的文件。
 * 
 * @param filename 文件名，不能为NULL
 * @return 操作状态：成功返回STATUS_OK，失败返回错误码
 * 
 * @code
 * Status_t status = file_delete("temp.txt");
 * @endcode
 */
Status_t file_delete(const char *filename)
{
    if (filename == NULL) {
        return STATUS_NULL_PTR;
    }

    if (remove(filename) != 0) {
        LOG_ERROR("删除文件失败: %s (%s)", filename, strerror(errno));
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

void demo_file_basics(void)
{
    print_separator("文件操作基础演示");

    printf("[文件指针 FILE*]\n");
    printf("  FILE是标准库定义的结构体类型\n");
    printf("  包含文件缓冲区、当前位置、状态等信息\n");

    printf("\n[基本操作流程]\n");
    printf("  1. fopen() - 打开文件\n");
    printf("  2. fread()/fwrite() - 读写数据\n");
    printf("  3. fclose() - 关闭文件\n");

    printf("\n[示例代码]\n");
    printf("  FILE *fp = fopen(\"test.txt\", \"w\");\n");
    printf("  if (fp != NULL) {\n");
    printf("      fprintf(fp, \"Hello, World!\\n\");\n");
    printf("      fclose(fp);\n");
    printf("  }\n");

    printf("\n[错误处理]\n");
    printf("  检查fopen返回值是否为NULL\n");
    printf("  使用perror()或strerror(errno)获取错误信息\n");
    printf("  检查fread/fwrite的返回值\n");
}

void demo_file_modes(void)
{
    print_separator("文件打开模式演示");

    printf("[文本模式]\n");
    printf("  \"r\"  - 只读, 文件必须存在\n");
    printf("  \"w\"  - 只写, 创建或清空\n");
    printf("  \"a\"  - 追加, 创建或追加到末尾\n");
    printf("  \"r+\" - 读写, 文件必须存在\n");
    printf("  \"w+\" - 读写, 创建或清空\n");
    printf("  \"a+\" - 读写, 创建或追加\n");

    printf("\n[二进制模式]\n");
    printf("  \"rb\"  - 二进制只读\n");
    printf("  \"wb\"  - 二进制只写\n");
    printf("  \"ab\"  - 二进制追加\n");
    printf("  \"rb+\" - 二进制读写\n");
    printf("  \"wb+\" - 二进制读写(清空)\n");
    printf("  \"ab+\" - 二进制读写(追加)\n");

    printf("\n[文本vs二进制模式]\n");
    printf("  Windows: 文本模式会转换 \\n <-> \\r\\n\n");
    printf("  Linux/macOS: 两种模式相同\n");
    printf("  建议: 处理非文本数据时使用二进制模式\n");

    printf("\n[模式选择建议]\n");
    printf("  配置文件、日志: 文本模式\n");
    printf("  数据记录、固件: 二进制模式\n");
    printf("  跨平台数据: 二进制模式\n");
}

void demo_text_file(void)
{
    print_separator("文本文件操作演示");

    const char *test_file = "build/test_text.txt";

    printf("[写入文本文件]\n");
    const char *content = "第一行内容\n第二行内容\n第三行内容\n";
    Status_t status = file_write_text(test_file, content);
    if (SUCCESS(status)) {
        printf("  写入成功: %s\n", test_file);
    }

    printf("\n[读取文本文件]\n");
    char buffer[256];
    status = file_read_text(test_file, buffer, sizeof(buffer));
    if (SUCCESS(status)) {
        printf("  读取内容:\n%s", buffer);
    }

    printf("\n[追加内容]\n");
    status = file_append_text(test_file, "追加的一行\n");
    if (SUCCESS(status)) {
        printf("  追加成功\n");
    }

    printf("\n[逐行读取]\n");
    FILE *fp = fopen(test_file, "r");
    if (fp != NULL) {
        char line[128];
        int32_t line_num = 0;
        while (fgets(line, sizeof(line), fp) != NULL) {
            line_num++;
            line[strcspn(line, "\n")] = '\0';
            printf("  行%d: %s\n", line_num, line);
        }
        fclose(fp);
    }

    printf("\n[格式化读写]\n");
    printf("  fprintf(fp, \"值: %d, 名: %s\\n\", value, name);\n");
    printf("  fscanf(fp, \"值: %d, 名: %s\", &value, name);\n");
}

void demo_binary_file(void)
{
    print_separator("二进制文件操作演示");

    const char *test_file = "build/test_binary.bin";

    printf("[写入二进制数据]\n");
    uint8_t write_data[] = {0x00, 0x01, 0x02, 0x03, 0xFF, 0xFE, 0xFD, 0xFC};
    Status_t status = file_write_binary(test_file, write_data, sizeof(write_data));
    if (SUCCESS(status)) {
        printf("  写入 %zu 字节\n", sizeof(write_data));
    }

    printf("\n[读取二进制数据]\n");
    uint8_t read_data[16];
    size_t actual_size;
    status = file_read_binary(test_file, read_data, sizeof(read_data), &actual_size);
    if (SUCCESS(status)) {
        printf("  读取 %zu 字节: ", actual_size);
        for (size_t i = 0; i < actual_size; i++) {
            printf("%02X ", read_data[i]);
        }
        printf("\n");
    }

    printf("\n[写入结构体]\n");
    typedef struct {
        uint32_t id;
        char name[16];
        float value;
    } Record_t;

    Record_t records[] = {
        {1, "Record1", 10.5f},
        {2, "Record2", 20.3f},
        {3, "Record3", 30.1f}
    };

    FILE *fp = fopen("build/records.dat", "wb");
    if (fp != NULL) {
        fwrite(records, sizeof(Record_t), 3, fp);
        fclose(fp);
        printf("  写入 %zu 条记录\n", ARRAY_SIZE(records));
    }

    printf("\n[读取结构体]\n");
    Record_t read_records[3];
    fp = fopen("build/records.dat", "rb");
    if (fp != NULL) {
        size_t count = fread(read_records, sizeof(Record_t), 3, fp);
        fclose(fp);
        printf("  读取 %zu 条记录:\n", count);
        for (size_t i = 0; i < count; i++) {
            printf("    [%zu] id=%u, name=%s, value=%.1f\n",
                   i, read_records[i].id, read_records[i].name, read_records[i].value);
        }
    }

    printf("\n[嵌入式应用]\n");
    printf("  - 固件升级文件\n");
    printf("  - 数据日志记录\n");
    printf("  - 配置参数存储\n");
    printf("  - 传感器数据导出\n");
}

void demo_file_position(void)
{
    print_separator("文件定位演示");

    const char *test_file = "build/test_seek.bin";

    uint8_t data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    file_write_binary(test_file, data, sizeof(data));

    printf("[fseek - 移动文件指针]\n");
    FILE *fp = fopen(test_file, "rb");
    if (fp != NULL) {
        printf("  SEEK_SET: 从文件开头\n");
        fseek(fp, 5, SEEK_SET);
        printf("    位置5的值: %d\n", fgetc(fp));

        printf("  SEEK_CUR: 从当前位置\n");
        fseek(fp, -2, SEEK_CUR);
        printf("    当前位置-2的值: %d\n", fgetc(fp));

        printf("  SEEK_END: 从文件末尾\n");
        fseek(fp, -3, SEEK_END);
        printf("    末尾-3的值: %d\n", fgetc(fp));

        fclose(fp);
    }

    printf("\n[ftell - 获取当前位置]\n");
    fp = fopen(test_file, "rb");
    if (fp != NULL) {
        printf("  初始位置: %ld\n", ftell(fp));
        fseek(fp, 5, SEEK_SET);
        printf("  移动后位置: %ld\n", ftell(fp));
        fclose(fp);
    }

    printf("\n[rewind - 回到开头]\n");
    printf("  rewind(fp) 等价于 fseek(fp, 0, SEEK_SET)\n");

    printf("\n[fgetpos/fsetpos - 更精确的定位]\n");
    printf("  用于处理大文件(超过long范围)\n");

    printf("\n[随机访问应用]\n");
    printf("  - 数据库索引查找\n");
    printf("  - 配置文件修改\n");
    printf("  - 日志文件分析\n");
}

void demo_file_error(void)
{
    print_separator("文件错误处理演示");

    printf("[常见错误]\n");
    printf("  1. 文件不存在\n");
    printf("  2. 权限不足\n");
    printf("  3. 磁盘空间不足\n");
    printf("  4. 文件被占用\n");

    printf("\n[错误检测函数]\n");
    printf("  feof(fp) - 是否到达文件末尾\n");
    printf("  ferror(fp) - 是否发生错误\n");
    printf("  clearerr(fp) - 清除错误标志\n");

    printf("\n[错误处理示例]\n");
    FILE *fp = fopen("nonexistent.txt", "r");
    if (fp == NULL) {
        printf("  打开失败: %s\n", strerror(errno));
        printf("  错误码: %d\n", errno);
    }

    printf("\n[安全编码建议]\n");
    printf("  1. 始终检查fopen返回值\n");
    printf("  2. 使用fread/fwrite返回值判断实际读写量\n");
    printf("  3. 关闭文件前检查错误\n");
    printf("  4. 使用perror输出有意义的错误信息\n");

    printf("\n[资源泄漏防护]\n");
    printf("  确保每个fopen都有对应的fclose\n");
    printf("  使用goto或do-while(0)进行清理\n");
}

void demo_config_file(void)
{
    print_separator("配置文件处理演示");

    const char *config_file = "build/config.ini";

    printf("[INI格式配置文件]\n");
    const char *config_content = 
        "# 配置文件示例\n"
        "[network]\n"
        "ip = 192.168.1.100\n"
        "port = 8080\n"
        "\n"
        "[sensor]\n"
        "interval = 1000\n"
        "enabled = true\n";

    file_write_text(config_file, config_content);
    printf("  配置文件内容:\n%s\n", config_content);

    printf("[解析配置文件]\n");
    FILE *fp = fopen(config_file, "r");
    if (fp != NULL) {
        char line[128];
        char section[32] = "";
        
        while (fgets(line, sizeof(line), fp) != NULL) {
            line[strcspn(line, "\n")] = '\0';
            
            if (line[0] == '#' || line[0] == '\0') {
                continue;
            }
            
            if (line[0] == '[') {
                char *end = strchr(line, ']');
                if (end != NULL) {
                    *end = '\0';
                    strncpy(section, line + 1, sizeof(section) - 1);
                    printf("  节: [%s]\n", section);
                }
            } else {
                char *eq = strchr(line, '=');
                if (eq != NULL) {
                    *eq = '\0';
                    char *key = line;
                    char *value = eq + 1;
                    while (*key == ' ') key++;
                    while (*value == ' ') value++;
                    printf("    %s.%s = %s\n", section, key, value);
                }
            }
        }
        fclose(fp);
    }

    printf("\n[JSON格式(简化解析)]\n");
    printf("  {\"ip\": \"192.168.1.100\", \"port\": 8080}\n");
    printf("  嵌入式推荐使用轻量级JSON库\n");

    printf("\n[嵌入式配置存储]\n");
    printf("  - EEPROM/Flash存储\n");
    printf("  - 结构体序列化\n");
    printf("  - CRC校验\n");
    printf("  - 默认值机制\n");
}

void demo_file_buffer(void)
{
    print_separator("文件缓冲演示");

    printf("[标准I/O缓冲]\n");
    printf("  全缓冲: 填满缓冲区后才刷新\n");
    printf("  行缓冲: 遇到换行符刷新\n");
    printf("  无缓冲: 立即写入\n");

    printf("\n[setvbuf - 设置缓冲模式]\n");
    printf("  setvbuf(fp, buffer, mode, size);\n");
    printf("  mode:\n");
    printf("    _IOFBF - 全缓冲\n");
    printf("    _IOLBF - 行缓冲\n");
    printf("    _IONBF - 无缓冲\n");

    printf("\n[fflush - 刷新缓冲区]\n");
    printf("  fflush(fp) - 刷新输出缓冲区\n");
    printf("  fflush(NULL) - 刷新所有输出流\n");

    printf("\n[缓冲区大小选择]\n");
    printf("  默认: 通常为4KB或8KB\n");
    printf("  大文件: 使用较大缓冲区\n");
    printf("  实时数据: 使用无缓冲或小缓冲区\n");

    printf("\n[嵌入式考虑]\n");
    printf("  - RAM有限时使用小缓冲区\n");
    printf("  - 关键数据及时刷新\n");
    printf("  - 掉电保护机制\n");
}
