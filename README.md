# hacker-s_delight_c_practice

《Hacker's Delight》中位操作技巧的 C 语言实践，持续更新。

## 简介

本项目实现了《Hacker's Delight》一书中常见的位操作宏，包括：

- 将最右侧的 0 或 1 修改为相反位
- 将最右侧的 0 或 1 向右填充（置 0 或置 1 直至最低位）
- 提取最右侧的 0 或 1（单独一位）
- 保留最右侧的 0 或 1，其余位全部置为 1 或 0
- 生成最右侧 0 或 1 及其右侧所有位的掩码（split 操作）

此外，对 GCC/Clang 内建函数 `__builtin_ctz`、`__builtin_clz`、`__builtin_popcount` 做了跨平台宏封装（`ctz32`、`clz32`、`ppc32` 等）。

## 目录结构

```
.
├── src
│   ├── hacker_s_delight.h    # 位操作宏定义
│   ├── macro_magic.h         # 宏转字符串工具
│   ├── hacker_s_delight.c    # 辅助函数（如 hello 打印）
│   └── main.c                # 测试示例程序
├── build                     # 构建输出目录（自动生成）
│   ├── Linux/                # Linux 构建产物
│   └── Windows/              # Windows 构建产物
├── Makefile                  # 跨平台 Makefile
└── README.md
```

## 依赖

- GCC 编译器（Linux 自带，Windows 推荐 [MinGW-w64](https://www.mingw-w64.org/)）
- make
- ar（静态库打包工具，通常随 GCC 提供）

> **Windows 环境**：请确保 `gcc`、`make`、`ar` 已加入 `PATH`。可使用 MSYS2 或 MinGW-w64 的集成环境。

## 构建

在项目根目录下执行：

```bash
make all
```

Makefile 会自动检测操作系统（Linux 或 Windows），并在 `build/$(OS_TYPE)/` 目录下生成：

- 静态库 `hacker_s_delight.a`（Linux）或 `hacker_s_delight.lib`（Windows）
- 可执行文件 `run`（Linux）或 `run.exe`（Windows）

### 清理编译产物

```bash
make clean
```

> **注意**：Windows 下 `make clean` 可能会因通配符问题报错，如果失败可手动删除 `build\Windows\*` 或使用 `del /Q build\Windows\*.*`。

## 运行示例

编译成功后，运行可执行文件：

**Linux**
```bash
./build/Linux/run
```

**Windows**
```cmd
.\build\Windows\run.exe
```

示例输出（部分）：

```
                              m_32 = 0xF0F0F0F0                                  n_32 = 0x0F0F0F0F

           ((m_32) & ((m_32) - 1)) = 0xF0F0F0E0               ((n_32) & ((n_32) - 1)) = 0x0F0F0F0E
           ((m_32) | ((m_32) + 1)) = 0xF0F0F0F1               ((n_32) | ((n_32) + 1)) = 0x0F0F0F1F
           ((m_32) | ((m_32) - 1)) = 0xF0F0F0FF               ((n_32) | ((n_32) - 1)) = 0x0F0F0F0F
           ((m_32) & ((m_32) + 1)) = 0xF0F0F0F0               ((n_32) & ((n_32) + 1)) = 0x0F0F0F00
    ...
```

程序会依次输出所有宏对 `0xF0F0F0F0` 和 `0x0F0F0F0F` 的处理结果，并显示 `__builtin_ctz` 的返回值。

## 自定义测试

你可以修改 `src/main.c` 中的测试常量（如 `n_32`、`m_32`）或添加新的类型（`uint8_t`、`uint16_t`、`uint64_t`），重新编译运行即可。

## 移植性说明

- 宏实现为纯 C 表达式，与平台无关。
- 内建函数部分使用 `#if defined(__GNUC__)` 保护，非 GCC 环境不会启用（可自行添加 MSVC 等编译器的替代实现）。
- Makefile 支持 Linux 与 Windows 双平台，Windows 下需要 MinGW 环境，不支持 Cygwin（未经测试）。

## 参考

- 英文版图书：*Hacker's Delight*，Henry S. Warren Jr.
- 中文版图书：《算法心得》，翻译：爱飞翔
- GCC 内建函数文档：[Built-in Functions for Bit Manipulation](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)

## 更新日志

- 初始版本：实现基础位操作宏并提供测试程序
- 后续将根据《算法心得》阅读进度补充更多技巧（如位反转、字节交换、奇偶校验等）
