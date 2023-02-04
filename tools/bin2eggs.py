#!/usr/bin/env python3

"""バイナリデータを受け取り、eggsloader用のダメタマゴに加工して出力する。
Example:
        $ python bin2eggs.py input.bin output_eggs.bin
"""

import sys
import math

PID = 0x00000000        # type 0 (ABCD)
EGG_SIZE = 0x88
HEADER_SIZE = 0x8
EGG_DATA1_SIZE = 0x1c
EGG_DATA2_SIZE = 0x58
EGG_DATA_SIZE = EGG_DATA1_SIZE + EGG_DATA2_SIZE   # 卵ひとつに書き込むデータサイズ
EGG_DATA1_OFFSET = 0x4
EGG_DATA2_OFFSET = 0x28
EGG_DATA_SIZE = EGG_DATA1_SIZE+EGG_DATA2_SIZE

def main():
    fpath = sys.argv[1]
    outpath = sys.argv[2]
    
    data = open(fpath, "rb").read()
    size = len(data)
    egg_num = math.ceil(size / EGG_DATA_SIZE)
    
    eggs = bytearray(EGG_SIZE * egg_num)
    for i in range(egg_num):
        chunk = data[i*EGG_DATA_SIZE: (i+1)*EGG_DATA_SIZE]
        eggs[i*EGG_SIZE: (i+1)*EGG_SIZE] = make_egg(chunk)

    # 1匹目のPIDの1,4バイト目に卵の総数と自動実行するか否かのフラグを書き込む。
    # 2,3バイト目はシャッフルタイプの指定に必要なので使わない。
    eggs[0] = egg_num   # eggNumber
    eggs[3] = 1         # AutoRun == TRUE
    
    open(outpath + "_eggs.bin", "wb").write(eggs)
    for i in range(egg_num):
        open(outpath + f"_egg{i:02}.bin", "wb").write(eggs[i*EGG_SIZE: (i+1)*EGG_SIZE])
        


def make_egg(data):
    egg = bytearray(EGG_SIZE)
    egg[0:4] = PID.to_bytes(4, 'little')
    egg[4] = 6  # ダメタマゴ&暗号化フラグON
    egg[8] = 1  # 種族 0001=フシギダネ
    
    # シャッフルタイプは0(ABCD)を想定している。
    # クラッシュをさけるために種族ID(0x8~0xc)と技ID(0x28~0x30)は使用しない。
    # そのためデータを2箇所に分割して格納する。
    data1 = data[:EGG_DATA1_SIZE]   # block A (0xc ~ 0x28)
    data2 = data[EGG_DATA1_SIZE:]   # block BCD (0x30 ~ 0x88)
    
    egg[8+EGG_DATA1_OFFSET:8+EGG_DATA1_OFFSET+len(data1)] = data1 
    egg[8+EGG_DATA2_OFFSET:8+EGG_DATA2_OFFSET+len(data2)] = data2
    
    return egg


if __name__ == "__main__":
    main()