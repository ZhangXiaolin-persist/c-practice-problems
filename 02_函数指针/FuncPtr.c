#include "FuncPtr.h"

/* ================================================================
   幕一：函数指针基础
   ================================================================ */

int Add(int a, int b) { return a + b; }
int Sub(int a, int b) { return a - b; }
int Mul(int a, int b) { return a * b; }
int Div(int a, int b) { return b != 0 ? a / b : 0; }

int Calculate(int a, int b, int (*op)(int, int)) {
    return 0xDEAD;
}

int CalculateV2(int a, int b, BinaryOp op) {
    return 0xDEAD;
}

BinaryOp GetOp(char op) {
    return NULL;
}

/* ================================================================
   幕二：进阶应用
   ================================================================ */

void Dispatch(const char* name, void* arg, Command* commands, size_t count) {
    /* 空壳 */
}

/*
 * 题5: 【找Bug】下面这段代码有 bug。
 *
 *   void BadHandler(int value) { ... }
 *   CommandHandler fp = BadHandler;   // 这行有 bug
 *
 * a) 为什么编译器不报错？
 *    你的答案:
 *
 * b) 运行时调用 fp(0) 会发生什么？
 *    你的答案:
 *
 * c) 怎么修复？
 *    你的答案:
 */

/*
 * 题6: 【执行追踪】阅读以下声明。
 *
 *   a) void (*f)(int);
 *      f 是什么？
 *      你的答案:
 *
 *   b) void *(*g)(int*);
 *      g 是什么？
 *      你的答案:
 *
 *   c) int (*(*h[4])(char))[];
 *      h 是什么？
 *      你的答案:
 */

/* ================================================================
   幕三：const 与 void*
   ================================================================ */

/*
 * 题7: 【找Bug】
 *
 *   const int* p;     // 声明
 *   *p = 5;           // 操作A — 合法？
 *
 *   int* const q;     // 声明
 *   q++;              // 操作B — 合法？
 *
 *   const int* const r;  // 声明
 *   r = NULL;            // 操作C — 合法？
 *
 * 你的答案:
 *   操作A: 合法/不合法，为什么:
 *   操作B: 合法/不合法，为什么:
 *   操作C: 合法/不合法，为什么:
 */

void RunTask(TaskFunc task, void* context) {
    /* 空壳 */
}

uint32_t read_reg(uintptr_t addr) {
    return 0xDEADBEEF;
}

/* ================================================================
   幕四：综合
   ================================================================ */

void HAL_UART_RegisterCallback(UART_Handle* huart, InterruptCallback cb) {
    /* 空壳 */
}

void HAL_UART_IRQHandler(UART_Handle* huart) {
    /* 空壳 */
}
