/* link.ld */

OUTPUT_FORMAT(elf32-i386) /* Output format = 32 bit executable and linkable format (elf) */
ENTRY(start) /* entry point of executable */
SECTIONS
 {
   . = 0x100000;
   .text : { *(.text) } /* Merge all .text sections together */
   .data : { *(.data) } /* Similarly all .data sections */
   .bss  : { *(.bss) }
 }

