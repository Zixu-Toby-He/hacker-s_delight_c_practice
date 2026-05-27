# 检测操作系统，非 Linux/Windows 环境报错
ifeq ($(OS),Windows_NT)
	OS_TYPE := Windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OS_TYPE := Linux
	else
		$(error Unsupported OS. Only Linux and Windows are supported.)
	endif
endif

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
	-Og                      \
	-ffunction-sections      \
	-fdata-sections

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