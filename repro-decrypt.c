#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>

#define CHECK(name, expr) do { ERR_clear_error(); int r_ = (expr); \
    printf("%-28s -> %d\n", name, r_); \
    if (r_ <= 0) { ERR_print_errors_fp(stdout); return 1; } } while (0)

int main(void)
{
    unsigned char key[16] = {0};
    unsigned char nonce[12] = {0};
    unsigned char tag[16] = {0x58,0xe2,0xfc,0xce,0xfa,0x7e,0x30,0x61,0x36,0x7f,0x1d,0x57,0xa4,0xe7,0x45,0x5a};
    int outl = 0;

    /* ImportKey: CryptoNative_EvpCipherCreatePartial */
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_reset(ctx);
    EVP_CIPHER_CTX_set_flags(ctx, EVP_CIPHER_CTX_FLAG_WRAP_ALLOW);
    CHECK("CipherInit_ex(partial)", EVP_CipherInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL, 0));
    CHECK("set_key_length", EVP_CIPHER_CTX_set_key_length(ctx, 16));
    CHECK("CipherInit_ex(key)", EVP_CipherInit_ex(ctx, NULL, NULL, key, NULL, -1));
    CHECK("ctrl GCM_SET_IVLEN", EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL));

    /* DecryptCore; empty spans are passed as NULL pointers */
    CHECK("CipherInit_ex(iv,dec)", EVP_CipherInit_ex(ctx, NULL, NULL, NULL, nonce, 0));
    CHECK("CipherUpdate(NULL,NULL,0)", EVP_CipherUpdate(ctx, NULL, &outl, NULL, 0));
    CHECK("ctrl GCM_SET_TAG", EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag));
    CHECK("CipherFinal_ex(NULL)", EVP_CipherFinal_ex(ctx, NULL, &outl));

    puts("Done");
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}
