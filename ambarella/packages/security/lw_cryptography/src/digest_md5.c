/*******************************************************************************
 * digest_md5.c
 *
 * History:
 *  2015/06/25 - [Zhi He] create file
 *
 * Copyright (C) 2015 Ambarella, Inc.
 *
 * This file and its contents ("Software") are protected by intellectual
 * property rights including, without limitation, U.S. and/or foreign
 * copyrights. This Software is also the confidential and proprietary
 * information of Ambarella, Inc. and its licensors. You may not use, reproduce,
 * disclose, distribute, modify, or otherwise prepare derivative works of this
 * Software or any portion thereof except pursuant to a signed license agreement
 * or nondisclosure agreement with Ambarella, Inc. or its authorized affiliates.
 * In the absence of such an agreement, you agree to promptly notify and return
 * this Software to Ambarella, Inc.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#include "cryptography_if.h"

#ifndef DNOT_INCLUDE_C_HEADER
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#else
#include <bldfunc.h>
#endif

typedef struct
{
    unsigned int total[2];
    unsigned int state[4];
    unsigned char buffer[64];

    unsigned char ipad[64];
    unsigned char opad[64];
} md5_ctx_t;

static void __zeroize(void* v, unsigned int n)
{
    volatile unsigned char* p = (volatile unsigned char*) v;
    while (n--) {
        *p++ = 0;
    }
}

#define DGET_UINT32_LE(n, b, i) \
{                                                       \
    (n) = ( (unsigned int) (b)[(i)    ]       )             \
        | ( (unsigned int) (b)[(i) + 1] <<  8 )             \
        | ( (unsigned int) (b)[(i) + 2] << 16 )             \
        | ( (unsigned int) (b)[(i) + 3] << 24 );            \
}

#define DPUT_UINT32_LE(n, b, i) \
{                                                               \
    (b)[(i)    ] = (unsigned char) ( ( (n)       ) & 0xFF );    \
    (b)[(i) + 1] = (unsigned char) ( ( (n) >>  8 ) & 0xFF );    \
    (b)[(i) + 2] = (unsigned char) ( ( (n) >> 16 ) & 0xFF );    \
    (b)[(i) + 3] = (unsigned char) ( ( (n) >> 24 ) & 0xFF );    \
}

static void __md5_init(md5_ctx_t *ctx)
{
    memset(ctx, 0, sizeof(md5_ctx_t));
}

static void __md5_free(md5_ctx_t *ctx)
{
    if (ctx == NULL) {
        return;
    }
    __zeroize(ctx, sizeof(md5_ctx_t));
}

static void __md5_starts(md5_ctx_t *ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
}

static void __md5_process(md5_ctx_t *ctx, const unsigned char data[64])
{
    unsigned int X[16], A, B, C, D;

    DGET_UINT32_LE(X[ 0], data,  0);
    DGET_UINT32_LE(X[ 1], data,  4);
    DGET_UINT32_LE(X[ 2], data,  8);
    DGET_UINT32_LE(X[ 3], data, 12);
    DGET_UINT32_LE(X[ 4], data, 16);
    DGET_UINT32_LE(X[ 5], data, 20);
    DGET_UINT32_LE(X[ 6], data, 24);
    DGET_UINT32_LE(X[ 7], data, 28);
    DGET_UINT32_LE(X[ 8], data, 32);
    DGET_UINT32_LE(X[ 9], data, 36);
    DGET_UINT32_LE(X[10], data, 40);
    DGET_UINT32_LE(X[11], data, 44);
    DGET_UINT32_LE(X[12], data, 48);
    DGET_UINT32_LE(X[13], data, 52);
    DGET_UINT32_LE(X[14], data, 56);
    DGET_UINT32_LE(X[15], data, 60);

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P(a,b,c,d,k,s,t)                                \
{                                                       \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;           \
}

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

    P( A, B, C, D,  0,  7, 0xD76AA478 );
    P( D, A, B, C,  1, 12, 0xE8C7B756 );
    P( C, D, A, B,  2, 17, 0x242070DB );
    P( B, C, D, A,  3, 22, 0xC1BDCEEE );
    P( A, B, C, D,  4,  7, 0xF57C0FAF );
    P( D, A, B, C,  5, 12, 0x4787C62A );
    P( C, D, A, B,  6, 17, 0xA8304613 );
    P( B, C, D, A,  7, 22, 0xFD469501 );
    P( A, B, C, D,  8,  7, 0x698098D8 );
    P( D, A, B, C,  9, 12, 0x8B44F7AF );
    P( C, D, A, B, 10, 17, 0xFFFF5BB1 );
    P( B, C, D, A, 11, 22, 0x895CD7BE );
    P( A, B, C, D, 12,  7, 0x6B901122 );
    P( D, A, B, C, 13, 12, 0xFD987193 );
    P( C, D, A, B, 14, 17, 0xA679438E );
    P( B, C, D, A, 15, 22, 0x49B40821 );

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

    P( A, B, C, D,  1,  5, 0xF61E2562 );
    P( D, A, B, C,  6,  9, 0xC040B340 );
    P( C, D, A, B, 11, 14, 0x265E5A51 );
    P( B, C, D, A,  0, 20, 0xE9B6C7AA );
    P( A, B, C, D,  5,  5, 0xD62F105D );
    P( D, A, B, C, 10,  9, 0x02441453 );
    P( C, D, A, B, 15, 14, 0xD8A1E681 );
    P( B, C, D, A,  4, 20, 0xE7D3FBC8 );
    P( A, B, C, D,  9,  5, 0x21E1CDE6 );
    P( D, A, B, C, 14,  9, 0xC33707D6 );
    P( C, D, A, B,  3, 14, 0xF4D50D87 );
    P( B, C, D, A,  8, 20, 0x455A14ED );
    P( A, B, C, D, 13,  5, 0xA9E3E905 );
    P( D, A, B, C,  2,  9, 0xFCEFA3F8 );
    P( C, D, A, B,  7, 14, 0x676F02D9 );
    P( B, C, D, A, 12, 20, 0x8D2A4C8A );

#undef F

#define F(x,y,z) (x ^ y ^ z)

    P( A, B, C, D,  5,  4, 0xFFFA3942 );
    P( D, A, B, C,  8, 11, 0x8771F681 );
    P( C, D, A, B, 11, 16, 0x6D9D6122 );
    P( B, C, D, A, 14, 23, 0xFDE5380C );
    P( A, B, C, D,  1,  4, 0xA4BEEA44 );
    P( D, A, B, C,  4, 11, 0x4BDECFA9 );
    P( C, D, A, B,  7, 16, 0xF6BB4B60 );
    P( B, C, D, A, 10, 23, 0xBEBFBC70 );
    P( A, B, C, D, 13,  4, 0x289B7EC6 );
    P( D, A, B, C,  0, 11, 0xEAA127FA );
    P( C, D, A, B,  3, 16, 0xD4EF3085 );
    P( B, C, D, A,  6, 23, 0x04881D05 );
    P( A, B, C, D,  9,  4, 0xD9D4D039 );
    P( D, A, B, C, 12, 11, 0xE6DB99E5 );
    P( C, D, A, B, 15, 16, 0x1FA27CF8 );
    P( B, C, D, A,  2, 23, 0xC4AC5665 );

#undef F

#define F(x,y,z) (y ^ (x | ~z))

    P( A, B, C, D,  0,  6, 0xF4292244 );
    P( D, A, B, C,  7, 10, 0x432AFF97 );
    P( C, D, A, B, 14, 15, 0xAB9423A7 );
    P( B, C, D, A,  5, 21, 0xFC93A039 );
    P( A, B, C, D, 12,  6, 0x655B59C3 );
    P( D, A, B, C,  3, 10, 0x8F0CCC92 );
    P( C, D, A, B, 10, 15, 0xFFEFF47D );
    P( B, C, D, A,  1, 21, 0x85845DD1 );
    P( A, B, C, D,  8,  6, 0x6FA87E4F );
    P( D, A, B, C, 15, 10, 0xFE2CE6E0 );
    P( C, D, A, B,  6, 15, 0xA3014314 );
    P( B, C, D, A, 13, 21, 0x4E0811A1 );
    P( A, B, C, D,  4,  6, 0xF7537E82 );
    P( D, A, B, C, 11, 10, 0xBD3AF235 );
    P( C, D, A, B,  2, 15, 0x2AD7D2BB );
    P( B, C, D, A,  9, 21, 0xEB86D391 );

#undef F

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
}

static void __md5_update(md5_ctx_t *ctx, const unsigned char *input, unsigned int ilen)
{
    unsigned int fill;
    unsigned int left;

    if( ilen == 0 )
        return;

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (unsigned int) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < (unsigned int) ilen) {
        ctx->total[1]++;
    }

    if (left && ilen >= fill) {
        memcpy((void *) (ctx->buffer + left), input, fill);
        __md5_process(ctx, ctx->buffer);
        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= 64) {
        __md5_process( ctx, input );
        input += 64;
        ilen  -= 64;
    }

    if (ilen > 0) {
        memcpy((void *) (ctx->buffer + left), input, ilen);
    }
}

static const unsigned char __g_md5_padding[64] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static void __md5_final(md5_ctx_t *ctx, unsigned char* output)
{
    unsigned int last, padn;
    unsigned int high, low;
    unsigned char msglen[8];

    high = ( ctx->total[0] >> 29 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );

    DPUT_UINT32_LE( low,  msglen, 0 );
    DPUT_UINT32_LE( high, msglen, 4 );

    last = ctx->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    __md5_update( ctx, __g_md5_padding, padn );
    __md5_update( ctx, msglen, 8 );

    DPUT_UINT32_LE( ctx->state[0], output,  0 );
    DPUT_UINT32_LE( ctx->state[1], output,  4 );
    DPUT_UINT32_LE( ctx->state[2], output,  8 );
    DPUT_UINT32_LE( ctx->state[3], output, 12 );
}

void digest_md5(const unsigned char* message, unsigned int len, unsigned char* digest)
{
    md5_ctx_t ctx;

    __md5_init(&ctx);
    __md5_starts(&ctx);
    __md5_update(&ctx, message, len);
    __md5_final(&ctx, digest);
    __md5_free(&ctx);
}

#ifndef DNOT_INCLUDE_C_HEADER
int digest_md5_file(const char* file, unsigned char* digest)
{
    FILE *f;
    unsigned int n;
    md5_ctx_t ctx;
    unsigned char buf[1024];

    if ((f = fopen(file, "rb")) == NULL) {
        return (-1);
    }

    __md5_init(&ctx);
    __md5_starts(&ctx);

    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        __md5_update(&ctx, buf, n);
    }

    __md5_final(&ctx, digest);
    __md5_free(&ctx);

    if (ferror(f) != 0) {
        fclose(f);
        return (-2);
    }

    fclose(f);
    return 0;
}
#endif

static void __md5_hmac_starts(md5_ctx_t *ctx, const unsigned char *key, unsigned int keylen)
{
    unsigned int i;
    unsigned char sum[16];

    if (keylen > 64) {
        digest_md5(key, keylen, sum);
        keylen = 16;
        key = sum;
    }

    memset(ctx->ipad, 0x36, 64);
    memset(ctx->opad, 0x5C, 64);

    for (i = 0; i < keylen; i++) {
        ctx->ipad[i] = (unsigned char)(ctx->ipad[i] ^ key[i]);
        ctx->opad[i] = (unsigned char)(ctx->opad[i] ^ key[i]);
    }

    __md5_starts(ctx);
    __md5_update(ctx, ctx->ipad, 64);

    __zeroize(sum, sizeof(sum));
}

static void __md5_hmac_update(md5_ctx_t *ctx, const unsigned char *input, unsigned int ilen)
{
    __md5_update(ctx, input, ilen);
}

static void __md5_hmac_finish(md5_ctx_t *ctx, unsigned char output[16])
{
    unsigned char tmpbuf[16];

    __md5_final( ctx, tmpbuf );
    __md5_starts( ctx );
    __md5_update( ctx, ctx->opad, 64 );
    __md5_update( ctx, tmpbuf, 16 );
    __md5_final( ctx, output );

    __zeroize( tmpbuf, sizeof( tmpbuf ) );
}

//static void __md5_hmac_reset(md5_ctx_t *ctx)
//{
//    __md5_starts(ctx);
//    __md5_update(ctx, ctx->ipad, 64);
//}

void digest_md5_hmac(const unsigned char* key, unsigned int keylen, const unsigned char* input, unsigned int ilen, unsigned char* output)
{
    md5_ctx_t ctx;

    __md5_init(&ctx);
    __md5_hmac_starts(&ctx, key, keylen);
    __md5_hmac_update(&ctx, input, ilen);
    __md5_hmac_finish(&ctx, output);
    __md5_free(&ctx);
}

#ifndef DNOT_INCLUDE_C_HEADER
void* digest_md5_init()
{
    md5_ctx_t* ctx = (md5_ctx_t*) malloc(sizeof(md5_ctx_t));
    if (ctx) {
        __md5_init(ctx);
        __md5_starts(ctx);
        return (void*) ctx;
    }

    printf("error: digest_md5_init, malloc fail\n");
    return NULL;
}

void digest_md5_update(void* ctx, const unsigned char* message, unsigned int len)
{
    if (ctx && message && len) {
        __md5_update((md5_ctx_t* )ctx, message, len);
    } else {
        printf("error: digest_md5_update, bad input parameters\n");
    }
    return;
}

void digest_md5_final(void* ctx, unsigned char* digest)
{
    if (ctx && digest) {
        __md5_final((md5_ctx_t* )ctx, digest);
    } else {
        printf("error: digest_md5_final, bad input parameters\n");
    }
}
#endif

