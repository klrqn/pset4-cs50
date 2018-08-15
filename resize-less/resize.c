#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize j infile outfile\n");
        return 1;
    }

    // make sure resize is under 100
    // uint8_t

    // remember filenames
    char *scale = argv[1];
    char *infile = argv[2];
    char *outfile = argv[3];

    int n = atoi(scale);
    // printf("n = %i\n", n);
    if (n > 100 || n < 1)
    {
        fprintf(stderr, "n must be a positive integer less than 100\n");
        return 2;
    }
    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    BITMAPFILEHEADER bf;
    // read infile's BITMAPFILEHEADER and assign to 'bf'
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    // read infile's BITMAPINFOHEADER and assign to 'bi'
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //Initialize the new_bf
    BITMAPFILEHEADER new_bf;
    BITMAPINFOHEADER new_bi;
    new_bf = bf;
    new_bi = bi;

    //Set newest
    new_bi.biHeight = bi.biHeight*n;
    new_bi.biWidth = bi.biWidth*n;

    //Padding for fseek, multiple of 4
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding =  (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //New Image Size
    new_bi.biSizeImage = (new_bi.biWidth * sizeof(RGBTRIPLE) + new_padding) * abs(new_bi.biHeight);
    new_bf.bfSize = new_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); //54 is BF+BI

    //open file to write output
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, old_biHeight = abs(bi.biHeight); i < old_biHeight; i++)
    {
        // scans over the same input line for n*output rows
        for (int l = 0; l < n; l++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                //BGR + BGR + BGR n # of times on width
                for (int r = 0; r < n; r++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // pass over input file's padding
            fseek(inptr, padding, SEEK_CUR);

            // make sure new file width is a multiple of 4 by adding new padding
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }

            //pushes cursor back and iterate # of n times on "L" row
            if (l < n - 1)
            {
                // By default, files are unsigned (nonnegative). Need to define negative signed long
                long offset = bi.biWidth * sizeof(RGBTRIPLE) + padding;
                // this shoves cursor back the entire length to start
                fseek(inptr, -offset, SEEK_CUR);
            }
        }
    }
    // close infile
    fclose(inptr);
    // close outfile
    fclose(outptr);
    // that's all folks
    return 0;
}
