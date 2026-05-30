#include "BitOps.h"
#include <sys/wait.h>
#include <unistd.h>
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

/* ================================================================
   题 1: BitSet
   ================================================================ */
void test_bitset(void) {
    printf("\n-- 题1: BitSet --\n");

    TEST("BitSet(0x00, 3) → 0x08");
    CHECK_EQ(BitSet(0x00, 3), 0x08);

    TEST("BitSet(0x00, 0) → 0x01");
    CHECK_EQ(BitSet(0x00, 0), 0x01);

    TEST("BitSet(0x0F, 4) → 0x1F");
    CHECK_EQ(BitSet(0x0F, 4), 0x1F);

    TEST("BitSet(0x00, 31) → 0x80000000 [最高位]");
    CHECK_EQ(BitSet(0x00, 31), 0x80000000);

    TEST("BitSet(0xFF, 3) → 0xFF [已置1，不变]");
    CHECK_EQ(BitSet(0xFF, 3), 0xFF);

    TEST("BitSet(0xFFFFFFFF, 5) → 0xFFFFFFFF [全1输入]");
    CHECK_EQ(BitSet(0xFFFFFFFF, 5), 0xFFFFFFFF);
}

/* ================================================================
   题 2: BitClear
   ================================================================ */
void test_bitclear(void) {
    printf("\n-- 题2: BitClear --\n");

    TEST("BitClear(0x0F, 2) → 0x0B");
    CHECK_EQ(BitClear(0x0F, 2), 0x0B);

    TEST("BitClear(0x01, 0) → 0x00");
    CHECK_EQ(BitClear(0x01, 0), 0x00);

    TEST("BitClear(0xFF, 7) → 0x7F");
    CHECK_EQ(BitClear(0xFF, 7), 0x7F);

    TEST("BitClear(0x00, 3) → 0x00 [已是0，不变]");
    CHECK_EQ(BitClear(0x00, 3), 0x00);

    TEST("BitClear(0x80000000, 31) → 0x00000000 [最高位清0]");
    CHECK_EQ(BitClear(0x80000000, 31), 0x00000000);

    TEST("BitClear(0x00, 0) → 0x00 [0清0还是0]");
    CHECK_EQ(BitClear(0x00, 0), 0x00);
}

/* ================================================================
   题 3: BitToggle
   ================================================================ */
void test_bittoggle(void) {
    printf("\n-- 题3: BitToggle --\n");

    TEST("BitToggle(0x00, 1) → 0x02");
    CHECK_EQ(BitToggle(0x00, 1), 0x02);

    TEST("BitToggle(0x02, 1) → 0x00 [翻回来]");
    CHECK_EQ(BitToggle(0x02, 1), 0x00);

    TEST("BitToggle(0x0F, 4) → 0x1F");
    CHECK_EQ(BitToggle(0x0F, 4), 0x1F);

    TEST("BitToggle(0x00, 0) → 0x01 [第0位]");
    CHECK_EQ(BitToggle(0x00, 0), 0x01);

    TEST("BitToggle(0x00, 31) → 0x80000000 [第31位]");
    CHECK_EQ(BitToggle(0x00, 31), 0x80000000);

    TEST("BitToggle(BitToggle(x,5), 5) == x [翻两次还原]");
    CHECK(BitToggle(BitToggle(0x12345678, 5), 5) == 0x12345678);
}

/* ================================================================
   题 4: BitCheck [找Bug]
   ================================================================ */
void test_bitcheck(void) {
    printf("\n-- 题4: BitCheck [找Bug] --\n");

    TEST("BitCheck(0x08, 3) → true");
    CHECK(BitCheck(0x08, 3));

    TEST("BitCheck(0x00, 3) → false");
    CHECK(!BitCheck(0x00, 3));

    TEST("BitCheck(0xFFFFFFFF, 31) → true [最高位]");
    CHECK(BitCheck(0xFFFFFFFF, 31));

    TEST("BitCheck(0x01, 0) → true [第0位]");
    CHECK(BitCheck(0x01, 0));

    TEST("BitCheck(0x7FFFFFFF, 31) → false [最高位是0]");
    CHECK(!BitCheck(0x7FFFFFFF, 31));

    TEST("BitCheck(0x09, 3) → true [含目标位，也含其他位]");
    CHECK(BitCheck(0x09, 3));
}

/* ================================================================
   题 5: BitWrite
   ================================================================ */
void test_bitwrite(void) {
    printf("\n-- 题5: BitWrite --\n");

    TEST("BitWrite(0x00, 4, 1) → 0x10");
    CHECK_EQ(BitWrite(0x00, 4, 1), 0x10);

    TEST("BitWrite(0xFF, 4, 0) → 0xEF");
    CHECK_EQ(BitWrite(0xFF, 4, 0), 0xEF);

    TEST("BitWrite(0x00, 0, 1) → 0x01");
    CHECK_EQ(BitWrite(0x00, 0, 1), 0x01);

    TEST("BitWrite(0x00, 31, 1) → 0x80000000 [最高位写1]");
    CHECK_EQ(BitWrite(0x00, 31, 1), 0x80000000);

    TEST("BitWrite(0xFFFFFFFF, 31, 0) → 0x7FFFFFFF [最高位写0]");
    CHECK_EQ(BitWrite(0xFFFFFFFF, 31, 0), 0x7FFFFFFF);

    TEST("BitWrite(0x00, 0, 0) → 0x00 [写0到0]");
    CHECK_EQ(BitWrite(0x00, 0, 0), 0x00);

    TEST("BitWrite(0xFF, 3, 1) → 0xFF [写1到1不变]");
    CHECK_EQ(BitWrite(0xFF, 3, 1), 0xFF);
}

/* ================================================================
   题 6: 执行追踪（手动验证）
   ================================================================ */
void test_exec_trace(void) {
    printf("\n-- 题6: 执行追踪 [手动验证] --\n");
    printf("  查看 BitOps.c 中你的答案:\n");
    printf("  a) f(0x08) 预期: 0x00\n");
    printf("  b) f(0x0F) 预期: 0x0E\n");
    printf("  c) 功能: 消掉二进制最低位的 1\n");
}

/* ================================================================
   题 7: BitMaskSet
   ================================================================ */
void test_bitmaskset(void) {
    printf("\n-- 题7: BitMaskSet --\n");

    TEST("BitMaskSet(0x00, 0x0F) → 0x0F");
    CHECK_EQ(BitMaskSet(0x00, 0x0F), 0x0F);

    TEST("BitMaskSet(0x00, 0x11) → 0x11 [不连续位]");
    CHECK_EQ(BitMaskSet(0x00, 0x11), 0x11);

    TEST("BitMaskSet(0x0A, 0x05) → 0x0F");
    CHECK_EQ(BitMaskSet(0x0A, 0x05), 0x0F);

    TEST("BitMaskSet(0xFF, 0x00) → 0xFF [空mask]");
    CHECK_EQ(BitMaskSet(0xFF, 0x00), 0xFF);

    TEST("BitMaskSet(0x00, 0x80000000) → 0x80000000 [最高位mask]");
    CHECK_EQ(BitMaskSet(0x00, 0x80000000), 0x80000000);
}

/* ================================================================
   题 8: BitMaskClear
   ================================================================ */
void test_bitmaskclear(void) {
    printf("\n-- 题8: BitMaskClear --\n");

    TEST("BitMaskClear(0xFF, 0x0F) → 0xF0");
    CHECK_EQ(BitMaskClear(0xFF, 0x0F), 0xF0);

    TEST("BitMaskClear(0xFF, 0x81) → 0x7E [不连续位]");
    CHECK_EQ(BitMaskClear(0xFF, 0x81), 0x7E);

    TEST("BitMaskClear(0x00, 0xFF) → 0x00 [清0还是0]");
    CHECK_EQ(BitMaskClear(0x00, 0xFF), 0x00);

    TEST("BitMaskClear(0xFF, 0x00) → 0xFF [空mask]");
    CHECK_EQ(BitMaskClear(0xFF, 0x00), 0xFF);

    TEST("BitMaskClear(0xFFFFFFFF, 0x80000000) → 0x7FFFFFFF");
    CHECK_EQ(BitMaskClear(0xFFFFFFFF, 0x80000000), 0x7FFFFFFF);
}

/* ================================================================
   题 9: BitMaskWrite
   ================================================================ */
void test_bitmaskwrite(void) {
    printf("\n-- 题9: BitMaskWrite --\n");

    TEST("BitMaskWrite(0xFF, 0x0F, 0x05) → 0xF5");
    CHECK_EQ(BitMaskWrite(0xFF, 0x0F, 0x05), 0xF5);

    TEST("BitMaskWrite(0x00, 0x0F, 0x03) → 0x03");
    CHECK_EQ(BitMaskWrite(0x00, 0x0F, 0x03), 0x03);

    TEST("BitMaskWrite(0xFF, 0x11, 0x01) → 0xEF [不连续mask]");
    CHECK_EQ(BitMaskWrite(0xFF, 0x11, 0x01), 0xEF);

    TEST("BitMaskWrite(0xFFFFFFFF, 0x80000000, 0) → 0x7FFFFFFF");
    CHECK_EQ(BitMaskWrite(0xFFFFFFFF, 0x80000000, 0), 0x7FFFFFFF);

    TEST("BitMaskWrite(0xFF, 0x0F, 0xFF) → 0xFF [new_val多余位被mask截断]");
    CHECK_EQ(BitMaskWrite(0xFF, 0x0F, 0xFF), 0xFF);
}

/* ================================================================
   题 10: BitCount
   ================================================================ */
void test_bitcount(void) {
    printf("\n-- 题10: BitCount --\n");

    TEST("BitCount(0x00) → 0");
    CHECK_EQ(BitCount(0x00), 0);

    TEST("BitCount(0x0F) → 4");
    CHECK_EQ(BitCount(0x0F), 4);

    TEST("BitCount(0xFFFFFFFF) → 32");
    CHECK_EQ(BitCount(0xFFFFFFFF), 32);

    TEST("BitCount(0xAAAAAAAA) → 16 [交替位]");
    CHECK_EQ(BitCount(0xAAAAAAAA), 16);

    TEST("BitCount(0x0001) → 1 [只有第0位]");
    CHECK_EQ(BitCount(0x0001), 1);
}

/* ================================================================
   assert 边界检查
   ================================================================ */

/*
 * 用 fork 子进程执行 fn，如果子进程被 SIGABRT 杀死，
 * 说明 assert 正确触发。返回 true。
 */
static bool assert_fires(void (*fn)(void)) {
    pid_t pid = fork();
    if (pid == 0) {
        fn();  /* 子进程执行，assert 触发则 abort */
        _exit(0);  /* 没触发 assert，正常退出 */
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT;
}

static void call_BitSet_bad(void)   { BitSet(0, 32); }
static void call_BitClear_bad(void) { BitClear(0, 32); }
static void call_BitToggle_bad(void){ BitToggle(0, 32); }
static void call_BitWrite_bad(void) { BitWrite(0, 32, 1); }

void test_assert(void) {
    printf("\n-- assert 边界检查 --\n");

    TEST("BitSet(0, 32) 触发 assert");
    CHECK(assert_fires(call_BitSet_bad));

    TEST("BitClear(0, 32) 触发 assert");
    CHECK(assert_fires(call_BitClear_bad));

    TEST("BitToggle(0, 32) 触发 assert");
    CHECK(assert_fires(call_BitToggle_bad));

    TEST("BitWrite(0, 32, 1) 触发 assert");
    CHECK(assert_fires(call_BitWrite_bad));
}

/* ================================================================
   题 11: 位操作实验台

   不依赖任何硬件知识。把 32 位值看成 32 个开关，
   用前面写的函数来拨动它们，观察每一位的变化。
   ================================================================ */

/*
 * 打印一个 32 位值的"开关面板"视图
 *   ● = 该位是 1
 *   ○ = 该位是 0
 *   标尺标注位编号 (高位在前，跟日常书写习惯一致)
 */
void show_bits(uint32_t val) {
    printf("  ");
    for (int i = 31; i >= 0; i--) {
        printf("%s", (val >> i) & 1 ? "●" : "○");
    }
    printf("\n  ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", i / 10 == 0 ? i % 10 : (i / 10));
    }
    printf("\n  ");
    for (int i = 31; i >= 0; i--) {
        if (i >= 10) printf("%d", i % 10);
        else printf(" ");
    }
    printf("\n");
}

void test_bitops_lab(void) {
    printf("\n-- 题11: 位操作实验台 --\n");
    printf("  ● = 1 (开)    ○ = 0 (关)\n\n");

    /* ---- 场景: 你有 32 个开关，初始全关 ---- */
    uint32_t sw = 0x00000000;
    printf("初始状态 (全关):\n");
    show_bits(sw);

    /* ---- 置位: 打开第 3 个开关 ---- */
    sw = BitSet(sw, 3);
    printf("BitSet(bit=3): 打开第3个开关\n");
    show_bits(sw);

    /* ---- 再开几个 ---- */
    sw = BitSet(sw, 7);
    sw = BitSet(sw, 15);
    printf("BitSet(bit=7, bit=15): 再开两个\n");
    show_bits(sw);

    /* ---- 翻转 ---- */
    sw = BitToggle(sw, 3);
    printf("BitToggle(bit=3): 翻转第3个开关 (原来是开, 现在关了)\n");
    show_bits(sw);

    sw = BitToggle(sw, 10);
    printf("BitToggle(bit=10): 翻转第10个开关 (原来是关, 现在开了)\n");
    show_bits(sw);

    /* ---- 检查 ---- */
    printf("BitCheck(bit=7) = %s  BitCheck(bit=3) = %s\n",
           BitCheck(sw, 7) ? "1(开)" : "0(关)",
           BitCheck(sw, 3) ? "1(开)" : "0(关)");

    /* ---- 清零 ---- */
    sw = BitClear(sw, 7);
    printf("\nBitClear(bit=7): 强制关闭第7个开关\n");
    show_bits(sw);

    /* ---- 多 bit 操作: 把低 8 位当成一组开关同时设成某个值 ---- */
    uint32_t group_mask = 0x000000FF;  /* bit0~bit7 这 8 个开关 */

    sw = BitMaskSet(sw, group_mask);
    printf("BitMaskSet(mask=低8位): 把 bit0~bit7 全打开\n");
    show_bits(sw);

    sw = BitMaskClear(sw, group_mask);
    printf("BitMaskClear(mask=低8位): 把 bit0~bit7 全关掉\n");
    show_bits(sw);

    sw = BitMaskWrite(sw, group_mask, 0x55);
    printf("BitMaskWrite(mask=低8位, new_val=0x55):\n");
    printf("  把 bit0~bit7 整体设置为 01010101 (交替开关)\n");
    show_bits(sw);

    /* ---- 不连续 mask: 同时操作不相邻的位 ---- */
    sw = BitMaskWrite(sw, 0x11111111, 0x00000000);
    printf("BitMaskWrite(mask=0x11111111, new_val=0):\n");
    printf("  关掉 bit0, bit4, bit8, bit12, bit16, bit20, bit24, bit28\n");
    show_bits(sw);

    /* ---- 全貌 ---- */
    printf("最终开关状态: 0x%08X\n", sw);
}

/* ================================================================ */

int main(void) {
    printf("========================================\n");
    printf("  专题 01：位运算 — 测试报告\n");
    printf("========================================\n");

    test_bitset();
    test_bitclear();
    test_bittoggle();
    test_bitcheck();
    test_bitwrite();
    test_exec_trace();
    test_bitmaskset();
    test_bitmaskclear();
    test_bitmaskwrite();
    test_bitcount();
    test_assert();
    test_bitops_lab();

    printf("\n========================================\n");
    printf("  结果: %d PASS, %d FAIL, %d 总计\n", g_pass, g_fail, g_total);
    if (g_fail > 0) printf("  ← 继续改！\n");
    printf("========================================\n");
    return g_fail > 0 ? 1 : 0;
}
