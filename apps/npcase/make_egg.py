# make the egg for NPCASE
# The eggs created should be placed in Box 18, 28th and 29th.
# Grab the 28th egg before talking to the NPC.

# The script rewrites 0x0206DAB0 to 0x0206DAC4.
# The script start at the 28th + 0x28 and run until the end of the 28th,
# then jump to the 29th + 0x30 and run to the end.

import sys


start_address = 0x0206DAB0
jump_offset = 0xFFFC70A8  # box 1, 1st
jump_offset += 537 * 0x88  # box 18, 28th
# pid 0x0000C000 => shuffle type 6 (BACD)
# block B must be first, otherwise PP would be rewritten. 
pid = 0x0000C000


def to_write_scritp(val, address):
    bin = bytearray(7)
    bin[0] = 7
    bin[2:6] = address.to_bytes(4,'little')
    bin[6] = val
    return bin


def main():
    input_path = sys.argv[1]
    output_path = sys.argv[2]
    program = open(input_path, mode="rb").read()
    
    # Convert to the script
    script = bytearray()
    for i, b in enumerate(program):
        address = start_address + i
        script.extend(to_write_scritp(b,address))
    script.extend(b'\x49\x00\xf1\x05')  # 0049 = play SE command
    script.extend(b'\x02\x00')  # 0002 = stop command
    script1 = script[:7*12]
    script2 = script[7*12:]
    
    # Make Pokemon 1
    egg1 = bytearray(0x88)
    egg1[0:4] = pid.to_bytes(4, 'little')
    egg1[4] = 6  # badegg & box encrypted flags
    egg1[-6:] = b'\x16\x00\x30\x00\x00\x00'  # command "jump by 0x30"
    # jump offset to box 1,1 + 0x28
    egg1[0x24:0x28] = jump_offset.to_bytes(4, 'little')
    egg1[0x28:0x28+len(script1)] = script1 # second block (= block D)
    
    # Make Pokemon 2
    egg2 = bytearray(0x88)
    egg2[0:4] = pid.to_bytes(4, 'little')
    egg2[4] = 6  # badegg & box encrypted flags
    egg2[0x28] = 1  # spiecies id
    egg2[0x30:0x30+len(script2)] = script2 # second block + 0x8
    
    open(output_path + "_egg00.bin", mode="wb").write(egg1)
    open(output_path + "_egg01.bin", mode="wb").write(egg2)
    open(output_path + "_eggs.bin", mode="wb").write(egg1 + egg2)


if __name__ == "__main__":
    main()