#include <openssl/ssl.h>


int generate_DH_parameters(int* p, int* g)

{
	EVP_PKEY_CTX* pctx;
	if(!(pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL)))
	{
		
	}
	if(!EVP_PKEY_paramgen_init(pctx))
	{

	}

	if(!EVP_PKEY_CTX_set_dh_paramgen_prime_len(pctx, 2048))
	{
		return 1;
	}


	return 0;

}


int encrypt_file(int shared_private_key, FILE* plaintext_fp)
{
	return 0;
}

int decrypt_file(int shared_private_key, FILE* ciphertext_fp)
{
	return 0;
}

