/**
 * @file struct_union.c
 * @brief 结构体与联合体模块实现
 * @details 演示C语言结构体、联合体、枚举、位域等知识点，涵盖以下核心内容：
 *          - 结构体基础：声明、定义、初始化、成员访问
 *          - 结构体操作：赋值、作为参数、作为返回值
 *          - 嵌套结构体：结构体包含结构体
 *          - 联合体基础：共用内存空间
 *          - 联合体应用：字节访问、类型转换、协议解析
 *          - 枚举类型：命名常量、提高可读性
 *          - 位域：精确控制位宽、寄存器映射
 *          - typedef：类型别名、提高可移植性
 *          - 数据结构：链表、队列等基础结构
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
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "struct_union.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/*============================================================================*/
/*                           结构体操作函数实现                                 */
/*============================================================================*/

/**
 * @brief 创建坐标点
 * @details 创建并初始化一个Point_t结构体。
 * 
 * @param x X坐标值
 * @param y Y坐标值
 * @return 初始化后的Point_t结构体
 * 
 * @code
 * Point_t p = point_create(3, 4);
 * printf("点坐标: (%d, %d)\n", p.x, p.y);
 * @endcode
 */
Point_t point_create(int32_t x, int32_t y)
{
    Point_t p = {x, y};
    return p;
}

/**
 * @brief 计算两点间距离的平方
 * @details 计算两点之间的欧几里得距离的平方。
 *          避免开方运算，提高效率，常用于距离比较。
 * 
 * @param p1 第一个点
 * @param p2 第二个点
 * @return 距离的平方
 * 
 * @note 如果任一指针为NULL，返回0
 * @note 返回平方值避免浮点运算
 * 
 * @code
 * Point_t p1 = {0, 0};
 * Point_t p2 = {3, 4};
 * int32_t dist_sq = point_distance_squared(&p1, &p2);
 * printf("距离平方: %d\n", dist_sq);  // 输出: 25
 * @endcode
 */
int32_t point_distance_squared(const Point_t *p1, const Point_t *p2)
{
    if (p1 == NULL || p2 == NULL) {
        return 0;
    }
    int32_t dx = p2->x - p1->x;
    int32_t dy = p2->y - p1->y;
    return dx * dx + dy * dy;
}

/**
 * @brief 初始化学生信息
 * @details 初始化Student_t结构体的各个字段。
 *          安全地复制姓名字符串，防止缓冲区溢出。
 * 
 * @param student 学生结构体指针
 * @param name 姓名（将被截断以适应缓冲区）
 * @param age 年龄
 * @param score 成绩
 * 
 * @code
 * Student_t student;
 * student_init(&student, "张三", 20, 85.5f);
 * @endcode
 */
void student_init(Student_t *student, const char *name, uint8_t age, float score)
{
    if (student == NULL || name == NULL) {
        return;
    }
    strncpy(student->name, name, sizeof(student->name) - 1);
    student->name[sizeof(student->name) - 1] = '\0';
    student->age = age;
    student->score = score;
}

/**
 * @brief 打印学生信息
 * @details 格式化输出学生结构体的所有字段。
 * 
 * @param student 学生结构体指针
 * 
 * @code
 * Student_t student = {"李四", 22, 90.0f};
 * student_print(&student);
 * // 输出: 姓名: 李四, 年龄: 22, 成绩: 90.00
 * @endcode
 */
void student_print(const Student_t *student)
{
    if (student == NULL) {
        return;
    }
    printf("  姓名: %s, 年龄: %d, 成绩: %.2f\n", 
           student->name, student->age, student->score);
}

/**
 * @brief 初始化设备信息
 * @details 初始化Device_t结构体的各个字段。
 *          设置初始状态为0。
 * 
 * @param device 设备结构体指针
 * @param id 设备ID
 * @param name 设备名称
 * @param type 设备类型
 * 
 * @code
 * Device_t device;
 * device_init(&device, 0x1234, "传感器A", 1);
 * @endcode
 */
void device_init(Device_t *device, uint32_t id, const char *name, uint8_t type)
{
    if (device == NULL || name == NULL) {
        return;
    }
    device->id = id;
    strncpy(device->name, name, sizeof(device->name) - 1);
    device->name[sizeof(device->name) - 1] = '\0';
    device->type = type;
    device->status = 0;
}

/**
 * @brief 设置设备状态
 * @details 修改设备的状态字段。
 * 
 * @param device 设备结构体指针
 * @param status 新状态值
 * @return 操作状态：成功返回STATUS_OK，失败返回STATUS_NULL_PTR
 * 
 * @code
 * Device_t device;
 * device_init(&device, 1, "设备", 0);
 * device_set_status(&device, 1);  // 设置为运行状态
 * @endcode
 */
Status_t device_set_status(Device_t *device, uint8_t status)
{
    if (device == NULL) {
        return STATUS_NULL_PTR;
    }
    device->status = status;
    return STATUS_OK;
}

/*============================================================================*/
/*                           演示函数实现                                       */
/*============================================================================*/

/**
 * @brief 结构体基础演示
 * @details 展示结构体的基本概念：
 *          - 结构体声明和定义
 *          - 结构体变量初始化
 *          - 成员访问（.和->）
 *          - 结构体大小和内存布局
 */
void demo_struct_basics(void)
{
    print_separator("结构体基础演示");

    printf("[结构体声明与定义]\n");
    struct Person {
        char name[20];
        int32_t age;
        float height;
    };
    
    struct Person person1 = {"张三", 25, 175.5f};
    struct Person person2 = {.name = "李四", .age = 30, .height = 180.0f};
    
    printf("  person1: %s, %d岁, %.1fcm\n", person1.name, person1.age, person1.height);
    printf("  person2: %s, %d岁, %.1fcm\n", person2.name, person2.age, person2.height);

    printf("\n[typedef简化]\n");
    Point_t p1 = point_create(3, 4);
    Point_t p2 = {6, 8};
    printf("  Point_t p1 = {%d, %d}\n", p1.x, p1.y);
    printf("  Point_t p2 = {%d, %d}\n", p2.x, p2.y);
    printf("  距离平方: %d\n", point_distance_squared(&p1, &p2));

    printf("\n[结构体大小与对齐]\n");
    printf("  sizeof(Point_t) = %zu 字节\n", sizeof(Point_t));
    printf("  sizeof(Student_t) = %zu 字节\n", sizeof(Student_t));
    printf("  sizeof(Device_t) = %zu 字节\n", sizeof(Device_t));

    printf("\n[成员访问]\n");
    printf("  结构体变量: person.name\n");
    printf("  结构体指针: ptr->name 或 (*ptr).name\n");

    printf("\n[结构体赋值]\n");
    Point_t p3;
    p3 = p1;
    printf("  p3 = p1; p3 = {%d, %d}\n", p3.x, p3.y);
    printf("  注意: 结构体赋值是深拷贝\n");
}

void demo_struct_operations(void)
{
    print_separator("结构体操作演示");

    printf("[结构体初始化函数]\n");
    Student_t student;
    student_init(&student, "王五", 22, 85.5f);
    student_print(&student);

    printf("\n[结构体数组]\n");
    Student_t class[3];
    student_init(&class[0], "学生A", 20, 90.0f);
    student_init(&class[1], "学生B", 21, 85.5f);
    student_init(&class[2], "学生C", 22, 78.0f);
    
    printf("  班级学生:\n");
    for (int32_t i = 0; i < 3; i++) {
        printf("    [%d] ", i);
        student_print(&class[i]);
    }

    printf("\n[结构体指针数组]\n");
    Student_t *students[3] = {&class[0], &class[1], &class[2]};
    printf("  通过指针访问:\n");
    for (int32_t i = 0; i < 3; i++) {
        printf("    students[%d]->name = %s\n", i, students[i]->name);
    }

    printf("\n[结构体作为函数参数]\n");
    printf("  值传递: 复制整个结构体(可能开销大)\n");
    printf("  指针传递: 只传递地址(推荐)\n");
    printf("  const指针: 防止修改, 同时避免拷贝\n");

    printf("\n[结构体比较]\n");
    Point_t a = {1, 2};
    Point_t b = {1, 2};
    Point_t c = {3, 4};
    printf("  a == b: %s (不能直接比较结构体)\n", 
           (memcmp(&a, &b, sizeof(Point_t)) == 0) ? "true" : "false");
    printf("  a == c: %s\n", 
           (memcmp(&a, &c, sizeof(Point_t)) == 0) ? "true" : "false");
    printf("  建议: 逐成员比较或使用memcmp\n");
}

void demo_nested_struct(void)
{
    print_separator("嵌套结构体演示");

    typedef struct {
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    } Time_t;

    typedef struct {
        uint16_t year;
        uint8_t month;
        uint8_t day;
    } Date_t;

    typedef struct {
        Date_t date;
        Time_t time;
        char event[64];
    } Event_t;

    printf("[日期时间结构体]\n");
    Event_t meeting = {
        .date = {2026, 3, 22},
        .time = {14, 30, 0},
        .event = "项目评审会议"
    };
    
    printf("  事件: %s\n", meeting.event);
    printf("  日期: %d年%d月%d日\n", 
           meeting.date.year, meeting.date.month, meeting.date.day);
    printf("  时间: %02d:%02d:%02d\n", 
           meeting.time.hour, meeting.time.minute, meeting.time.second);

    printf("\n[链表节点结构体]\n");
    typedef struct Node {
        int32_t data;
        struct Node *next;
    } Node_t;

    Node_t n3 = {30, NULL};
    Node_t n2 = {20, &n3};
    Node_t n1 = {10, &n2};

    printf("  链表: ");
    Node_t *current = &n1;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");

    printf("\n[自引用结构体]\n");
    printf("  struct Node { int data; struct Node *next; };\n");
    printf("  注意: 只能使用指针, 不能包含自身实例\n");

    printf("\n[结构体嵌套层级]\n");
    printf("  嵌套层级不宜过深, 影响可读性\n");
    printf("  建议最多3层嵌套\n");
}

void demo_union_basics(void)
{
    print_separator("联合体基础演示");

    printf("[联合体声明与大小]\n");
    union Data {
        int32_t i;
        float f;
        char str[8];
    };

    printf("  sizeof(union Data) = %zu (取最大成员大小)\n", sizeof(union Data));
    printf("  所有成员共享同一内存空间\n");

    printf("\n[联合体成员访问]\n");
    union Data data;
    
    data.i = 0x12345678;
    printf("  data.i = 0x%08X\n", data.i);
    printf("  data.str[0] = 0x%02X (小端序)\n", (uint8_t)data.str[0]);
    
    data.f = 3.14159f;
    printf("  data.f = %.5f\n", data.f);
    printf("  data.i = 0x%08X (被覆盖)\n", data.i);

    printf("\n[联合体vs结构体]\n");
    printf("  结构体: 成员独立存储, 大小为成员之和(含对齐)\n");
    printf("  联合体: 成员共享存储, 大小为最大成员\n");
    printf("  结构体: 可同时访问所有成员\n");
    printf("  联合体: 同一时间只能使用一个成员\n");

    printf("\n[初始化]\n");
    union Data u1 = { .i = 100 };
    union Data u2 = { .f = 2.5f };
    union Data u3 = { .str = "Hello" };
    printf("  u1.i = %d\n", u1.i);
    printf("  u2.f = %.1f\n", u2.f);
    printf("  u3.str = %s\n", u3.str);
}

void demo_union_applications(void)
{
    print_separator("联合体应用演示");

    printf("[字节访问联合体]\n");
    ByteAccess_t byte_access;
    byte_access.value = 0x12345678;
    
    printf("  value = 0x%08X\n", byte_access.value);
    printf("  bytes[0] = 0x%02X\n", byte_access.bytes[0]);
    printf("  bytes[1] = 0x%02X\n", byte_access.bytes[1]);
    printf("  bytes[2] = 0x%02X\n", byte_access.bytes[2]);
    printf("  bytes[3] = 0x%02X\n", byte_access.bytes[3]);

    printf("\n[浮点数内部表示]\n");
    FloatConverter_t conv;
    conv.f = -13.625f;
    
    printf("  float值: %.3f\n", conv.f);
    printf("  IEEE754表示: 0x%08X\n", conv.u);
    printf("  符号位: %d\n", (conv.u >> 31) & 1);
    printf("  指数: %d\n", ((conv.u >> 23) & 0xFF) - 127);
    printf("  尾数: 0x%06X\n", conv.u & 0x7FFFFF);

    printf("\n[协议数据单元]\n");
    typedef struct {
        uint8_t type;
        uint8_t length;
        union {
            uint8_t u8_data[8];
            uint16_t u16_data[4];
            uint32_t u32_data[2];
            float f32_data[2];
        } payload;
    } ProtocolPacket_t;

    ProtocolPacket_t packet;
    packet.type = 1;
    packet.length = 4;
    packet.payload.u32_data[0] = 0xDEADBEEF;
    packet.payload.u32_data[1] = 0xCAFEBABE;
    
    printf("  包类型: %d, 长度: %d\n", packet.type, packet.length);
    printf("  数据: 0x%08X 0x%08X\n", 
           packet.payload.u32_data[0], packet.payload.u32_data[1]);

    printf("\n[嵌入式寄存器访问]\n");
    printf("  通过联合体可以同时访问整个寄存器和各个位域\n");
    printf("  常用于硬件寄存器操作\n");
}

void demo_enum_basics(void)
{
    print_separator("枚举演示");

    printf("[枚举声明与使用]\n");
    enum Weekday {
        SUNDAY = 0,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY
    };

    enum Weekday today = WEDNESDAY;
    printf("  today = %d (WEDNESDAY)\n", today);

    printf("\n[枚举值]\n");
    printf("  默认从0开始, 依次递增\n");
    printf("  可以指定任意值\n");
    printf("  SUNDAY=%d, MONDAY=%d, ..., SATURDAY=%d\n", 
           SUNDAY, MONDAY, SATURDAY);

    printf("\n[传感器类型枚举]\n");
    const char *sensor_names[] = {
        [SENSOR_TEMP] = "温度传感器",
        [SENSOR_HUMIDITY] = "湿度传感器",
        [SENSOR_PRESSURE] = "压力传感器",
        [SENSOR_LIGHT] = "光照传感器"
    };

    for (int32_t i = 0; i < SENSOR_MAX; i++) {
        printf("  [%d] %s\n", i, sensor_names[i]);
    }

    printf("\n[状态机枚举]\n");
    typedef enum {
        STATE_INIT = 0,
        STATE_IDLE,
        STATE_RUNNING,
        STATE_ERROR,
        STATE_MAX
    } SystemState_t;

    SystemState_t state = STATE_IDLE;
    printf("  当前状态: %d\n", state);

    printf("\n[枚举vs宏定义]\n");
    printf("  枚举: 有类型检查, 调试时显示名称\n");
    printf("  宏: 无类型检查, 调试时显示数值\n");
    printf("  推荐: 使用枚举定义相关常量\n");

    printf("\n[枚举大小]\n");
    printf("  sizeof(enum) 通常等于 sizeof(int)\n");
    printf("  可以指定较小类型: typedef enum __attribute__((packed)) {...}\n");
}

void demo_bit_field(void)
{
    print_separator("位域演示");

    printf("[位域声明]\n");
    struct Flags {
        uint32_t is_enabled : 1;
        uint32_t mode : 2;
        uint32_t priority : 3;
        uint32_t channel : 4;
        uint32_t reserved : 22;
    };

    printf("  struct Flags大小: %zu 字节\n", sizeof(struct Flags));

    struct Flags flags = {
        .is_enabled = 1,
        .mode = 2,
        .priority = 5,
        .channel = 7
    };

    printf("  is_enabled: %u\n", flags.is_enabled);
    printf("  mode: %u\n", flags.mode);
    printf("  priority: %u\n", flags.priority);
    printf("  channel: %u\n", flags.channel);

    printf("\n[配置寄存器模拟]\n");
    ConfigReg_t config = {
        .enable = 1,
        .mode = 2,
        .speed = 5
    };
    
    printf("  enable: %u\n", config.enable);
    printf("  mode: %u\n", config.mode);
    printf("  speed: %u\n", config.speed);
    printf("  整体值: 0x%08X\n", *(uint32_t *)&config);

    printf("\n[位域注意事项]\n");
    printf("  1. 位域不能取地址\n");
    printf("  2. 位域布局依赖编译器(可移植性问题)\n");
    printf("  3. 跨字节位域需谨慎\n");
    printf("  4. 嵌入式建议使用宏操作寄存器\n");

    printf("\n[位域vs位操作宏]\n");
    printf("  位域: 代码清晰, 但可移植性差\n");
    printf("  宏: 可移植性好, 但代码可读性稍差\n");
    printf("  嵌入式推荐: 使用BIT_SET/BIT_CLEAR等宏\n");
}

void demo_typedef_usage(void)
{
    print_separator("typedef用法演示");

    printf("[基本类型别名]\n");
    typedef uint8_t byte_t;
    typedef uint16_t word_t;
    typedef uint32_t dword_t;
    
    byte_t b = 0xAA;
    word_t w = 0x1234;
    dword_t d = 0xDEADBEEF;
    
    printf("  byte_t b = 0x%02X\n", b);
    printf("  word_t w = 0x%04X\n", w);
    printf("  dword_t d = 0x%08X\n", d);

    printf("\n[结构体别名]\n");
    printf("  typedef struct { ... } Point_t;\n");
    printf("  使用: Point_t p; (无需struct关键字)\n");

    printf("\n[函数指针别名]\n");
    typedef int32_t (*CompareFunc_t)(const void *, const void *);
    typedef void (*EventHandler_t)(int32_t event);
    
    printf("  CompareFunc_t cmp;  // 比较函数指针\n");
    printf("  EventHandler_t handler;  // 事件处理函数指针\n");

    printf("\n[数组类型别名]\n");
    typedef int32_t IntArray10_t[10];
    IntArray10_t arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    printf("  IntArray10_t arr = {");
    for (int32_t i = 0; i < 10; i++) {
        printf("%d%s", arr[i], (i < 9) ? ", " : "");
    }
    printf("};\n");

    printf("\n[typedef vs #define]\n");
    printf("  typedef: 编译时处理, 有作用域, 类型检查\n");
    printf("  #define: 预处理时替换, 无作用域, 无类型检查\n");
    printf("  推荐: 定义类型别名使用typedef\n");
}

void demo_data_structure(void)
{
    print_separator("数据结构演示");

    printf("[动态数组结构]\n");
    typedef struct {
        int32_t *data;
        size_t size;
        size_t capacity;
    } DynamicArray_t;

    printf("  结构: { data指针, 当前大小, 容量 }\n");
    printf("  操作: init, push, pop, get, resize, destroy\n");

    printf("\n[链表结构]\n");
    typedef struct LinkedListNode {
        int32_t data;
        struct LinkedListNode *next;
    } LinkedListNode_t;

    typedef struct {
        LinkedListNode_t *head;
        LinkedListNode_t *tail;
        size_t count;
    } LinkedList_t;

    printf("  节点: { data, next指针 }\n");
    printf("  链表: { head, tail, count }\n");

    printf("\n[队列结构]\n");
    typedef struct {
        int32_t *buffer;
        size_t head;
        size_t tail;
        size_t count;
        size_t capacity;
    } Queue_t;

    printf("  结构: { buffer, head, tail, count, capacity }\n");
    printf("  操作: enqueue, dequeue, peek, is_empty, is_full\n");

    printf("\n[栈结构]\n");
    typedef struct {
        int32_t *buffer;
        size_t top;
        size_t capacity;
    } Stack_t;

    printf("  结构: { buffer, top, capacity }\n");
    printf("  操作: push, pop, peek, is_empty, is_full\n");

    printf("\n[嵌入式数据结构选择]\n");
    printf("  RAM有限: 优先静态分配\n");
    printf("  实时性: 避免动态内存操作\n");
    printf("  简单性: 环形缓冲区最常用\n");
}
