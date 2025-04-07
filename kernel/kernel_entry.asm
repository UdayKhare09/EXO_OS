; kernel/kernel_entry.asm
[bits 32]
[extern main]
call main     ; Call C main function
jmp $         ; Infinite loop if main returns