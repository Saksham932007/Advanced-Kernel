; Multiboot2 header
MAGIC    equ 0xe85250d6                ; multiboot2 magic number
ARCH     equ 0                         ; protected mode i386
LENGTH   equ multiboot_end - multiboot_start
CHECKSUM equ -(MAGIC + ARCH + LENGTH)  ; checksum

section .multiboot
align 8
multiboot_start:
    dd MAGIC
    dd ARCH
    dd LENGTH
    dd CHECKSUM
    
    ; End tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
multiboot_end:

; Stack
section .bss
align 16
stack_bottom:
    resb 16384  ; 16 KiB stack
stack_top:

; Entry point
section .text
global start
start:
    ; Set up stack
    mov esp, stack_top
    
    ; Reset EFLAGS
    push 0
    popf
    
    ; Call kernel main
    extern kernel_main
    call kernel_main
    
    ; Hang if kernel returns
    cli
.hang:
    hlt
    jmp .hang