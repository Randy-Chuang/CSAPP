	.file	"goodcnt.c"
	.text
	.globl	thread
	.type	thread, @function
thread:
.LFB47:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movslq	(%rdi), %rbp
	testq	%rbp, %rbp
	jle	.L2
	movl	$0, %ebx
.L3:
	movl	$mutex, %edi
	call	P
	movq	cnt(%rip), %rax
	addq	$1, %rax
	movq	%rax, cnt(%rip)
	movl	$mutex, %edi
	call	V
	addq	$1, %rbx
	cmpq	%rbx, %rbp
	jg	.L3
.L2:
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE47:
	.size	thread, .-thread
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"usage: %s <niters>\n"
.LC1:
	.string	"BOOM! cnt=%ld\n"
.LC2:
	.string	"OK cnt=%ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB46:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	cmpl	$2, %edi
	je	.L6
	movq	(%rsi), %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %edi
	call	exit
.L6:
	movq	8(%rsi), %rdi
	movl	$10, %edx
	movl	$0, %esi
	call	strtol
	cltq
	movq	%rax, 24(%rsp)
	movl	$1, %edx
	movl	$0, %esi
	movl	$mutex, %edi
	call	Sem_init
	leaq	24(%rsp), %rcx
	movl	$thread, %edx
	movl	$0, %esi
	leaq	16(%rsp), %rdi
	call	Pthread_create
	leaq	24(%rsp), %rcx
	movl	$thread, %edx
	movl	$0, %esi
	leaq	8(%rsp), %rdi
	call	Pthread_create
	movl	$0, %esi
	movq	16(%rsp), %rdi
	call	Pthread_join
	movl	$0, %esi
	movq	8(%rsp), %rdi
	call	Pthread_join
	movq	cnt(%rip), %rax
	movq	24(%rsp), %rdx
	addq	%rdx, %rdx
	cmpq	%rax, %rdx
	je	.L7
	movq	cnt(%rip), %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	jmp	.L8
.L7:
	movq	cnt(%rip), %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
.L8:
	movl	$0, %eax
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE46:
	.size	main, .-main
	.comm	mutex,32,32
	.globl	cnt
	.bss
	.align 8
	.type	cnt, @object
	.size	cnt, 8
cnt:
	.zero	8
	.ident	"GCC: (GNU) 4.8.1"
	.section	.note.GNU-stack,"",@progbits
