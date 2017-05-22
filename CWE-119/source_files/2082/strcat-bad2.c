/*
  PLOVER: NUM.OBO, BUFF.OVER
*/

/*
	Off-by-one error in bounds checking of strcat()

    input: ./a.out a234567890 b234567890
*/


#include <stdio.h>
#include <string.h>

#define MAXSIZE 20

void test(char *str, char *str2){
    char pre[2] = "<";
    char buf[MAXSIZE] = "";
    char post[2] = ">";
    if(strlen(str) < MAXSIZE)
	strcpy(buf, str);
    printf(" strcpy: %s%s%s\n", pre, buf, post);
    if(strlen(buf) + strlen(str2) <= MAXSIZE) // theoretical integer overflow
	strcat(buf, str2); // CWE-121
    printf("results: %s%s%s\n", pre, buf, post);
}

int main(int argc, char **argv){
    char *userstr;
    char *userstr2;
    if(argc > 2){
	userstr = argv[1];
	userstr2 = argv[2];
	test(userstr,userstr2);
    }
    printf("done\n");
    return 0;
}
