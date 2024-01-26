#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void sorter(long* list, long count, long opcode) {
    asm (
        "movq %0, %%rax;"      // Load base address of list into rax
        "movq %1, %%rbx;"      // Load the opcode (sorting direction) into rbx
        "movq %2, %%rcx;"      // Load count (number of elements) into rcx
        "xorq %%rdi, %%rdi;"   // Initialize rdi (serves as the 'x' counter for outer loop) to 0

    "outer_loop:"
        "cmpq %%rcx, %%rdi;"   // Compare x (rdi) with count (rcx)
        "jge done;"            // If x >= count, end sorting

        "movq %%rdi, %%rsi;"   // Initialize rsi as y (inner loop counter), set it to x
        "movq (%%rax, %%rdi, 8), %%r8;"   // Load list[x] into r8

    "inner_loop:"
        "cmpq %%rcx, %%rsi;"   // Compare y (rsi) with count (rcx)
        "jge next_outer;"      // If y >= count, go to next iteration of outer loop
        "movq (%%rax, %%rsi, 8), %%r9;"   // Load list[y] into r9

        // Check for ascending order (opcode = 1)
        "cmpq $1, %%rbx;"      // Compare opcode (rbx) with 1 (for ascending)
        "je ascending_check;"  // If opcode is 1, jump to ascending check

        // For descending order
        "cmpq %%r8, %%r9;"     // Compare list[x] (r8) with list[y] (r9)
        "jg swap;"             // If list[x] > list[y], swap for descending order
        "jmp next_inner;"      // Go to next iteration of inner loop

    "ascending_check:"
        "cmpq %%r8, %%r9;"     // Compare list[x] (r8) with list[y] (r9) again
        "jl swap;"             // If list[x] < list[y], swap for ascending order

    "next_inner:"
        "incq %%rsi;"          // Increment y (inner loop counter)
        "jmp inner_loop;"      // Continue inner loop

    "swap:"  // Swap list[x] and list[y] using xchg instruction
        "xchgq %%r8, (%%rax, %%rsi, 8);"  // Swap r8 with list[y]
        "xchgq %%r9, (%%rax, %%rdi, 8);"  // Swap r9 with list[x]
        "jmp next_inner;"      // Go to next iteration of inner loop

    "next_outer:"
        "incq %%rdi;"          // Increment x (outer loop counter)
        "jmp outer_loop;"      // Continue outer loop

    "done:"  // End of sorting algorithm
        :
        : "m" (list), "m" (opcode), "m" (count)
        : "rax", "rbx", "rcx", "rdi", "rsi", "r8", "r9"  // Clobbered registers
    );
}



int main(int argc, char** argv)
{
long numlist[1000], asc;
FILE *fptr;
long i = 0;
if (argc != 3)
{
printf("Usage: %s filename asc_des\n", argv[0]);
return 1;
}
asc = atol (argv[2]);
asc = (asc == 1) ? 1 : 2;
printf("\n");
fptr = fopen((argv[1]), "rtc");
if (fptr == NULL)
printf( "File %s was not opened\n",argv[1] );
else
{
/* Set pointer to beginning of file: */
fseek( fptr, 0L, SEEK_SET );
/* Read data from file: */
while ( fscanf(fptr, "%ld", &numlist[i]) != EOF )
{
printf( "%ld ", numlist[i] );
i++;
}
printf( "\n\nNumber of integer = %ld\n", i );
printf( "Ascend_or_Descend = %ld\n\n", asc );
fclose( fptr );
}
sorter( numlist, i, asc);
for (int j = 0; j < i; j++)
printf( "%ld ", numlist[j] );
printf("\n");
printf("\n");
return 0;
}








