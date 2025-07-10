#include <stdio.h>
#include <string.h>

#define MAX 100  // Maximum size for input strings

// Function to perform XOR division used in CRC
void xor_division(char *data, char *generator, char *remainder) {
    int data_len = strlen(data);         // Length of the data including appended 0s
    int gen_len = strlen(generator);     // Length of the generator polynomial
    char temp[MAX];                      // Temporary buffer to hold intermediate data

    // Initialize temp with the first gen_len bits of data
    strncpy(temp, data, gen_len);
    temp[gen_len] = '\0';

    // Perform division one bit at a time
    for (int i = 0; i <= data_len - gen_len; i++) {
        if (temp[0] == '1') {
            // XOR current bits with generator bits
            for (int j = 1; j < gen_len; j++) {
                temp[j - 1] = (temp[j] == generator[j] ? '0' : '1');
            }
        } else {
            // If first bit is 0, XOR with 0s (just shift)
            for (int j = 1; j < gen_len; j++) {
                temp[j - 1] = temp[j];
            }
        }
        // Add the next bit from data to temp
        temp[gen_len - 1] = data[gen_len + i];
    }

    // The remainder is the CRC
    strncpy(remainder, temp, gen_len - 1);
    remainder[gen_len - 1] = '\0';
}

// Sender side function
void crcSender() {
    char data[MAX], generator[MAX], appended_data[MAX], crc[MAX];
    int m;

    // Input data and generator polynomial
    printf("Enter data bits: ");
    scanf("%s", data);
    printf("Enter generator polynomial (in binary): ");
    scanf("%s", generator);

    // Degree of the generator polynomial (number of 0s to append)
    m = strlen(generator) - 1;

    // Copy original data and append m zeros for CRC calculation
    strcpy(appended_data, data);
    for (int i = 0; i < m; i++) {
        strcat(appended_data, "0");  // Append m zero bits
    }

    // Perform division to get CRC
    xor_division(appended_data, generator, crc);
    printf("CRC: %s\n", crc);

    // Append CRC to original data for transmission
    strcat(data, crc);
    printf("Transmitted data (data + CRC): %s\n", data);
}

// Receiver side function
void crcReceiver() {
    char received[MAX], generator[MAX], remainder[MAX];

    // Input received data and generator polynomial
    printf("Enter received data bits (data + CRC): ");
    scanf("%s", received);
    printf("Enter generator polynomial (in binary): ");
    scanf("%s", generator);

    // Perform division on received data
    xor_division(received, generator, remainder);

    // Check if the remainder contains only zeros
    int error = 0;
    for (int i = 0; i < strlen(remainder); i++) {
        if (remainder[i] != '0') {
            error = 1;
            break;
        }
    }

    // Display result based on remainder
    if (error) {
        printf("Error detected in received data!\n");
    } else {
        printf("No error. Data received correctly.\n");
    }
}

// Main function to provide menu-driven option
int main() {
    int choice;

    printf("1. CRC Sender\n");
    printf("2. CRC Receiver\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        crcSender();     // Call sender function
    } else if (choice == 2) {
        crcReceiver();   // Call receiver function
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}
