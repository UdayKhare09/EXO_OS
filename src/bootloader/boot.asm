; bootloader/boot.asm
[bits 16]
[org 0x7C00]

section .text
    global _start

_start:
    ; Initialize segment registers
    cli
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Print welcome message
    mov si, welcome_msg
    call print_string

    ; Load kernel from disk
    call load_kernel

    ; Switch to protected mode
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    ; Initialize segment registers for protected mode
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up stack
    mov ebp, 0x90000
    mov esp, ebp

    ; Jump to kernel
    call KERNEL_OFFSET

    ; Hang if kernel returns
    jmp $

[bits 16]
; Load kernel from disk
load_kernel:
    mov ah, 0x02    ; BIOS read sector function
    mov al, 20      ; Number of sectors to read
    mov ch, 0       ; Cylinder number
    mov dh, 0       ; Head number
    mov cl, 2       ; Sector number (starts from 1, boot sector is 1)
    mov bx, KERNEL_OFFSET
    int 0x13        ; BIOS interrupt
    jc disk_error   ; Jump if error (carry flag set)
    ret

disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $

; Print string function
print_string:
    pusha
    mov ah, 0x0E    ; BIOS teletype function
print_loop:
    lodsb           ; Load next character
    or al, al       ; Check if AL is 0 (end of string)
    jz print_done
    int 0x10        ; Print character
    jmp print_loop
print_done:
    popa
    ret

; Data section
welcome_msg db 'EXO_OS Bootloader starting...', 0x0D, 0x0A, 0
disk_error_msg db 'Disk read error!', 0x0D, 0x0A, 0

; GDT (Global Descriptor Table)
gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0

    ; Code segment descriptor
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10011010b ; Flags (Present, Ring 0, Code, Executable, Readable)
    db 11001111b ; Flags (Granularity, 32-bit) + Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)

    ; Data segment descriptor
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10010010b ; Flags (Present, Ring 0, Data, Writable)
    db 11001111b ; Flags (Granularity, 32-bit) + Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of GDT
    dd gdt_start                 ; Start address of GDT

; Constants
CODE_SEG equ 0x08  ; Code segment offset in GDT
DATA_SEG equ 0x10  ; Data segment offset in GDT
KERNEL_OFFSET equ 0x1000  ; Where to load the kernel in memory

; Boot sector padding and signature
times 510-($-$$) db 0   ; Fill the rest of sector with 0s
dw 0xAA55               ; Boot signature