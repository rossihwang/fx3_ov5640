#!/bin/bash

ELF2IMG=${FX3_INSTALL_PATH}/util/elf2img/elf2img
USAGE="Usage: $0 <elf_file> <img_file> [img_type](i2c, spi)"

ELF_FILE=${1:? ${USAGE}}
IMG_FILE=${2:? ${USAGE}}
TYPE=${3:-spi}

if [[ $TYPE == "i2c" ]]
then
    ${ELF2IMG} -i ${ELF_FILE} -o ${IMG_FILE} -i2cconf 0x1c -vectorload yes
else
    ${ELF2IMG} -i ${ELF_FILE} -o ${IMG_FILE} -i2cconf 0x00 -vectorload yes
fi
