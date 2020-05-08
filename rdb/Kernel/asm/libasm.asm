GLOBAL cpuVendor
GLOBAL getRTC
GLOBAL kbFlag
GLOBAL kbGet
GLOBAL _inportb
GLOBAL _outportb
GLOBAL _set_rsp
GLOBAL getRegisters
GLOBAL _fillstack
GLOBAL _sti_and_halt
GLOBAL _halt
GLOBAL _sti_set_rsp_and_halt
GLOBAL spin_lock
GLOBAL spin_unlock
section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getRTC:
    push rbp
    mov rbp, rsp

    mov rax, rdi
 
    out 70h, al
    in al, 71h
 
    mov rsp, rbp
    pop rbp
    ret

kbGet:
	push rbp
	mov rbp, rsp

	mov rax, 0
	in al, 0x60

	mov rsp, rbp
	pop rbp
	ret

_inportb:

    push rbp
    mov rbp, rsp
    mov rdx, rdi
    mov rax, 0
    in al, dx
    mov rsp, rbp
    pop rbp
    ret

_outportb:

    push rbp
    mov rbp, rsp
    mov rax, rsi
    mov rdx, rdi
    out dx, al
    mov rsp, rbp
    pop rbp
    ret
	
_set_rsp:

    mov rsp, rdi
    ret


_fillstack:

    push rbp
	mov rbp, rsp

    mov  rsp, rdi
    push 0x0       ;SS
    push rdi       ;RSP
    push 0x202     ;RFLAGS
    push 0x8       ;CS
    push rsi       ;RIP
    push 0x1       ;rax
    push 0x2       ;rbx
    push 0x3       ;rcx
    push r8        ;rdx
    push 0x5       ;rbp
    push rdx       ;rdi
    push rcx       ;rsi
    push 0x0       ;r8
    push 0x9       ;r9
    push 0xA       ;r10
    push 0xb       ;r11
    push 0xc       ;r12
    push 0xd       ;r13
    push 0xe       ;r14
    push 0xf       ;r15

    mov rsp, rbp
    pop rbp
    ret


_sti_and_halt:
    sti
    hlt
    ret

_sti_set_rsp_and_halt:
    mov rsp, rax
    sti
    hlt
    ret




; The lock variable. 1 = locked, 0 = unlocked.

spin_lock:
    mov     rax, 1          ; Set the EAX register to 1.
    xchg    rax, [rdi]      ; Atomically swap the EAX register with
                            ;  the lock variable.
                            ; This will always store 1 to the lock, leaving
                            ;  the previous value in the EAX register.
    test    rax, rax        ; Test EAX with itself. Among other things, this will
                            ;  set the processor's Zero Flag if EAX is 0.
                            ; If EAX is 0, then the lock was unlocked and
                            ;  we just locked it.
                            ; Otherwise, EAX is 1 and we didn't acquire the lock.
    jnz     spin_lock       ; Jump back to the MOV instruction if the Zero Flag is
                            ;  not set; the lock was previously locked, and so
                            ; we need to spin until it becomes unlocked.
    ret                     ; The lock has been acquired, return to the calling
                            ;  function.

spin_unlock:
     xor     rax, rax       ; Set the EAX register to 0.
     xchg    rax, [rdi]       ; Atomically swap the EAX register with
                            ;  the lock variable.
     ret                    ; The lock has been released.


