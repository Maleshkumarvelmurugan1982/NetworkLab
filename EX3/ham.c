#include <stdio.h>
#include <stdlib.h>

int isPowerOfTwo(int x) { return x && !(x & (x - 1)); }

int calculateParityBits(int k) {
    int r = 0;
    while ((1 << r) < (k + r + 1)) r++;
    return r;
}

int main() {
    int k, r, n, *hamming, *data_bits, *received, error_pos = 0;
    printf("Enter number of data bits: ");
    scanf("%d", &k);
    r = calculateParityBits(k);
    n = k + r;
    hamming = calloc(n + 1, sizeof(int));
    data_bits = malloc(k * sizeof(int));
    printf("Enter %d data bits (MSB to LSB):\n", k);
    for (int i = 0; i < k; i++) scanf("%d", &data_bits[i]);
    for (int i = 1, j = k - 1; i <= n; i++)
        if (!isPowerOfTwo(i)) hamming[i] = data_bits[j--];
    for (int i = 0; i < r; i++) {
        int p = 1 << i, parity = 0;
        for (int j = 1; j <= n; j++)
            if (j & p) parity ^= hamming[j];
        hamming[p] = parity;
    }
    printf("Encoded Hamming code: ");
    for (int i = n; i >= 1; i--) printf("%d", hamming[i]);
    printf("\nEnter received %d bits (MSB to LSB):\n", n);
    received = calloc(n + 1, sizeof(int));
    for (int i = 0; i < n; i++) {
        int bit; scanf("%d", &bit);
        received[n - i] = bit;
    }
    for (int i = 0; i < r; i++) {
        int p = 1 << i, parity = 0;
        for (int j = 1; j <= n; j++)
            if (j & p) parity ^= received[j];
        if (parity) error_pos += p;
    }
    if (error_pos == 0) printf("No error detected.\n");
    else {
        printf("Error detected at bit position: %d\n", error_pos);
        received[error_pos] ^= 1;
        printf("Corrected data: ");
        for (int i = n; i >= 1; i--) printf("%d", received[i]);
        printf("\n");
    }
    free(hamming); free(data_bits); free(received);
    return 0;
}
