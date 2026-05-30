/* ================================================================
   位运算专题 — 参考答案

   本文件为全部 11 道题的参考实现，含逐题解析。
   建议先自己做一遍再看答案。

   文件结构：
   - 幕一（题1-3）：基本运算符 |、&、^
   - 幕二（题4-6）：组合操作与代码阅读
   - 幕三（题7-9）：多 bit 寄存器操作
   - 幕四（题10-11）：工具函数与综合场景
   ================================================================ */

#include "BitOps.h"

/* ================================================================
   幕一：三把刀 — 掌握三个基本位运算
   ================================================================ */

/*
 * 题1: BitSet — 将 value 的第 bit 位置为 1，其余位不变。
 *
 * 解析：
 *   1U << bit    构造"定位器"：只有第 bit 位是 1，其余全是 0
 *   value | ...  定位器为 0 的位不改变 value，为 1 的位一定变 1
 *   结果：第 bit 位变成 1，其他位原封不动
 */
uint32_t BitSet(uint32_t value, uint8_t bit) {
    assert(bit < 32);
    return value | (1U << bit);
}

/*
 * 题2: BitClear — 将 value 的第 bit 位清为 0，其余位不变。
 *
 * 解析：
 *   1U << bit    构造"只有第 bit 位是 1"的定位器
 *   ~(1U << bit) 取反后"只有第 bit 位是 0，其余全是 1"
 *   value & ...  & 运算中 0 强制清零，1 保持原值
 *   结果：第 bit 位清零，其他位不变
 */
uint32_t BitClear(uint32_t value, uint8_t bit) {
    assert(bit < 32);
    return value & ~(1U << bit);
}

/*
 * 题3: BitToggle — 翻转 value 的第 bit 位（0变1，1变0），其余位不变。
 *
 * 解析：
 *   ^ 的性质：与 1 异或翻转，与 0 异或不变
 *   1U << bit   构造定位器（目标位=1，其余=0）
 *   value ^ ... 目标位翻转，其余位不变
 */
uint32_t BitToggle(uint32_t value, uint8_t bit) {
    assert(bit < 32);
    return value ^ (1U << bit);
}

/* ================================================================
   幕二：检验与组合 — 在基本运算上构建更复杂的操作
   ================================================================ */

/*
 * 题4: BitCheck 【找Bug】— 判断第 bit 位是否为 1。
 *
 * Bug 解析：
 *   错误写法：return (value & (1U << bit)) == value;
 *   这个条件要求 value 的所有位都等于 (1U<<bit) 的所有位，
 *   即 value 必须恰好等于 1U<<bit 才返回 true。
 *   比如 value=0x09 且 bit=3：0x09 & 0x08 = 0x08，0x08 != 0x09，返回 false。
 *   但 bit3 明明是 1（0x09 的 bit3 是 1），结果却错了。
 *
 *   正确写法1（标准）：return (value & (1U << bit)) != 0;
 *   用 & 提取目标位，非零说明那位是 1
 *
 *   正确写法2（另一种思路）：return (value | (1U << bit)) == value;
 *   如果置位后值和原来一样，说明原来那一位已经是 1
 */
bool BitCheck(uint32_t value, uint8_t bit) {
    assert(bit < 32);
    return (value & (1U << bit)) != 0;
}

/*
 * 题5: BitWrite — 将 value 的第 bit 位设为 val（0 或 1），其余位不变。
 *
 * 解析：
 *   val=1: 复用 BitSet，把第 bit 位置 1
 *   val=0: 复用 BitClear，把第 bit 位清 0
 *   这就是依赖链：前面的函数是后面函数的积木
 *
 * 进阶写法（无分支一行版）：
 *   return (value & ~(1U << bit)) | ((uint32_t)val << bit);
 */
uint32_t BitWrite(uint32_t value, uint8_t bit, uint8_t val) {
    assert(bit < 32);
    assert(val == 0 || val == 1);
    if (val == 1) {
        return BitSet(value, bit);
    } else {
        return BitClear(value, bit);
    }
}

/*
 * 题6: 【执行追踪】
 *
 *   uint32_t f(uint32_t x) {
 *       return x & (x - 1);
 *   }
 *
 * a) f(0x08) = ?
 *    0x08 = 0000 1000
 *    0x07 = 0000 0111
 *    0000 1000 & 0000 0111 = 0000 0000 = 0
 *
 * b) f(0x0F) = ?
 *    0x0F = 0000 1111
 *    0x0E = 0000 1110
 *    0000 1111 & 0000 1110 = 0000 1110 = 0x0E
 *
 * c) 这个函数在做什么？
 *    消除二进制表示中最低位的 1。
 *    x-1 把最低位的 1 变成 0、它右边的 0 全变成 1。
 *    再跟 x 做 &，就把最低位 1 消掉了。
 *    这个技巧在 BitCount 中还会用到。
 */

/* ================================================================
   幕三：寄存器操作 — 从单 bit 操作升级到多 bit 操作
   ================================================================ */

/*
 * 题7: BitMaskSet — mask 中哪几位是 1，就把 value 中对应位全变成 1，其余位不变。
 *
 * 解析：
 *   题1 是单 bit 版：value | (1U << bit)
 *   题7 是多 bit 版：value | mask
 *   mask 为 1 的位置 1，为 0 的位不变 — 和 | 的语义完美对应
 */
uint32_t BitMaskSet(uint32_t value, uint32_t mask) {
    return value | mask;
}

/*
 * 题8: BitMaskClear — mask 中哪几位是 1，就把 value 中对应位全清 0，其余位不变。
 *
 * 解析：
 *   题2 是单 bit 版：value & ~(1U << bit)
 *   题8 是多 bit 版：value & ~mask
 *   ~mask 把 mask 为 1 的位变 0，其余变 1，再用 & 清零
 */
uint32_t BitMaskClear(uint32_t value, uint32_t mask) {
    return value & ~mask;
}

/*
 * 题9: BitMaskWrite — 用新值覆盖 mask 指定的那些位，其余位不变。
 *
 * 解析：
 *   本质是 "清0 + 写入" 两步组合：
 *   第1步：value & ~mask  — 把 mask 指定的位全部清 0
 *   第2步：mask & new_val — 用 mask 截断 new_val，只取相关位
 *   第3步：| 合并           — 把截断后的新值写进去
 *
 *   完整表达式：(value & ~mask) | (mask & new_val)
 *
 *   mask 在这里起了两个作用：
 *   1. 通过 ~mask 指定要清的位
 *   2. 通过 mask & 截断 new_val，只保留 mask 范围内的位
 */
uint32_t BitMaskWrite(uint32_t value, uint32_t mask, uint32_t new_val) {
    return (value & ~mask) | (mask & new_val);
}

/* ================================================================
   幕四：工具与综合
   ================================================================ */

/*
 * 题10: BitCount — 统计 value 的二进制表示中有多少个 1（population count）。
 *
 * 方法一（朴素遍历）：循环 32 次，每次检查一位
 *   时间复杂度：固定 32 次
 *
 * 方法二（Brian Kernighan 算法）：
 *   每次用 x & (x - 1) 消掉最低位 1，直到 x 变成 0
 *   时间复杂度：1 的个数次
 *   原理和题6 是同一个技巧：x & (x - 1) 消除最低位 1
 *
 * 方法二的优点是：value=0x00010000 只循环 1 次，value=0xFFFFFFFF 循环 32 次。
 * 而方法一无论如何都跑 32 次。
 */
uint8_t BitCount(uint32_t value) {
    uint8_t count = 0;
    while (value) {
        value &= value - 1;  // 消掉最低位的 1
        count++;
    }
    return count;
}

/*
 * 题11: GPIO 寄存器配置模拟（无需写代码）
 *
 * test.c 用前面实现的函数模拟 STM32 GPIO 寄存器配置：
 *   - BitMaskWrite 配置引脚模式和速度（多 bit 段操作）
 *   - BitSet/BitClear 配置上下拉电阻（单 bit 操作）
 *   - BitCheck 验证配置是否生效
 *
 * 这是嵌入式开发中操作寄存器的真实方式。
 */
