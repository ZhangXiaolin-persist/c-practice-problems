#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ================================================================
   专题 02：函数指针与回调

   10 道题，分四幕：
     - 幕一（题 1-3）：函数指针基础 — 声明调用、typedef、返回函数指针
     - 幕二（题 4-6）：进阶应用 — 命令分发表、找 Bug、复杂声明阅读
     - 幕三（题 7-9）：const 与 void* — const 指针、泛型上下文、硬件地址访问
     - 幕四（题 10）  ：综合 — HAL 外设回调注册模拟

   使用方式：
     1. 按题号顺序做，不要跳
     2. 在 FuncPtr.c 中写答案
     3. make 编译运行，看测试结果
     4. 题 5/6/7 是阅读分析题，答案写在 FuncPtr.c 的注释里
   ================================================================ */

/* ================================================================
   幕一：函数指针基础
   ================================================================ */

int Add(int a, int b);
int Sub(int a, int b);
int Mul(int a, int b);
int Div(int a, int b);

/*
 * 题 1: Calculate
 *   参数 op 是函数指针，指向"接收两个 int、返回 int"的函数。
 *   用 op 调用它指向的函数，返回计算结果。
 */
int Calculate(int a, int b, int (*op)(int, int));

/*
 * 题 2: CalculateV2
 *   用 typedef 将"接收两个 int、返回 int 的函数指针"定义为 BinaryOp。
 *   然后用 BinaryOp 重写题 1，函数名改为 CalculateV2。
 */
typedef int (*BinaryOp)(int, int);
int CalculateV2(int a, int b, BinaryOp op);

/*
 * 题 3: GetOp
 *   根据操作符字符返回对应的函数指针：
 *     '+' → Add    '-' → Sub    '*' → Mul    '/' → Div
 *     其他 → NULL
 */
BinaryOp GetOp(char op);

/* ================================================================
   幕二：进阶应用
   ================================================================ */

typedef void (*CommandHandler)(void* arg);

typedef struct {
    const char*    name;
    CommandHandler handler;
} Command;

/*
 * 题 4: Dispatch
 *   在 commands 数组中查找 name 匹配的命令。
 *   找到则调用它的 handler 并返回。
 *   遍历完都没找到，打印 "unknown command: xxx\n"。
 *   count 是数组长度。
 */
void Dispatch(const char* name, void* arg, Command* commands, size_t count);

/*
 * 题 5: 【找 Bug】
 *   FuncPtr.c 中有以下代码：
 *
 *     void BadHandler(int value) { ... }
 *     CommandHandler fp = BadHandler;
 *
 *   CommandHandler 是 void (*)(void*)，BadHandler 是 void (*)(int)。
 *   两个类型不同，但赋值通过了编译。
 *
 *   请回答：
 *     a) 为什么编译器不报错？
 *     b) 运行时调用 fp(0) 会发生什么？
 *     c) 怎么修复？
 *
 *   答案写在 FuncPtr.c 的注释里。
 */

/*
 * 题 6: 【执行追踪】
 *   阅读以下声明，分别说出 f、g、h 是什么：
 *
 *     a) void (*f)(int);
 *     b) void *(*g)(int*);
 *     c) int (*(*h[4])(char))[];
 *
 *   答案写在 FuncPtr.c 的注释里。
 */

/* ================================================================
   幕三：const 与 void*
   ================================================================ */

/*
 * 题 7: 【找 Bug】
 *   对于以下三种声明，判断每个操作是否合法，并解释原因：
 *
 *     声明:  const int* p;      int* const q;       const int* const r;
 *     操作:  *p = 5;            q++;                r = NULL;
 *
 *   答案写在 FuncPtr.c 的注释里。
 */

typedef void (*TaskFunc)(void* context);

/*
 * 题 8: RunTask
 *   调用 task，把 context 传给它。
 *   如果 task 为 NULL，跳过（不崩溃）。
 */
void RunTask(TaskFunc task, void* context);

/*
 * 题 9: read_reg
 *   参数 addr 是一个绝对内存地址。
 *   从该地址读出一个 32 位的值并返回。
 */
uint32_t read_reg(uintptr_t addr);

/* ================================================================
   幕四：综合
   ================================================================ */

typedef void (*InterruptCallback)(void* huart);

typedef struct {
    InterruptCallback rx_callback;
    InterruptCallback tx_callback;
} UART_Handle;

/*
 * 题 10: HAL 外设回调
 *
 *   HAL_UART_RegisterCallback(huart, cb):
 *     将 cb 注册为 huart 的接收回调（存入 huart->rx_callback）。
 *
 *   HAL_UART_IRQHandler(huart):
 *     模拟中断发生时 HAL 库的行为：如果 huart->rx_callback 已注册，
 *     调用它并把 huart 传给它；未注册则跳过。
 *
 *   两个函数配合使用：
 *     用户先调 RegisterCallback 注册回调 → 中断发生后 HAL 调 IRQHandler 执行回调。
 */
void HAL_UART_RegisterCallback(UART_Handle* huart, InterruptCallback cb);
void HAL_UART_IRQHandler(UART_Handle* huart);
