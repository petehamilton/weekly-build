;; kernel.asm
bits 32 ; specify that we should generate code to run on 32bit-mode processor
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

global start ;lets linker know where "start" is
extern kmain ; kmain defined in C file

start:
  cli ;clear interrupts (disable them)
  mov esp, stack_space ;set stack pointer to be where we allocated 8KB of memory
  call kmain
  hlt ; halt CPU

section .bss
resb 8192 ; 8KB for stack, resb = reserve memory bytes
stack_space:
