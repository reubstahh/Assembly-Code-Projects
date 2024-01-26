#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student
{
long ID; /* 8 bytes in the 64-bit environment */
char name[24];
};
/* This function returns the char* to the name string if the input sid has a match
on the student ID
from the array of student structures */
long search_by_id (char* list, long count, long sid)
{
long index=0;
asm
(
"movq %1, %%rax;" /* pointer to the beginning of the array of
structures */
"movq %2, %%rbx;" /* student ID */
"movq %3, %%rcx;" /* number of array entries */
"xorq %%rdx, %%rdx;"
"loop_start:"
"cmpq %%rdx, %%rcx;"
"je not_found;"
"movq (%%rax), %%rsi;" /* moving list[i].ID to %%rsi */
"cmpq %%rsi, %%rbx;" /* compare list[i].ID to the input sid */
"je found;"
"addq $32, %%rax;" /* 32 = size of each record for 64-bit; In 32-bit, it
is 28 */
"inc %%rdx;"
"jmp loop_start;"
"not_found:"
"xorq %%rax, %%rax;"
"jmp done;"
"found:"
"addq $8, %%rax;" /* skip over 8 bytes from the beginning of ID to the
name field */
"done:"
: "=a" (index)
: "m" (list), "m" (sid), "m" (count)
:
);
return index;
}

long search_by_name(char* list, long count, char* token) {
    struct student *students = (struct student*) list; // Convert the list pointer to an array of students
    long index; // Initialize the loop index

    // Perform case-insensitive search in C
    for (index = 0; index < count; ++index) { // Loop through the students
        if (strcasecmp(students[index].name, token) == 0) { // Compare student name with token
            // Found the student, now use assembly to retrieve the ID
            long id;
            asm(
                "movq %1, %%rax;" // Load the base address of the list
                "movq %2, %%rdx;" // Load the index of the found student
                "imulq $32, %%rdx;" // Multiply index by size of struct (32 bytes)
                "addq %%rax, %%rdx;" // Add to base address to get address of the student
                "movq (%%rdx), %%rax;" // Retrieve the ID from the student struct
                : "=a" (id) // Output (ID)
                : "r" (list), "r" (index) // Inputs (list, index)
                : "rdx" // Clobbered registers
            );
            return id; // Return the ID of the found student
        }
    }

    return 0; // Student not found, return 0
}



int main(int argc, char** argv)
{
char *token;
char *sname=NULL;
FILE *fptr;
long id = 0;
long sid = 0;
long i = 0;
struct student list[100]; /* array of structure */
if (argc != 4)
{
printf("Usage: %s filename token student_id\n", argv[0]);
return 1;
}
token = argv[2];
sid = atol(argv[3]);
printf("\n");
fptr = fopen((argv[1]), "rtc");
if (fptr == NULL)
printf( "File %s was not opened\n",argv[1] );
else
{
/* Set pointer to beginning of file: */
fseek( fptr, 0L, SEEK_SET );
/* Read data from file: */
while ( fscanf(fptr, "%s", (char*) &list[i].name) != EOF )
{
list[i].ID = i+1001;
printf( "%s ", list[i].name );
i++;
}
printf( "\n\nNumber of names = %ld\n", i );
printf( "Search Token = %s\n", token );
fclose( fptr );
}
printf( "\nStudent Id = %ld; Name = %s.\n\n", sid,
(char*)search_by_id((char*)list, i, sid));
id = search_by_name ((char*)list, i, token);
if (id)
printf( "Student Id = %ld; Name = %s\n", id, list[id-(1001)].name );
else
printf( "Student is not found.\n");
return 0;
}