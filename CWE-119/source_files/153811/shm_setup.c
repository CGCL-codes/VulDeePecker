/*#define _GNU_SOURCE*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    int shmsz;            /* size of shared mem (default 128) */
    int shmid;            /* id of shared mem segment */
    key_t key;            /* 'name' of shared memory */
    char *shm, *s;        /* pointer to shared mem */
    char *str = NULL;            /* data to put into the shared mem */
    int c;
    int errors = 0;
    char *out_filename = NULL;
    FILE *out_file = NULL;
    
    opterr = 0;

    /* Get options */
    while ((c = getopt(argc, argv, "k:s:m:o:h")) != -1) {
        switch(c) {
            case 'k':
                if ((key = strtol(optarg, NULL, 10)) < 1000) {
                    fprintf(stderr, "Key needs to be a 4 digit number.\n");
                    errors++;
                }
                break;
            case 's':
                if ((shmsz = atoi(optarg)) <= 0) {
                    fprintf(stderr, "Size needs to be an integer.\n");
                    errors++;
                }
                break;
            case 'o':
                out_filename = optarg;
                break;
            case 'm':
                str = optarg;
                break;
            case 'h':
                printf("Usage: shm_server -k <key> -s <size> -m <data>\nex) shm_server -k 1234 -s 128 -m 'Hello World'\n");
                return 0;
                break;
            case '?':
                fprintf(stderr, "Illegal argument!\n");
                errors++;
                break;
            }
    }
    
    if (!str) {
        fprintf(stderr, "No data provided.\n");
        errors++;
    } else if (strlen(str) >= shmsz) {
        fprintf(stderr, "Data too large for shared memory segment.\n");
        errors++;
    }
      
    if (errors) {
        printf("Usage: shm_server -k <key> -s <size> -m <data>\nex) shm_server -k 1234 -s 128 -m 'Hello World'\n");
        return 1;
    }
    
    /* Create the shared segment */
    if ((shmid = shmget(key, shmsz, IPC_CREAT | 0666)) < 0) {
        fprintf(stderr, "shmget error\n");
        return 1;
    }
    
    /* attach segment to our data space */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        fprintf(stderr, "shmat error\n");
        return 1;
    }
    
    /* put stuff in memory */
    s = shm;        /* pointer to memory in shm */

    /* Clear memory */
    memset(s, 0, shmsz);
    
    /* copy in the data */
    strcpy(shm, str);
    
    /* write out the shared memory id */
    if (out_filename) {
        if ((out_file = fopen(out_filename, "w"))) {
            fprintf(out_file, "%d", shmid);
            fclose(out_file);
            out_file = NULL;
        } else {
            fprintf(stderr, "Failed to open output file: \"%s\"\n", out_filename);
            fprintf(stdout, "%d", shmid);
        }
    } else {
        fprintf(stdout, "%d", shmid);
    }
    
    /* wait for input */
    fprintf(stderr, "Press any key to exit...");
    getchar();
    fprintf(stderr, "\n");

    return 0;
}
