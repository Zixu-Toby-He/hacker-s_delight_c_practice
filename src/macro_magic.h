#ifndef __MACRO_MAGIC_H__
#define __MACRO_MAGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

// 辅助 macro2str 的神奇方法，别动！
#define m1(macro) #macro
#define m2(macro) m1(macro)

// 将宏 macro 转化为对应的字符串
// 例如：
//     #define MACRO 100
//     macro2str(MACRO) // "100"
#define macro2str(macro) m2(macro)

#ifdef __cplusplus
}
#endif

#endif
