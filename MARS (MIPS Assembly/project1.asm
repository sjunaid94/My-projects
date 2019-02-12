addu $8,  $0, 	3	#starting value of the seed
addi  $9,  $0 , 0x2010	#starting memory address
addu  $10,  $0, 1	#counter in the loop1
addi  $14,  $0, 16 	#number of values yon want to store
addi $14, $14, -1	
addu $15,  $0,  $8		#temporary hold the value of register 8
addi $16, $0, 0x000000ff	#mask to get the right-most 8 bits
addi $17, $0, 0xff000000	#mask to get the left-most 8 bits
addi $20, $0, 0X0000ffff	#mask to get the first 16 bits after merging
sw  $8, ( $9)			#storing the intital value of seed in the memory 

#Middle squaring 
outerLoop:

	innerLoop:
		slt  $11,  $10,  $15	#if counter is less than the current seed value
		beq  $11,  $0, out	#if it is not less than the current seed jump to loop 'out' 
		addu  $8,  $8,  $15	#update the register 8 by adding 
		addu  $10,  $10, 1	#incrementing the counter
		bne  $11,  $0, innerLoop	
	out:
		addi  $9,  $9, 4 	#updating the memory array
		and $18, $8, $16 	#to get the right-most bits
		and $19, $8, $17 	#to get the left-most 8 bits	
		srl, $19, $19,16 	#logical shifting right to 16 bits
		or $8, $19, $18 	#merge both left-most and the right-most bits
		and $8, $8, $20	  	#sign etending to the zeroes after the first 16 bits
		sw  $8, ( $9)    	#storing the middle square in the memory array 
		addi  $14,  $14, -1 	#counter for the outer loop, suppose to run 16 times
		addu  $15,  $0,  $8
		addi  $10,   $0, 1 	#intialzing the counter back to 1
		beq  $14,  $0, end	#if counter equals to zero, exit out
		beq  $14,  $14, outerLoop	#if counter for outer loop is equal to itself keep looping
	end: 
		addi  $14,  $0, 16 	#number of values yon want to store
		addi $9, $0, 0x2010
		addi $13, $0, 0
		addi $22, $0, 0
		addi $11, $0, 1
		lw $8, ($9)
		addi $23, $0, 0x2000
		#Hamming weight
	loop3:
		innerLoop2:
			 
 			and $21, $8, $11
			beq $21, 1, count
 			srl $8, $8, 1 		#shift $8 by one bit to the right
 			beq $8, $0, done
 			j innerLoop2
			count: 
				addi $22, $22, 1 	#if 1 bit is found increment
			 	srl $8, $8, 1		#shift $8 by one bit to the right
			 	beq $8, $0, done
 				j innerLoop2		#jump to innerLoop2
			done:	
				addi $9, $9, 4		#incrementing the memory address
				addi  $14,  $14, -1 	#decrementing the counter for the outer loop
				lw $8, ($9)		#updating the register 8
				beq  $14,  $0, end1	#if it is equal to zero exit out and do the avg
				beq  $14,  $14, loop3	#if it is equal keep on looping
			end1:
				srl $14,$22,4		#dividing the value in regiter 22 by 16
				sw $14, ($23)		#storing the avg hamming weight into memory
				addi $9, $0, 0x2010	#intializing the register with the starting memory address
				lw $8,($9)		#loading
				lw $13, 4($9)		#loading the neighbour value
				addi $14, $0, 15	#counter for the outer loop
				addi $12, $0, 1		#masking wiht 1
				addi $16, $0, 0		#intializing the counter to zero
				addi $20, $0, 0x2004
				
				#Hamming distances for Si to Si+1
			loop4:
				xor $11, $8, $13		#hamming distance between neighbours
				innerLoop3:
 					and $21, $11, $12	#AND between the register 11 and register 12
					beq $21, 1, count1	#if there is a one increment the counter
 					srl $11, $11, 1 	#shift $11 by one bit to the right
 					beq $11, $0, done1	#if there are no ones left just exit out	
 					j innerLoop3
 				count1:
 					addi $16, $16,1 	#counter for the number of 1's
 					srl $11, $11, 1		#shifting right by one bit
 					beq $11, $0, done1	#if there are no 1's left just exit out
 					j innerLoop3		#jump back in the loop	
 				done1:
 					addi $9, $9,4		#incrementing the address by 4 bytes
 					lw $8,($9)		#loading from memory into register 8
					lw $13, 4($9)		#loading the neighbour value from the memory array into register 13
 					addi $14, $14, -1	# decrementing the outer loop counter
 					beq $14, $0, end2	
 					beq $14, $14, loop4
 					
				end2:
					#For so and s15
					addi $9, $0, 0x2010
					addi $13, $0, 0x204C
					lw $8,($9)
					lw $14, ($13)
				 	xor $11, $14, $8
				 	innerLoop4:
						and $21, $11, $12
						beq $21, 1, count2
						srl $11, $11, 1
						beq $11, 0, end3
						j innerLoop4
				count2:
					addi $16, $16, 1
					srl $11, $11, 1
					beq $11, 0, end3
					j innerLoop4
				end3:
					srl $16, $16 ,4		#average of the hamming distances
					sw $16, ($20)		#storing the result into memory array
					
