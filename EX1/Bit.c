#include <stdio.h>
#include <string.h>

#define MAX_LEN 1024

int main() {
    int choice;
    char bitData[MAX_LEN];
    int len, i, j, count;

    printf("Choose the operation:\n");
    printf("1. Bit Stuffing\n");
    printf("2. Bit Destuffing\n");
    printf("Enter your choice (1-2): ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:  // Bit Stuffing
            printf("Enter bit data (string of 0's and 1's): ");
            scanf("%s", bitData);
            len = strlen(bitData);

            // Bit Stuffing Logic
            j = 0;
            count = 0;
            char stuffed[MAX_LEN];

            for (i = 0; i < len; i++) {
                stuffed[j++] = bitData[i];  // Copy bit data
                if (bitData[i] == '1') {
                    count++;  // Count consecutive 1's
                } else {
                    count = 0;  // Reset on 0
                }

                if (count == 5) {  // After 5 consecutive 1's, stuff a 0
                    stuffed[j++] = '0';
                    count = 0;  // Reset count after stuffing
                }
            }
            stuffed[j] = '\0';  // Null-terminate the stuffed data
            printf("Stuffed data: %s\n", stuffed);
            break;

        case 2:  // Bit Destuffing
            printf("Enter stuffed bit data (string of 0's and 1's): ");
            scanf("%s", bitData);
            len = strlen(bitData);

            // Bit Destuffing Logic
            j = 0;
            count = 0;
            char destuffed[MAX_LEN];

            for (i = 0; i < len; i++) {
                destuffed[j++] = bitData[i];  // Copy bit data
                if (bitData[i] == '1') {
                    count++;  // Count consecutive 1's
                } else {
                    count = 0;  // Reset on 0
                }

                if (count == 5 && bitData[i + 1] == '0') {  // If 5 consecutive 1's are followed by 0, skip the 0
                    i++;  // Skip the stuffed '0'
                }
            }
            destuffed[j] = '\0';  // Null-terminate the destuffed data
            printf("Destuffed data: %s\n", destuffed);
            break;

        default:
            printf("Invalid choice!\n");
            break;
    }

    return 0;
}