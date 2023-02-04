MEMORY {
    RAM : ORIGIN = 0x02290008, LENGTH = 2k
}
SECTIONS {
    .text : {
        *(.text.startup)
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
    } > RAM
    .rodata : {
        *(.rodata*)
        . = ALIGN(4);
    } > RAM
    .data : {
        *(.data*)
        . = ALIGN(4);
    } > RAM
    .got : {
        *(.got*)
        . = ALIGN(4);
    }
}