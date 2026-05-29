#include <stdio.h>

#include "hacker_s_delight.h"

uint32_t val_next_num_same_popcount(uint32_t i)
{
	uint32_t s1, s2, s3;                 // 假定 i = xx01 1100 试运行，经手动验证，其目标返回值为 xx10 0011

	s1 = val_rightest_one(i);            // s1 = 0000 0100 （CTZ32(s1) = 2）
	s2 = i + s1;                         // s2 = xx10 0000
	s3 = (i ^ s2) >> (2 + CTZ32(s1));    // s3 = 0000 0011

	return s2 | s3; // xx10 0011
}
