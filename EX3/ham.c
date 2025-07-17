#include <stdio.h>

int main() {
    int data[12] = {0};
    int i, c1, c2, c3, c4, error_pos;

    int data_bits_pos[7] = {11,10,9,7,6,5,3};
    for(i = 0; i < 7; i++) {
        scanf("%d", &data[data_bits_pos[i]]);
    }

    c1 = data[1] ^ data[3] ^ data[5] ^ data[7] ^ data[9] ^ data[11];
    data[1] = c1;

    c2 = data[2] ^ data[3] ^ data[6] ^ data[7] ^ data[10] ^ data[11];
    data[2] = c2;

    c3 = data[4] ^ data[5] ^ data[6] ^ data[7];
    data[4] = c3;

    c4 = data[8] ^ data[9] ^ data[10] ^ data[11];
    data[8] = c4;

    printf("Encoded data: ");
    for(i = 11; i >= 1; i--) {
        printf("%d", data[i]);
    }
    printf("\n");

    int received[12] = {0};
    printf("Enter received 11 bits (bit 11 to bit 1):\n");
    for(i = 11; i >= 1; i--) {
        scanf("%d", &received[i]);
    }

    c1 = received[1] ^ received[3] ^ received[5] ^ received[7] ^ received[9] ^ received[11];
    c2 = received[2] ^ received[3] ^ received[6] ^ received[7] ^ received[10] ^ received[11];
    c3 = received[4] ^ received[5] ^ received[6] ^ received[7];
    c4 = received[8] ^ received[9] ^ received[10] ^ received[11];

    error_pos = c4 * 8 + c3 * 4 + c2 * 2 + c1;

    if(error_pos == 0) {
        printf("No error detected in received data.\n");
    } else {
        printf("Error detected at bit position: %d (from right)\n", error_pos);
        received[error_pos] = 1 - received[error_pos];

        printf("Corrected data: ");
        for(i = 11; i >= 1; i--) {
            printf("%d", received[i]);
        }
        printf("\n");
    }

    return 0;
}
