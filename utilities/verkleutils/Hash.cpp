#include "Hash.h"

using namespace verkle::utils;

Hash::Hash() : m_ctx(EVP_MD_CTX_new())
{
    EVP_DigestInit_ex(m_ctx, EVP_sha256(), NULL);
}

Hash::~Hash()
{
    EVP_MD_CTX_free(m_ctx);
}

void Hash::update(const void* data, size_t len)
{
    EVP_DigestUpdate(m_ctx, data, len);
}

void Hash::finalize(unsigned char out[32])
{
    unsigned int _len;
    EVP_DigestFinal_ex(m_ctx, out, &_len);
}