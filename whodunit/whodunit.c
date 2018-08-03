#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
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

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            if (triple.rgbtRed == 0xff)
            {
                triple.rgbtRed = 0x00;
            }

            // triple.rgbtBlue = 0xff;

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

// TODO
/* Open File
   Update Header's info for outfile
   read clue's scanline, pixel by pixel
   change pixel's color as necessary
   write verdict's scanline, pixel by pixel */

/* Copy.c
   opens a file
   updates header info for outfile
   reads each scanline, pixel by pixel
   writes each pixel into the output file's scanline */

// biWidth     - width of image(in pixels) not including padding
// biHeight    - height of image (pixels)
// biSizeImage - total size of image (in bytes) including pixels and padding
// bfSize      - total size of file (in bytes) - pixels, padding headers
// bf.bfSize   - bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)

/*

ff0000 - blue
ffffff - white
0000ff - RED

RGBTRIPLE STRUCT
// make a green pixel
RGBTRIPLE triple;
triple.rgbtBlue = 0x00;
triple.rgbtGreen = 0xff;
triple.rgbtRed = 0x00;

if (triple.rgbtBlue == 0xff)
{
    printf("I'm feeling blue!");
}

triple.rgbtGreen = 0x00;
triple.rgbtRed = 0x00;

*/