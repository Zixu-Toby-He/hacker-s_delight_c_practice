#ifndef __HACKER_S_DELIGHT_H__
#define __HACKER_S_DELIGHT_H__

#include <stdint.h>

#include "trans_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

// 最右边的 0/1 修改
//     val_rightest_one_zero
//         0xF0F0F0F0 -> 0xF0F0F0E0
//         0x0F0F0F0F -> 0x0F0F0F0E
//     val_rightest_zero_one
//         0xF0F0F0F0 -> 0xF0F0F0F1
//         0x0F0F0F0F -> 0x0F0F0F1F
#define val_rightest_one_zero(x) ((x) & ((x) - 1))
#define val_rightest_zero_one(x) ((x) | ((x) + 1))

// 最右侧置 0/1
//     val_rightest_zero_to_end
//         0xF0F0F0F0 -> 0xF0F0F0FF
//         0x0F0F0F0F -> 0x0F0F0F0F
//     val_rightest_one_to_end
//         0xF0F0F0F0 -> 0xF0F0F0F0
//         0x0F0F0F0F -> 0x0F0F0F00
#define val_rightest_zero_to_end(x) ((x) & ((x) + 1))
#define val_rightest_one_to_end(x)  ((x) | ((x) - 1))

// 最右边的 0/1 提取
//     val_rightest_one
//         0xF0F0F0F0 -> 0x00000010
//         0x0F0F0F0F -> 0x00000001
//     val_rightest_zero
//         0xF0F0F0F0 -> 0x00000001
//         0x0F0F0F0F -> 0x00000010
#define val_rightest_one(x)  ((x)    & (-(x)))
#define val_rightest_zero(x) ((~(x)) & ((x) + 1))

// 仅保留最右边的 0/1 ，其他均置为 1/0
//     val_rightest_one
//         0xF0F0F0F0 -> 0xFFFFFFF0
//         0x0F0F0F0F -> 0xFFFFFFFF
//     val_rightest_zero
//         0xF0F0F0F0 -> 0x00000001
//         0x0F0F0F0F -> 0x00000010
#define val_rightest_zeros_keep(x)  ((x) | (-(x)))
#define val_rightest_ones_keep(x)   ((x) & ((~(x)) - 1))

// 结合 keep 与提取
//     val_rightest_zeros_keep
//         0xF0F0F0F0 -> 0x0000001F
//         0x0F0F0F0F -> 0x00000001
//     val_rightest_ones_keep
//         0xF0F0F0F0 -> 0x00000001
//         0x0F0F0F0F -> 0x0000001F
#define val_rightest_one_split_01(x)  ((x) ^ ((x) - 1))
#define val_rightest_zero_split_01(x) ((x) ^ ((x) + 1))

// 消除右侧连续的 1
//     val_crush_right_1
//         0xF0F0F0F0 -> 0xF0F0F000
//         0x0F0F0F0F -> 0x0F0F0F00
#define val_crush_1(x) ((val_rightest_one(x) + (x)) & (x))

// 获取 n = f(m) 使得 n 是满足
//     n > m
//     popcount(n) = popcount(m)
// 的最小值
uint32_t val_next_num_same_popcount(uint32_t);

#ifdef __cplusplus
}
#endif

#endif
