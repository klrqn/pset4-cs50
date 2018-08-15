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

    // init variables
    BYTE buffer[BLOCK_SIZE];    // create a buffer array of 512 bytes
    int filenumber = 1;         // create a file number counter
    FILE* img = NULL;           // create a file pointer

    // cycle through file by 512 byte block
    while (fread(buffer, BLOCK_SIZE, 1, inptr) == 1)
    {
        // check the first 4 bytes for jpeg signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] = 0xe0 || buffer[3] == 0xe1)) // ?
        {
            if (img != NULL)
            {
                fclose(img);
            }

            char title[8];
            sprintf(title, "%03d.jpg", filenumber);

            // open a new img file to write to
            img = fopen(title, "w");

            // iterate title
            filenumber++;
        }

        if (img != NULL)
        {
            fwrite(buffer, BLOCK_SIZE, 1, img);
        }
    }

    if (img != NULL)
    {
        fclose(img);
    }

    fclose(inptr);
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