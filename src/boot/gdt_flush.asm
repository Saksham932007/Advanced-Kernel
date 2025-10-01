; GDT flush function
global gdt_flush_asm

gdt_flush_asm:
    mov eax, [esp + 4]  ; Get GDT pointer from stack
    lgdt [eax]          ; Load GDT
    
    mov ax, 0x10        ; 0x10 is offset in GDT to data segment
    mov ds, ax          ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    jmp 0x08:.flush    ; 0x08 is offset to code segment
.flush:
    ret