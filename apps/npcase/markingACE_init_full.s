@ marking ACE entry point
@ When marking a pokemon, call the address of the pokemon + 8.
@ This function passes two arguments to the callee;
@ r0 is the address of the marked pokeomon, and
@ r1 corresponds to the selected marks.

@ starts from 0x0206dab0
.arch armv5te
.text
.code	16
.thumb_func
blx 0x020ce510   @ Clean entire Data cache
blx 0x020ce5d8   @ Flush entire Instruction cache
ldrb r1, [r4]    @ r1 = marks
ldr r0, [sp]     @ r0 = marked pokemon
mov r4, r0
add r4, #0x9
blx r4
nop
