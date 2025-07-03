#include <stdio.h>
#define MAX_SIZE 1000  // Maximum size for input string

int main() {
    char input[MAX_SIZE], output[2 * MAX_SIZE];  // Buffers for input and output
    char flag, esc;  // User-defined FLAG and ESC characters
    int choice;      // Menu choice

    // Read input string (including newline if user presses Enter)
    printf("Enter the input string: ");
    fgets(input, MAX_SIZE, stdin);

    // Note: The newline character at the end of input (if any) is NOT removed here

    // Read the FLAG character from user
    printf("Enter the FLAG character: ");
    scanf(" %c", &flag);  // Leading space to skip any leftover whitespace

    // Read the ESC character from user
    printf("Enter the ESC character: ");
    scanf(" %c", &esc);

    // Display menu options
    printf("\nMenu:\n");
    printf("1. Byte Stuffing\n");
    printf("2. Byte De-stuffing\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    int i, j;

    if (choice == 1) {
        // Byte Stuffing: Insert ESC before FLAG or ESC characters in input
        j = 0;  // Output index initialization
        for (i = 0; input[i] != '\0'; i++) {
            if (input[i] == flag || input[i] == esc) {
                output[j++] = esc;  // Insert ESC before special char
            }
            output[j++] = input[i];  // Copy current character
        }
        output[j] = '\0';  // Null-terminate output string

        printf("\nAfter Byte Stuffing: %s\n", output);

    } else if (choice == 2) {
        // Byte De-stuffing: Remove ESC characters before FLAG or ESC
        j = 0;  // Output index initialization
        for (i = 0; input[i] != '\0'; i++) {
            if (input[i] == esc) {
                i++;  // Skip ESC character and copy the next char directly
            }
            output[j++] = input[i];  // Copy current character
        }
        output[j] = '\0';  // Null-terminate output string

        printf("\nAfter Byte De-stuffing: %s\n", output);

    } else {
        // Invalid menu choice handling
        printf("Invalid choice! Please enter 1 or 2.\n");
    }

    return 0;
}
