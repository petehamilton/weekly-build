# 2015-05-30 - Let's write a Kernel

Article:
http://arjunsreedharan.org/post/82710718100/kernel-101-lets-write-a-kernel
Courtesy of: [@doismellburning](https://twitter.com/doismellburning)
Time: ~1-2h of tinkering
~~~

A very nice guide to creating your own kernel, took a bit of tweaking to make it
work via vagrant on OSX but the main article was solid.

End result in my case was a VNC session to a qemu emulation for my first kernel,
pretty fun!

For the proper tutorial, see the [original
article](http://arjunsreedharan.org/post/82710718100/kernel-101-lets-write-a-kernel), the below skims the main content and just outlines getting it set up on OSX via vagrant.

#### 1. Set up vagrant

```
$ vagrant init hashicorp/precise32
```

Edit `Vagrantfile` to include the line:

```
config.vm.network "forwarded_port", guest: 5901, host: 6901
```

This will later allow us to connect via VNC screensharing

#### 2. Create asm for entrypoint + bootloader

Create an assembly file for the entrypoint and multiboot headers:

```
;;kernel.asm

;nasm directive - 32 bit
bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

global start
extern kmain	        ;kmain is defined in the c file

start:
  cli 			;block interrupts
  mov esp, stack_space	;set stack pointer
  call kmain
  hlt		 	;halt the CPU

section .bss
resb 8192		;8KB for stack
stack_space:
```

#### 3. Main Kernel Code

Create the main kernel code (in this case to output a message to the 80x25 screen's text memory):

```c
/*
*  kernel.c
*/
void kmain(void)
{
	const char *str = "my first kernel";
	char *vidptr = (char*)0xb8000; 	//video mem begins here.
	unsigned int i = 0;
	unsigned int j = 0;

	/* this loops clears the screen
	* there are 25 lines each of 80 columns; each element takes 2 bytes */
	while(j < 80 * 25 * 2) {
		/* blank character */
		vidptr[j] = ' ';
		/* attribute-byte - light grey on black screen */
		vidptr[j+1] = 0x07; 		
		j = j + 2;
	}

	j = 0;

	/* this loop writes the string to video memory */
	while(str[j] != '\0') {
		/* the character's ascii */
		vidptr[i] = str[j];
		/* attribute-byte: give character black bg and light grey fg */
		vidptr[i+1] = 0x07;
		++j;
		i = i + 2;
	}
	return;
}
```

#### 4. Linker Script

```ld
/*
*  link.ld
*/
OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
 {
   . = 0x100000;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
 ```

#### 5. Build Kernel

```
$ nasm -f elf32 kernel.asm -o kasm.o
```

```
$ gcc -m32 -c kernel.c -o kc.o
```

```
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o
```

#### 6. Run Kernel

You could do this using [qemu](http://wiki.qemu.org/Main_Page) and display
output using curses by passing the `-curses` flag, however it's fun to try
something different so I connected via VNC.

The above sends the output
to a new vnc session which you can connect to on port 5901. You need to set a password to use
OSX screen sharing to connect, the VNC connection is available on port 6901 as
per the Vagrantfile earlier.

```
$ qemu-system-i386 -kernel kernel -no-kvm -vnc :1,password -monitor stdio
QEMU 1.0 monitor - type 'help' for more information
(qemu) change vnc password
Password: ***
```

- `-no-kvm` to preventdd `Could not access KVM kernel module: No such file or directory`
- `-vnc :1,password` to send output to a VNC session on port 5901
- `monitor stdio` to allow us to define a VNC password

DONE!

![](final-result.png)
