#include <stdio.h>
#include <stdlib.h>

long add(long op1, long op2) {
    long output = 0;
    asm (
        "movq %1, %%rax;"
        "addq %2, %%rax;"
        : "=a" (output)
        : "m" (op1), "m" (op2)
    );
    return output;
}

long mult(long op1, long op2) {
    long result = 0;
    asm (
        "xor %%rax, %%rax;"          
        "check_op2_zero:"            
        "testq %2, %2;"              
        "jz end;"                    
        
        "check_lsb:"                 
        "testq $1, %2;"              
        "jz skip_add;"               
        
        "add %%rbx, %%rax;"          
        
        "jo overflow_detected;" 
        
        "skip_add:"
        "shlq $1, %1;"               
        "shrq $1, %2;"               
        "jmp check_op2_zero;"        
        
        "overflow_detected:"
        "jmp end;" 
        
        "end:"
        : "=a" (result)
        : "b" (op1), "c" (op2)
        : "cc" 
    );
    return result;
}


long mod (long op1, long op2) {
    long output=0;
    long local_op2=op2;
    asm (
        "movq %1, %%rax;"
        "movq %2, %%rbx;"
        "cdq;"
        "idivq %%rbx;"
        "movq %%rdx, %%rax;"
        : "=a" (output)
        : "m" (op1), "m" (local_op2)
        :
    );
    return output;
}

long shift (long op1, long direction, long number_of_bits) {
    long output=0;
    long is_left = direction == 0 ? 1 : 0;

    asm (
        "movq %1, %%rax;"
        "movq %2, %%rbx;"
        "movq %3, %%rcx;"
        "cmpq $1, %%rbx;"
        "jz Shift_Left ;"
        "sarq %%cl, %%rax ;"
        "jmp done;"
        "Shift_Left:"
        "salq %%cl, %%rax;"
        "done:"
        : "=a" (output)
        : "m" (op1), "m" (is_left), "m" (number_of_bits)
        :
    );
    return output;
}

long factorial(long n) {
    if (n < 1 || n > 20) {
        return -1;
    } 

    long result = 1; 
    
    asm (
        "movq %1, %%rcx;"       
        "movq $1, %%rax;"       

        "loop_start:"
        "cmpq $1, %%rcx;"       
        "jle loop_end;"         

        "imulq %%rcx, %%rax;"   
        "decq %%rcx;"           
        "jmp loop_start;"       

        "loop_end:"
        "movq %%rax, %0;"       

        : "=r" (result)         
        : "r" (n)               
        : "rax", "rcx", "cc"    
    );
    return result;
} 

long XOR(long op1, long op2) {
    long output = 0;
    asm (
        "movq %1, %%rax;"
        "xorq %2, %%rax;"
        : "=a" (output)
        : "m" (op1), "m" (op2)
    );
    return output;
}

long rotate(long op1, long direction, long number_of_bits) {
    long output = 0;
    asm (
        "movq %[input], %%rax;"      
        "movq %[bits], %%rcx;"       
        "cmpq $0, %[dir];"           
        "jz 1f;"                     
        "rorq %%cl, %%rax;"          
        "jmp 2f;"
        "1:"
        "rolq %%cl, %%rax;"          
        "2:"
        : [output] "=a" (output)      
        : [input] "r" (op1), 
          [dir] "r" (direction), 
          [bits] "r" (number_of_bits) 
        : "cc"                        
    );
    return output;
}

int main(int argc, char** argv){
long op1, op2, result;
op1 = op2 = result = 0;
if (argc != 3)
{
printf("Usage: %s op1 op2 (two integers)\n", argv[0]);
return 1;
}
op1 = atol(argv[1]);
op2 = atol(argv[2]);
printf("Operand 1 = %ld x%016lx Operand 2 = %ld x%016lx\n", op1,op1,op2,op2);
result = add(op1, op2);
printf("Add():\t%10ld x%016lx\n", result, result);

result = XOR(op1, op2);
printf("XOR():\t%10ld x%016lx\n", result, result);

result = mult(op1, op2);
printf("Mult():\t%10ld x%016lx\n", result, result);
if (op2 == 0)
{
printf("Mod Error: Divide by 0\n");
result = 0;
}
else
{
result = mod(op1, op2);
printf("Mod():\t%10ld x%016lx\n", result, result);
} 
if (op2 < 0)
{
printf("Error: Shift count must be >= 0\n");
result = 0;
}
else
{
result = shift(op1, 0, op2);
printf("ShiftL:\t%10ld x%016lx\n", result, result);
result = shift(op1, 1, op2);
printf("ShiftR:\t%10ld x%016lx\n", result, result);
}

if (op2 < 0)
{
printf("Error: Rotate count must be >= 0\n");
result = 0;
}
else
{
result = rotate(op1, 0, op2);
printf("RotateL:%10ld x%016lx\n", result, result);
result = rotate(op1, 1, op2);
printf("RotateR:%10ld x%016lx\n", result, result);
} 

if (op1 <= 0)
{
printf("Error: Factorial input must be a positive integer >=1\n");
result = 0;
}
else
{
result = factorial(op1);
printf("Fact():\t%10ld x%016lx\n\n", result, result);
} 
return 0;
}








