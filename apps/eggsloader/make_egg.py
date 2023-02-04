input_file = "eggsloader.bin"
output_file = "eggsloader_egg.bin"


pid = 0x0000C000  # type 6	BACD

def main():
    data = open(input_file, "rb").read()
    egg = bytearray(0x88)
    egg[0:4] = pid.to_bytes(4, 'little')
    egg[4] = 6  # badegg & box encrypted flags
    
    # The program starts at block B + 0x8
    start = 0x10
    end = start + len(data)
    assert end <= 0x88, f"Overcapacity! {end:4x} > 0x88"
    
    egg[start:end] = data
    
    open(output_file, "wb").write(egg)


if __name__ == "__main__":
    main()