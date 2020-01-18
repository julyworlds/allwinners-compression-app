#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "az100_i.h"

int main(int argc, char **argv)
{
    if (argc > 3) {
        char* method;
        FILE* inFile;
        FILE* outFile;
        size_t file_len;
        size_t az100_len;
        size_t ioResult;
        int result;
        void* inBuffer;
        void* outBuffer;
        void* outBuffer2;

        method = argv[1];
        inFile = fopen((const char*)argv[2], "rb");
        outFile = fopen((const char*)argv[3], "wb");
        if (inFile == NULL || outFile == NULL) {
            printf("can't open file!\n");
            return 1;
        }
        fseek(inFile, 0, SEEK_END);
        file_len = ftell(inFile);
        // reset pointer
        fseek(inFile, 0, SEEK_SET);
        inBuffer = malloc(file_len);
        // read input file
        ioResult = fread(inBuffer, 1, file_len, inFile);
        if (ioResult != file_len) {
            printf("can't read file\n");
            return 1;
        }
        if (strcmp(method, "d") == 0) {
            // check if file is compressed
            result = AZ100_IsCompress(inBuffer, file_len);
            if (result != 1) {
                printf("file is not compressed!\n");
                return 1;
            }
            printf("file is compressed!\n");
            az100_len = AZ100_GetUncompressSize(inBuffer, file_len);
            printf("decompressing... %ld bytes to %ld bytes\n", file_len, az100_len);
            outBuffer = malloc(az100_len);
            result = AZ100_Uncompress(inBuffer, file_len, outBuffer, az100_len);
            if (result != 0) {
                printf("failed to uncompress!\n");
                return 1;
            }
            ioResult = fwrite(outBuffer, 1, az100_len, outFile);
            if (ioResult != az100_len) {
                printf("can't write file\n");
                return 1;
            }
            printf("done!\n");
            fclose(inFile);
            fclose(outFile);
            return 0;
        } else if (strcmp(method, "c") == 0) {
            printf("compressing... %ld bytes\n", file_len);
            outBuffer = malloc(file_len);
            memset(outBuffer, 0, file_len);
            outBuffer2 = malloc(file_len);
            memset(outBuffer2, 0, file_len);
            result = AZ100_Compress(inBuffer, file_len, outBuffer, &az100_len);
            if (result != 0) {
                printf("failed to compress!\n");
                return 1;
            }
            printf("size compressed: %ld bytes\n", az100_len);
            ioResult = fwrite(outBuffer, 1,
                              az100_len + AZ100_HEADER_LEN + 5, outFile);
            if (ioResult != az100_len + AZ100_HEADER_LEN + 5) {
                printf("can't write file\n");
                return 1;
            }
            printf("done!\n");
            fclose(inFile);
            fclose(outFile);
            return 0;
        }
        printf("invalid option!\n");
        return 1;
    }
    printf("invalid params!\n");
    return 1;
}
