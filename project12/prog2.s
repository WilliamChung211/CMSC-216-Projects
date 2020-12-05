# Name: William Chung
# Section Number: 0101

# This program reads two integers into global
# variables x and y. It passes these into parameters
# of a function with then counts and returns how many
# y is in x. This value is then printed in main.
	
	.data

x:	.word 0		# int x	
y:	.word 0		# int y
result:	.word 0		# int result

	.text


count_digit:
				# prologue
	sub	$sp, $sp, 16	# set new stack pointer
	sw	$ra, 16($sp)	# save return address in stack
	sw	$fp, 12($sp)	# save old frame pointer in stack
	add	$fp, $sp, 16	# set new frame pointer

	
	li	$t0, -1		# count = -1
	sw	$t0, 8($sp)	

	lw	$t0, 4($fp)	
	bltz	$t0, endif	# if digit is less than 0
	bgt	$t0, 9, endif	# or greater than 9, ends
	
	li	$t0, 0		# count= 0
	sw	$t0, 8($sp)

	lw	$t0, 8($fp)	
	bgez	$t0, is_pos	# if num >= 0 skip

	mul	$t0, $t0, -1	# num= -num 
	sw	$t0, 8($fp)
	
is_pos:	lw	$t0, 8($fp)	
	bnez	$t0, else	# if num !=0 go to else block
	lw	$t1, 4($fp)
	bnez	$t1, else	# if digit !=0 go to else block
	li	$t0, 1		# count= 1 
	sw	$t0, 8($sp)
	j	endif		# skip else block
else:
loop:	lw  	$t0, 8($fp)
	
	blez	$t0, endif	# if num <=0, done with loop
	lw	$t1, 8($fp)	# keeps going till finished counting
	
	rem	$t1, $t1, 10	
	sw	$t1, 4($sp)	# rightmost_digit= num % 10

	lw	$t0, 4($sp)
	lw	$t1, 4($fp)
	bne	$t0, $t1, skip	# rightmost_digit != digit then skip
	
	lw	$t2, 8($sp)	# if not, counter is updated
	add	$t2, $t2, 1	
	sw	$t2, 8($sp)	# count++
	
skip:	lw	$t0, 8($fp)	
	div	$t0, $t0, 10
	sw	$t0, 8($fp)	# num /= 10
	j 	loop
	
endif:	lw	$t0, 8($sp)	# return count
	move	$v0, $t0

				# epilogue
	lw	$ra, 16($sp)	# restore $ra
	lw	$fp, 12($sp)	# restore $rp
	add	$sp, $sp, 16	# reset $fp
	jr	$ra		# ret to caller

	
main:	li	$sp, 0x7ffffffc	# init $sp
	
	li	$v0, 5		
	syscall
	sw	$v0, x		# scans int to x
	
	li	$v0, 5
	syscall
	sw	$v0, y		# scans int to y

	lw	$t0, x
	sw	$t0, ($sp)
	sub	$sp, $sp, 4	# push arg x

	lw	$t0, y
	sw	$t0, ($sp)
	sub	$sp, $sp, 4	# push arg y
	
	jal	count_digit	# call count_digit

	add	$sp, $sp, 8	# pop args
	move	$t0, $v0	# stores return value
	sw	$t0, result

	lw	$a0, result	# going to print what counter returned
	li	$v0, 1		# prints return value
	syscall

	li	$v0, 11
	li	$a0, 10
	syscall			# prints newline
	
	li	$v0, 10		# quits
	syscall


