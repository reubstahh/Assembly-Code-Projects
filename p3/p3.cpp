
// This is the main project file for VC++ application project
// generated using an Application Wizard.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt.h"
int main(int argc, char** argv)
{
char *in_string, out_string[128];
long enc_flag;
long rc = 0;
long dir = 0, count = 0;
if (argc < 3)
{
printf("Usage #1: %s input_string encrypt_flag\n", argv[0]);
printf("Usage #2: %s input_string encrypt_flag direction shiftcount\n",
argv[0]);
return 1;
}
in_string = argv[1];
enc_flag = atol(argv[2]);
enc_flag = (enc_flag == 0) ? 0 : 1;
if (argc == 5)
{
dir = atol(argv[3]);
count = atol(argv[4]);
if ((dir < 0 || dir > 1) || (count < 0))
{
printf("Invalid direction or shiftcount\n");
return 1;
}
set_key(dir, count);
}
get_key(&dir, &count);
printf("Current keys: Direction=%ld, Shiftcount=%ld\n", dir, count);
rc = s_encrypt( in_string, out_string, enc_flag);
printf( "Source string = %s\n", in_string);
printf( "Target string = %s\n", out_string);
printf( "Number of characters converted = %ld\n\n", rc );
return rc;
}
