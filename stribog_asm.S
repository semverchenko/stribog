.section .text

.extern lbox_fast

.macro LPS_macro offset
	xorq %rsi, %rsi

	movzbq %r8b, %rax
	xorq lbox_fast+256*8*0(,%rax,8), %rsi

	movzbq %r9b, %rbx
	xorq lbox_fast+256*8*1(,%rbx,8), %rsi

	movzbq %r10b, %rcx
	xorq lbox_fast+256*8*2(,%rcx,8), %rsi

	movzbq %r11b, %rdx
	xorq lbox_fast+256*8*3(,%rdx,8), %rsi

	movzbq %r12b, %rax
	xorq lbox_fast+256*8*4(,%rax,8), %rsi

	movzbq %r13b, %rbx
	xorq lbox_fast+256*8*5(,%rbx,8), %rsi

	movzbq %r14b, %rcx
	xorq lbox_fast+256*8*6(,%rcx,8), %rsi

	movzbq %r15b, %rdx
	xorq lbox_fast+256*8*7(,%rdx,8), %rsi

	movq %rsi, 8*\offset(%rdi)
	shr $8, %r8
	shr $8, %r9
	shr $8, %r10
	shr $8, %r11
	shr $8, %r12
	shr $8, %r13
	shr $8, %r14
	shr $8, %r15
.endm

.global LPS
.type LPS, @function
LPS:
	pushq %rbp
	movq %rsp, %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	xorq %rax, %rax
	xorq %rbx, %rbx
	xorq %rcx, %rcx
	xorq %rdx, %rdx

	movq 0x00(%rdi), %r8 
	movq 0x08(%rdi), %r9 
	movq 0x10(%rdi), %r10
	movq 0x18(%rdi), %r11
	movq 0x20(%rdi), %r12
	movq 0x28(%rdi), %r13
	movq 0x30(%rdi), %r14
	movq 0x38(%rdi), %r15

	LPS_macro 0
	LPS_macro 1
	LPS_macro 2
	LPS_macro 3
	LPS_macro 4
	LPS_macro 5
	LPS_macro 6
	LPS_macro 7

	popq %rbx
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	movq %rbp, %rsp
	popq %rbp
	ret

.global LPSX
.type LPSX, @function
LPSX:
	pushq %rbp
	movq %rsp, %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	xorq %rax, %rax
	xorq %rbx, %rbx
	xorq %rcx, %rcx
	xorq %rdx, %rdx

	movq 0x00(%rdi), %r8 
	movq 0x08(%rdi), %r9 
	movq 0x10(%rdi), %r10
	movq 0x18(%rdi), %r11
	movq 0x20(%rdi), %r12
	movq 0x28(%rdi), %r13
	movq 0x30(%rdi), %r14
	movq 0x38(%rdi), %r15

	xorq 0x00(%rsi), %r8 
	xorq 0x08(%rsi), %r9 
	xorq 0x10(%rsi), %r10
	xorq 0x18(%rsi), %r11
	xorq 0x20(%rsi), %r12
	xorq 0x28(%rsi), %r13
	xorq 0x30(%rsi), %r14
	xorq 0x38(%rsi), %r15

	LPS_macro 0
	LPS_macro 1
	LPS_macro 2
	LPS_macro 3
	LPS_macro 4
	LPS_macro 5
	LPS_macro 6
	LPS_macro 7

	popq %rbx
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	movq %rbp, %rsp
	popq %rbp
	ret
	ret

.global add512
.type add512, @function
add512:
	movq 0x00(%rsi), %rax
	addq %rax, 0x00(%rdi)
	movq 0x08(%rsi), %rax
	adcq %rax, 0x08(%rdi)
	movq 0x10(%rsi), %rax
	adcq %rax, 0x10(%rdi)
	movq 0x18(%rsi), %rax
	adcq %rax, 0x18(%rdi)
	movq 0x20(%rsi), %rax
	adcq %rax, 0x20(%rdi)
	movq 0x28(%rsi), %rax
	adcq %rax, 0x28(%rdi)
	movq 0x30(%rsi), %rax
	adcq %rax, 0x30(%rdi)
	movq 0x38(%rsi), %rax
	adcq %rax, 0x38(%rdi)
	ret

/* vim: set ft=gas : */
