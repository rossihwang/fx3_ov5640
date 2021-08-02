#!/bin/bash

ELF2IMG=${FX3_INSTALL_PATH}/util/elf2img/elf2img

if (($# < 2))
then
    echo "Usage: $0 <elf_file> <img_file> [img_type](i2c, spi)"
    exit
fi

ELF_FILE=$1
IMG_FILE=$2
DEV=${3:-spi}

if [[ $DEV == "i2c" ]]
then
    ${ELF2IMG} -i ${ELF_FILE} -o ${IMG_FILE} -i2cconf 0x1c -vectorload yes
else
    ${ELF2IMG} -i ${ELF_FILE} -o ${IMG_FILE} -i2cconf 0x00 -vectorload yes
fi
