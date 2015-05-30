;;kernel.asm

;nasm directive - generate code to run in 32 bit processor mode
bits 32

section .text
        ;headers to conform to multiboot spec
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. sum of these 3 fields should be zero

global start        ;lets the linker know where "start" is
extern kmain            ;kmain is defined in the c file

start:
  cli           ;clear interrupts (disable/block them)
  mov esp, stack_space  ;set stack pointer to point the allocated 8KB memory
  call kmain        ;call the main kernel function
  hlt           ;halt the CPU

section .bss
resb 8192       ;reserve bytes (8KB) for stack
stack_space:        ;label indicates start of allocated memory above
