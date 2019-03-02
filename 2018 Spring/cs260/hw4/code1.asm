.data 
	str: .space 64
	input: .asciiz "Enter string:\n"
	output: .asciiz "String is converted \n"


.text

main:
	la $a0,input
	li $v0,4
	syscall


	la $a0,str
	li $a1, 100
	li $v0,8 
	syscall


init:
	la $s0,str 
	lb $s1, ($s0)   
	la $s2 'a'  
	li $t0, 0   

howmanya:
	beq $s1, $zero, start 
	bne $s1, $s2, then
	addi $t0, $t0, 1
then:
	addi $s0, $s0, 1
	lb $s1, ($s0)
	j howmanya
	
start:
	la $s0,str 
	lb $s1, ($s0)   
	la $s2, 'a'
	li $s3, 0
	li $t1, 0

condition:

	beq $s1, $zero, print  
	beq $s1, 'a', theresa  
	sub $s4, $s0, $s3, 
	sb $s1, ($s4)
	j continues

theresa: 
	addi $t1, $t1, 1
	bne $t0, $t1, continues
	addi $s3, $s3, 1
continues:
	addi $s0, $s0, 1
	lb $s1, ($s0)
	j condition

print:
	la $a0, output
	li $v0,4
	syscall

	la $a0, str
	li $v0, 4
	syscall

end:

	li $v0, 10
	syscall
