#include <stdio.h>
#include <stdint.h>
#include <string.h>

void pc1_256_encrypt_block(const uint8_t *key, const uint8_t *plain, uint8_t *cipher, size_t block_length);
void pc1_256_decrypt_block(const uint8_t *key, const uint8_t *cipher, uint8_t *plain, size_t block_length);

void printarray(const uint8_t *key, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("%d ", *(key + i));
        if (i == length - 1)
        {
            printf("\n");
        }
    }
}

int main()
{
    uint8_t cle[32];
    memccpy(&cle, &"0123456789ABCDEF0123456789ABCDEF", 0, sizeof(cle));

    uint8_t plain[5];
    uint8_t cipher[5];
    memccpy(&plain, &"hello", 0, sizeof(plain));
    printarray((const uint8_t *)&plain, sizeof(plain));

    pc1_256_encrypt_block((const uint8_t *)&cle, (const uint8_t *)&plain, (uint8_t *)&cipher, sizeof(plain));
    printarray((const uint8_t *)&cipher, sizeof(cipher));

    pc1_256_decrypt_block((const uint8_t *)&cle, (const uint8_t *)&cipher, (uint8_t *)&plain, sizeof(plain));
    printarray((const uint8_t *)&plain, sizeof(plain));
}