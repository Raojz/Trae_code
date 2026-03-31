/**
 * @file struct_union.h
 * @brief 结构体与联合体模块头文件
 * @details 涵盖结构体、联合体、枚举、位域等复合数据类型知识点。
 *          这些类型是嵌入式数据建模的核心工具，用于硬件寄存器映射、协议定义等。
 * 
 * @section main_features 主要内容
 * - 结构体基础：声明、定义、初始化、成员访问
 * - 结构体操作：赋值、作为参数、作为返回值
 * - 嵌套结构体：结构体包含结构体
 * - 联合体基础：共用内存空间
 * - 联合体应用：字节访问、类型转换、协议解析
 * - 枚举类型：命名常量、提高可读性
 * - 位域：精确控制位宽、寄存器映射
 * - typedef：类型别名、提高可移植性
 * - 数据结构：链表、队列等基础结构
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 硬件寄存器映射：使用结构体和位域
 * - 通信协议定义：数据包结构
 * - 配置参数管理：结构体存储配置
 * - 联合体用于大小端转换、浮点数解析
 * - 枚举用于状态机状态定义
 * 
 * @section memory_layout 内存布局
 * - 结构体成员按声明顺序存储
 * - 可能存在填充字节（对齐）
 * - 联合体所有成员共享同一内存
 * - 位域的具体布局依赖编译器
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 使用Packed属性可取消结构体填充
 * @note 位域在跨平台时需注意兼容性
 */

#ifndef STRUCT_UNION_H
#define STRUCT_UNION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 结构体基础演示
 * @details 展示结构体的基本概念：
 *          - 结构体声明和定义
 *          - 结构体变量初始化
 *          - 成员访问（.和->）
 *          - 结构体大小和内存布局
 */
void demo_struct_basics(void);

/**
 * @brief 结构体操作演示
 * @details 展示结构体的操作：
 *          - 结构体赋值
 *          - 结构体作为函数参数
 *          - 结构体作为返回值
 *          - 结构体数组
 */
void demo_struct_operations(void);

/**
 * @brief 嵌套结构体演示
 * @details 展示结构体的嵌套使用：
 *          - 结构体包含结构体
 *          - 匿名结构体
 *          - 自引用结构体（链表节点）
 */
void demo_nested_struct(void);

/**
 * @brief 联合体基础演示
 * @details 展示联合体的基本概念：
 *          - 联合体声明和定义
 *          - 共用内存空间
 *          - 联合体大小
 *          - 成员访问
 */
void demo_union_basics(void);

/**
 * @brief 联合体应用演示
 * @details 展示联合体的典型应用：
 *          - 字节访问（大小端解析）
 *          - 浮点数内部表示
 *          - 协议数据解析
 *          - 数据类型转换
 */
void demo_union_applications(void);

/**
 * @brief 枚举类型演示
 * @details 展示枚举的使用：
 *          - 枚举定义和使用
 *          - 枚举值指定
 *          - 枚举与整数转换
 *          - 枚举在状态机中的应用
 */
void demo_enum_basics(void);

/**
 * @brief 位域演示
 * @details 展示位域的使用：
 *          - 位域声明和定义
 *          - 位宽指定
 *          - 硬件寄存器映射
 *          - 位域的注意事项
 * @note 位域布局依赖编译器，跨平台需注意
 */
void demo_bit_field(void);

/**
 * @brief typedef使用演示
 * @details 展示typedef的使用：
 *          - 基本类型别名
 *          - 结构体别名
 *          - 函数指针别名
 *          - 提高可移植性
 */
void demo_typedef_usage(void);

/**
 * @brief 数据结构演示
 * @details 展示基础数据结构的实现：
 *          - 链表节点和操作
 *          - 队列结构
 *          - 栈结构
 *          - 嵌入式中的应用
 */
void demo_data_structure(void);

/*============================================================================*/
/*                           类型定义                                           */
/*============================================================================*/

/**
 * @brief 二维坐标点结构体
 * @details 表示平面上的一个点
 */
typedef struct {
    int32_t x;  /**< X坐标 */
    int32_t y;  /**< Y坐标 */
} Point_t;

/**
 * @brief 学生信息结构体
 * @details 用于演示结构体的基本使用
 */
typedef struct {
    char name[32];  /**< 姓名 */
    uint8_t age;    /**< 年龄 */
    float score;    /**< 成绩 */
} Student_t;

/**
 * @brief 设备信息结构体
 * @details 演示嵌入式设备信息管理
 */
typedef struct {
    uint32_t id;        /**< 设备ID */
    char name[16];      /**< 设备名称 */
    uint8_t type;       /**< 设备类型 */
    uint8_t status;     /**< 设备状态 */
} Device_t;

/**
 * @brief 字节访问联合体
 * @details 用于32位数据的字节级访问，常用于：
 *          - 大小端转换
 *          - 通信协议解析
 *          - 寄存器位操作
 */
typedef union {
    uint32_t value;     /**< 32位整体访问 */
    uint8_t bytes[4];   /**< 字节数组访问 */
    struct {
        uint32_t byte0 : 8;  /**< 第0字节 */
        uint32_t byte1 : 8;  /**< 第1字节 */
        uint32_t byte2 : 8;  /**< 第2字节 */
        uint32_t byte3 : 8;  /**< 第3字节 */
    };  /**< 位域访问 */
} ByteAccess_t;

/**
 * @brief 浮点数转换联合体
 * @details 用于浮点数与整数之间的转换，
 *          可查看浮点数的内部表示（IEEE 754格式）
 */
typedef union {
    float f;        /**< 浮点数表示 */
    uint32_t u;     /**< 无符号整数表示 */
    int32_t i;      /**< 有符号整数表示 */
} FloatConverter_t;

/**
 * @brief 传感器类型枚举
 * @details 定义支持的传感器类型
 */
typedef enum {
    SENSOR_TEMP = 0,      /**< 温度传感器 */
    SENSOR_HUMIDITY = 1,  /**< 湿度传感器 */
    SENSOR_PRESSURE = 2,  /**< 气压传感器 */
    SENSOR_LIGHT = 3,     /**< 光照传感器 */
    SENSOR_MAX            /**< 传感器类型数量 */
} SensorType_t;

/**
 * @brief 配置寄存器位域结构体
 * @details 演示硬件寄存器的位域映射，
 *          注意：位域布局依赖编译器
 */
typedef struct {
    uint32_t enable : 1;     /**< 使能位 */
    uint32_t mode : 2;       /**< 模式选择 */
    uint32_t speed : 3;      /**< 速度设置 */
    uint32_t reserved : 26;  /**< 保留位 */
} ConfigReg_t;

/*============================================================================*/
/*                           结构体操作函数                                     */
/*============================================================================*/

/**
 * @brief 创建坐标点
 * @param x X坐标
 * @param y Y坐标
 * @return Point_t结构体
 */
Point_t point_create(int32_t x, int32_t y);

/**
 * @brief 计算两点间距离的平方
 * @details 避免开方运算，提高效率
 * @param p1 第一个点
 * @param p2 第二个点
 * @return 距离的平方
 */
int32_t point_distance_squared(const Point_t *p1, const Point_t *p2);

/**
 * @brief 初始化学生信息
 * @param student 学生结构体指针
 * @param name 姓名
 * @param age 年龄
 * @param score 成绩
 */
void student_init(Student_t *student, const char *name, uint8_t age, float score);

/**
 * @brief 打印学生信息
 * @param student 学生结构体指针
 */
void student_print(const Student_t *student);

/**
 * @brief 初始化设备信息
 * @param device 设备结构体指针
 * @param id 设备ID
 * @param name 设备名称
 * @param type 设备类型
 */
void device_init(Device_t *device, uint32_t id, const char *name, uint8_t type);

/**
 * @brief 设置设备状态
 * @param device 设备结构体指针
 * @param status 新状态
 * @return 操作状态
 */
Status_t device_set_status(Device_t *device, uint8_t status);

#ifdef __cplusplus
}
}
#endif

#endif
