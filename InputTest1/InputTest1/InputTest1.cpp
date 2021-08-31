#include <iostream>
#include <conio.h>
#include "zlib/zlib.h"
#include <cstdio>
#include <string>
#include <ctime>
#include <windows.h>

using namespace std;
/*
struct saveFile
{
    ULONG compressedSize;   // size of the data array, compressed
    ULONG uncompressedSize; // size when data gets uncompressed
    char what[16];          // what is this file?  may want to
                            // indicate what kind of data is saved here.

    // add whatever other info you need
    // here.

    BYTE* data;            // the actual compressed data
};
*/ 
int main()
{ /*
#pragma region _make some data to compress_
        int sizeDataOriginal = 6 * NUM_WORDS_TO_USE + 1;
        BYTE* dataOriginal = (BYTE*)malloc(sizeDataOriginal);
        dataOriginal[sizeDataOriginal - 1] = NULL; // null terminator.
#pragma endregion

#pragma region compress the data
        ULONG sizeDataCompressed = (sizeDataOriginal * 1.1) + 12;
        BYTE* dataCompressed = (BYTE*)malloc(sizeDataCompressed);
        int z_result = compress(

            dataCompressed,         // destination buffer,
                                    // must be at least
                                    // (1.01X + 12) bytes as large
                                    // as source.. we made it 1.1X + 12bytes

            &sizeDataCompressed,    // pointer to var containing
                                    // the current size of the
                                    // destination buffer.
                                    // WHEN this function completes,
                                    // this var will be updated to
                                    // contain the NEW size of the
                                    // compressed data in bytes.

            dataOriginal,           // source data for compression

            sizeDataOriginal);    // size of source data in bytes

        switch (z_result)
        {
        case Z_OK:
            printf("***** SUCCESS! *****\n");
            break;

        case Z_MEM_ERROR:
            printf("out of memory\n");
            exit(1);    // quit.
            break;

        case Z_BUF_ERROR:
            printf("output buffer wasn't large enough!\n");
            exit(1);    // quit.
            break;
        }

        printf("*******************************\n");
        printf("* DATA COMPRESSION COMPLETE!! :\n");
        printf("*\n");
        printf("* Compressed size is %d bytes\n", sizeDataCompressed);
        printf("* This is what it looks like:\n\n--\n");
#pragma endregion

#pragma region save compressed data to disk
        printf("********************************\n");
        printf("* Saving compressed data to disk\n\n");
        FILE* out = fopen("savedData.dat", "wb");
 
            if (out == NULL)
            {
                printf("Couldn't open output file for writing\n");
                exit(1);    //quit
            }
            fwrite(dataCompressed, sizeDataCompressed, 1, out);
            fclose(out);
            out = NULL;
#pragma endregion

#pragma region read in data from disk
        printf("********************************\n");
        printf("* Reading in data from save file\n");

        FILE* readFile = fopen("savedData.dat", "rb");
        if (readFile == NULL)
        {
            printf("Couldn't open input file for reading\n");
            exit(1);    //quit
        }

        fseek(readFile, 0, SEEK_END);
        ULONG fileLength = ftell(readFile);
        rewind(readFile);
        BYTE* dataReadInCompressed = (BYTE*)malloc(fileLength);

        fread(dataReadInCompressed, fileLength, 1, readFile);

        fclose(readFile);
        readFile = NULL;

        printf("*\n* This is what I read from the saved file:\n");
        printf("\n--\n");
        if (showOutput)
        {
            for (int i = 0; i < fileLength; i++)
            {
                putchar(dataReadInCompressed[i]);
            }
            printf("\n--\n\n");
        }
        else
            printf(" (suppressed)\n--\n\n");
#pragma endregion

#pragma region decompress the read-in data
        printf("*******************************\n");
        printf("* Decompressing your data . . .\n");
        ULONG sizeDataUncompressed = sizeDataOriginal;
        BYTE* dataUncompressed = (BYTE*)malloc(sizeDataUncompressed);

        //////////////
        // now uncompress
        z_result = uncompress(

            dataUncompressed,       // destination for the uncompressed
                                    // data.  This should be the size of
                                    // the original data, which you should
                                    // already know.

            &sizeDataUncompressed,  // length of destination (uncompressed)
                                    // buffer

            dataReadInCompressed,   // source buffer - the compressed data

            sizeDataCompressed);   // length of compressed data in bytes

        switch (z_result)
        {
        case Z_OK:
            printf("***** SUCCESS! *****\n");
            break;

        case Z_MEM_ERROR:
            printf("out of memory\n");
            exit(1);    // quit.
            break;

        case Z_BUF_ERROR:
            printf("output buffer wasn't large enough!\n");
            exit(1);    // quit.
            break;
        }


        printf("************************\n");
        printf("* Uncompressed size is %d bytes\n", sizeDataUncompressed);
        printf("* Your UNCOMPRESSED data looks like this:\n");

        printf("\n--\n");
        if (showOutput)
        {
            for (int i = 0; i < sizeDataUncompressed; i++)
            {
                putchar(dataUncompressed[i]);
            }
            printf("\n--\n\n");
        }
        else
            printf(" (suppressed)\n--\n\n");
#pragma endregion

#pragma region compare decompressed data with original data
        if (memcmp(dataOriginal, dataUncompressed, sizeDataOriginal) == 0)
        {
            printf("* SEE?  It was EXACTLY the same.\n");
        }
        else
        {
            printf("\n\n=====================================\n"
                "Oh. . . dear.  There is a problem.  The uncompressed data "
                "isn't exactly the same as the original data.  Your data "
                "may be corrupted.  WHOOPS!!\n"
                "Please make sure if that you are reading and writing "
                "any file i/o in BINARY MODE.");
        }
#pragma endregion

        free(dataOriginal);
        free(dataCompressed);
        free(dataReadInCompressed);
        free(dataUncompressed);
    system("PAUSE"); */

char a = 1;
UINT8 b = 2, c = 214748366;
char* d = &a;


    return 0;
}
