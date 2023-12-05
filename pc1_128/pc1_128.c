// #include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct State
{
    uint16_t ax;
    uint16_t bx;
    uint16_t cx;
    uint16_t dx;
    uint16_t si;
    uint16_t x1a2;
    uint16_t x1a0[8];
    uint16_t res;
    uint16_t i;
    uint16_t inter;
} S;

static void code(S *state);
static void assemble(uint8_t cle[17],  S *state);

void pc1_128_encrypt_block(const uint8_t *key, const uint8_t *plain, uint8_t *cipher, size_t block_length)
{
    S state;
    uint8_t cle[17]; /* les variables sont definies de facon globale */
    int16_t cfc = 0, cfd = 0, c = 0;

    state.si = 0;
    state.x1a2 = 0;
    state.i = 0;

    // copy key
    memccpy(cle, key, 0, sizeof(cle));

    for (size_t index = 0; index < block_length; index++)
    {
        c = *(plain + index);

        assemble(cle, &state);
        cfc = state.inter >> 8;
        cfd = state.inter & 255; /* cfc^cfd = random byte */

        /* K ZONE !!!!!!!!!!!!! */
        /* here the mix of c and cle[compte] is before the encryption of c */

        for (int compte = 0; compte <= 15; compte++)
        {
            /* we mix the plaintext byte with the key */
            cle[compte] = cle[compte] ^ c;
        }

        c = c ^ (cfc ^ cfd);
        *(cipher + index) = (uint8_t)c;
    }
}

void pc1_128_decrypt_block(const uint8_t *key, const uint8_t *cipher, uint8_t *plain, size_t block_length)
{
    struct State state;
    uint8_t cle[17]; /* les variables sont definies de facon globale */
    int16_t cfc = 0, cfd = 0, c = 0;

    state.si = 0;
    state.x1a2 = 0;
    state.i = 0;

    // copy key
    memccpy(cle, key, 0, sizeof(cle));

    for (size_t index = 0; index < block_length; index++)
    {
        c = *(cipher + index);

        assemble(cle, &state);
        cfc = state.inter >> 8;
        cfd = state.inter & 255; /* cfc^cfd = random byte */

        /* K ZONE !!!!!!!!!!!!! */
        /* here the mix of c and cle[compte] is after the decryption of c */

        c = c ^ (cfc ^ cfd);

        for (int compte = 0; compte <= 15; compte++)
        {
            /* we mix the plaintext byte with the key */
            cle[compte] = cle[compte] ^ c;
        }

        *(plain + index) = (uint8_t)c;
    }
}

static void code(S *state)
{
    int16_t tmp;

    (*state).dx = (*state).x1a2 + (*state).i;
    (*state).ax = (*state).x1a0[(*state).i];
    (*state).cx = 0x015a;
    (*state).bx = 0x4e35;

    tmp = (*state).ax;
    (*state).ax = (*state).si;
    (*state).si = tmp;

    tmp = (*state).ax;
    (*state).ax = (*state).dx;
    (*state).dx = tmp;

    if ((*state).ax != 0)
    {
        (*state).ax = (*state).ax * (*state).bx;
    }

    tmp = (*state).ax;
    (*state).ax = (*state).cx;
    (*state).cx = tmp;

    if ((*state).ax != 0)
    {
        (*state).ax = (*state).ax * (*state).si;
        (*state).cx = (*state).ax + (*state).cx;
    }

    tmp = (*state).ax;
    (*state).ax = (*state).si;
    (*state).si = tmp;
    (*state).ax = (*state).ax * (*state).bx;
    (*state).dx = (*state).cx + (*state).dx;

    (*state).ax = (*state).ax + 1;

    (*state).x1a2 = (*state).dx;
    (*state).x1a0[(*state).i] = (*state).ax;

    (*state).res = (*state).ax ^ (*state).dx;
    (*state).i = (*state).i + 1;
}

static void assemble(uint8_t cle[17], S *state)
{
    (*state).x1a0[0] = (cle[0] * 256) + cle[1];
    code(state);
    (*state).inter = (*state).res;

    (*state).x1a0[1] = (*state).x1a0[0] ^ ((cle[2] * 256) + cle[3]);
    code(state);
    (*state).inter = (*state).inter ^ (*state).res;

    (*state).x1a0[2] = (*state).x1a0[1] ^ ((cle[4] * 256) + cle[5]);
    code(state);
    (*state).inter = (*state).inter ^ (*state).res;

    (*state).x1a0[3] = (*state).x1a0[2] ^ ((cle[6] * 256) + cle[7]);
    code(state);
    (*state).inter = (*state).inter ^ (*state).res;

    (*state).x1a0[4] = (*state).x1a0[3] ^ ((cle[8] * 256) + cle[9]);
    code(state);
    (*state).inter = (*state).inter ^ (*state).res;

    (*state).x1a0[5] = (*state).x1a0[4] ^ ((cle[10] * 256) + cle[11]);
    code(state);
    (*state).inter = (*state).inter ^ (*state).res;

    (*state).x1a0[6] = (*state).x1a0[5] ^ ((cle[12] * 256) + cle[13]);
    code(state);
    (*state).inter = (*state).inter ^ (*state).res;

    (*state).x1a0[7] = (*state).x1a0[6] ^ ((cle[14] * 256) + cle[15]);
    code(state);
    (*state).inter = (*state).inter ^ (*state).res;

    (*state).i = 0;
}
