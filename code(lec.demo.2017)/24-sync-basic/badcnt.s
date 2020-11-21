	.file	"badcnt.c"
	.text
	.globl	thread
	.type	thread, @function
thread:
.LFB47:
	.cfi_startproc
	movq	(%rdi), %rcx
	testq	%rcx, %rcx
	jle	.L2
	movl	$0, %eax
.L3:
	movq	cnt(%rip), %rdx
	addq	$1, %rdx
	movq	%rdx, cnt(%rip)
	addq	$1, %rax
	cmpq	%rcx, %rax
	jne	.L3
.L2:
	movl	$0, %eax
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
	je	.L5
	movq	(%rsi), %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %edi
	call	exit
.L5:
	movq	8(%rsi), %rdi
	movl	$10, %edx
	movl	$0, %esi
	call	strtol
	cltq
	movq	%rax, 24(%rsp)
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
	je	.L6
	movq	cnt(%rip), %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	jmp	.L7
.L6:
	movq	cnt(%rip), %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
.L7:
	movl	$0, %eax
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE46:
	.size	main, .-main
	.globl	cnt
	.bss
	.align 8
	.type	cnt, @object
	.size	cnt, 8
cnt:
	.zero	8
	.ident	"GCC: (GNU) 4.8.1"
	.section	.note.GNU-stack,"",@progbits
