#!/bin/bash

ELF2IMG=${FX3_INSTALL_PATH}/util/elf2img/elf2img

if [[ $1 == "i2c" ]]
then
    ${ELF2IMG} -i $2 -o $3 -i2cconf 0x1c -vectorload yes
else
    ${ELF2IMG} -i $2 -o $3 -i2cconf 0x00 -vectorload yes
fi
