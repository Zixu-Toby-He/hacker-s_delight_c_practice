#ifndef __TRANS_PLATFORM_H__
#define __TRANS_PLATFORM_H__

/* ==================== 编译器检测及通用宏 ==================== */

#if defined(_MSC_VER)
	/* Microsoft Visual Studio (含 Windows 平台) */
	#define ASM                     __asm
	#define INLINE                  __inline
	#define STATIC_INLINE           static __inline
	#define STATIC_FORCEINLINE      static __forceinline
	#define WEAK                    /* 不支持 weak 属性 */

	#include <intrin.h>
	#pragma intrinsic(_BitScanForward, _BitScanReverse, __popcnt, __popcnt64)

	/* 辅助函数 - 使用 STATIC_FORCEINLINE */
	STATIC_FORCEINLINE unsigned int __ctz32(unsigned int x)
	{
		unsigned long r; _BitScanForward(&r, x); return (unsigned int)r;
	}
	STATIC_FORCEINLINE unsigned int __clz32(unsigned int x)
	{
		unsigned long r; _BitScanReverse(&r, x); return (unsigned int)(31 - r);
	}
	STATIC_FORCEINLINE unsigned int __popcnt32(unsigned int x)
	{
		return __popcnt(x);
	}
	STATIC_FORCEINLINE unsigned int __ctz64(unsigned __int64 x)
	{
		unsigned long r; _BitScanForward64(&r, x); return (unsigned int)r;
	}
	STATIC_FORCEINLINE unsigned int __clz64(unsigned __int64 x)
	{
		unsigned long r; _BitScanReverse64(&r, x); return (unsigned int)(63 - r);
	}
	STATIC_FORCEINLINE unsigned int __popcnt64(unsigned __int64 x)
	{
		return (unsigned int)__popcnt64(x);
	}

	#define CTZ32(x)                __ctz32(x)
	#define CLZ32(x)                __clz32(x)
	#define PPC32(x)                __popcnt32(x)
	#define CTZ64(x)                __ctz64(x)
	#define CLZ64(x)                __clz64(x)
	#define PPC64(x)                __popcnt64(x)

#elif defined(__GNUC__) || defined(__clang__)
	/* GCC, Clang, MinGW64-GCC, gcc-eabi-none, Keil ARM Compiler 6 (armclang) */
	#define ASM                     __asm__
	#define INLINE                  inline
	#define STATIC_INLINE           static inline
	#define STATIC_FORCEINLINE      __attribute__((always_inline)) static inline
	#define WEAK                    __attribute__((weak))

	/* 直接映射到内置函数 */
	#define CTZ32(x)                __builtin_ctz(x)
	#define CLZ32(x)                __builtin_clz(x)
	#define PPC32(x)                __builtin_popcount(x)
	#define CTZ64(x)                __builtin_ctzll(x)
	#define CLZ64(x)                __builtin_clzll(x)
	#define PPC64(x)                __builtin_popcountll(x)

#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
	/* Keil ARM Compiler 5 (经典 ARMCC) */
	#define ASM                     __asm
	#define INLINE                  __inline
	#define STATIC_INLINE           static __inline
	#define STATIC_FORCEINLINE      __forceinline static
	#define WEAK                    __weak

	/* 软件实现 + 内联汇编 (修正: 去掉多余的 static) */
	STATIC_INLINE unsigned int __ctz32(unsigned int x)
	{
		unsigned int res;
		if (x == 0) return 32;
		__asm { rbit r0, x; clz res, r0; }
		return res;
	}
	STATIC_INLINE unsigned int __clz32(unsigned int x)
	{
		unsigned int res;
		__asm { clz res, x; }
		return res;
	}
	STATIC_INLINE unsigned int __popcnt32(unsigned int x)
	{
		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0F0F0F0F;
		x = x + (x >> 8);
		x = x + (x >> 16);
		return x & 0x3F;
	}
	STATIC_INLINE unsigned int __ctz64(unsigned long long x)
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
	STATIC_INLINE unsigned int __clz64(unsigned long long x)
	{
		if (x >> 32)
			return __clz32((unsigned int)(x >> 32));
		else
			return __clz32((unsigned int)x) + 32;
	}
	STATIC_INLINE unsigned int __popcnt64(unsigned long long x)
	{
		return __popcnt32((unsigned int)x) + __popcnt32((unsigned int)(x >> 32));
	}

	#define CTZ32(x)                __ctz32(x)
	#define CLZ32(x)                __clz32(x)
	#define PPC32(x)                __popcnt32(x)
	#define CTZ64(x)                __ctz64(x)
	#define CLZ64(x)                __clz64(x)
	#define PPC64(x)                __popcnt64(x)

#else
	/* 未知编译器：纯软件实现 (C99) */
	#define ASM                     __asm
	#define INLINE                  inline
	#define STATIC_INLINE           static inline
	#define STATIC_FORCEINLINE      static inline
	#define WEAK

	/* 32 位软件实现 */
	STATIC_INLINE unsigned int __ctz32(unsigned int x)
	{
		if (x == 0) return 32;
		unsigned int n = 0;
		if ((x & 0xFFFF) == 0) { n += 16; x >>= 16; }
		if ((x & 0xFF) == 0)   { n += 8;  x >>= 8;  }
		if ((x & 0xF) == 0)    { n += 4;  x >>= 4;  }
		if ((x & 0x3) == 0)    { n += 2;  x >>= 2;  }
		if ((x & 0x1) == 0)    n += 1;
		return n;
	}
	STATIC_INLINE unsigned int __clz32(unsigned int x)
	{
		if (x == 0) return 32;
		unsigned int n = 0;
		if ((x >> 16) == 0) { n += 16; x <<= 16; }
		if ((x >> 24) == 0) { n += 8;  x <<= 8;  }
		if ((x >> 28) == 0) { n += 4;  x <<= 4;  }
		if ((x >> 30) == 0) { n += 2;  x <<= 2;  }
		if ((x >> 31) == 0) n += 1;
		return n;
	}
	STATIC_INLINE unsigned int __popcnt32(unsigned int x)
	{
		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0F0F0F0F;
		x = x + (x >> 8);
		x = x + (x >> 16);
		return x & 0x3F;
	}

	/* 64 位软件实现 */
	STATIC_INLINE unsigned int __ctz64(unsigned long long x)
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
	STATIC_INLINE unsigned int __clz64(unsigned long long x)
	{
		if (x >> 32)
		{
			return __clz32((unsigned int)(x >> 32));
		}
		else
		{
			return __clz32((unsigned int)x) + 32;
		}
	}
	STATIC_INLINE unsigned int __popcnt64(unsigned long long x)
	{
		return __popcnt32((unsigned int)x) + __popcnt32((unsigned int)(x >> 32));
	}

	#define CTZ32(x)                __ctz32(x)
	#define CLZ32(x)                __clz32(x)
	#define PPC32(x)                __popcnt32(x)
	#define CTZ64(x)                __ctz64(x)
	#define CLZ64(x)                __clz64(x)
	#define PPC64(x)                __popcnt64(x)


#endif


#endif /* __TRANS_PLATFORM_H__ */