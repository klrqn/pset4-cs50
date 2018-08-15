// recover.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(void)
{
    // open card file
    FILE* inptr = fopen("card.raw", "r");

    if (inptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "card.raw file not working,  \n");
        return 1;
    }

    // FILE* img = NULL;

    // init variables
    BYTE buffer[512];
    // int fnumber = 0;
    int searchjpeg = 1; // 1 if true, 0 if false

    // cycle through file by 512 MB block
    while (fread(&buffer, 512, 1, inptr) == 1)
    {
        // start of a new jpeg?
        if (searchjpeg == 1)
        {
            // is this the beginning of a jpeg
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] = 0xe0 || buffer[3] == 0xe1))
            {
                searchjpeg = 0;
                printf("yes\n");
            }
            else
            {
                printf("no\n");
            }
        }
    }

    // filename
    // char title[8];

    // repeat until end of card
        // read 512 bytes into a buffer
        // start of a new jpg?
            // yes...?
            // no ...?
        // already found a jpg?
            // no...
            // yes ...
    // close any remaining files
}