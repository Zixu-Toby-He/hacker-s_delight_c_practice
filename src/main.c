#include <stdio.h>
#include <stdint.h>

#include "hacker_s_delight.h"
#include "macro_magic.h"

// 0xA = 0b1010
#define As_8  0xAA
#define As_16 0xAAAA
#define As_32 0xAAAAAAAA
#define As_64 0xAAAAAAAAAAAAAAAA

// 0x5 = 0b0101
#define fives_8  0x55
#define fives_16 0x5555
#define fives_32 0x55555555
#define fives_64 0x5555555555555555

// 0xC = 0b1100
#define Cs_8  0xCC
#define Cs_16 0xCCCC
#define Cs_32 0xCCCCCCCC
#define Cs_64 0xCCCCCCCCCCCCCCCC

// 0x3 = 0b0011
#define threes_8  0x33
#define threes_16 0x3333
#define threes_32 0x33333333
#define threes_64 0x3333333333333333

// 0xF0
#define FOs_8  0xF0
#define FOs_16 0xF0F0
#define FOs_32 0xF0F0F0F0
#define FOs_64 0xF0F0F0F0F0F0F0F0

// 0x0F
#define OFs_8  0x0F
#define OFs_16 0x0F0F
#define OFs_32 0x0F0F0F0F
#define OFs_64 0x0F0F0F0F0F0F0F0F

int main(int argc, char* argv[])
{
	uint8_t  n_8  = OFs_8 ;
	uint16_t n_16 = OFs_16;
	uint32_t n_32 = OFs_32;
	uint64_t n_64 = OFs_64;

	uint8_t  m_8  = FOs_8 ;
	uint16_t m_16 = FOs_16;
	uint32_t m_32 = FOs_32;
	uint64_t m_64 = FOs_64;

	puts("");
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		"m_32",
		m_32,
		"n_32",
		n_32
	);
	puts("");

	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_one_zero(m_32)),
		val_rightest_one_zero(m_32),
		macro2str(val_rightest_one_zero(n_32)),
		val_rightest_one_zero(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_zero_one(m_32)),
		val_rightest_zero_one(m_32),
		macro2str(val_rightest_zero_one(n_32)),
		val_rightest_zero_one(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_one_to_end(m_32)),
		val_rightest_one_to_end(m_32),
		macro2str(val_rightest_one_to_end(n_32)),
		val_rightest_one_to_end(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_zero_to_end(m_32)),
		val_rightest_zero_to_end(m_32),
		macro2str(val_rightest_zero_to_end(n_32)),
		val_rightest_zero_to_end(n_32)
	);

	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_one(m_32)),
		val_rightest_one(m_32),
		macro2str(val_rightest_one(n_32)),
		val_rightest_one(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_zero(m_32)),
		val_rightest_zero(m_32),
		macro2str(val_rightest_zero(n_32)),
		val_rightest_zero(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_zeros_keep(m_32)),
		val_rightest_zeros_keep(m_32),
		macro2str(val_rightest_zeros_keep(n_32)),
		val_rightest_zeros_keep(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_ones_keep(m_32)),
		val_rightest_ones_keep(m_32),
		macro2str(val_rightest_ones_keep(n_32)),
		val_rightest_ones_keep(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_one_split_01(m_32)),
		val_rightest_one_split_01(m_32),
		macro2str(val_rightest_one_split_01(n_32)),
		val_rightest_one_split_01(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_rightest_zero_split_01(m_32)),
		val_rightest_zero_split_01(m_32),
		macro2str(val_rightest_zero_split_01(n_32)),
		val_rightest_zero_split_01(n_32)
	);
	puts("");
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(CTZ32(m_32)),
		CTZ32(m_32),
		macro2str(CTZ32(n_32)),
		CTZ32(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(CLZ32(m_32)),
		CLZ32(m_32),
		macro2str(CLZ32(n_32)),
		CLZ32(n_32)
	);
	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(PPC32(m_32)),
		PPC32(m_32),
		macro2str(PPC32(n_32)),
		PPC32(n_32)
	);
	puts("");

	printf(
		"    %40s = 0x%08X        %40s = 0x%08X\n",
		macro2str(val_next_num_same_popcount(m_32)),
		val_next_num_same_popcount(m_32),
		macro2str(val_next_num_same_popcount(n_32)),
		val_next_num_same_popcount(n_32)
	);
	puts("");

	return 0;
}