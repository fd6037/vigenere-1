// Jonathon Elfar

// vig.c
// Enciphers and deciphers text using a Vigenere cipher.
// usage: vig [ -v ] [ -d ] key [ infile [ outfile ] ]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#define NUMBER_OF_LETTERS_IN_ALPHABET 26

void print_usage() {
    fprintf(stderr, "usage: vig [ -v ] [ -d ] key [ infile [ outfile ] ]\n");
}

void print_open_output_file_error() {
    fprintf(stderr, "Failed to open up output file.\n");
}

void print_open_input_file_error() {
    fprintf(stderr, "Failed to open up input file.\n");
}

void decode(FILE *inputFp, FILE *outputFp, char *key) {
    char c;
    int cipherC, index = 0;
    while((c = getc(inputFp)) != EOF) {
        if(isalpha(c)) {
            index = index % strlen(key);
            cipherC = toupper(c);
            cipherC -= toupper(key[index]);
            cipherC %= NUMBER_OF_LETTERS_IN_ALPHABET;
            if(cipherC < 0) {
                cipherC += NUMBER_OF_LETTERS_IN_ALPHABET;
            }
            cipherC += 'A';
            index++;
        } else { //pass input through for non "A-Za-z"
            cipherC = c;
        }
        fprintf(outputFp, "%c", cipherC);
    }
}

void encode(FILE *inputFp, FILE *outputFp, char *key) {
    char c;
    int cipherC, index = 0;
    while((c = getc(inputFp)) != EOF) {
        if(isalpha(c)) {
            index = index % strlen(key);
            cipherC = toupper(key[index]);
            cipherC += toupper(c);
            cipherC %= NUMBER_OF_LETTERS_IN_ALPHABET;
            cipherC += 'A';
            index++;
        } else { //pass input through for non "A-Za-z"
            cipherC = c;
        }
        fprintf(outputFp, "%c", cipherC);
    }
}

 //opens files from argv or stdin/stdout by default
int openFilePointers(FILE **inputFp, char *inputFileName, FILE **outputFp,
                     char *outputFileName, int hasInputFiles) {
    if(hasInputFiles) {
        if(!inputFileName) {
            print_usage();
            return 3;
        }

        *inputFp = fopen(inputFileName, "r");
        if(!*inputFp) {
            print_open_input_file_error();
            return 3;
        }

        if(outputFileName) {
            *outputFp = fopen(outputFileName, "w");
            if(!*outputFp) {
                print_open_output_file_error();
                return 3;
            }
        } else {
            *outputFp = stdout;
        }

    } else {
        *inputFp = stdin;
        *outputFp = stdout;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char *key;
    int option = 0;
    FILE *inputFp, *outputFp;
    if(argc < 2)
    {
        print_usage();
        return 1;
    }
    
    int verbose = 0, decipher = 0;

    //The d flag performs a decoding rather than an encoding
    while ((option = getopt(argc, argv,"vd")) != -1) {
        switch (option) {
             case 'v' : 
                 verbose = 1;
                 break;
             case 'd' : 
                 decipher = 1;
                 break;
             //case 'l' : length = atoi(optarg); 
                 //break;
             //case 'b' : breadth = atoi(optarg);
                 //break;
             default: 
                 print_usage(); 
                 return 2;
        }
    }

    //Get our key
    key = argv[optind];
    int error = openFilePointers(&inputFp, argv[optind+1], 
                                 &outputFp, argv[optind+2], 
                                 optind+1 < argc);
    if(error) {
        fclose(inputFp);
        fclose(outputFp);
        return error;
    }

    if(decipher) {
        decode(inputFp, outputFp, key);
    } else {
        encode(inputFp, outputFp, key);
    }
    fclose(inputFp);
    fclose(outputFp);
    return 0;
}
