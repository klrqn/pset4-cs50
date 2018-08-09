#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        printf("usage: squares rows cols scale\n");
        return 1;
    }

    int row = atoi(argv[1]);
    int col = atoi(argv[2]);
    int scale = atoi(argv[3]);

    // make sure rows and columns add up to an even number.
    for (int i = 0; i < row * scale + (row * scale % 2); i++)
    {
        for (int j = 0; j < col * scale + (col * scale % 2); j++)
        {
            printf(" 0 ");
        }

        printf("\n");
    }




}