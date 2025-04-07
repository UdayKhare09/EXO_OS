; Simple bootloader to load a C kernel
[org 0x7c00]      ; Tell assembler where this code will be loaded
[bits 16]         ; Start in 16-bit real mode

KERNEL_OFFSET equ 0x1000  ; Memory offset where kernel will be loaded

; Main entry point
start:
    ; Setup segment registers
    xor ax, ax    ; Clear AX register
    mov ds, ax    ; Set Data Segment to 0
    mov es, ax    ; Set Extra Segment to 0
    mov ss, ax    ; Set Stack Segment to 0
    mov sp, 0x7C00 ; Set Stack Pointer to 0x7C00

    ; Save boot drive number
    mov [BOOT_DRIVE], dl

    ; Display welcome message
    mov si, MSG_REAL_MODE
    call print_string

    ; Load kernel from disk
    call load_kernel

    ; Switch to protected mode
    call switch_to_pm

    jmp $  ; Never executed

; Function to load kernel from disk
load_kernel:
    mov si, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET  ; Set destination address
    mov dh, 15            ; Load 15 sectors (after bootloader)
    mov dl, [BOOT_DRIVE]   ; Drive number
    call disk_load
    ret

; Load DH sectors from drive DL into ES:BX
disk_load:
    push dx               ; Store DX for later comparison

    mov ah, 0x02          ; BIOS read sector function
    mov al, dh            ; Number of sectors to read
    mov ch, 0             ; Cylinder 0
    mov dh, 0             ; Head 0
    mov cl, 2             ; Start from sector 2 (sector after boot sector)

    int 0x13              ; BIOS interrupt for disk operations

    jc disk_error         ; Jump if error (carry flag set)

    pop dx                ; Restore DX
    cmp dh, al            ; Compare sectors read with sectors expected
    jne disk_error        ; Error if they don't match
    ret

disk_error:
    mov si, DISK_ERROR_MSG
    call print_string
    jmp $

; Function to print a string (16-bit mode)
print_string:
    pusha
    mov ah, 0x0E

.repeat:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .repeat

.done:
    popa
    ret

; Switch to protected mode
switch_to_pm:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT

    ; Enter protected mode
    mov eax, cr0
    or eax, 1              ; Set PE bit
    mov cr0, eax

    ; Far jump to 32-bit code
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    ; Setup segment registers for 32-bit mode
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000       ; New stack

    ; Display success message
    mov ebx, MSG_PROT_MODE
    call print_string_pm

    ; Jump to the kernel
    call KERNEL_OFFSET

    jmp $                  ; Infinite loop if kernel returns

; 32-bit print function
print_string_pm:
    pusha
    mov edx, 0xb8000       ; Video memory address

.loop:
    mov al, [ebx]          ; Get character
    test al, al            ; Check if end of string
    jz .done

    mov ah, 0x0F           ; White on black
    mov [edx], ax          ; Store character + attribute

    add ebx, 1             ; Next character
    add edx, 2             ; Next video memory position
    jmp .loop

.done:
    popa
    ret

; GDT
gdt_start:
    dd 0, 0                    ; Null descriptor
code_segment:
    dw 0xFFFF                  ; Limit
    dw 0                       ; Base
    db 0                       ; Base
    db 10011010b               ; Access Byte
    db 11001111b               ; Flags + Limit
    db 0                       ; Base
data_segment:
    dw 0xFFFF                  ; Limit
    dw 0                       ; Base
    db 0                       ; Base
    db 10010010b               ; Access Byte
    db 11001111b               ; Flags + Limit
    db 0                       ; Base
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size
    dd gdt_start               ; Address

; Constants
CODE_SEG equ code_segment - gdt_start
DATA_SEG equ data_segment - gdt_start

; Data section
BOOT_DRIVE: db 0
MSG_REAL_MODE: db 'Started in 16-bit Real Mode', 13, 10, 0
MSG_LOAD_KERNEL: db 'Loading kernel into memory', 13, 10, 0
MSG_PROT_MODE: db 'Successfully switched to 32-bit Protected Mode', 0
DISK_ERROR_MSG: db 'Disk read error!', 13, 10, 0

; Padding and magic number
times 510-($-$$) db 0  ; Fill with zeros up to 510 bytes
dw 0xAA55              ; Boot signature