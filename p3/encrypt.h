/*
encrypt.h - This file contains the function prototype for the encrypt/decrypt
function as well as the global variables inside the DLL
*/
extern void get_key (long *dir, long *count);
extern void set_key (long dir, long count);
extern long s_encrypt (char *src, char* dest, long en_flag);
