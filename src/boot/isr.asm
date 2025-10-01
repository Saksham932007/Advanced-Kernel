; Interrupt Service Routines (ISRs)
; This file contains the assembly stubs for all CPU exceptions

; Common interrupt handler
extern interrupt_handler

; ISR with no error code
%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    cli
    push byte 0     ; Push dummy error code
    push byte %1    ; Push interrupt number
    jmp isr_common_stub
%endmacro

; ISR with error code
%macro ISR_ERRCODE 1
global isr%1
isr%1:
    cli
    push byte %1    ; Push interrupt number
    jmp isr_common_stub
%endmacro

; Define ISRs for CPU exceptions
ISR_NOERRCODE 0     ; Division By Zero Exception
ISR_NOERRCODE 1     ; Debug Exception
ISR_NOERRCODE 2     ; Non Maskable Interrupt Exception
ISR_NOERRCODE 3     ; Breakpoint Exception
ISR_NOERRCODE 4     ; Into Detected Overflow Exception
ISR_NOERRCODE 5     ; Out of Bounds Exception
ISR_NOERRCODE 6     ; Invalid Opcode Exception
ISR_NOERRCODE 7     ; No Coprocessor Exception
ISR_ERRCODE   8     ; Double Fault Exception
ISR_NOERRCODE 9     ; Coprocessor Segment Overrun Exception
ISR_ERRCODE   10    ; Bad TSS Exception
ISR_ERRCODE   11    ; Segment Not Present Exception
ISR_ERRCODE   12    ; Stack Fault Exception
ISR_ERRCODE   13    ; General Protection Fault Exception
ISR_ERRCODE   14    ; Page Fault Exception
ISR_NOERRCODE 15    ; Unknown Interrupt Exception
ISR_NOERRCODE 16    ; Coprocessor Fault Exception
ISR_ERRCODE   17    ; Alignment Check Exception
ISR_NOERRCODE 18    ; Machine Check Exception
ISR_NOERRCODE 19    ; SIMD Floating-Point Exception
ISR_NOERRCODE 20    ; Reserved
ISR_NOERRCODE 21    ; Reserved
ISR_NOERRCODE 22    ; Reserved
ISR_NOERRCODE 23    ; Reserved
ISR_NOERRCODE 24    ; Reserved
ISR_NOERRCODE 25    ; Reserved
ISR_NOERRCODE 26    ; Reserved
ISR_NOERRCODE 27    ; Reserved
ISR_NOERRCODE 28    ; Reserved
ISR_NOERRCODE 29    ; Reserved
ISR_NOERRCODE 30    ; Reserved
ISR_NOERRCODE 31    ; Reserved

; Common ISR stub
isr_common_stub:
    pusha           ; Push all general purpose registers
    
    mov ax, ds      ; Save data segment
    push eax
    
    mov ax, 0x10    ; Load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call interrupt_handler  ; Call C interrupt handler
    
    pop eax         ; Restore data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa            ; Restore all general purpose registers
    add esp, 8      ; Clean up pushed error code and ISR number
    sti             ; Re-enable interrupts
    iret            ; Return from interrupt