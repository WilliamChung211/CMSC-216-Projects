/*
 * Name: William Chung
 * Login ID: Wchung1
 * ID Number: 116609968
 * Section: 0101 
 *
 *This program manipulates instructions
 * for a fictional processor. The file
 * has functions where you can print 
 * instructions, relocate, and
 * disassemble instructions. There is
 * also a function to check valid 
 * instructions
 */

#include "machine.h"
#include <stdio.h>

static void print_reg(Word word, int reg_num, Operation opp);

/*Given a word, prints the instructions and returns if it was valid*/
int print_instruction(Word instruction){
  
  Operation opp;
  int address;
  char* opps [15]={"plus", "diff", "mult", "quot", "mod",
		   "and", "or", "not", "br", "read", "wrt",
		   "move", "lw", "sw", "li"};

  if (!is_valid_instruction(instruction)) 
    return 0;

  /*prints the operation based on the oppcode*/
  opp=instruction >> 28;
  printf("%-7s",opps[opp]);

  /*Prints all the registers based on the last 9 bits*/
  print_reg(instruction & 511,3,opp);

  /*If it has a memory address it prints it*/
  if (opp == BR || opp >= LW) {
    address = (( instruction & ~((1 << 9) - 1) ) &  ((1 << 28) - 1) ) >> 9 ;
    printf("%05d", address);
  }
  
  return 1;
}

/*prints the register names*/
static void print_reg (Word word, int reg_num, const Operation opp) {

  if (reg_num > 0) {
   
    print_reg(word >> 3, reg_num -1, opp);
    
    word&=7;

    /*checks if it has a register and prints the register*/
    if( !( (reg_num == 2 && opp >= BR && opp != MOVE) ||
	   (reg_num == 3 && opp >= NOT) ) ) {
      printf("R");
      printf("%u",word);
      printf("  ");
    }
    
  }
    
}

/*converts machine language to assemply lanague */
int disassemble(const Word program[], unsigned int instr_num,
		unsigned int data_num) {

  
  int counter = 0;

  /*checks for any invalid parameters */
  if( (program == NULL) || instr_num <= 0 || instr_num > 3072
      || (instr_num + data_num > 3072) ) {
    return 0;
  }


  /*Prints all instructions*/
  for (;counter < instr_num;counter++) {

    if (is_valid_instruction(program[counter])) {
       	printf("%04x: ", counter * 4);
	print_instruction(program[counter]);
	printf("\n");
    }

    /*if any instruction is not valid it stops printing*/
    else {
      return 0;	
    }
      
  }

  /*prints all the data in hex up to 8 places*/
  for (;counter<data_num+instr_num;counter++) {
     printf("%04x: ", counter*4);
     printf("%08x", program[counter]);
     printf("\n");
  }

  return 1;
}

/*checks to see if the instruction is valid*/
int is_valid_instruction(Word word) {

  Word temp;
  int i;
  int address;
  int opp;

  /* could be invalid if not a valid operation*/
  opp = word >> 28;
  if (opp == 15) 
    return 0;
  
  temp = word  & 511;

  /* could be invalid if any register is 7*/
  for(i=0; i < 2; i++) {
    
    if ( (temp & 7) == 7) 
      return 0;
    
    temp>>=3;

  }

  /*could be invalid if 1st register is 0 or 6 except exceptions*/
  if ( (temp & 7) == 7 || ( ( (temp & 7) == 6 || (temp & 7)
			      == 0 ) && opp != BR  && opp
			    != WRT && opp != SW ) )
    return 0;


  /*LI can have any address*/
  if (opp!=LI || opp==BR || opp >= LW) {

    address = (( word & ~((1 << 9) - 1) ) &  ((1<<28) -1) ) >> 9 ;

    /*memory address cannot be more than memory and must be divisble by 4*/
    if (address >= 524288 || address%4 != 0 )
      return 0;
  }
    
    return 1;
    
}

/*moves a program to another part of memory */
int relocate(const Word program[], unsigned int instr_num,
             unsigned int data_num, int offset, Word relocated_program[],
             unsigned short *const instrs_changed){


  int i;
  int address;
  int new_address;
  int copy;
  int opp;

  /*checks for invalid parameters*/
  if( program == NULL || relocated_program == NULL || instrs_changed == NULL ||
      offset % 4 != 0 || instr_num <= 0 || instr_num > 3072 ||
      (instr_num + data_num > 3072) ) {
    return 0;
  }

  /*puts one program to the other program array*/
  for(i=0; i<instr_num; i++) {

    /*if not valid instruction stops relocating*/
    if (!is_valid_instruction(program[i]))
	return 0;
	
    opp=program[i] >> 28;
    copy=program[i];
    
    if (opp == BR || opp >= LW) {
     
      address = (( program[i] & ~((1 << 9) - 1) ) &  ((1 << 28) - 1) ) >> 9;
      new_address = address + offset;

      /*if not valid address stops relocating*/
      if (new_address >= 524288 && new_address < 0) {
	return 0;
      }
     
      copy+=(offset << 9 );
      (*instrs_changed)++;
    }

    relocated_program[i] = copy;
  }

    /*relocates the data too*/
   for (;i<data_num+instr_num;i++) 
     relocated_program[i]=program[i];

   return 1;

}
