#include <openssl/evp.h>
#include <openssl/dh.h>
#include <openssl/bn.h>
#include <openssl/core_names.h>
#include <openssl/param_build.h>
#include <iostream>

bool generate_and_print_dh_params(int prime_len, int generator, EVP_PKEY* returnedPkey) {
    bool success = false;
    //EVP_PKEY* params = nullptr;
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
    /*if //(EVP_PKEY_paramgen(gen_ctx, &params) <= 0) {
        (EVP_PKEY_paramgen(gen_ctx, &pkey) <= 0)
    {
        std::cerr << "Error generating parameters." << std::endl;
        break;
    }

    // Print parameters

    // Generate a keypair (optional, only if you want to print p and g)
    int retKeygenInit = EVP_PKEY_keygen_init(gen_ctx);
    int retKeygenGo = EVP_PKEY_keygen(gen_ctx, &pkey);
    if (retKeygenInit <= 0 || retKeygenGo <= 0) {
        std::cerr << "Error generating keypair for printing parameters." << std::endl;
        std::cout << "retkeygeninit:" << retKeygenInit << " RetKeygenGo:" << retKeygenGo << std::endl;
        break;
    }
    */

    if (EVP_PKEY_generate(gen_ctx, &pkey) <= 0)
    {
        std::cout << "error1" << std::endl;
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
    if (!success)
    {
        if (gen_ctx) EVP_PKEY_CTX_free(gen_ctx);
        //if (params) EVP_PKEY_free(params);
        if (pkey) EVP_PKEY_free(pkey);
        if (p) BN_free(p);
        if (g) BN_free(g);
    }
    else
    {
        if (gen_ctx) EVP_PKEY_CTX_free(gen_ctx);
        //if (params) EVP_PKEY_free(params);
        //if (pkey) EVP_PKEY_free(pkey);
        if (p) BN_free(p);
        if (g) BN_free(g);

        returnedPkey = pkey;
    }




    return success;
}

int mainKeyGenTest(void) {

    EVP_PKEY* pkey = nullptr;
    // Example usage: Generate DH parameters with a 2048-bit prime and 2 as the generator
    if (!generate_and_print_dh_params(2048, 2, pkey)) {
        std::cerr << "Failed to generate and print DH parameters." << std::endl;
        return 1;
    }


    

    return 0;
}





























