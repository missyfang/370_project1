/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

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
int check_offset(int);
int duplicate_label(struct label arr[], int );

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

    struct label v1[100];
    int arr_size= 0;   // index in array i.e arra
    int i = 0;   // line #
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){ // read labels and addresses in to array
        if(*label){
            v1[arr_size].address = i;
            strcpy(v1[arr_size].name, label);
           // fprintf(outFilePtr, "label is %s\n", v1[arr_size].name);
         //   fprintf(outFilePtr, "address is %d\n", v1[arr_size].address);
            arr_size++; }
        i++; }
   
    duplicate_label(v1, arr_size);    // check no duplicate label in array

    
    rewind(inFilePtr);
    int line_num = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        int decimal = 0;
        int reg_a = 0;
        int reg_b = 0;
        int label_ad = 0;
        int offset = 0;
        if(!strcmp(opcode,".fill")){                          // if .fill return arg0
            if(!isNumber(arg0)){
                decimal = search_array(v1, arr_size, arg0);
            }
            else{
            decimal = atoi(arg0);
            }
        }
        else if(!strcmp(opcode,"noop") || !strcmp(opcode,"halt")){   // if noop or jalt return opcode
            decimal = opcode_num(opcode) << 22;
        }
        else if(!strcmp(opcode,"jalr")){
            decimal = opcode_num(opcode) << 22;
            reg_a = atoi(arg0) << 19;
            reg_b = atoi(arg1) << 16;
            decimal = decimal | reg_a;
            decimal = decimal | reg_b;
        }
            
        else{
            decimal = opcode_num(opcode) << 22;
            reg_a = atoi(arg0) << 19;
            reg_b = atoi(arg1) << 16;
            decimal = decimal | reg_a;
            decimal = decimal | reg_b;
            if( !isNumber(arg2)){
                label_ad = search_array(v1, arr_size, arg2);    // search array for label
                if(!strcmp(opcode,"beq")){
                    offset = label_ad - line_num - 1;             // equation for beq offset
                }
                else{ offset = search_array(v1, arr_size, arg2);}   // if lw or ls set offset to lavel address
                if(offset < 0) {                                 // if offset is negative
                        offset = mask_neg(offset);                    // check in range and mask
                        decimal = decimal |offset;
                }
                else{
                   offset = check_offset(offset);                 // checking offset in range
                   decimal = decimal |offset;
                }
                }
            else if(!strcmp(opcode,"add") || !strcmp(opcode,"nor") ){                // if add or nor add arg2
                decimal = decimal | atoi(arg2);
            }
            else{   if(atoi(arg2) < 0){                             // if arg is a number
                    offset = mask_neg(atoi(arg2));
                    decimal = decimal |offset;
                }
                else{
                    offset = check_offset(atoi(arg2));
                    decimal = decimal | offset; }
            }
        }
            line_num ++;
            fprintf(outFilePtr,"%d\n", decimal);            // print decimalto file
    
        }
                    
    exit(0);
    
}
// My helper functions
// checks if duplicate labels exist in array
int duplicate_label(struct label arr[], int size){
    int i = 0;
    while (i <= size){
        int j = i+1;
        char * temp = arr[i].name;
        while (j <= size){
            char * temp2 = arr[j].name;
            if (!strcmp(temp2, temp)){
                printf("duplicate label");
                exit(1);
            }
            j++;
        }
        i++;
    }
        return 1;
}
// check offset in range and cuts off 16 most sig bits 32bit to 16bit int
int check_offset(int offset){
    int temp = 0;
    if(offset <= 32767 && -32768 <= offset ){
        temp = offset  << 16;
        offset = temp >> 16;
        return offset;
    }
    printf("opcode not in range");
    exit(1);
}
// searches the array for a given label and according address
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
    exit(1) ;
}
// checks offset in range and masks the neg
int mask_neg(int arg){
    check_offset(arg);
    int num = arg & 0xffff;
    return num;
}

// return correct number for opcode and exits is not opcode isnt recognized
int opcode_num(char * opcode){
    if(!strcmp(opcode,"add")){
        return 0; }
    if(!strcmp(opcode,"nor")){
        return 1; }
    if(!strcmp(opcode,"lw")){
        return 2; }
    if(!strcmp(opcode,"sw")){
        return 3; }
    if(!strcmp(opcode,"beq")){
        return 4; }
    if(!strcmp(opcode,"jalr")){
        return 5; }
    if(!strcmp(opcode,"halt")){
        return 6; }
    if(!strcmp(opcode,"noop")){
        return 7; }
   printf("invalid opcode"); 
    exit(1);
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


