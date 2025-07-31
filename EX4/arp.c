#include <stdio.h>
#include <string.h>

#define MAX 100

struct ARPEntry {
    char ip[20];
    char mac[20];
};

int main() {
    struct ARPEntry table[MAX];
    int n;

    // Step 1 & 2: Get IP-MAC pairs from user
    printf("Enter number of IP-MAC entries: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nEntry %d:\n", i + 1);
        printf("  Enter IP address: ");
        scanf("%s", table[i].ip);
        printf("  Enter MAC (Ethernet) address: ");
        scanf("%s", table[i].mac);
    }

    int choice;
    int found;

    // Step 9: Menu to perform ARP, RARP or Exit
    while (1) {
        printf("\n--- ARP/RARP MENU ---\n");
        printf("1. Perform ARP (IP → MAC)\n");
        printf("2. Perform RARP (MAC → IP)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // Step 3: Get IP and perform ARP
                char ip_input[20];
                found = 0;
                printf("Enter IP address to find MAC: ");
                scanf("%s", ip_input);

                for (int i = 0; i < n; i++) {
                    if (strcmp(table[i].ip, ip_input) == 0) {
                        // Step 4: Display equivalent MAC
                        printf("Ethernet (MAC) address: %s\n", table[i].mac);
                        found = 1;
                        break;
                    }
                }
                // Step 5: If not found
                if (!found) {
                    printf("MAC address for IP %s not found.\n", ip_input);
                }
                break;
            }

            case 2: {
                // Step 6: Get MAC and perform RARP
                char mac_input[20];
                found = 0;
                printf("Enter MAC address to find IP: ");
                scanf("%s", mac_input);

                for (int i = 0; i < n; i++) {
                    if (strcmp(table[i].mac, mac_input) == 0) {
                        // Step 7: Display equivalent IP
                        printf("IP address: %s\n", table[i].ip);
                        found = 1;
                        break;
                    }
                }
                // Step 8: If not found
                if (!found) {
                    printf("IP address for MAC %s not found.\n", mac_input);
                }
                break;
            }

            case 3:
                // Step 10: Exit option
                printf("Exiting ARP/RARP simulation.\n");
                return 0;

            default:
                printf("Invalid choice. Please select 1, 2, or 3.\n");
        }
    }

    return 0;
}
