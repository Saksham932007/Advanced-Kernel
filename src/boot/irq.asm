; IRQ handling stubs
extern irq_handler

; IRQ macros
%macro IRQ 2
global irq%1
irq%1:
    cli
    push byte 0     ; Push dummy error code
    push byte %2    ; Push IRQ number
    jmp irq_common_stub
%endmacro

; Define IRQs
IRQ 0,  32    ; Timer
IRQ 1,  33    ; Keyboard
IRQ 2,  34    ; Cascade
IRQ 3,  35    ; COM2
IRQ 4,  36    ; COM1
IRQ 5,  37    ; LPT2
IRQ 6,  38    ; Floppy
IRQ 7,  39    ; LPT1
IRQ 8,  40    ; CMOS Real-time clock
IRQ 9,  41    ; Free for peripherals
IRQ 10, 42    ; Free for peripherals
IRQ 11, 43    ; Free for peripherals
IRQ 12, 44    ; PS2 Mouse
IRQ 13, 45    ; FPU / Coprocessor / Inter-processor
IRQ 14, 46    ; Primary ATA Hard Disk
IRQ 15, 47    ; Secondary ATA Hard Disk

; Common IRQ stub
irq_common_stub:
    pusha           ; Push all general purpose registers
    
    mov ax, ds      ; Save data segment
    push eax
    
    mov ax, 0x10    ; Load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call irq_handler    ; Call C IRQ handler
    
    pop eax         ; Restore data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa            ; Restore all general purpose registers
    add esp, 8      ; Clean up pushed error code and IRQ number
    sti             ; Re-enable interrupts
    iret            ; Return from interrupt