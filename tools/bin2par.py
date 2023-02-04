import struct
from typing import List

class bin2Text:
    def __init__(self, fpath, address=0, pointer=0) -> None:
        self.uints = binOpen(fpath)
        self.address = address
        self.pointer = pointer
    
    def toString(self, dec=False):
        return uintToString(self.uints, dec)

    def toWordWriteParCode(self, nozero=False) -> List[str]:
        return fourBytesWriteCode(self.uints, self.address, nozero)

    def toWordWritePointerParCode(self) -> List[str]:
        return fourBytesWritePointerCode(self.uints, self.pointer, self.address)

    def toByteWriteScript(self, dec=False, nozero=False) -> List[str]:
        parCode = self.toWordWriteParCode(nozero)
        return ByteWriterScript.fromParCodes(parCode, dec, nozero)


def binOpen(fpath) -> List[int]:
    with open(fpath, "rb") as f:
        return binToUints(f.read())

def binToUints(bytes: bytes) -> List[bytes]:
    l = []
    bytes = align(bytes)
    while len(bytes) > 0:
        four = bytes[:4]
        l.append(struct.unpack("<I", four)[0])
        bytes = bytes[4:]
    return l

def uintToString(uints: List[int], dec: bool) -> List[str]:
    result = []
    for uint in uints:
        if dec:
            result.append(f"{uint:d}")
        else:
            result.append(f"{uint:08X}")
    return result

def fourBytesWriteCode(units: List[int], startAddress: int, nozero: bool) -> List[str]:
    code = []
    for offset, uint in enumerate(units):
        if nozero and (uint == 0):
            continue
        address = startAddress + offset*4
        code.append(f"{address:08X} {uint:08X}")
    return code

def fourBytesWritePointerCode(units: List[int], pointer: int, offset: int) -> List[str]:
    code = []
    for step, uint in enumerate(units):
        line1 = f"B{pointer:07X} 00000000"
        line2 = f"{offset + step*4:08X} {uint:08X}"
        line3 = f"D2000000 00000000"
        code.extend([line1, line2, line3])
    return code

def align(data: bytes):
    r = len(data) % 4
    if r == 0:
        return data
    else:
        return data + bytes([0]*(4-r))


class ByteWriterScript:
    def fromParCodes(codes: List[str], dec=False, nozero=False) -> List[str]:
        result = []
        for code in codes:
            for script in ByteWriterScript.fromParCode(code, nozero):
                if dec:
                    result.append(f"{script:d}")
                else:
                    result.append(f"{script:16x}")
        return result
        
    def fromParCode(code: str, nozero: bool) -> List[int]:
        result = []
        if code == "":
            return result
        address = int(code.split()[0], 16)
        value = int(code.split()[1], 16)
        bytes = splitToBytes(value)
        for i, byte in enumerate(bytes):
            if nozero and (byte == 0):
                continue
            script = ByteWriterScript.toLoadAdrsVal(address + i, byte)
            result.append(script)
        return result
    
    def toLoadAdrsVal(address: int, value: int) -> str:
        return 0x0200000000000007 + address*0x10000 + value*0x1000000000000


def splitToBytes(word: int) -> List[int]:
    bytes = [0]*4
    bytes[0] = (word & 0x000000FF)
    bytes[1] = (word & 0x0000FF00) >> 8
    bytes[2] = (word & 0x00FF0000) >> 16
    bytes[3] = (word & 0xFF000000) >> 24
    return bytes