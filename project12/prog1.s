# Name: William Chung
# Login: Wchung1
# ID Number: 116609968
# Section Number: 0101

# This program reads an integer from input
# , stores it inito a global variable, and
# if the variable is posiive or 0, prints
# the sum of the cubes of the first n
# positive integers. If the variable is
# negative, it prints -1.
	
	.data

n:	.word 0			# int n	;
ans:	.word -1		# int ans= -1 ;

	.text

main:	li	$v0, 5		# scan an int
	syscall
	move	$t0, $v0
	sw	$t0, n		#the int is moved to n
	lw	$t0, n
	bltz	$t0, endif	# skips if negative 
	lw	$t0, n
	lw	$t1, n
	add	$t1, $t1, 1	# (n+1)
	mul	$t1, $t0, $t1	# n * (n+1)
	div	$t1, $t1, 2	# (n * (n+1) / 2)
	lw	$t2, n
	add 	$t2, $t2, 1	# same as above comments
	mul	$t2, $t0, $t2
	div	$t2, $t2, 2
	mul	$t1, $t1, $t2	# the two are multiplied
	sw	$t1, ans	# these operations represented
				# a formula to calculate
				# a sum of cubes and are
				# stored in ans

endif:	li	$v0, 1		# prints ans
	lw	$a0, ans
	syscall

	li	$v0, 11		# prints new line
	li	$a0, 10
	syscall

	li	$v0, 10		# exits program
	syscall
	
