#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // 4 arguments
    if (argc != 4)
    {
        printf("Usage: ./copy num infile outfile\n");
        return 1;
    }

    //Checks for num
    int num = atoi(argv[1]);

    if (num < 1 || num > 100)
    {
        printf("Must be between 0 and 100\n");
        return 2;
    }


    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }
    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // create struct of .bmp fileheader
    BITMAPFILEHEADER bf;
    // read input file's BITMAPFILEHEADER
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // create struct of .bmp infoheader
    BITMAPINFOHEADER bi;
    // read infile's BITMAPINFOHEADER
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    // Initialize new headers and set them to the original
    BITMAPFILEHEADER bf_new;
    BITMAPINFOHEADER bi_new;
    bf_new = bf;
    bi_new = bi;

    // set new headers height/width to old * arg[1]
    bi_new.biHeight = bi.biHeight*num;
    bi_new.biWidth = bi.biWidth*num;

    // update padding
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding =  (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // update image size
    bi_new.biSizeImage = (bi_new.biWidth *sizeof(RGBTRIPLE) + new_padding) * abs(bi_new.biHeight);
    // bfSize = biSizeImage + bi.biSize (fixed) + bf.bfSize
    bf_new.bfSize = bi_new.biSizeImage + bi_new.biSize + bf_new.bfSize;

    // write .bmp headers to output file
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);

    printf("bf_new.bfSize = %i\n", bf_new.bfSize);
    printf("bf.bfSize = %i\n", bf.bfSize);

    // iterate over infile's scanlines (height)
    for (int i = 0; i < abs(bi.biHeight); i++)
    {
        // multiply input files read by the factor n given in argv[1]
        for (int n = 0; n < num; n++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from input file
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to output file # number of times
                for (int r = 0; r < num; r++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // pass over input file's padding
            fseek(inptr, padding, SEEK_CUR);

            //Add % 4 padding to new image size
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }

            // move cursor of input file back "n" num times
            if (n < num - 1)
            {
                long offset = bi.biWidth * sizeof(RGBTRIPLE) + padding;
                fseek(inptr, -offset, SEEK_CUR);
            }
        }
    }

    printf("bf.bfSize = %i\n", bf.bfSize);

    // close infile
    fclose(inptr);
    // close outfile
    fclose(outptr);
    // that's all folks
    return 0;
}