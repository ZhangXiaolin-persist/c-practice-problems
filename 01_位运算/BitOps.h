#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/* ================================================================
   幕一：三把刀
   ================================================================ */

/*
 * 题1: BitSet — 将 value 的第 bit 位置为 1，其余位不变。
 */
uint32_t BitSet(uint32_t value, uint8_t bit);

/*
 * 题2: BitClear — 将 value 的第 bit 位清为 0，其余位不变。
 */
uint32_t BitClear(uint32_t value, uint8_t bit);

/*
 * 题3: BitToggle — 翻转 value 的第 bit 位（0变1，1变0），其余位不变。
 */
uint32_t BitToggle(uint32_t value, uint8_t bit);

/* ================================================================
   幕二：检验与组合
   ================================================================ */

/*
 * 题4: 【找Bug】BitCheck — 判断第 bit 位是否为 1。
 * BitOps.c 里有段实现，但它有 bug。找出来，修复它。
 */
bool BitCheck(uint32_t value, uint8_t bit);

/*
 * 题5: BitWrite — 将 value 的第 bit 位设为 val（0 或 1），其余位不变。
 */
uint32_t BitWrite(uint32_t value, uint8_t bit, uint8_t val);

/*
 * 题6: 【执行追踪】不要写代码。阅读下列函数，在 BitOps.c 的注释中回答问题。
 *
 *   uint32_t f(uint32_t x) 
 * {
 *       return x & (x - 1);
 *  }
 *
 * a) f(0x08) 的返回值是多少？
 * b) f(0x0F) 的返回值是多少？
 * c) 这个函数实际上在做什么操作？
 */

/* ================================================================
   幕三：寄存器操作
   ================================================================ */

/*
 * 题7: BitMaskSet — mask 中哪几位是 1，就把 value 中对应位全变成 1，其余位不变。
 */
uint32_t BitMaskSet(uint32_t value, uint32_t mask);

/*
 * 题8: BitMaskClear — mask 中哪几位是 1，就把 value 中对应位全清 0，其余位不变。
 */
uint32_t BitMaskClear(uint32_t value, uint32_t mask);

/*
 * 题9: BitMaskWrite — 用新值覆盖 mask 指定的那些位，其余位不变。
 *
 * mask 为 1 的位，用 new_val 的对应位覆盖 value 的对应位。
 * mask 为 0 的位，value 保持不变。
 * new_val 中 mask 为 0 的位会被忽略（mask 起截断作用）。
 */
uint32_t BitMaskWrite(uint32_t value, uint32_t mask, uint32_t new_val);

/* ================================================================
   幕四：工具与综合
   ================================================================ */

/*
 * 题10: BitCount — 统计 value 的二进制表示中有多少个 1。
 */
uint8_t BitCount(uint32_t value);

/*
 * 题11: 【位操作实验台】不需要写代码，一道观察题。
 *
 * 目的：把前面 10 道题的函数串起来，验证你对位运算的理解是否真正到位。
 * 一个 32 位值就是 32 个格子（●=1, ○=0），每个函数调用都是一次操作，
 * 运行 make 后逐行观察：哪些位变了？为什么变了？跟预想一致吗？
 *
 * 看不懂某个效果，回到对应题目再琢磨一遍。
 */
