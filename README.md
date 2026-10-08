Steps to reproduce:

1. `make` to build.
2. Run `./repro-encrypt` and `./repro-decrypt` to reproduce issues on SymCrypt-OpenSSL.
3. Run them with `OPENSSL_CONF=/dev/null` to use the default provider. Observe they do not reproduce with "stock" OpenSSL's default provider.
