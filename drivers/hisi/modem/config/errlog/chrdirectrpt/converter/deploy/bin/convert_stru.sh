#!/bin/bash
# Convert Stru File
# Copyright (c) Huawei Technologies Co., Ltd. 2010-2019. All rights reserved.

set -e

build_product=hi3660

if [ $# -ge 1 ]
then
    build_product="${1}"
fi 

converter_dir=../modem/config/errlog/chrdirectrpt/converter/deploy/bin/
build_dir=../../../../../../../build

cd $(dirname "${0}")
rm -f ChrEvent.c
rm -f *.bin

./convertrelationevent.py

cd "${build_dir}"
rm -rf ./delivery/"${build_product}"

./HiBuild.sh product="${build_product}" tools_config_chr_tlas
./HiBuild.sh product="${build_product}" tools_config_chr_tlnas
./HiBuild.sh product="${build_product}" tools_config_chr_gas
./HiBuild.sh product="${build_product}" tools_config_chr_gucnas
./HiBuild.sh product="${build_product}" tools_config_chr_cttf
./HiBuild.sh product="${build_product}" tools_config_chr_cas
./HiBuild.sh product="${build_product}" tools_config_chr_was
./HiBuild.sh product="${build_product}" tools_config_chr_nrrc

cp delivery/"${build_product}"/lib/*.o "${converter_dir}"/

cd "${converter_dir}"
for i in $(ls *.o)
do
    mv "${i}" $(echo "${i}"|sed 's/\.o/\.bin/')
done

echo "Using the Windows-tool ConvertElf.exe to get the xml files please ..."
read -p 'Wait for your finished the ConvertElf operation!'

cd $(dirname "${0}")
rm -f ChrEvent.c
rm -f *.bin

