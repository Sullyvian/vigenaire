#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char letter(int r) {
    return (r + 26) % 26 + 'A';
}

int rank(char a) {
    return (a - 'A' + 26) % 26;
}

char encryptLetter(char a, char k) {
    return letter(((rank(a)) + rank(k)));
}

char decryptLetter(char b, char k) {
    return letter((rank(b) - rank(k)));
}

int isLetter(char a) {
    return ('A' <= a) && (a <= 'Z');
}

void printUsage() {
    printf("USAGE:\n");
    printf("    ./vigenaire [-d] [-e] <key> <input>\n\n");
    printf("     -d   decrypt the given file\n\n");
    printf("     -e   encrypt the given file\n\n");
    printf(
        "  <key>   Key used to translate the message. It should be a capital "
        "letter string without any space.\n\n");
    printf(
        "<input>   Text file to decrypt/encrypt. Only capital letters will be "
        "translated.");
}

int getFileSize(FILE *file) {
    fseek(file, 0L, SEEK_END);  // go to end
    int size = ftell(file);     // get current file position
    rewind(file);               // go back to start of file
    return size;
}

int getStringSize(char *s) {
    int n = 0;
    for (int i = 0; s[i] != '/0'; i++) {
        n++;
        if (!isLetter(s[i])) {
            printf("KEY_ERROR : Key should only be composed of capital unaccentuated letters");
            printUsage();
            exit(EXIT_FAILURE);
        }
    }
    return n;
}

void translate(char *msg, const char *key, char (*convert)(char, char)) {
    int keyLength = getStringSize(key);
    int j = 0;
    for (int i = 0; msg[i] != '\0'; i++) {
        if (isLetter(msg[i])) {
            char k = key[j % keyLength];
            msg[i] = (*convert)(msg[i], k);
            j++;
        }
    }
}

void main(int argc, char *argv[]) {
    // first arg : encrypt or decrypt option
    char (*convert)(char, char);
    if (!strcmp(argv[1], "-d")) {
        convert = &decryptLetter;
        // printf("DÉCODAGE");
    } else if (!strcmp(argv[1], "-e")) {
        convert = &encryptLetter;
        // printf("CODAGE");
    } else {
        printUsage();
        exit(EXIT_FAILURE);
    }

    // second arg : key
    const char *key = argv[2];

    // third arg : input text
    FILE *fileIn = fopen(argv[3], "r");
    if (fileIn == NULL) {
        printf("'%s' does not exist\n", argv[3]);
        exit(EXIT_FAILURE);
    }

    const int fileSize = getFileSize(fileIn);
    char *msg = malloc(fileSize + 1);

    fread(msg, 1, fileSize, fileIn);
    msg[fileSize] = '\0';
    fclose(fileIn);

    // encryption / decryption
    translate(msg, key, convert);

    // writing in output file
    FILE *fileOut = fopen("output", "w");
    fprintf(fileOut, "%s", msg);
    fclose(fileOut);
    free(msg);
    msg = NULL;
}