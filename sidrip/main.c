#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {
    printf("SID stripper v0.0.1\n");
    if (argc != 2) {
        printf("Usage sidstrip <sidfilename>\n");
        exit(0);
    }


    FILE *iptr;
    int size;
    iptr = fopen(argv[1],"rb");
    if(iptr == NULL) {
      printf("ERROR: Unable to open file: %s\n",argv[1]);   
      exit(1);             
    }

    fseek(iptr, 0, SEEK_END);
    size = ftell(iptr);
    fseek(iptr, 0, SEEK_SET);

    printf("Filesize: %i\n",size);
    unsigned char *buffer = malloc(size);
    if (buffer == NULL) {
        printf("ERROR: Unable allocate memory.\n");
        exit(5);
    }

    fread(buffer,size,1,iptr);

    fclose(iptr);

    char magicID[4];
    short version=0;
    short dataoffset=0;
    short loadaddress=0;
    short initaddress=0;
    short playaddress=0;
    short songs=0;
    short startsong=0;

    memcpy(magicID,buffer,4);
    version = buffer[5] | buffer[4] <<8;
    dataoffset = buffer[7] | buffer[6] <<8;
    loadaddress = buffer[9] | buffer[8] <<8;
    if ( loadaddress == 0 ) {
        loadaddress = buffer[dataoffset] | buffer[dataoffset+1] <<8;;
    }
    initaddress = buffer[11] | buffer[10] <<8;
    playaddress = buffer[13] | buffer[12] <<8;
    songs = buffer[15] | buffer[14] <<8;
    startsong = buffer[17] | buffer[16] <<8;

    printf("magicID: %s\n",magicID);
    printf("version: %04x\n",version);
    printf("dataoffset: %04x\n",dataoffset);
    printf("loadaddress: %04x\n",loadaddress);
    printf("initaddress: %04x\n",initaddress);
    printf("playaddress: %04x\n",playaddress);
    printf("songs: %i\n",songs);
    printf("startsong: %i\n",startsong);


    int namesize = sizeof argv[1];
    char filename[namesize];
    strcpy(filename,argv[1]);
    filename[namesize-2]='d';
    filename[namesize-1]='a';
    filename[namesize]='t';
//    char *ext = strrchr(filename, '.');
    
    FILE *optr;
    optr = fopen(filename,"wb");
    if(optr == NULL) {
      printf("ERROR: Unable save file: %s\n",filename);
      exit(1);             
    }
    fwrite(buffer+dataoffset,size-dataoffset,1,optr);
    fclose(optr);

    printf("%s saved.\n",filename);
/*
    for (int count = 0; count < 256; count ++) {
        printf("%02x ",buffer[count]);
        int temp = count % 15;
        if ( temp == 14 ) {
            printf("\n");
        }
    }
*/
    if (buffer != NULL) {
        free(buffer);
    }

    return 0;
}
