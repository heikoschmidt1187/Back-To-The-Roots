#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>

/* https://datatracker.ietf.org/doc/html/rfc1320 */

#define F(X, Y, Z) ((X & Y) | (~X & Z))
#define G(X, Y, Z) ((X & Y) | (X & Z) | (Y & Z))
#define H(X, Y, Z) (X ^ Y ^ Z)

#define ROT(X, l, r) ((X << l) | (X >> r))
#define ROTL(X, n) ROT(X, n, (32 - n))
#define ROTR(X, n) ROT(X, (32 - n), n)

#define MD4_ROUND1(a, b, c, d, k, s) (a = ROTL((a + F(b, c, d) + k), s));
#define MD4_ROUND2(a, b, c, d, k, s) (a = ROTL((a + G(b, c, d) + k + 0x5A827999U), s))
#define MD4_ROUND3(a, b, c, d, k, s) (a = ROTL((a + H(b, c, d) + k + 0x6ED9EBA1U), s))

typedef struct
{
    uint64_t byte_len;
    uint32_t W[4];

} Md4Context;

static void Md4_Init(Md4Context *const ctx);
static void Md4_Process(Md4Context *const ctx, const uint32_t data[16]);

void Md4(char const *const msg)
{
    if (!msg)
        return;

    Md4Context ctx;
    Md4_Init(&ctx);

    /* process 512 bit (64 byte) chunks of data */
    size_t rem_bytes = strlen(msg);

    while (true)
    {
        if (rem_bytes >= 64)
        {
            /* enough data */
            Md4_Process(&ctx, (uint32_t *)(&msg[ctx.byte_len]));
            ctx.byte_len += 64;
            rem_bytes -= 64;
        }
        else
        {
            /* need padding at the end */
            uint8_t data[64];
            memset(data, 0, sizeof(data));

            if (rem_bytes > 0)
            {
                memcpy((void *)&data[0], (void *)&msg[ctx.byte_len], rem_bytes * sizeof(uint8_t));
                ctx.byte_len += rem_bytes;
            }

            /* pad a 1 bit - it's guaranteed to fit */
            data[rem_bytes] = 0x80U;

            if (rem_bytes + 1 >= 56)
            {
                /* length doesn't fit, process zero padded */
                Md4_Process(&ctx, (uint32_t *)data);
                memset(data, 0, sizeof(data));
            }
            /* add length and process */
            uint64_t len = ctx.byte_len * 8U;
            memcpy((void *)&data[56], (void *)&len, sizeof(len));
            Md4_Process(&ctx, (uint32_t *)data);
            break;
        }
    }

    printf("MD4 for \"%s\" is: ", msg);
    for (uint8_t i = 0; i < 4; ++i)
        printf("%08x", htonl(ctx.W[i]));
    printf("\n");
}

static void Md4_Init(Md4Context *const ctx)
{
    if (!ctx)
        return;

    ctx->byte_len = 0U;
    ctx->W[0] = 0x67452301U;
    ctx->W[1] = 0xefcdab89U;
    ctx->W[2] = 0x98badcfeU;
    ctx->W[3] = 0x10325476U;
}

static void Md4_Process(Md4Context *const ctx, const uint32_t data[16])
{
    if (!ctx)
        return;

    uint32_t a = ctx->W[0];
    uint32_t b = ctx->W[1];
    uint32_t c = ctx->W[2];
    uint32_t d = ctx->W[3];

    /* round 1 */
    MD4_ROUND1(a, b, c, d, data[0], 3);
    MD4_ROUND1(d, a, b, c, data[1], 7);
    MD4_ROUND1(c, d, a, b, data[2], 11);
    MD4_ROUND1(b, c, d, a, data[3], 19);

    MD4_ROUND1(a, b, c, d, data[4], 3);
    MD4_ROUND1(d, a, b, c, data[5], 7);
    MD4_ROUND1(c, d, a, b, data[6], 11);
    MD4_ROUND1(b, c, d, a, data[7], 19);

    MD4_ROUND1(a, b, c, d, data[8], 3);
    MD4_ROUND1(d, a, b, c, data[9], 7);
    MD4_ROUND1(c, d, a, b, data[10], 11);
    MD4_ROUND1(b, c, d, a, data[11], 19);

    MD4_ROUND1(a, b, c, d, data[12], 3);
    MD4_ROUND1(d, a, b, c, data[13], 7);
    MD4_ROUND1(c, d, a, b, data[14], 11);
    MD4_ROUND1(b, c, d, a, data[15], 19);

    /* round 2 */
    MD4_ROUND2(a, b, c, d, data[0], 3);
    MD4_ROUND2(d, a, b, c, data[4], 5);
    MD4_ROUND2(c, d, a, b, data[8], 9);
    MD4_ROUND2(b, c, d, a, data[12], 13);

    MD4_ROUND2(a, b, c, d, data[1], 3);
    MD4_ROUND2(d, a, b, c, data[5], 5);
    MD4_ROUND2(c, d, a, b, data[9], 9);
    MD4_ROUND2(b, c, d, a, data[13], 13);

    MD4_ROUND2(a, b, c, d, data[2], 3);
    MD4_ROUND2(d, a, b, c, data[6], 5);
    MD4_ROUND2(c, d, a, b, data[10], 9);
    MD4_ROUND2(b, c, d, a, data[14], 13);

    MD4_ROUND2(a, b, c, d, data[3], 3);
    MD4_ROUND2(d, a, b, c, data[7], 5);
    MD4_ROUND2(c, d, a, b, data[11], 9);
    MD4_ROUND2(b, c, d, a, data[15], 13);

    /* round 3 */
    MD4_ROUND3(a, b, c, d, data[0], 3);
    MD4_ROUND3(d, a, b, c, data[8], 9);
    MD4_ROUND3(c, d, a, b, data[4], 11);
    MD4_ROUND3(b, c, d, a, data[12], 15);

    MD4_ROUND3(a, b, c, d, data[2], 3);
    MD4_ROUND3(d, a, b, c, data[10], 9);
    MD4_ROUND3(c, d, a, b, data[6], 11);
    MD4_ROUND3(b, c, d, a, data[14], 15);

    MD4_ROUND3(a, b, c, d, data[1], 3);
    MD4_ROUND3(d, a, b, c, data[9], 9);
    MD4_ROUND3(c, d, a, b, data[5], 11);
    MD4_ROUND3(b, c, d, a, data[13], 15);

    MD4_ROUND3(a, b, c, d, data[3], 3);
    MD4_ROUND3(d, a, b, c, data[11], 9);
    MD4_ROUND3(c, d, a, b, data[7], 11);
    MD4_ROUND3(b, c, d, a, data[15], 15);

    ctx->W[0] += a;
    ctx->W[1] += b;
    ctx->W[2] += c;
    ctx->W[3] += d;
}
