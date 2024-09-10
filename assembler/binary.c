#include <stdio.h>


/* This is helper file for binary array functionality
Each machine instruction in hack computer is 16-bit, and so we handle only int[16] binary arrays
*/


void int_to_binary(int num, int* binary) {
    for(int i=0;i<16;i++) {
        binary[i] = num%2;
        num = num/2;
    }
}


void print_binary(int* binary) {
    for(int i=15;i>=0;i--) {
        printf("%d", binary[i]);
    }
    printf("\n");
}


void binary_to_str(int* binary, char* binary_str) {
    // char array needs to have a bit extra for string termination, min length = 17 bytes

    for(int i=0;i<16;i++) {
        binary_str[i] = '0' + binary[15-i];
    }
    binary_str[16] = '\0';
}


void int_to_binary_str(int num, char* binary_str) {
    int binary[16];
    int_to_binary(num, binary);
    binary_to_str(binary, binary_str);
}

