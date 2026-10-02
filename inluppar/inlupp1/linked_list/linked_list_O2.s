	.file	"linked_list.c"
	.text
	.p2align 4
	.globl	ioopm_list_is_empty
	.type	ioopm_list_is_empty, @function
ioopm_list_is_empty:
.LFB39:
	.cfi_startproc
	endbr64
	cmpq	$0, 24(%rdi)
	sete	%al
	ret
	.cfi_endproc
.LFE39:
	.size	ioopm_list_is_empty, .-ioopm_list_is_empty
	.p2align 4
	.globl	ioopm_list_create
	.type	ioopm_list_create, @function
ioopm_list_create:
.LFB40:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$32, %esi
	movl	$1, %edi
	call	calloc@PLT
	movq	%rax, 16(%rax)
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE40:
	.size	ioopm_list_create, .-ioopm_list_create
	.p2align 4
	.globl	entry_create
	.type	entry_create, @function
entry_create:
.LFB41:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$16, %esi
	movq	%rdi, %rbx
	movl	$1, %edi
	call	calloc@PLT
	movq	%rbx, (%rax)
	movq	$0, 8(%rax)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE41:
	.size	entry_create, .-entry_create
	.p2align 4
	.globl	ioopm_list_destroy
	.type	ioopm_list_destroy, @function
ioopm_list_destroy:
.LFB43:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	8(%rdi), %rbx
	testq	%rbx, %rbx
	je	.L8
	.p2align 4,,10
	.p2align 3
.L9:
	movq	%rbx, %rdi
	movq	8(%rbx), %rbx
	call	free@PLT
	testq	%rbx, %rbx
	jne	.L9
.L8:
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE43:
	.size	ioopm_list_destroy, .-ioopm_list_destroy
	.p2align 4
	.globl	ioopm_list_append
	.type	ioopm_list_append, @function
ioopm_list_append:
.LFB44:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	movl	$16, %esi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movl	$1, %edi
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	calloc@PLT
	movq	16(%rbx), %rdx
	movq	%rbp, (%rax)
	movq	$0, 8(%rax)
	movq	%rax, 8(%rdx)
	addq	$1, 24(%rbx)
	movq	%rax, 16(%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE44:
	.size	ioopm_list_append, .-ioopm_list_append
	.p2align 4
	.globl	ioopm_list_prepend
	.type	ioopm_list_prepend, @function
ioopm_list_prepend:
.LFB45:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	movl	$16, %esi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movl	$1, %edi
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	calloc@PLT
	movq	8(%rbx), %rdx
	movq	%rbp, (%rax)
	movq	%rdx, 8(%rax)
	movq	24(%rbx), %rdx
	movq	%rax, 8(%rbx)
	testq	%rdx, %rdx
	jne	.L18
	movq	%rax, 16(%rbx)
.L18:
	addq	$1, %rdx
	movq	%rdx, 24(%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE45:
	.size	ioopm_list_prepend, .-ioopm_list_prepend
	.p2align 4
	.globl	ioopm_list_head
	.type	ioopm_list_head, @function
ioopm_list_head:
.LFB46:
	.cfi_startproc
	endbr64
	movq	8(%rdi), %rax
	testq	%rax, %rax
	je	.L23
	movq	(%rax), %rax
	ret
	.p2align 4,,10
	.p2align 3
.L23:
	movl	$4294967295, %eax
	ret
	.cfi_endproc
.LFE46:
	.size	ioopm_list_head, .-ioopm_list_head
	.p2align 4
	.globl	ioopm_list_last
	.type	ioopm_list_last, @function
ioopm_list_last:
.LFB47:
	.cfi_startproc
	endbr64
	cmpq	$0, 8(%rdi)
	je	.L27
	movq	16(%rdi), %rax
	movq	(%rax), %rax
	ret
	.p2align 4,,10
	.p2align 3
.L27:
	movl	$4294967295, %eax
	ret
	.cfi_endproc
.LFE47:
	.size	ioopm_list_last, .-ioopm_list_last
	.p2align 4
	.globl	ioopm_list_insert
	.type	ioopm_list_insert, @function
ioopm_list_insert:
.LFB49:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	movl	$1, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rsi, %rbx
	movl	$16, %esi
	call	calloc@PLT
	movq	%rbp, %rdx
	movq	%r12, (%rax)
	movq	$0, 8(%rax)
	testq	%rbx, %rbx
	je	.L32
	movq	%rbx, %rcx
	leaq	-1(%rbx), %rsi
	testb	$1, %bl
	je	.L29
	movq	8(%rbp), %rdx
	movq	%rsi, %rcx
	testq	%rsi, %rsi
	je	.L32
	.p2align 4,,10
	.p2align 3
.L29:
	movq	8(%rdx), %rdx
	movq	8(%rdx), %rdx
	subq	$2, %rcx
	jne	.L29
.L32:
	movq	8(%rdx), %rcx
	movq	%rcx, 8(%rax)
	movq	%rax, 8(%rdx)
	movq	24(%rbp), %rdx
	cmpq	%rbx, %rdx
	jne	.L31
	movq	%rax, 16(%rbp)
.L31:
	addq	$1, %rdx
	movq	%rdx, 24(%rbp)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE49:
	.size	ioopm_list_insert, .-ioopm_list_insert
	.p2align 4
	.globl	ioopm_list_remove
	.type	ioopm_list_remove, @function
ioopm_list_remove:
.LFB50:
	.cfi_startproc
	endbr64
	movq	24(%rdi), %rcx
	testq	%rcx, %rcx
	je	.L68
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rax
	movq	%rsi, %rdx
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	testq	%rsi, %rsi
	je	.L52
	leaq	-1(%rsi), %rdi
	testb	$1, %sil
	je	.L49
	movq	8(%rbx), %rax
	movq	%rdi, %rdx
	testq	%rdi, %rdi
	je	.L52
	.p2align 4,,10
	.p2align 3
.L49:
	movq	8(%rax), %rax
	movq	8(%rax), %rax
	subq	$2, %rdx
	jne	.L49
.L52:
	movq	8(%rax), %rdi
	subq	$1, %rcx
	movq	8(%rdi), %rdx
	movq	%rdx, 8(%rax)
	cmpq	%rsi, %rcx
	je	.L69
.L51:
	movq	(%rdi), %rbp
	call	free@PLT
	subq	$1, 24(%rbx)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L69:
	.cfi_restore_state
	movq	%rax, 16(%rbx)
	jmp	.L51
	.p2align 4,,10
	.p2align 3
.L68:
	.cfi_def_cfa_offset 8
	.cfi_restore 3
	.cfi_restore 6
	movl	$4294967295, %eax
	ret
	.cfi_endproc
.LFE50:
	.size	ioopm_list_remove, .-ioopm_list_remove
	.p2align 4
	.globl	ioopm_list_get
	.type	ioopm_list_get, @function
ioopm_list_get:
.LFB51:
	.cfi_startproc
	endbr64
	movq	24(%rdi), %rdx
	movq	8(%rdi), %rax
	cmpq	%rsi, %rdx
	jb	.L71
	testq	%rdx, %rdx
	je	.L71
	xorl	%edx, %edx
	testq	%rsi, %rsi
	je	.L73
	testb	$1, %sil
	je	.L72
	movq	8(%rax), %rax
	movl	$1, %edx
	cmpq	$1, %rsi
	je	.L73
	.p2align 4,,10
	.p2align 3
.L72:
	movq	8(%rax), %rax
	addq	$2, %rdx
	movq	8(%rax), %rax
	cmpq	%rdx, %rsi
	jne	.L72
.L73:
	movq	(%rax), %rax
	ret
	.p2align 4,,10
	.p2align 3
.L71:
	movl	$4294967295, %eax
	ret
	.cfi_endproc
.LFE51:
	.size	ioopm_list_get, .-ioopm_list_get
	.p2align 4
	.globl	ioopm_list_size
	.type	ioopm_list_size, @function
ioopm_list_size:
.LFB52:
	.cfi_startproc
	endbr64
	movq	24(%rdi), %rax
	ret
	.cfi_endproc
.LFE52:
	.size	ioopm_list_size, .-ioopm_list_size
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
