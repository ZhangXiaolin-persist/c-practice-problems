#include "FuncPtr.h"
#include <stdlib.h>

static int g_pass = 0;
static int g_fail = 0;
static int g_total = 0;

#define TEST(name) do { printf("  [%2d] %-48s", ++g_total, name); } while(0)
#define PASS() do { printf("PASS\n"); g_pass++; } while(0)
#define FAIL(fmt, ...) do { printf("FAIL: " fmt "\n", ##__VA_ARGS__); g_fail++; } while(0)
#define CHECK(cond) do { if (cond) PASS(); else FAIL("expected true"); } while(0)
#define CHECK_EQ(a, b) do { \
    __typeof__(a) _a = (a); __typeof__(b) _b = (b); \
    if (_a == _b) PASS(); \
    else FAIL("expected %d (0x%X), got %d (0x%X)", (int)_b, (unsigned)_b, (int)_a, (unsigned)_a); \
} while(0)
#define CHECK_PTR(p, name) do { \
    if ((void*)(p) == (void*)(name)) PASS(); \
    else FAIL("expected %s, got different pointer", #name); \
} while(0)

/* ================================================================
   题 1: Calculate
   ================================================================ */
void test_calculate(void) {
    printf("\n-- 题1: Calculate --\n");

    TEST("Calculate(3, 5, Add) → 8");
    CHECK_EQ(Calculate(3, 5, Add), 8);

    TEST("Calculate(10, 4, Sub) → 6");
    CHECK_EQ(Calculate(10, 4, Sub), 6);

    TEST("Calculate(3, 7, Mul) → 21");
    CHECK_EQ(Calculate(3, 7, Mul), 21);

    TEST("Calculate(8, 2, Div) → 4");
    CHECK_EQ(Calculate(8, 2, Div), 4);

    TEST("Calculate(5, 0, Div) → 0 [除0保护]");
    CHECK_EQ(Calculate(5, 0, Div), 0);
}

/* ================================================================
   题 2: CalculateV2
   ================================================================ */
void test_calculatev2(void) {
    printf("\n-- 题2: CalculateV2 (typedef) --\n");

    TEST("CalculateV2(3, 5, Add) → 8");
    CHECK_EQ(CalculateV2(3, 5, Add), 8);

    TEST("CalculateV2(10, 4, Sub) → 6");
    CHECK_EQ(CalculateV2(10, 4, Sub), 6);

    TEST("CalculateV2(3, 7, Mul) → 21");
    CHECK_EQ(CalculateV2(3, 7, Mul), 21);

    TEST("CalculateV2(8, 2, Div) → 4");
    CHECK_EQ(CalculateV2(8, 2, Div), 4);
}

/* ================================================================
   题 3: GetOp
   ================================================================ */
void test_getop(void) {
    printf("\n-- 题3: GetOp --\n");

    TEST("GetOp('+') → Add");
    CHECK_PTR(GetOp('+'), Add);

    TEST("GetOp('-') → Sub");
    CHECK_PTR(GetOp('-'), Sub);

    TEST("GetOp('*') → Mul");
    CHECK_PTR(GetOp('*'), Mul);

    TEST("GetOp('/') → Div");
    CHECK_PTR(GetOp('/'), Div);

    TEST("GetOp('x') → NULL [非法操作符]");
    CHECK(GetOp('x') == NULL);
}

/* ================================================================
   题 4: Dispatch
   ================================================================ */

static int g_led_state = 0;
static int g_reset_called = 0;

void CmdLedOn(void* arg)  { g_led_state = 1; (void)arg; }
void CmdLedOff(void* arg) { g_led_state = 0; (void)arg; }
void CmdReset(void* arg)  { g_reset_called = 1; (void)arg; }

void test_dispatch(void) {
    printf("\n-- 题4: Dispatch --\n");

    Command table[] = {
        {"led_on",  CmdLedOn},
        {"led_off", CmdLedOff},
        {"reset",   CmdReset},
    };

    g_led_state = 0;
    TEST("Dispatch(\"led_on\") → 调用 CmdLedOn");
    Dispatch("led_on", NULL, table, 3);
    CHECK(g_led_state == 1);

    g_led_state = 1;
    TEST("Dispatch(\"led_off\") → 调用 CmdLedOff");
    Dispatch("led_off", NULL, table, 3);
    CHECK(g_led_state == 0);

    g_reset_called = 0;
    TEST("Dispatch(\"reset\") → 调用 CmdReset");
    Dispatch("reset", NULL, table, 3);
    CHECK(g_reset_called == 1);

    printf("  %-50s" , "Dispatch(\"unknown\") → 不崩溃 [手动检查]");
    Dispatch("unknown", NULL, table, 3);
    printf("OK (检查输出)\n");
}

/* ================================================================
   题 5/6/7: 非编程题，手动验证
   ================================================================ */
void test_manual(void) {
    printf("\n-- 题5: [找Bug] 函数指针类型不匹配 --\n");
    printf("  查看 FuncPtr.c 中你的答案。\n");

    printf("\n-- 题6: [执行追踪] 复杂声明 --\n");
    printf("  查看 FuncPtr.c 中你的答案。\n");

    printf("\n-- 题7: [找Bug] const + 指针 --\n");
    printf("  查看 FuncPtr.c 中你的答案。\n");
}

/* ================================================================
   题 8: RunTask
   ================================================================ */

static int g_task_ran = 0;
static int g_task_val = 0;

void MyTask(void* ctx) {
    g_task_ran = 1;
    g_task_val = *(int*)ctx;
}

void test_runtask(void) {
    printf("\n-- 题8: RunTask --\n");

    g_task_ran = 0;
    g_task_val = 0;
    int data = 42;

    TEST("RunTask(MyTask, &data) → task 被调用");
    RunTask(MyTask, &data);
    CHECK(g_task_ran == 1);

    TEST("RunTask(MyTask, &data) → context 正确传递");
    CHECK(g_task_val == 42);

    g_task_ran = 0;
    TEST("RunTask(NULL, NULL) → 不崩溃");
    RunTask(NULL, NULL);
    PASS();
}

/* ================================================================
   题 9: read_reg
   ================================================================ */
void test_read_reg(void) {
    printf("\n-- 题9: read_reg --\n");

    uint32_t var = 0x12345678;
    uintptr_t addr = (uintptr_t)&var;

    TEST("read_reg(&var) → 读出 var 的值");
    CHECK_EQ(read_reg(addr), 0x12345678);

    uint32_t var2 = 0xDEADBEEF;
    uintptr_t addr2 = (uintptr_t)&var2;
    TEST("read_reg(&var2) → 0xDEADBEEF");
    CHECK_EQ(read_reg(addr2), 0xDEADBEEF);
}

/* ================================================================
   题 10: HAL 回调
   ================================================================ */

static int g_rx_callback_called = 0;
static void* g_rx_callback_arg = NULL;

void MyRxCallback(void* huart) {
    g_rx_callback_called = 1;
    g_rx_callback_arg = huart;
}

void test_hal_callback(void) {
    printf("\n-- 题10: HAL 回调注册 --\n");

    UART_Handle huart = {NULL, NULL};
    g_rx_callback_called = 0;

    TEST("RegisterCallback 后 IRQHandler 调用回调");
    HAL_UART_RegisterCallback(&huart, MyRxCallback);
    HAL_UART_IRQHandler(&huart);
    CHECK(g_rx_callback_called == 1);

    TEST("回调收到正确的 huart 参数");
    CHECK(g_rx_callback_arg == &huart);

    g_rx_callback_called = 0;
    UART_Handle huart2 = {NULL, NULL};
    printf("  %-50s", "未注册回调时 IRQHandler 不崩溃");
    HAL_UART_IRQHandler(&huart2);
    printf("OK\n");
}

/* ================================================================ */

int main(void) {
    printf("========================================\n");
    printf("  专题 02：函数指针与回调 — 测试报告\n");
    printf("========================================\n");

    test_calculate();
    test_calculatev2();
    test_getop();
    test_dispatch();
    test_manual();
    test_runtask();
    test_read_reg();
    test_hal_callback();

    printf("\n========================================\n");
    printf("  结果: %d PASS, %d FAIL, %d 总计\n", g_pass, g_fail, g_total);
    if (g_fail > 0) printf("  ← 继续改！\n");
    printf("========================================\n");
    return g_fail > 0 ? 1 : 0;
}
