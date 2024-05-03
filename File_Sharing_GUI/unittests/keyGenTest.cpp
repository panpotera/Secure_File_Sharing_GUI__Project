#include <openssl/evp.h>
#include <openssl/dh.h>
#include <openssl/bn.h>
#include <openssl/core_names.h>
#include <openssl/param_build.h>
#include <iostream>

bool generate_and_print_dh_params(int prime_len, int generator) {
    bool success = false;
    EVP_PKEY* params = nullptr;
    EVP_PKEY_CTX* gen_ctx = nullptr;
    EVP_PKEY* pkey = nullptr;


    BIGNUM* p = nullptr;
    BIGNUM* g = nullptr;

    do{
    // Create context for parameter generation
    gen_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_DH, nullptr);
    if (!gen_ctx || EVP_PKEY_paramgen_init(gen_ctx) <= 0) {
        std::cerr << "Error initializing parameter generation context." << std::endl;
        break;
    }

    // Set the DH parameter generation properties
    if (EVP_PKEY_CTX_set_dh_paramgen_prime_len(gen_ctx, prime_len) <= 0 ||
        EVP_PKEY_CTX_set_dh_paramgen_generator(gen_ctx, generator) <= 0) {
        std::cerr << "Error setting parameter generation properties." << std::endl;
        break;
    }

    // Generate parameters
    if (EVP_PKEY_paramgen(gen_ctx, &params) <= 0) {
        std::cerr << "Error generating parameters." << std::endl;
        break;
    }

    // Print parameters

    // Generate a keypair (optional, only if you want to print p and g)
    if (EVP_PKEY_keygen_init(gen_ctx) <= 0 || EVP_PKEY_keygen(gen_ctx, &pkey) <= 0) {
        std::cerr << "Error generating keypair for printing parameters." << std::endl;
        break;
    }


    EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_FFC_P, &p);
    EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_FFC_G, &g);

    std::cout << "p (prime number) = ";
    BN_print_fp(stdout, p);
    std::cout << std::endl;

    std::cout << "g (generator) = ";
    BN_print_fp(stdout, g);
    std::cout << std::endl;

    success = true;

    } while (0);



    if (gen_ctx) EVP_PKEY_CTX_free(gen_ctx);
    if (params) EVP_PKEY_free(params);
    if (pkey) EVP_PKEY_free(pkey);
    if (p) BN_free(p);
    if (g) BN_free(g);

    return success;
}

int mainKeyGenTest(void) {
    // Example usage: Generate DH parameters with a 2048-bit prime and 2 as the generator
    if (!generate_and_print_dh_params(2048, 2)) {
        std::cerr << "Failed to generate and print DH parameters." << std::endl;
        return 1;
    }
    std::cout << "done" << std::endl;
    return 0;
}






























/*#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>

// Error handling function (replace with proper error handling for a real application)
void handleErrors() {
  ERR_print_errors_fp(stderr);
  exit(1);
}

// Generate a DH parameter pair
bool generateDHParameters(EVP_PKEY_CTX* ctx, BIGNUM* p, BIGNUM* g) {
  if (EVP_PKEY_CTX_init(ctx) <= 0) {
    handleErrors();
  }
  if (EVP_PKEY_CTX_set_algorithm_type(ctx, EVP_PKEY_DH) <= 0) {
    handleErrors();
  }
  if (EVP_PKEY_CTX_ctrl(ctx, EVP_PKEY_OP_DHUTIL, 0, p) <= 0) {
    handleErrors();
  }
  if (EVP_PKEY_CTX_ctrl(ctx, EVP_PKEY_OP_DHUTIL, 2, g) <= 0) {
    handleErrors();
  }
  if (EVP_PKEY_generate(ctx) <= 0) {
    handleErrors();
  }
  EVP_PKEY_CTX_free(ctx);
  return true;
}

// Generate a shared secret using Diffie-Hellman
std::vector<unsigned char> generateSharedSecret(EVP_PKEY* pubKeyB, BIGNUM* privKeyA) {
  EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new();
  if (!ctx) {
    handleErrors();
  }
  EVP_PKEY_CTX_ctrl(ctx, EVP_PKEY_OP_A, 0, pubKeyB);
  EVP_PKEY_CTX_ctrl(ctx, EVP_PKEY_OP_B, 0, privKeyA);
  int sharedSecretLen = EVP_PKEY_size(pubKeyB);
  std::vector<unsigned char> sharedSecret(sharedSecretLen, 0);
  if (EVP_PKEY_derive(ctx, sharedSecret.data(), &sharedSecretLen) <= 0) {
    handleErrors();
  }
  EVP_PKEY_CTX_free(ctx);
  return sharedSecret;
}

// Derive an encryption key from the shared secret using HKDF
std::vector<unsigned char> deriveEncryptionKey(const std::vector<unsigned char>& sharedSecret, size_t keyLen) {
  EVP_KDF_CTX* kdfCtx = EVP_KDF_CTX_new();
  if (!kdfCtx) {
    handleErrors();
  }
  const std::string info = "HKDF for encryption key";
  if (EVP_KDF_Init(kdfCtx, EVP_PKEY_derive, nullptr, sharedSecret.data(), sharedSecret.size()) <= 0) {
    handleErrors();
  }
  EVP_KDF_CTX_ctrl(kdfCtx, EVP_KDF_CTRL_SET_HKDF_INFO, info.length(), const_cast<char*>(info.c_str()));
  std::vector<unsigned char> key(keyLen, 0);
  if (EVP_KDF_Ctrl(kdfCtx, key.data(), nullptr, keyLen) <= 0) {
    handleErrors();
  }
  EVP_KDF_CTX_free(kdfCtx);
  return key;
}

int main()
{
  // Initialize OpenSSL
  OpenSSL_add_all_algorithms();
  ERR_load_crypto_strings();

  // Generate DH parameters (alice and bob would do this separately)
  BIGNUM* p = BN_new();
  BIGNUM* g = BN_new();
  EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new();
  if (!generateDHParameters(ctx, p, g)) {
    return 1;
  }




  // Generate alice's private key
  EVP_PKEY* privKeyA = EVP_PKEY_new();
  if (!EVP_PKEY_assign_DH(privKeyA, p, g)) {
    handleErrors();
  }

  return 0;
}
*/
