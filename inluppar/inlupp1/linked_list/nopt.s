	.file	"linked_list.c"
# GNU C17 (Ubuntu 13.3.0-6ubuntu2~24.04.1) version 13.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 13.3.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O3 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.p2align 4
	.globl	ioopm_list_is_empty
	.type	ioopm_list_is_empty, @function
ioopm_list_is_empty:
.LFB39:
	.cfi_startproc
	endbr64	
# linked_list.c:13:     return list->size == 0; 
	cmpq	$0, 24(%rdi)	#, list_3(D)->size
	sete	%al	#, tmp86
# linked_list.c:14: }
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
	subq	$8, %rsp	#,
	.cfi_def_cfa_offset 16
# linked_list.c:18:     ioopm_list_t *linked_list = calloc(1, sizeof(ioopm_list_t));
	movl	$32, %esi	#,
	movl	$1, %edi	#,
	call	calloc@PLT	#
# linked_list.c:19:     linked_list->last = &linked_list->sentinel;
	movq	%rax, 16(%rax)	# tmp84, linked_list_4->last
# linked_list.c:21: }
	addq	$8, %rsp	#,
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
	pushq	%rbx	#
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	movl	$16, %esi	#,
# linked_list.c:24: {
	movq	%rdi, %rbx	# tmp86, value
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	movl	$1, %edi	#,
	call	calloc@PLT	#
# linked_list.c:26:     entry->value = value; 
	movq	%rbx, (%rax)	# value, entry_3->value
# linked_list.c:27:     entry->next = NULL;
	movq	$0, 8(%rax)	#, entry_3->next
# linked_list.c:29: }
	popq	%rbx	#
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
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp	# tmp85, list
	pushq	%rbx	#
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp	#,
	.cfi_def_cfa_offset 32
# linked_list.c:46:     entries_destroy_tail(list->sentinel.next);
	movq	8(%rdi), %rbx	# list_3(D)->sentinel.next, next
# linked_list.c:33:     if (entry == NULL)
	testq	%rbx, %rbx	# next
	je	.L8	#,
	.p2align 4,,10
	.p2align 3
.L9:
	movq	%rbx, %rdi	# next, entry
# linked_list.c:37:     list_entry_t *next = entry->next;
	movq	8(%rbx), %rbx	# entry_11->next, next
# linked_list.c:38:     free(entry);
	call	free@PLT	#
# linked_list.c:33:     if (entry == NULL)
	testq	%rbx, %rbx	# next
	jne	.L9	#,
.L8:
# linked_list.c:60: }
	addq	$8, %rsp	#,
	.cfi_def_cfa_offset 24
# linked_list.c:47:     free(list); 
	movq	%rbp, %rdi	# list,
# linked_list.c:60: }
	popq	%rbx	#
	.cfi_def_cfa_offset 16
	popq	%rbp	#
	.cfi_def_cfa_offset 8
# linked_list.c:47:     free(list); 
	jmp	free@PLT	#
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
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp	# tmp92, value
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	movl	$16, %esi	#,
# linked_list.c:62: {
	pushq	%rbx	#
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx	# tmp91, list
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	movl	$1, %edi	#,
# linked_list.c:62: {
	subq	$8, %rsp	#,
	.cfi_def_cfa_offset 32
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	call	calloc@PLT	#
# linked_list.c:66:     list->last->next = entry;
	movq	16(%rbx), %rdx	# list_5(D)->last, list_5(D)->last
# linked_list.c:26:     entry->value = value; 
	movq	%rbp, (%rax)	# value, entry_9->value
# linked_list.c:27:     entry->next = NULL;
	movq	$0, 8(%rax)	#, entry_9->next
# linked_list.c:66:     list->last->next = entry;
	movq	%rax, 8(%rdx)	# tmp89, _1->next
# linked_list.c:67:     list->size++; 
	addq	$1, 24(%rbx)	#, list_5(D)->size
# linked_list.c:69:     list->last = entry;
	movq	%rax, 16(%rbx)	# tmp89, list_5(D)->last
# linked_list.c:70: }
	addq	$8, %rsp	#,
	.cfi_def_cfa_offset 24
	popq	%rbx	#
	.cfi_def_cfa_offset 16
	popq	%rbp	#
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
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp	# tmp94, value
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	movl	$16, %esi	#,
# linked_list.c:73: {
	pushq	%rbx	#
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx	# tmp93, list
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	movl	$1, %edi	#,
# linked_list.c:73: {
	subq	$8, %rsp	#,
	.cfi_def_cfa_offset 32
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	call	calloc@PLT	#
# linked_list.c:77:     entry->next = list->sentinel.next;
	movq	8(%rbx), %rdx	# list_6(D)->sentinel.next, list_6(D)->sentinel.next
# linked_list.c:26:     entry->value = value; 
	movq	%rbp, (%rax)	# value, entry_11->value
# linked_list.c:77:     entry->next = list->sentinel.next;
	movq	%rdx, 8(%rax)	# list_6(D)->sentinel.next, entry_11->next
# linked_list.c:83:     if (list->size == 0)
	movq	24(%rbx), %rdx	# list_6(D)->size, _2
# linked_list.c:79:     list->sentinel.next = entry; 
	movq	%rax, 8(%rbx)	# tmp89, list_6(D)->sentinel.next
# linked_list.c:83:     if (list->size == 0)
	testq	%rdx, %rdx	# _2
	jne	.L18	#,
# linked_list.c:85:         list->last = entry; 
	movq	%rax, 16(%rbx)	# tmp89, list_6(D)->last
.L18:
# linked_list.c:88:     list->size++;  
	addq	$1, %rdx	#, tmp91
	movq	%rdx, 24(%rbx)	# tmp91, list_6(D)->size
# linked_list.c:89: }
	addq	$8, %rsp	#,
	.cfi_def_cfa_offset 24
	popq	%rbx	#
	.cfi_def_cfa_offset 16
	popq	%rbp	#
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
# linked_list.c:94:     if (list->sentinel.next == NULL)
	movq	8(%rdi), %rax	# list_4(D)->sentinel.next, _1
# linked_list.c:94:     if (list->sentinel.next == NULL)
	testq	%rax, %rax	# _1
	je	.L23	#,
# linked_list.c:99:     return list->sentinel.next->value; 
	movq	(%rax), %rax	# _1->value, D.4166
# linked_list.c:101: }
	ret	
	.p2align 4,,10
	.p2align 3
.L23:
# linked_list.c:96:         return int_elem(-1);
	movl	$4294967295, %eax	#, D.4166
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
# linked_list.c:106:     if (list->sentinel.next == NULL)
	cmpq	$0, 8(%rdi)	#, list_5(D)->sentinel.next
	je	.L27	#,
# linked_list.c:110:     return list->last->value; 
	movq	16(%rdi), %rax	# list_5(D)->last, list_5(D)->last
	movq	(%rax), %rax	# _2->value, D.4171
# linked_list.c:112: }
	ret	
	.p2align 4,,10
	.p2align 3
.L27:
# linked_list.c:108:         return int_elem(-1);
	movl	$4294967295, %eax	#, D.4171
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
	pushq	%r12	#
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdx, %r12	# tmp96, value
	pushq	%rbp	#
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rsi, %rbp	# tmp95, index
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	movl	$16, %esi	#,
# linked_list.c:125: {
	pushq	%rbx	#
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
# linked_list.c:125: {
	movq	%rdi, %rbx	# tmp94, list
# linked_list.c:25:     list_entry_t *entry = calloc(1, sizeof(list_entry_t));
	movl	$1, %edi	#,
	call	calloc@PLT	#
# linked_list.c:128:     list_entry_t *previous = find_previous_entry(list->sentinel.next, index, 0); 
	movq	8(%rbx), %rdx	# list_8(D)->sentinel.next, _1
# linked_list.c:26:     entry->value = value; 
	movq	%r12, (%rax)	# value, entry_16->value
# linked_list.c:27:     entry->next = NULL;
	movq	$0, 8(%rax)	#, entry_16->next
# linked_list.c:116:     if (i == index)
	testq	%rbp, %rbp	# index
	je	.L34	#,
.L29:
	jmp	.L29	#
	.p2align 4,,10
	.p2align 3
.L34:
# linked_list.c:131:     entry->next = previous->next;
	movq	8(%rdx), %rcx	# _1->next, _2
# linked_list.c:131:     entry->next = previous->next;
	movq	%rcx, 8(%rax)	# _2, entry_16->next
# linked_list.c:133:     previous->next = entry;
	movq	%rax, 8(%rdx)	# tmp91, _1->next
# linked_list.c:137:     if (index == list->size)
	movq	24(%rbx), %rdx	# list_8(D)->size, _3
# linked_list.c:137:     if (index == list->size)
	testq	%rdx, %rdx	# _3
	jne	.L31	#,
# linked_list.c:139:         list->last = entry;
	movq	%rax, 16(%rbx)	# tmp91, list_8(D)->last
.L31:
# linked_list.c:142:     list->size++; 
	addq	$1, %rdx	#, tmp92
	movq	%rdx, 24(%rbx)	# tmp92, list_8(D)->size
# linked_list.c:144: }
	popq	%rbx	#
	.cfi_def_cfa_offset 24
	popq	%rbp	#
	.cfi_def_cfa_offset 16
	popq	%r12	#
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
# linked_list.c:13:     return list->size == 0; 
	movq	24(%rdi), %rax	# MEM[(const struct ioopm_list_t *)list_9(D)].size, _11
# linked_list.c:149:     if (ioopm_list_is_empty(list))
	testq	%rax, %rax	# _11
	je	.L45	#,
# linked_list.c:147: {
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx	#
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx	# tmp95, list
	subq	$8, %rsp	#,
	.cfi_def_cfa_offset 32
# linked_list.c:155:     list_entry_t *previous = find_previous_entry(list->sentinel.next, index, 0); 
	movq	8(%rdi), %rdx	# list_9(D)->sentinel.next, _1
# linked_list.c:116:     if (i == index)
	testq	%rsi, %rsi	# index
	je	.L46	#,
.L38:
	jmp	.L38	#
	.p2align 4,,10
	.p2align 3
.L46:
# linked_list.c:156:     list_entry_t *current = previous->next;   
	movq	8(%rdx), %rdi	# _1->next, current
# linked_list.c:160:     previous->next = current->next; 
	movq	8(%rdi), %rcx	# current_12->next, _2
# linked_list.c:160:     previous->next = current->next; 
	movq	%rcx, 8(%rdx)	# _2, _1->next
# linked_list.c:163:     if (index == list->size - 1)
	cmpq	$1, %rax	#, _11
	je	.L47	#,
.L40:
# linked_list.c:168:     elem_t value = current->value; 
	movq	(%rdi), %rbp	# current_12->value, value
# linked_list.c:169:     free(current); 
	call	free@PLT	#
# linked_list.c:170:     list->size--;
	subq	$1, 24(%rbx)	#, list_9(D)->size
# linked_list.c:173: }
	addq	$8, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 24
# linked_list.c:172:     return value; 
	movq	%rbp, %rax	# value, D.4183
# linked_list.c:173: }
	popq	%rbx	#
	.cfi_def_cfa_offset 16
	popq	%rbp	#
	.cfi_def_cfa_offset 8
	ret	
	.p2align 4,,10
	.p2align 3
.L47:
	.cfi_restore_state
# linked_list.c:165:         list->last = previous; 
	movq	%rdx, 16(%rbx)	# _1, list_9(D)->last
	jmp	.L40	#
	.p2align 4,,10
	.p2align 3
.L45:
	.cfi_def_cfa_offset 8
	.cfi_restore 3
	.cfi_restore 6
# linked_list.c:151:         return int_elem(-1); 
	movl	$4294967295, %eax	#, D.4183
# linked_list.c:173: }
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
# linked_list.c:180:     if (list->size == 0 || index > list->size)
	movq	24(%rdi), %rdx	# list_7(D)->size, _1
# linked_list.c:177:     list_entry_t *current = list->sentinel.next;
	movq	8(%rdi), %rax	# list_7(D)->sentinel.next, current
# linked_list.c:180:     if (list->size == 0 || index > list->size)
	cmpq	%rsi, %rdx	# index, _1
	jb	.L49	#,
	testq	%rdx, %rdx	# _1
	je	.L49	#,
# linked_list.c:186:     for (size_t i = 0; i < index; i++)
	xorl	%edx, %edx	# i
# linked_list.c:186:     for (size_t i = 0; i < index; i++)
	testq	%rsi, %rsi	# index
	je	.L51	#,
	testb	$1, %sil	#, index
	je	.L50	#,
# linked_list.c:188:         current = current->next; 
	movq	8(%rax), %rax	# current_19->next, current
# linked_list.c:186:     for (size_t i = 0; i < index; i++)
	movl	$1, %edx	#, i
# linked_list.c:186:     for (size_t i = 0; i < index; i++)
	cmpq	$1, %rsi	#, index
	je	.L51	#,
	.p2align 4,,10
	.p2align 3
.L50:
# linked_list.c:188:         current = current->next; 
	movq	8(%rax), %rax	# current_19->next, current
# linked_list.c:186:     for (size_t i = 0; i < index; i++)
	addq	$2, %rdx	#, i
# linked_list.c:188:         current = current->next; 
	movq	8(%rax), %rax	# current_19->next, current
# linked_list.c:186:     for (size_t i = 0; i < index; i++)
	cmpq	%rdx, %rsi	# i, index
	jne	.L50	#,
.L51:
# linked_list.c:190:     return current->value;
	movq	(%rax), %rax	# current_20->value, D.4192
# linked_list.c:191: }
	ret	
	.p2align 4,,10
	.p2align 3
.L49:
# linked_list.c:182:         return int_elem(-1); 
	movl	$4294967295, %eax	#, D.4192
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
# linked_list.c:195:     return list->size;
	movq	24(%rdi), %rax	# list_2(D)->size, list_2(D)->size
# linked_list.c:196: }
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
