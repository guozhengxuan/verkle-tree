#pragma once
#include <openssl/evp.h>

namespace verkle::utils
{
class Hash
{
private:
    EVP_MD_CTX* m_ctx;
public:
    Hash();
    ~Hash();
    void update(const void* data, size_t len);
    void finalize(unsigned char out[32]);
};
}