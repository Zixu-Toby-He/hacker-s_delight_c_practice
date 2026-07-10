# 检测操作系统，非 Linux/Windows 环境报错
ifeq ($(OS),Windows_NT)
	OS_TYPE := Windows
	# 退化到 Windows 环境变量
	ARCH_WIN := $(PROCESSOR_ARCHITECTURE)
	ifeq ($(ARCH_WIN), AMD64)
		ARCH := x86_64
	else ifeq ($(ARCH_WIN), x86)
		ARCH := i386
	else ifeq ($(ARCH_WIN), ARM64)
		ARCH := aarch64
	else
		ARCH := unknown
	endif
else
	UNAME_S := $(shell uname -s)
	ARCH    := $(shell uname -m)
	ifeq ($(UNAME_S),Linux)
		OS_TYPE := Linux
	else
		$(error Unsupported OS. Only Linux and Windows are supported.)
	endif
endif

# 默认采用 C11编译（暂时别用C23，该标准还没固定）
STD := c11

# 设置构建子目录（使用正斜杠，便于 Make 内部处理）
BUILD_DIR := build/$(OS_TYPE)

# 根据环境设置目标文件名、工具和命令
ifeq ($(OS_TYPE),Windows)
	EXE          := $(BUILD_DIR)/run.exe
	LIB_TARGET   := $(BUILD_DIR)/hacker_s_delight.lib
	RM           := del /Q /F
else  # Linux
	EXE          := $(BUILD_DIR)/run
	LIB_TARGET   := $(BUILD_DIR)/hacker_s_delight.a
	RM           := rm -f
endif

# 编译优化选项
OPTS :=                      \
	-Ofast                   \
	-ffunction-sections      \
	-fdata-sections

ifneq ($(STD) ,)
OPTS += -std=$(STD)
endif

# 根据架构添加专用选项
ifeq ($(ARCH), x86_64)
    # 本地 x86_64，启用所有本地支持的指令集（包含 BMI, POPCNT, AVX2 等）
    OPTS += -march=native -mtune=native
else ifeq ($(ARCH), i386)
    OPTS += -march=i686 -mtune=generic
else ifeq ($(ARCH), i686)
    OPTS += -march=i686 -mtune=generic
else ifeq ($(ARCH), aarch64)
    # ARM 64 位（如树莓派 4、鲲鹏、Apple M 系列）
    OPTS += -march=armv8-a+fp+simd+crypto -mtune=native
else ifeq ($(ARCH), armv7l)
    # ARM 32 位（如树莓派 2/3）
    OPTS += -march=armv7-a -mfpu=neon -mtune=cortex-a7
else ifeq ($(ARCH), armv6l)
    # 较老的 ARM（如树莓派 1）
    OPTS += -march=armv6 -mfpu=vfp -mtune=arm1176jzf-s
else
    # 未知架构：不加额外选项，或只加保守的通用选项
    # OPTS += -march=generic
endif


# 源文件目录与目标文件
SRC_DIR  := src
LIB_OBJ  := $(BUILD_DIR)/hacker_s_delight.o
MAIN_OBJ := $(BUILD_DIR)/main.o

# 默认目标
all: $(BUILD_DIR) $(EXE)

# 创建构建目录（使用 order-only 依赖，确保目录存在）
$(BUILD_DIR):
ifeq ($(OS_TYPE),Windows)
	mkdir ".\$(BUILD_DIR)"
else  # Linux
	mkdir -p $@
endif

# 编译规则（先确保构建目录存在）
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/hacker_s_delight.h | $(BUILD_DIR)
	gcc -c $< $(OPTS) -o $@

# 生成当前环境需要的库
$(LIB_TARGET): $(LIB_OBJ)
	ar rcs $@ $^

# 链接可执行文件
$(EXE): $(MAIN_OBJ) $(LIB_TARGET)
	gcc $^ $(OPTS) -o $@

# 清理当前环境的构建产物
clean:
ifeq ($(OS_TYPE),Linux)
	-$(RM) $(BUILD_DIR)/*
else
	$(RM) ".\$(BUILD_DIR)\*.*"
endif

# 伪目标
.PHONY: all clean