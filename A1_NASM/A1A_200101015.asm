; Name - Anindya Vijayvargeeya
; Roll - 200101015

; SET A - Q11

section .data
    document times 1000 db 0   ; array for storing the document
    doclen dq 1000             ; length of the document
    freq times 128 dd 0        ; array for storing the frequency of each character 
    fmt_msg db "Characters along with their frequencies - ", 0xA, 0  ; format string for printing the message
    fmt_sp db "Space : %d", 0xA, 0 ;
    fmt db "%c : %d", 0xA, 0   ; format string for printing the character and its frequency

section .text
    default rel
    extern printf
    global main

main:
    push r12              ; save the preserved registers

    ; first taking input from the user
    mov rax, 3               ; system call number for read
    mov rbx, 0               ; file descriptor for stdin
    mov rcx, document        ; buffer to read into
    mov rdx, [doclen]        ; length of buffer
    int 0x80                 ; call kernel

    ; looping through the document while maintaining the freq array
    mov rcx, [doclen]
    xor r12, r12             ; index of the current character in the document
process_characters:
    cmp r12, rcx             ; compare the index of the current character to the length of the document
    je print_results         ; if they are equal, jump to the print_results label

    movsx rax , BYTE[document + r12]     ; move the current character to rax
    mov rdx, [freq + 4*rax]      
    inc rdx                  ; increment the number of occurrences of the current character
    mov [freq + 4*rax], rdx  ; store the new number of occurrences of the current character
    inc r12                  ; increment the index of the current character in the document
    jmp process_characters   ; jump back


print_results:
    ; using r12 as the current character in the array
    mov r12, 32  ; 32 is the ascii value of the space character

    ; first printing the fmt_msg string
    mov rdi, fmt_msg
    xor rax,rax
    push rbp
    call printf
    pop rbp 

   ; now printing the space character
    mov rdi, fmt_sp
    movsxd rsi, DWORD[freq + 4*r12]
    cmp rsi, 0
    je print_loop            ; won't be printing if the freq is 0
    xor rax, rax
    push rbp
    call printf
    pop rbp

print_loop:
    inc r12
    cmp r12, 127             ; r12 is the current pos in the array
    je exit                  ; if reached the end then exit

    movsxd rcx, DWORD[freq + 4*r12]
    cmp rcx, 0
    je print_loop

    ; get the output printed in the form " i : f[i] \n"
    mov	rdi, fmt
    mov	rsi, r12             ; i
    mov	rdx, rcx             ; frequency of char i
    xor rax, rax
    ; Call printf
    push rbp
    call printf 
    pop	rbp	  
    jmp print_loop           ; jump back

exit:
    pop r12 
    mov rax, 1               ; system call number for exit
    mov rbx, 0               ; exit code
    int 0x80                 ; call kernel
