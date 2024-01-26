
/*
encrypt.cpp - This file contains the code to encrypt and decrypt an input string
and output the modified string.
*/
#include "key.h"
#include <string.h>
long s_encrypt (char *src, char* dest, long en_flag);
void get_key (long *dir, long *count);
void set_key (long dir, long count);
void get_key (long *dir, long *count)
{
*dir = direction;
*count = shiftcount;
}
void set_key (long dir, long count)
{
direction = dir;
shiftcount = count;
}
long s_encrypt (char *src, char* dest, long en_flag)
{
long characters_changed = 0;
long len = strlen(src);
long count = shiftcount % 26; /* adjust shiftcount if > 26 */

asm(
    // Initialize registers
    "movq %[src], %%rsi;"      // Source string pointer
    "movq %[dest], %%rdi;"     // Destination string pointer
    "movq %[len], %%rcx;"      // String length
    "movq %[count], %%r8;"     // Shift count
    "movq %[direction], %%r9;" // Direction
    "movq %[en_flag], %%r10;"  // Encryption/Decryption flag
    "xorq %%rax, %%rax;"       // Characters changed counter set to 0

    // Loop to process each character
    "loop_start: "
    "cmpq $0, %%rcx;"          // Check if end of string
    "je loop_end;"             // Jump to end if yes

    // Load character from source
    "movb (%%rsi), %%bl;"      // Load byte from src into BL

    // Check if character is alphabetic and process
    "cmpb $0x41, %%bl;"        // Compare with 'A'
    "jl check_lowercase;"      // Jump to check lowercase if less than 'A'
    "cmpb $0x5A, %%bl;"        // Compare with 'Z'
    "jg check_lowercase;"      // Jump to check lowercase if greater than 'Z'

    // Logic for uppercase processing
    "subb $0x41, %%bl;"        // Adjust range to 0-25
    "cmpq $0, %%r10;"          // Check encryption/decryption flag
    "je encryption_upper;"     // Jump to encryption if flag is 0
    "jmp decryption_upper;"    // Jump to decryption if flag is 1

"encryption_upper:"
    "cmpq $0, %%r9;"           // Check direction
    "je forward_shift_upper;"  // Forward shift if direction is 0
    "jmp backward_shift_upper;" // Backward shift if direction is 1

"decryption_upper:"
    "cmpq $0, %%r9;"           // Check direction
    "je backward_shift_upper;" // Backward shift if direction is 0
    "jmp forward_shift_upper;" // Forward shift if direction is 1

"forward_shift_upper:"
    "addb %%r8b, %%bl;"        // Apply forward shift
    "cmpb $0x19, %%bl;"        // Compare with 25 ('Z' - 'A')
    "jle no_wrap_upper;"       // No wrap-around needed
    "subb $26, %%bl;"          // Wrap around
    "no_wrap_upper:"
    "addb $0x41, %%bl;"        // Adjust back to ASCII range
    "jmp char_processed;"      // Character processed

"backward_shift_upper:"
    "subb %%r8b, %%bl;"        // Apply backward shift
    "cmpb $0x00, %%bl;"        // Compare with 0 ('A' - 'A')
    "jge no_wrap_upper_b;"     // No wrap-around needed
    "addb $26, %%bl;"          // Wrap around
    "no_wrap_upper_b:"
    "addb $0x41, %%bl;"        // Adjust back to ASCII range
    "jmp char_processed;"      // Character processed


    "check_lowercase:"
    // Check if lowercase (a-z)
    "cmpb $0x61, %%bl;"        // Compare with 'a'
    "jl not_alpha;"            // Jump if less than 'a'
    "cmpb $0x7A, %%bl;"        // Compare with 'z'
    "jg not_alpha;"            // Jump if greater than 'z'

    // Logic for lowercase processing
    "subb $0x61, %%bl;"        // Adjust range to 0-25
    "cmpq $0, %%r10;"          // Check encryption/decryption flag
    "je encryption_lower;"     // Jump to encryption if flag is 0
    "jmp decryption_lower;"    // Jump to decryption if flag is 1

"encryption_lower:"
    "cmpq $0, %%r9;"           // Check direction
    "je forward_shift_lower;"  // Forward shift if direction is 0
    "jmp backward_shift_lower;" // Backward shift if direction is 1

"decryption_lower:"
    "cmpq $0, %%r9;"           // Check direction
    "je backward_shift_lower;" // Backward shift if direction is 0
    "jmp forward_shift_lower;" // Forward shift if direction is 1

"forward_shift_lower:"
    "addb %%r8b, %%bl;"        // Apply forward shift
    "cmpb $0x19, %%bl;"        // Compare with 25 ('z' - 'a')
    "jle no_wrap_lower;"       // No wrap-around needed
    "subb $26, %%bl;"          // Wrap around
    "no_wrap_lower:"
    "addb $0x61, %%bl;"        // Adjust back to ASCII range
    "jmp char_processed;"      // Character processed

"backward_shift_lower:"
    "subb %%r8b, %%bl;"        // Apply backward shift
    "cmpb $0x00, %%bl;"        // Compare with 0 ('a' - 'a')
    "jge no_wrap_lower_b;"     // No wrap-around needed
    "addb $26, %%bl;"          // Wrap around
    "no_wrap_lower_b:"
    "addb $0x61, %%bl;"        // Adjust back to ASCII range
    "jmp char_processed;"      // Character processed

    "not_alpha:"
    // If not alphabetic, just copy character to dest
    "movb %%bl, (%%rdi);"
    "jmp update_pointers;"

    "char_processed:"
    // Toggle case by flipping the 6th bit
    "xorb $0x20, %%bl;"

    // Write processed character to destination
    "movb %%bl, (%%rdi);"
    "inc %%rax;"               // Increment characters_changed

    "update_pointers:"
    // Update pointers and counters
    "inc %%rsi;"               // Increment source pointer
    "inc %%rdi;"               // Increment destination pointer
    "dec %%rcx;"               // Decrement length counter

    "jmp loop_start;"          // Jump back to start of loop
"loop_end: "
    // Null-terminate the destination string
    "movb $0, (%%rdi);"

    // Output the count of changed characters
    "movq %%rax, %[changed];"  // Store characters_changed in output variable
    : [changed] "=r" (characters_changed)
    : [src] "r" (src), [dest] "r" (dest), [len] "r" (len), [count] "r" (count), [direction] "r" (direction), [en_flag] "r" (en_flag)
    : "rsi", "rdi", "rcx", "rax", "r8", "r9", "r10", "rbx"
);




return characters_changed;
}





















