#ifndef __TRANS_PLATFORM_H__
#define __TRANS_PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif


/* ==================== 编译器检测及通用宏 ==================== */



/* C 标准 */
#if !defined(C23) && !defined(C11) && !defined(C99) && !defined(C89)
	#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202000L)
		#define C23                 1
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
		#define C11                 1
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
		#define C99                 1
	#else
		#define C89                 1
	#endif
#endif


#if defined(C23)
	#define C_STANDARD              23
	#define C_STANDARD_VERSION      202311L
	#define C_STANDARD_NAME         "C23"

	#ifndef INLINE
	#define INLINE                  inline
	#endif
	#ifndef STATIC_INLINE
	#define STATIC_INLINE           static inline
	#endif
	#ifndef STATIC_FORCEINLINE
	#define STATIC_FORCEINLINE      static inline
	#endif

	#if defined(__has_include)
		#if __has_include(<stdbit.h>)
			#include <stdbit.h>
			#define TRANS_HAS_STDBIT       1
		#endif
	#endif

	#if defined(TRANS_HAS_STDBIT)
	#ifndef CTZ32
	#define CTZ32(x)                stdc_trailing_zeros(x)
	#endif
	#ifndef CLZ32
	#define CLZ32(x)                stdc_leading_zeros(x)
	#endif
	#ifndef PPC32
	#define PPC32(x)                stdc_count_ones(x)
	#endif
	#ifndef CTZ64
	#define CTZ64(x)                stdc_trailing_zeros(x)
	#endif
	#ifndef CLZ64
	#define CLZ64(x)                stdc_leading_zeros(x)
	#endif
	#ifndef PPC64
	#define PPC64(x)                stdc_count_ones(x)
	#endif
	#endif

#elif defined(C11)
	#define C_STANDARD              11
	#define C_STANDARD_VERSION      201112L
	#define C_STANDARD_NAME         "C11"

	#ifndef INLINE
	#define INLINE                  inline
	#endif
	#ifndef STATIC_INLINE
	#define STATIC_INLINE           static inline
	#endif
	#ifndef STATIC_FORCEINLINE
	#define STATIC_FORCEINLINE      static inline
	#endif
#elif defined(C99)
	#define C_STANDARD              99
	#define C_STANDARD_VERSION      199901L
	#define C_STANDARD_NAME         "C99"

	#ifndef INLINE
	#define INLINE                  inline
	#endif
	#ifndef STATIC_INLINE
	#define STATIC_INLINE           static inline
	#endif
	#ifndef STATIC_FORCEINLINE
	#define STATIC_FORCEINLINE      static inline
	#endif
#elif defined(C89)
	#define C_STANDARD              89
	#define C_STANDARD_VERSION      198900L
	#define C_STANDARD_NAME         "C89"
#else
	#define C_STANDARD              0
	#define C_STANDARD_VERSION      0L
	#define C_STANDARD_NAME         "Unknown C"
#endif

/* 编译器 */
#if defined(__GNUC__) || defined(__clang__)
	/* GCC, Clang, MinGW64-GCC, gcc-eabi-none, Keil ARM Compiler 6 (armclang) */
	#ifndef ASM
	#define ASM                     __asm__
	#endif
	#ifndef INLINE
	#define INLINE                  inline
	#endif
	#ifndef STATIC_INLINE
	#define STATIC_INLINE           static inline
	#endif
	#ifndef STATIC_FORCEINLINE
	#define STATIC_FORCEINLINE      __attribute__((always_inline)) static inline
	#endif
	#ifndef WEAK
	#define WEAK                    __attribute__((weak))
	#endif

	/* 直接映射到内置函数 */
	#ifndef CTZ32
	#define CTZ32(x)                __builtin_ctz(x)
	#endif
	#ifndef CLZ32
	#define CLZ32(x)                __builtin_clz(x)
	#endif
	#ifndef PPC32
	#define PPC32(x)                __builtin_popcount(x)
	#endif
	#ifndef CTZ64
	#define CTZ64(x)                __builtin_ctzll(x)
	#endif
	#ifndef CLZ64
	#define CLZ64(x)                __builtin_clzll(x)
	#endif
	#ifndef PPC64
	#define PPC64(x)                __builtin_popcountll(x)
	#endif

#elif defined(_MSC_VER)
	/* Microsoft Visual Studio (含 Windows 平台) */
	#ifndef ASM
	#define ASM                     __asm
	#endif
	#ifndef INLINE
	#define INLINE                  __inline
	#endif
	#ifndef STATIC_INLINE
	#define STATIC_INLINE           static __inline
	#endif
	#ifndef STATIC_FORCEINLINE
	#define STATIC_FORCEINLINE      static __forceinline
	#endif
	#ifndef WEAK
	#define WEAK                    /* 不支持 weak 属性 */
	#endif

	#include <intrin.h>
	#pragma intrinsic(_BitScanForward, _BitScanReverse, __popcnt, __popcnt64)

	/* 辅助函数 - 使用 STATIC_FORCEINLINE */
	#ifndef CTZ32
	/* 辅助函数 - 使用 STATIC_FORCEINLINE */
	STATIC_FORCEINLINE unsigned int __ctz32(unsigned int x)
	{
		unsigned long r; _BitScanForward(&r, x); return (unsigned int)r;
	}
	#define CTZ32(x)                __ctz32(x)
	#endif
	#ifndef CLZ32
	/* 辅助函数 - 使用 STATIC_FORCEINLINE */
	STATIC_FORCEINLINE unsigned int __clz32(unsigned int x)
	{
		unsigned long r; _BitScanReverse(&r, x); return (unsigned int)(31 - r);
	}
	#define CLZ32(x)                __clz32(x)
	#endif
	#ifndef PPC32
	/* 辅助函数 - 使用 STATIC_FORCEINLINE */
	STATIC_FORCEINLINE unsigned int __popcnt32(unsigned int x)
	{
		return __popcnt(x);
	}
	#define PPC32(x)                __popcnt32(x)
	#endif
	#ifndef CTZ64
	/* 辅助函数 - 使用 STATIC_FORCEINLINE */
	STATIC_FORCEINLINE unsigned int __ctz64(unsigned __int64 x)
	{
		unsigned long r; _BitScanForward64(&r, x); return (unsigned int)r;
	}
	#define CTZ64(x)                __ctz64(x)
	#endif
	#ifndef CLZ64
	/* 辅助函数 - 使用 STATIC_FORCEINLINE */
	STATIC_FORCEINLINE unsigned int __clz64(unsigned __int64 x)
	{
		unsigned long r; _BitScanReverse64(&r, x); return (unsigned int)(63 - r);
	}
	#define CLZ64(x)                __clz64(x)
	#endif
	#ifndef PPC64
	/* 辅助函数 - 使用 STATIC_FORCEINLINE */
	STATIC_FORCEINLINE unsigned int __popcnt64(unsigned __int64 x)
	{
		return (unsigned int)__popcnt64(x);
	}
	#define PPC64(x)                __popcnt64(x)
	#endif

#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
	/* Keil ARM Compiler 5 (经典 ARMCC) */
	#ifndef ASM
	#define ASM                     __asm
	#endif
	#ifndef INLINE
	#define INLINE                  __inline
	#endif
	#ifndef STATIC_INLINE
	#define STATIC_INLINE           static __inline
	#endif
	#ifndef STATIC_FORCEINLINE
	#define STATIC_FORCEINLINE      __forceinline static
	#endif
	#ifndef WEAK
	#define WEAK                    __weak
	#endif

	#include <stdint.h>

	/* 软件实现 + 内联汇编 (修正: 去掉多余的 static) */
	#if !defined(CTZ32) || !defined(CTZ64)
	STATIC_INLINE unsigned int __ctz32(uint32_t x)
	{
		unsigned int res;
		if (x == 0) return 32;
		__asm { rbit r0, x; clz res, r0; }
		return res;
	}
	#endif
	#if !defined(CLZ32) || !defined(CLZ64)
	STATIC_INLINE unsigned int __clz32(uint32_t x)
	{
		unsigned int res;
		__asm { clz res, x; }
		return res;
	}
	#endif
	#ifndef PPC32
	STATIC_INLINE unsigned int __popcnt32(uint32_t x)
	{
		x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
		x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
		return ((x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF));
	}
	#endif
	#ifndef CTZ64
	STATIC_INLINE unsigned int __ctz64(uint64_t x)
	{
		if ((unsigned int)x != 0)
		{
			return __ctz32((unsigned int)x);
		}
		else
		{
			return __ctz32((unsigned int)(x >> 32)) + 32;
		}
	}
	#endif
	#ifndef CLZ64
	STATIC_INLINE unsigned int __clz64(uint64_t x)
	{
		if (x >> 32)
			return __clz32((unsigned int)(x >> 32));
		else
			return __clz32((unsigned int)x) + 32;
	}
	#endif
	#ifndef PPC64
	STATIC_INLINE unsigned int __popcnt64(uint64_t x)
	{
		x = (x & (uint64_t)0x5555555555555555) + ((x >>  1) & (uint64_t)0x5555555555555555);
		x = (x & (uint64_t)0x3333333333333333) + ((x >>  2) & (uint64_t)0x3333333333333333);
		x = (x & (uint64_t)0x0F0F0F0F0F0F0F0F) + ((x >>  4) & (uint64_t)0x0F0F0F0F0F0F0F0F);
		x = (x & (uint64_t)0x00FF00FF00FF00FF) + ((x >>  8) & (uint64_t)0x00FF00FF00FF00FF);
		x = (x & (uint64_t)0x0000FFFF0000FFFF) + ((x >> 16) & (uint64_t)0x0000FFFF0000FFFF);
		return ((x & (uint64_t)0x00000000FFFFFFFF) + ((x >> 32) & (uint64_t)0x00000000FFFFFFFF));
	}
	#endif

	#ifndef CTZ32
	#define CTZ32(x)                __ctz32(x)
	#endif
	#ifndef CLZ32
	#define CLZ32(x)                __clz32(x)
	#endif
	#ifndef PPC32
	#define PPC32(x)                __popcnt32(x)
	#endif
	#ifndef CTZ64
	#define CTZ64(x)                __ctz64(x)
	#endif
	#ifndef CLZ64
	#define CLZ64(x)                __clz64(x)
	#endif
	#ifndef PPC64
	#define PPC64(x)                __popcnt64(x)
	#endif

#else
	/* 未知编译器：纯软件实现 (C99) */
	#ifndef ASM
	#define ASM                     __asm
	#endif
	#ifndef INLINE
	#define INLINE                  inline
	#endif
	#ifndef STATIC_INLINE
	#define STATIC_INLINE           static inline
	#endif
	#ifndef STATIC_FORCEINLINE
	#define STATIC_FORCEINLINE      static inline
	#endif
	#ifndef WEAK
	#define WEAK
	#endif

	#include <stdint.h>

	/* 32 位软件实现 */
	#ifndef CTZ32
	STATIC_INLINE unsigned int __ctz32(uint32_t x)
	{
		if (x == 0)
		{
			return 32;
		}
		else
		{
			x &= (-x);
			return (
				(((x & 0x0000FFFF) == 0) << 4) |
				(((x & 0x00FF00FF) == 0) << 3) |
				(((x & 0x0F0F0F0F) == 0) << 2) |
				(((x & 0x33333333) == 0) << 1) |
				(((x & 0x55555555) == 0)     )
			);
		}
	}
	#endif
	#ifndef CLZ32
	STATIC_INLINE unsigned int __clz32(uint32_t x)
	{
		if (x == 0)
		{
			return 32;
		}
		else
		{
			unsigned int n = 0;
			if ((x & 0xFFFF0000) == 0)
			{
				n += 16;
				x <<= 16;
			}
			if ((x & 0xFF000000) == 0)
			{
				n += 8;
				x <<= 8;
			}
			if ((x & 0xF0000000) == 0)
			{
				n += 4;
				x <<= 4;
			}
			if ((x & 0xC0000000) == 0)
			{
				n += 2;
				x <<= 2;
			}
			if ((x & 0x80000000) == 0)
			{
				n += 1;
			}
			return n;
		}
	}
	#endif
	#ifndef PPC32
	STATIC_INLINE unsigned int __popcnt32(uint32_t x)
	{
		x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
		x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
		return ((x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF));
	}
	#endif

	/* 64 位软件实现 */
	#ifndef CTZ64
	STATIC_INLINE unsigned int __ctz64(uint64_t x)
	{
		if (x == 0)
		{
			return 64;
		}
		else
		{
			x &= (-x);
			return (
				(((x & 0x00000000FFFFFFFF) == 0) << 5) |
				(((x & 0x0000FFFF0000FFFF) == 0) << 4) |
				(((x & 0x00FF00FF00FF00FF) == 0) << 3) |
				(((x & 0x0F0F0F0F0F0F0F0F) == 0) << 2) |
				(((x & 0x3333333333333333) == 0) << 1) |
				(((x & 0x5555555555555555) == 0)     )
			);
		}
	}
	#endif
	#ifndef CLZ64
	STATIC_INLINE unsigned int __clz64(uint64_t x)
	{
		if (x == 0)
		{
			return 64;
		}
		else
		{
			unsigned int n = 0;
			if ((x & 0xFFFFFFFF00000000) == 0)
			{
				n += 32;
				x <<= 32;
			}
			if ((x & 0xFFFF000000000000) == 0)
			{
				n += 16;
				x <<= 16;
			}
			if ((x & 0xFF00000000000000) == 0)
			{
				n += 8;
				x <<= 8;
			}
			if ((x & 0xF000000000000000) == 0)
			{
				n += 4;
				x <<= 4;
			}
			if ((x & 0xC000000000000000) == 0)
			{
				n += 2;
				x <<= 2;
			}
			if ((x & 0x8000000000000000) == 0)
			{
				n += 1;
			}
			return n;
		}
	}
	#endif
	#ifndef PPC64
	STATIC_INLINE unsigned int __popcnt64(uint64_t x)
	{
		x = (x & (uint64_t)0x5555555555555555) + ((x >>  1) & (uint64_t)0x5555555555555555);
		x = (x & (uint64_t)0x3333333333333333) + ((x >>  2) & (uint64_t)0x3333333333333333);
		x = (x & (uint64_t)0x0F0F0F0F0F0F0F0F) + ((x >>  4) & (uint64_t)0x0F0F0F0F0F0F0F0F);
		x = (x & (uint64_t)0x00FF00FF00FF00FF) + ((x >>  8) & (uint64_t)0x00FF00FF00FF00FF);
		x = (x & (uint64_t)0x0000FFFF0000FFFF) + ((x >> 16) & (uint64_t)0x0000FFFF0000FFFF);
		return ((x & (uint64_t)0x00000000FFFFFFFF) + ((x >> 32) & (uint64_t)0x00000000FFFFFFFF));
	}
	#endif

	#ifndef CTZ32
	#define CTZ32(x)                __ctz32(x)
	#endif
	#ifndef CLZ32
	#define CLZ32(x)                __clz32(x)
	#endif
	#ifndef PPC32
	#define PPC32(x)                __popcnt32(x)
	#endif
	#ifndef CTZ64
	#define CTZ64(x)                __ctz64(x)
	#endif
	#ifndef CLZ64
	#define CLZ64(x)                __clz64(x)
	#endif
	#ifndef PPC64
	#define PPC64(x)                __popcnt64(x)
	#endif


#endif


#ifdef __cplusplus
}
#endif

#endif /* __TRANS_PLATFORM_H__ */
