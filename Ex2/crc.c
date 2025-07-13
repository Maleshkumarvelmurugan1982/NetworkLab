#include <stdio.h>
#include <string.h>
#define MAX 100
void xor_div(char *data, char *gen, char *rem) {
    int dlen = strlen(data), glen = strlen(gen);
    char temp[MAX];
    strncpy(temp, data, glen); temp[glen] = '\0';
    for (int i = 0; i <= dlen - glen; i++) {
        if (temp[0] == '1')
            for (int j = 1; j < glen; j++)
                temp[j - 1] = (temp[j] == gen[j] ? '0' : '1');
        else
            for (int j = 1; j < glen; j++)
                temp[j - 1] = temp[j];
        temp[glen - 1] = data[glen + i];
    }
    strncpy(rem, temp, glen - 1); rem[glen - 1] = '\0';
}
void sender() {
    char data[MAX], gen[MAX], app_data[MAX], crc[MAX];
    printf("Enter data bits: "); scanf("%s", data);
    printf("Enter generator: "); scanf("%s", gen);
    strcpy(app_data, data);
    for (int i = 0; i < strlen(gen) - 1; i++) strcat(app_data, "0");
    xor_div(app_data, gen, crc);
    strcat(data, crc);
    printf("CRC: %s\nTransmitted data: %s\n", crc, data);
}
void receiver() {
    char recv[MAX], gen[MAX], rem[MAX];
    printf("Enter received data: "); scanf("%s", recv);
    printf("Enter generator: "); scanf("%s", gen);
    xor_div(recv, gen, rem);
    if (strchr(rem, '1'))
        printf("Error detected in received data!\n");
    else
        printf("No error. Data received correctly.\n");
}
int main() {
    int choice;
    printf("1. Sender\n2. Receiver\nEnter choice: ");
    scanf("%d", &choice);
    (choice == 1) ? sender() : (choice == 2) ? receiver() : printf("Invalid choice.\n");
    return 0;
}

