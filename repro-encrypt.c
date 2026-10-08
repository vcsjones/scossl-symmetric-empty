#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>

#define CHECK(name, expr) do { ERR_clear_error(); int r_ = (expr); \
    printf("%-28s -> %d\n", name, r_); fflush(stdout); \
    if (r_ <= 0) { ERR_print_errors_fp(stdout); return 1; } } while (0)

int main(void)
{
    unsigned char key[16] = {0};
    unsigned char nonce[12] = {0};
    unsigned char tag[16] = {0};
    int outl = 0;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_reset(ctx);
    EVP_CIPHER_CTX_set_flags(ctx, EVP_CIPHER_CTX_FLAG_WRAP_ALLOW);
    CHECK("CipherInit_ex(partial)", EVP_CipherInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL, 0));
    CHECK("set_key_length", EVP_CIPHER_CTX_set_key_length(ctx, 16));
    CHECK("CipherInit_ex(key)", EVP_CipherInit_ex(ctx, NULL, NULL, key, NULL, -1));
    CHECK("ctrl GCM_SET_IVLEN", EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL));

    /* EncryptCore; empty spans are passed as NULL pointers */
    CHECK("CipherInit_ex(iv,enc)", EVP_CipherInit_ex(ctx, NULL, NULL, NULL, nonce, 1));
    CHECK("CipherUpdate(NULL,NULL,0)", EVP_CipherUpdate(ctx, NULL, &outl, NULL, 0));
    CHECK("CipherFinal_ex(NULL)", EVP_CipherFinal_ex(ctx, NULL, &outl));
    CHECK("ctrl GCM_GET_TAG", EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag));

    for (int i = 0; i < 16; i++) printf("%02x", tag[i]);
    puts("\nDone");
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}
