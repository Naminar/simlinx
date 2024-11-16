	.file	"fib.cc"
	.option pic
	.attribute arch, "rv64i2p1"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z19fibonacci_recursivei
	.type	_Z19fibonacci_recursivei, @function
_Z19fibonacci_recursivei:
.LFB0:
	.cfi_startproc
	li	a5,1
	ble	a0,a5,.L3
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sd	ra,24(sp)
	sd	s0,16(sp)
	sd	s1,8(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	.cfi_offset 9, -24
	mv	s0,a0
	addiw	a0,a0,-1
	call	_Z19fibonacci_recursivei
	mv	s1,a0
	addiw	a0,s0,-2
	call	_Z19fibonacci_recursivei
	addw	a0,s1,a0
	ld	ra,24(sp)
	.cfi_restore 1
	ld	s0,16(sp)
	.cfi_restore 8
	ld	s1,8(sp)
	.cfi_restore 9
	addi	sp,sp,32
	.cfi_def_cfa_offset 0
	jr	ra
.L3:
	li	a0,1
	ret
	.cfi_endproc
.LFE0:
	.size	_Z19fibonacci_recursivei, .-_Z19fibonacci_recursivei
	.align	2
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	addi	sp,sp,-16
	.cfi_def_cfa_offset 16
	sd	ra,8(sp)
	.cfi_offset 1, -8
	li	a0,7
	call	_Z19fibonacci_recursivei
	ld	ra,8(sp)
	.cfi_restore 1
	addi	sp,sp,16
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
