	.file	"asso.c"
	.text
	.p2align 4
	.globl	sum_array
	.type	sum_array, @function
sum_array:
.LFB0:
	.cfi_startproc
	testq	%rsi, %rsi
	je	.L4
	leaq	(%rdi,%rsi,4), %rdx
	xorl	%eax, %eax
	.p2align 4
	.p2align 4
	.p2align 3
.L3:
	addl	(%rdi), %eax
	addq	$4, %rdi
	cmpq	%rdx, %rdi
	jne	.L3
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	sum_array, .-sum_array
	.p2align 4
	.globl	copy_struct
	.type	copy_struct, @function
copy_struct:
.LFB1:
	.cfi_startproc
	movq	%rdi, %rax
	ret
	.cfi_endproc
.LFE1:
	.size	copy_struct, .-copy_struct
	.p2align 4
	.globl	multiply_normal
	.type	multiply_normal, @function
multiply_normal:
.LFB2:
	.cfi_startproc
	leal	(%rdi,%rdi), %eax
	ret
	.cfi_endproc
.LFE2:
	.size	multiply_normal, .-multiply_normal
	.p2align 4
	.globl	test_inline
	.type	test_inline, @function
test_inline:
.LFB9:
	.cfi_startproc
	leal	(%rdi,%rdi), %eax
	ret
	.cfi_endproc
.LFE9:
	.size	test_inline, .-test_inline
	.p2align 4
	.globl	test_normal
	.type	test_normal, @function
test_normal:
.LFB11:
	.cfi_startproc
	leal	(%rdi,%rdi), %eax
	ret
	.cfi_endproc
.LFE11:
	.size	test_normal, .-test_normal
	.p2align 4
	.globl	simplified_branch
	.type	simplified_branch, @function
simplified_branch:
.LFB6:
	.cfi_startproc
	movl	$42, %eax
	ret
	.cfi_endproc
.LFE6:
	.size	simplified_branch, .-simplified_branch
	.p2align 4
	.globl	dereference_chain
	.type	dereference_chain, @function
dereference_chain:
.LFB7:
	.cfi_startproc
	movq	(%rdi), %rax
	movq	(%rax), %rax
	movl	8(%rax), %eax
	ret
	.cfi_endproc
.LFE7:
	.size	dereference_chain, .-dereference_chain
	.ident	"GCC: (GNU) 16.1.1 20260430"
	.section	.note.GNU-stack,"",@progbits
