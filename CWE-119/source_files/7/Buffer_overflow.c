void example(char *s) {
char buf[1024];
strcpy(buf, s);
}

int main(int argc, char **argv) {
example(argv[1]);
}
