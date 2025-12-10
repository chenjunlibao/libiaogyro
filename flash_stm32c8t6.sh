#!/bin/bash
# STM32C8T6 JTAG 烧录脚本（支持命令行指定 hex 文件）
# 使用方式：./flash_stm32c8t6.sh [你的固件.hex路径]

# ====================== 配置项（根据实际情况修改）======================
# OpenOCD 配置文件路径（替换为你的 stm32c8t6_jtag.cfg 路径）
OPENOCD_CFG="./stm32c8t6_jtag.cfg"
# ======================================================================

# 1. 检查命令行参数（必须传入 hex 文件路径）
if [ $# -ne 1 ]; then
    echo -e "\033[31m错误：请指定要烧录的 hex 文件路径！\033[0m"
    echo -e "使用示例：\033[32m./flash_stm32c8t6.sh ./firmware.hex\033[0m"
    exit 1
fi

# 2. 提取传入的 hex 文件路径
HEX_FILE="$1"

# 3. 检查 hex 文件是否存在
if [ ! -f "$HEX_FILE" ]; then
    echo -e "\033[31m错误：hex 文件不存在！路径：$HEX_FILE\033[0m"
    exit 1
fi

# 4. 检查 OpenOCD 配置文件是否存在
if [ ! -f "$OPENOCD_CFG" ]; then
    echo -e "\033[31m错误：OpenOCD 配置文件不存在！路径：$OPENOCD_CFG\033[0m"
    exit 1
fi

# 5. 检查 OpenOCD 是否安装
if ! command -v openocd &> /dev/null; then
    echo -e "\033[31m错误：未安装 OpenOCD！请先安装 OpenOCD\033[0m"
    exit 1
fi

# 6. 开始烧录流程
echo -e "\033[34m========================================\033[0m"
echo -e "\033[34m开始烧录 STM32C8T6，hex 文件：$HEX_FILE\033[0m"
echo -e "\033[34m========================================\033[0m"

# 核心烧录命令（JTAG 模式，适配 STM32C8T6）
openocd -f "$OPENOCD_CFG" -c "init; reset halt; \
    flash erase_sector \$_FLASHNAME 0 0 7; \  # 擦除 C8T6 全片 64KB Flash（0-7扇区）
    flash write_image erase \"$HEX_FILE\"; \   # 烧录 hex 文件（自动识别地址）
    verify_image \"$HEX_FILE\"; \              # 校验烧录结果
    reset run; \                              # 复位并运行芯片
    shutdown;"

# 7. 检查烧录是否成功
if [ $? -eq 0 ]; then
    echo -e "\033[32m========================================\033[0m"
    echo -e "\033[32m烧录成功！STM32C8T6 已运行新固件\033[0m"
    echo -e "\033[32m========================================\033[0m"
else
    echo -e "\033[31m========================================\033[0m"
    echo -e "\033[31m烧录失败！请检查：\033[0m"
    echo -e "\033[31m  1. JTAG 接线是否正确\033[0m"
    echo -e "\033[31m  2. 仿真器是否正常识别\033[0m"
    echo -e "\033[31m  3. hex 文件是否损坏\033[0m"
    echo -e "\033[31m========================================\033[0m"
    exit 1
fi

