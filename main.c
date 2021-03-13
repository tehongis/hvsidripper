#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const int BUFFERSIZE = 65536;

int main(int argc, char **argv) {

    struct stat stats;
    char sidfname[256];
    int filesize;

    if (argc > 1)
        {

        strcpy(sidfname, argv[1]);

        if (stat(sidfname, &stats) == 0) 
            {
                filesize = stats.st_size;
                printf("Filesize: %d\n",filesize);
            }
        else
            {
                printf("Unable to find file: %s\n",sidfname);
                exit(-1);
            }        
        }
    else
        {
            printf("Convertsid v1.0\n");
            printf("Usage: Convertsid [filename]\n");
            exit(0);
        }



    FILE* sidfile = fopen(sidfname, "rb" );
    if (!sidfile) {
        printf("Error: could not open file %s\n", sidfname);
        return -1;
        }

    char siddata[BUFFERSIZE];
    int bytes = fread(siddata, sizeof(char), BUFFERSIZE, sidfile);

    fclose(sidfile);

    printf("Bytes read: %d\n",bytes);

/*
    while ( (int bytes = fread(buffer, sizeof(char), BUFFERSIZE, filp)) > 0 ) {
        // Do something with the bytes, first elements of buffer.
        // For example, reversing the data and forget about it afterwards!
        for (char *beg = buffer, *end=buffer + bytes; beg < end; beg++, end-- ) {
           swap(*beg, *end);
        }
    }
*/

    return 0;
}