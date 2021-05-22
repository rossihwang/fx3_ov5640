#!/bin/bash
DOWNLOAD_FX3=${FX3_SDK_ROOT}/cyusb_linux_1.0.5/src/download_fx3
FLASHPROG_FW=./cyfxflashprog.img

# check image file
if [[ -z $1 ]]
then
    echo "usage: flashprog.sh <image_file> [target]"
fi

# check target
if [[ -z $2 ]]
then
    TARGET=SPI # default target
else
    TARGET=$2
fi

FOUND=$(lsusb | grep 04b4:00f3 | wc -l)
echo ${FOUND}
if [[ ${FOUND} == 1 ]]
then
    echo "USB boot succeed!"
    ${DOWNLOAD_FX3} -t RAM -i ${FLASHPROG_FW}
fi

until [ $(lsusb | grep 04b4:4720 | wc -l) == "1" ]
do
    echo "Wait for flashprog..."
    sleep 0.1
done

${DOWNLOAD_FX3} -t ${TARGET} -i $1
