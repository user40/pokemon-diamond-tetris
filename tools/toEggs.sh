#!/bin/bash

# バイナリデータをeggsloader用のタマゴ(複数)に変換し、そのPARコードを生成する
# タマゴはボックス1の2体目から配置する

BIN=$1
NAME=${BIN%.bin}
SCRIPTDIR=$(cd $(dirname $(readlink -f $0));pwd)

# バイナリをダメタマゴに変換
python3 ${SCRIPTDIR}/bin2eggs.py ${BIN} ${NAME}_eggs.bin

# PARスクリプトを生成
${SCRIPTDIR}/converter.py -i ${NAME}_eggs.bin -o ${NAME}_eggs_par.txt --pointer -a 0xC3A0 -p 0x02108818

# desmume用PARスクリプトを生成
${SCRIPTDIR}/converter.py --mode=parFlat -i ${NAME}_eggs.bin -o ${NAME}_eggs_parflat.txt --pointer -a 0xC3A0 -p 0x02108818
