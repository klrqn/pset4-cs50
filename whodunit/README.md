# Questions

## What's `stdint.h`?

stdint.h is a header file in the C standard library introduced in the C99 standard library section 7.18 to allow programmers to write
more portable code by providing a set of typedefs that specify ===exact-width integer types===, together with the defined minimum and maximum
allowable values for each type, using macros .

The naming convention for exact-width integer types is intN_t for signed int and uintN_t for unsigned int [1] . For example int8_t
and uint64_t amongst others could be declared together with defining their corresponding ranges INT8_MIN to INT8_MAX and 0 (zero) to UINT64_MAX

---

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Using well-defined types makes the code far easier and safer to port, as you won't get any surprises when for example one machine
interprets int as 16-bit and another as 32-bit. With stdint.h, what you type is what you get. Using int etc also makes it hard to
detect dangerous type promotions.

Another advantage is that by using int8_t instead of char, you know that you always get a signed 8 bit variable. char can be signed
or unsigned, it is implementation-defined behavior and varies between compilers. Therefore, the default char is plain dangerous to
use in code that should be portable.

https://stackoverflow.com/questions/9834747/reasons-to-use-or-not-stdint

---

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

`BYTE`  = 8     = 1 byte
`DWORD` = 32    = 4 bytes
`LONG`  = 32    = 4 bytes
`WORD`  = 16    = 2 bytes

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

---

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats
## (with high probability) are generally called "magic numbers."

0x424d - hexadecimal
16973  - decimal
BM     - ASCII

see bfType

---

## What's the difference between `bfSize` and `biSize`?

`bfSize` The size, in bytes, of the bitmap file.
`biSize` The number of bytes required by the structure...?

biSize > The number of bytes required by the structure. (what is the structure exactly ?)
    The structure is the struct BITMAPINFOHEADER. That is a fixed value.

biSizeImage > The size, in bytes, of the image. bfSize > The size, in bytes, of the bitmap file. (what is the difference between image & bitmap file ?)
    biSizeImage is the whole image size, bfSize is the same, but you have to add the size of the 2 header files.

---

## What does it mean if `biHeight` is negative?

The height of the bitmap, in pixels. If biHeight is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner.
If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.

---

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

if the file isn't a bitmap, doesn't start with BM?

---

## Why is the third argument to `fread` always `1` in our code?

TODO

because we are reading a constant size, tagBITMAPFILEHEADER & BITMAPINFOHEADER are const

---

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

1 sizeof(RGBTRIPLE) = 3 bytes

---

## What does `fseek` do?

// skip over padding, if any
fseek   -   allows you to rewind or fast-forward within a file

---

## What is `SEEK_CUR`?

Specifies that offset is a count of characters from the current file position. This count may be positive or negative.

## Whodunit?

TODO
