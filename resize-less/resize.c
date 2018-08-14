#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    // make sure resize is under 100
    // uint8_t
    int num = atoi(argv[1]);
    // printf("n = %i\n", num);
    if (num > 100 || num < 1)
    {
        fprintf(stderr, "n must be a positive integer less than 100\n");
        return 2;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

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

    // init new headers and set to original .bmp
    BITMAPFILEHEADER new_bf;
    new_bf = bf;
    BITMAPINFOHEADER new_bi;
    new_bi = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // get the padding of the infile image
    int old_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // increase the height, witdth, and padding of the header info for the outfile
    new_bi.biHeight = bi.biHeight * num;
    new_bi.biWidth = bi.biWidth * num;
    int new_padding = (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE)) + new_padding) * abs(bi.biHeight);
    // bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // out_bi.biSizeImage = abs(out_bi.biHeight) * (out_bi.biWidth * sizeof(RGBTRIPLE) + out_padding);
    // out_bf.bfSize = (out_bi.biSizeImage + 54);

    // :( resizes small.bmp correctly when n is 2
    //     Header field bfSize doesn't match. Expected 0xae, not 0xb4 - 174 / 180 = 6
    // :( resizes small.bmp correctly when n is 3                                           + 12
    //     Header field bfSize doesn't match. Expected 0x132, not 0x144 - 306 / 324 = 18
    // :( resizes small.bmp correctly when n is 4                                           + 18
    //     Header field bfSize doesn't match. Expected 0x1e6, not 0x20a - 486 / 522 = 36
    // biSizeImage - total size of image (in bytes) including pixels and padding 3*4 / 12
    new_bi.biSizeImage = ((sizeof(RGBTRIPLE) * new_bi.biWidth) + new_padding) * abs(new_bi.biHeight);

    // bfSize - total size of file (in bytes) - pixels, padding headers
    new_bf.bfSize = new_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < abs(bi.biHeight); i++)
    {
        // by the scale provided
        for (int n = 0; n < num; n++)
        {
            // iterate over pixels in scanline
            for (int k = 0; k < abs(new_bi.biHeight) + new_padding; k++)
            {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int l = 0; l < num; l++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }

            // move cursor of input file back "n" num times
            if (n < num - 1)
            {
                fseek(inptr, -(bi.biWidth * (int)sizeof(RGBTRIPLE)), SEEK_CUR);
            }
        }
        // skip over padding if any
        fseek(inptr, old_padding, SEEK_CUR);
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

/*

TODO

- open file
- update outfile's header info
- read infile's scanline pixel by pixel
- resize horizontially
    - remember padding!
- resize vertically

UPDATE HEADER


// biWidth     - width of image(in pixels) not including padding
// biHeight    - height of image (pixels)
// biSizeImage - total size of image (in bytes) including pixels and padding 3*4 / 12
// bfSize      - total size of file (in bytes) - pixels, padding headers
// bf.bfSize   - bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)

old - bi.biWidth        new - bi.biwidth *= n
old - bi.biHeight       new - bi.biHeight *= n
old - bi.biSizeImage    new - bi.biSizeImage ???n
old - bf.bfSize         new - bf.bfSize ???

*/