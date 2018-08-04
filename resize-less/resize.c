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
    uint8_t num = atoi(argv[1]);
    // printf("n = %i\n", num);
    if (num > 100 || n < 2)
    {
        fprintf(stderr, "n must be less than 100\n");
        return 2;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
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

    // alter new files header for new size BELOW TODO
    // bfSize > The size, in bytes, of the bitmap file

    // biSizeImage > The size, in bytes, of the image.

    // bfSize = biSizeImage + bi.biSize (fixed) + bf.bfSize

    // change header values to understand updated size of new image

    // small bf.bfSize = 90
    // large bf.bfSize = 486
    // int paddingInfile = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biHeight *= num;
    bi.biWidth *= num;

    // updating padding for new width
    int paddingOutfile = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // old - bi.biSizeImage    new - bi.biSizeImage ???n
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + paddingOutfile) * abs(bi.biHeight);

    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight)*num; i < biHeight; i++)
    {
        //for factor times - num
        for (int f = 0; f < num; f++)
        {
            // temporary storage

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {

                RGBTRIPLE triple;
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // skip over padding, if any
        fseek(inptr, paddingOutfile, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < paddingOutfile; k++)
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