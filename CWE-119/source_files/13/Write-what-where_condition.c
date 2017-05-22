#define BUFSIZE 256
int main(int argc, char **argv) {
char *buf1 = (char *) malloc(BUFSIZE);
char *buf2 = (char *) malloc(BUFSIZE);
strcpy(buf1, argv[1]);
free(buf2);
}

