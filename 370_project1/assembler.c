/**
 * Project 1
 * Assembler code fragment for LC-2K
 */
// read in label
// search array
// mask neg
// return opcode number
// if .fill
// if halt or noop
// if add or nor
// else and if beq
// according appends
// write to file

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
struct label{
    int address;
    char name[7];
};
int search_array( struct label arr[] , int , char *);
int mask_neg(int);
int opcode_num(char *);

int
main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        /* reached end of file */
    }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    if (!strcmp(opcode, "add")) {
        /* do whatever you need to do for opcode "add" */
    }
    
   
    struct label v1[100];
    int arr_size= 0;   // index in array i.e arra
    int i = 0;   // line #
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){ // read labels and addresses in to array
        if(*label){
            v1[arr_size].address = i;
            strcpy(v1[arr_size].name, label);;
            fprintf(outFilePtr, "label is %s\n", v1[arr_size].name);
            fprintf(outFilePtr, "address is %d\n", v1[arr_size].address);
            arr_size++; }
        i++; }
    /*
  
    rewind(inFilePtr);
    int temp = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if(isNumber(arg2) != 1 && strcmp(opcode,".fill")){
            temp = search_array(v1, arr_size, arg2);
            fprintf(outFilePtr, "label is %s\n", arg2);
            fprintf(outFilePtr, "the fuck is %d\n", temp);
        } }
     */
       

    return(0);
}
// My helper functions

int search_array( struct label arr[], int size , char * label){
    int i = 0;
    int address = 0;
    while (i <= size){
        if(!strcmp(label,arr[i].name)){
            address = arr[i].address;
            return address;
        }
        i++;
    }
    return - 1;  //later change to exit
}
int mask_neg(int arg){
    int num = arg & 7;
    return num;
}

int opcode_num(char * opcode){
    int num = -1;
    if(!strcmp(opcode,"add")){
        num = 0; }
    if(!strcmp(opcode,"nor")){
        num = 1; }
    if(!strcmp(opcode,"lw")){
        num = 2; }
    if(!strcmp(opcode,"sw")){
        num = 3; }
    if(!strcmp(opcode,"beq")){
        num = 4; }
    if(!strcmp(opcode,"jalr")){
        num = 5; }
    if(!strcmp(opcode,"halt")){
        num = 6; }
    if(!strcmp(opcode,"noop")){
        num = 7; }
    return num;
}
/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if successfully read
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
        char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }

    /* is there a label? */
    char *ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int
isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}


