#!/usr/bin/env python3

usage = '''
バイナリファイルをPARコード等に変換します。
オプション:
  -i                入力するバイナリファイルのパス
  -o                出力するファイルのパス
  -a                バイナリを配置するアドレス（--pointerオプションを付ける場合アドレスオフセット）
  -p                参照するポインターのアドレス
  --mode=par        バイナリを書き込むPARコードを出力
  --mode=parFlat    バイナリを書き込むPARコードを出力（desmume用）
  --mode=word       バイナリをワード(4byte)に切り分けて出力（16進法）
  --mode=wordDec    バイナリをワード(4byte)に切り分けて出力（10進法）
  --mode=script     バイナリをメモリに書き込むゲーム内スクリプト(..0007)を出力（16進法）
  --mode=scriptDec  バイナリをメモリに書き込むゲーム内スクリプト(..0007)を出力（10進法）
  --mode=csv        未実装
  --nozero          書き込む値が0の場合は無視する
  --pointer         PARでポインタコードを出力する
'''

from bin2par import *
import os
import sys
import enum
from collections import deque
from typing import List

def main():
    option = parse(sys.argv[1:])
    bin = bin2Text(option.inputPath, option.address, option.pointer)
    with open(option.outputPath, 'w', encoding='utf-8', newline='\n') as f:
        if option.mode == Mode.par:
            if option.isPointer:
                f.writelines("\n".join(bin.toWordWritePointerParCode()))
            else:
                f.writelines("\n".join(bin.toWordWriteParCode()))
        elif option.mode == Mode.parFlat:
            lines = bin.toWordWritePointerParCode() if option.isPointer else bin.toWordWriteParCode()
            lines = list(map(lambda x: x.replace(' ', ''), lines))
            # Split every 300 lines
            codes = chunks(lines, 300)
            scriptName = os.path.basename(option.inputPath)
            for i, code in enumerate(codes):
                line = 'AR 1 ' + ','.join(code) + f';{scriptName}_{i:02}\n'
                f.write(line)
        elif option.mode == Mode.word:
            f.writelines("\n".join(bin.toString(dec=False)))
        elif option.mode == Mode.wordDec:
            f.writelines("\n".join(bin.toString(dec=True)))
        elif option.mode == Mode.script:
            f.writelines("\n".join(bin.toByteWriteScript(dec=False, nozero=option.nozero)))
        elif option.mode == Mode.scriptDec:
            f.writelines("\n".join(bin.toByteWriteScript(dec=True, nozero=option.nozero)))
        elif option.mode == Mode.csv:
            pass
        else:
            raise Exception()


class Mode(enum.Enum):
    par = 'par'
    parFlat = 'parFlat'
    word = 'word'
    wordDec = 'wordDec'
    script = "script"
    scriptDec = 'scriptDec'
    csv = 'csv'

class Option:
    def __init__(self, 
        inputPath="",
        outputPath="",
        address="0",
        pointer="0",
        mode=Mode.par,
        nozero=False,
        isPointer=False,
        ):
        self.inputPath = inputPath
        self.outputPath = outputPath
        self.address = int(address, 16)
        self.pointer = int(pointer, 16)
        self.mode = mode
        self.nozero = nozero
        self.isPointer = isPointer
    
    def setAddress(self, address):
        self.address = int(address, 16)

    def setPointer(self, pointer):
        self.pointer = int(pointer, 16)


def parse(args: List[str]) -> Option:
    if len(args) == 0:
        print(usage)
        exit(0)

    option = Option()
    argument_from_queue = deque(args)
    while argument_from_queue:
        argument = argument_from_queue.popleft()
        if argument in ['-h', '--help']:
            print(usage)
            sys.exit(0)
        elif argument == '-i':
            option.inputPath = argument_from_queue.popleft()
        elif argument == '-o':
            option.outputPath = argument_from_queue.popleft()
        elif argument == '-a':
            option.setAddress(argument_from_queue.popleft())
        elif argument == '-p':
            option.setPointer(argument_from_queue.popleft())
        elif argument == '--mode=par':
            option.mode = Mode.par
        elif argument == '--mode=parFlat':
            option.mode = Mode.parFlat
        elif argument == '--mode=word':
            option.mode = Mode.word
        elif argument == '--mode=wordDec':
            option.mode = Mode.wordDec
        elif argument == '--mode=script':
            option.mode = Mode.script
        elif argument == '--mode=scriptDec':
            option.mode = Mode.scriptDec
        elif argument == '--mode=csv':
            option.mode = Mode.csv
        elif argument == '--nozero':
            option.nozero = True
        elif argument == '--pointer':
            option.isPointer = True
        else:
            print(f"Bad argument '{argument}'")
            print("With '-h' option to show usage")
            sys.exit(0)

    return option


def chunks(l, n):
    for i in range(0, len(l), n):
        yield l[i:i + n]


if __name__ == "__main__":
    main()