#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("usage: squares rows cols");
        return 1;
    }

    int row = atoi(argv[1]);
    int col = atoi(argv[2]);

    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            printf("0");
        }
        printf("\n");
    }




}