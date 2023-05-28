; Name - Anindya Vijayvargeeya
; Roll - 200101015

; SET B - Q6

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Defining some useful macros ;;
%macro scan_int 1
    lea rdi, [fmt_s]
    lea esi, [%1]
    xor rax, rax
    call scanf
%endmacro

%macro print_int 1
    lea	rdi, [fmt_p]
    movsxd	rsi, DWORD %1
    xor rax, rax
    call printf 
%endmacro 

%macro print_string 1
    mov	rdi, %1
    xor rax, rax
    call printf 
%endmacro

%macro mov_to_t 2
    mov eax, [arr+ITEM_SIZE*%2]
    mov [t_arr+ITEM_SIZE*%1], eax
%endmacro 

%define ITEM_SIZE 4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .data
    N dq 10
    arr times 100 dd 0              ; stores the input matrix
    t_arr times 100 dd 0            ; stores its transpose
    fmt_p db "%d ", 0  
    fmt_s db "%d", 0   
    fmt_msg db "Transpose of this matrix is - ",10, 0        
    fmt_nl db 10, 0     
    fmt_Nrange db "N should be between 1 and 10", 10, 0
    i dd 0
    j dd 0
section .text
    default rel
    extern printf
    extern scanf
    global main

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Main starts here ;;;;;;;;;;;;;;;;;
main:
    push r12  ; pushing preserved registers into the stack 
    push r13
    push r14
    push r15
    push rbp
    ;;;;;;;; INPUT ;;;;;;;;;;;;;;;
    scan_int N             ; scan N

    mov r12, [N]

    ; input validation first - N can needs to beween 1 and 10
    cmp r12, 0             ; check if N 
    jle err_exit
    cmp r12, 11
    jge err_exit

    imul r12, [N]          ; setting r12 = N^2
    xor r13, r13
  scan_arr:
    scan_int arr+ITEM_SIZE*r13
    inc r13
    cmp r13, r12           ; scan N^2 elements
    jl scan_arr

    ;;;;;; BUILD TRANSPOSE ;;;;;;;;
    xor r13, r13           ; r13 stores the row number
    xor r14, r14           ; r14 stores the col number
    xor r15, r15
    ;  r12 = N^2
  build_mat:
    mov_to_t r15, r13      ; copies content of arr[r13] to t_arr[r15]
    inc r15
    add r13, [N]           ; r13 = r13 + N, to move to next row
    cmp r13, r12
    jl build_mat
    
    inc r14                ; r14 = r14 + 1, to move to next col
    cmp r14, [N]           ; reached the end of the matrix
    jge g_out
    mov r13, r14           ; start with the first element of the column
    jmp build_mat

  g_out:

    ;;;;;;; PRINT TRANSPOSE ;;;;;;;
    
    print_string fmt_msg

    ; again we need to maintain r13 and r14 to print newlines at apropriate places
    xor r13, r13           ; resetting registers
    xor r14, r14
    xor r15, r15
    ;  r12 = N^2

  print_mat:
    print_int [t_arr+r15*ITEM_SIZE]   ; print the element at r15 index 
    inc r15
    add r13, [N]           
    cmp r13, r12
    jl print_mat
    
    print_string fmt_nl    ; print newline after each row
    inc r14 
    cmp r14, [N]
    jge exit
    mov r13, r14 
    jmp print_mat


    ;;;;;;;;;;; EXIT ;;;;;;;;;;;;;;;
  exit:
    pop rbp                ; popping preserved registers from the stack
    pop r15
    pop r14
    pop r13
    pop r12
    mov rax, 1             ; system call number for exit
    mov rbx, 0             ; exit code
    int 0x80               ; call kernel

  err_exit:
    print_string fmt_Nrange
    pop rbp                ; popping preserved registers from the stack
    pop r15
    pop r14
    pop r13
    pop r12
    mov rax, 1             ; system call number for exit
    mov rbx, 1             ; exit code
    int 0x80  