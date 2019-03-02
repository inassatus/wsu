.data

.text
main:
	li $s0, 12
	addi $t0, $s0, 0
	addi $t1, $t0, -5 
	jal simpleEx
	addi $s1, $t4, 0

	addi $t0, $zero, 14
	addi $t1, $s0, 0
	jal simpleEx
	add $s1, $s1, $t4
	
	li $v0, 1
	move $a0, $s1
	syscall

	li $v0, 10
	syscall

simpleEx:
	li $t2, 7
	mul $t3, $t1, 2
	add $t4, $t0, $t3
	sub $t4, $t4, $t2
	jr $ra
	
	