#pragma unmanaged
#include <iostream>
#include "../src/CryptingLibraries.h"
#include <fstream>


const char* FileToEncrypt_Path = "korn__.mp3";
const char* bobIP = "192.168.0.30";
const char* FileToDecrypt_Path = "Decrypted_korn__.mp3";
const char* aliceIP = "192.168.0.11";
const short port = 32755;


int main(int argc, char** argv)
{
    std::cout << "le epic" << std::endl;
    int ALICE_2____OR___BOB_3 = -1;
    if (argc == 2)
    {
        ALICE_2____OR___BOB_3 = atoi(argv[1]);
    }

    ALICE_2____OR___BOB_3 = 2;


    if (ALICE_2____OR___BOB_3 == 2)
        AliceFileCrypt aliceObj((char*)FileToEncrypt_Path, (char*)bobIP, port);
    else if (ALICE_2____OR___BOB_3 == 3)
        BobFileCrypt bobObj((char*)FileToDecrypt_Path, (char*)aliceIP, port);
    else
    {
        std::cout << "invalid command line args" << std::endl;
    }

    std::cout << "done" << std::endl;
    return 0;
}







/*
#pragma warning(disable: 4996)
#include <iostream>
#include <fstream>
#include <random>
#include <openssl/applink.c>
#include <openssl/dh.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#define SHA256OUTPUTSIZE    32
#define MAX_FILE_SIZE       100000000 //Max file size is 100 MB
#define MAX_MALLOC_SIZE     1000000000
#define GCM_TAG_SIZE        16


void fcopy(const char* srcPATH, const char* dstPATH);
int aliceKeyGen(EVP_PKEY** dh_1RET);
int bobKeyGenAndDeriveSecretKey(unsigned char** bobSecretKeyRET);
int AliceDeriveSecretKey(EVP_PKEY* dh_1IN, unsigned char** aliceSecretKeyRET);
int EncryptFile(char* FPath, unsigned char* secretKey,
    unsigned char** ivRET, unsigned char** tagRET,
    unsigned char** cipherTextRET, int* ciphertext_len);
int DecryptData(unsigned char* cipherText, int ciphertext_len,
    unsigned char* secretKey, unsigned char* iv,
    unsigned char* tag, unsigned char** plainTextRET,
    int* plaintext_lenRET);


int main(int argc, char** argv)
{
    EVP_PKEY* Alice_DHKey = nullptr;
    EVP_PKEY* Bob_DHKey = nullptr;
    unsigned char* Bob_sSecretKey = nullptr;
    unsigned char* Alice_sSecretKey = nullptr;





    //gen alice key and  the parameters
    if (!aliceKeyGen(&Alice_DHKey))
    {
        std::cout << "couldtn gen alice key" << std::endl;
        return -1;
    }


    //SEND TO BOB  --- THIS IS A TEST FUNCTION!!!
    //  In reality we would send the contents of the file form alice to bob over network
    fcopy("tmp/pubKeySendA2B", "tmp/pubKeyRecvA2B");
    //send params
    fcopy("tmp/paramsAlice", "tmp/paramsBob");

    if (!bobKeyGenAndDeriveSecretKey(&Bob_sSecretKey))
    {
        std::cout << "couldtn gen bob key or secret" << std::endl;
        return -1;
    }

    //send bob publice key back to alice
    // --- THIS IS A TEST FUNCTION!!!
    fcopy("tmp/pubKeySendB2A", "tmp/pubKeyRecvB2A");



    if (!AliceDeriveSecretKey(Alice_DHKey, &Alice_sSecretKey))
    {
        std::cout << "couldtn derive alice secret" << std::endl;
        return -1;
    }





    //testing algorithm: compare bobs secret key and alice secret key...
    //                  Since this is symmetric encyption. they should be same.

    //print bob
    std::cout << std::endl<<"bob secret key: ";
    for (int i = 0; i<SHA256OUTPUTSIZE; i++) {
        std::cout << std::hex << (unsigned int)Bob_sSecretKey[i];
    }
    std::cout << std::endl << "alc secret key: ";
    for (int i = 0; i<SHA256OUTPUTSIZE; i++) {
        std::cout << std::hex << (unsigned int)Alice_sSecretKey[i];
    }
    std::cout << std::endl;




    //test encypting a file
    unsigned char* IV = nullptr;
    unsigned char* tag = nullptr;
    unsigned char* cipherText = nullptr;
    int cipherText_len = 0;
    if (0!= (EncryptFile((char*)"pantera_GDE.mp3", Alice_sSecretKey,
        &IV, &tag, &cipherText, &cipherText_len)))
    {
        std::cout << "encryption failed" << std::endl;
        return -1;
    }
    unsigned char* plainText = nullptr;
    int plainText_len = 0;
    if (0 != (DecryptData(cipherText, cipherText_len, Bob_sSecretKey,
        IV, tag, &plainText, &plainText_len)))
    {
        std::cout << "decryption failed" << std::endl;
        return -1;
    }

    FILE* pt_output = nullptr;
    pt_output = fopen("decrypted_pantera_GDE.mp3", "wb");
    fwrite(plainText, sizeof(unsigned char), plainText_len, pt_output);
    fclose(pt_output);

    return 0;
}

void fcopy(const char* srcPATH, const char* dstPATH)
{
    int ch;
    FILE* src = nullptr;
    FILE* dst = nullptr;
    src = fopen(srcPATH, "r");
    if (src == nullptr)
    {
        std::cout << "11111" << std::endl;
        return;
    }

    dst = fopen(dstPATH, "w");
    if (dst == nullptr)
    {
        std::cout << "22222" << std::endl;
        return;
    }

    // Read character by character and write to destination
    while ((ch = fgetc(src)) != EOF) {
        if (fputc(ch, dst) == EOF) {
            std::cout << "error putting chars" << std::endl;
        }
    }


    fclose(src);
    fclose(dst);
}

int aliceKeyGen(EVP_PKEY** dh_1RET)
{
    int success = 1;
    EVP_PKEY* dh_1 = NULL;
    DH* dhParams = nullptr;
    EVP_PKEY* params = NULL;
    EVP_PKEY_CTX* kctx1 = NULL;
    FILE* fpSend = nullptr;
    FILE* paramSend = nullptr;

    //make params DH* object
    if (NULL == (dhParams = DH_get_2048_256()))
    {
        std::cout << "error 4" << std::endl;
        success = 0;
    }

    // Initalise Diffie Hellman parameter PKEY
    if (NULL == (params = EVP_PKEY_new())) {
        std::cout << "error 3" << std::endl;
        success = 0;
    }

    // Set Diffie Hellman paramerers
    if (1 != EVP_PKEY_assign(params, EVP_PKEY_DHX, dhParams)) {
        std::cout << "error 444" << std::endl;
        success = 0;
    }

    // Initalise client 1 PKEY Context
    if (!(kctx1 = EVP_PKEY_CTX_new(params, NULL))) {
        std::cout << "error 5" << std::endl;
        success = 0;
    }
    if (!kctx1) {
        std::cout << "error 7" << std::endl;
        success = 0;
    }

    // Initalise both contexts key generators
    if (1 != EVP_PKEY_keygen_init(kctx1)) {
        std::cout << "error 9" << std::endl;
        success = 0;
    }

    // Generate DH public and private keys for client 1
    if (1 != EVP_PKEY_keygen(kctx1, &dh_1)) {
        std::cout << "error 11" << std::endl;
        success = 0;
    }

    //first convert alice keypair to a public key file,
    //  read the file and send the file to bob with params
    fpSend = fopen("tmp/pubKeySendA2B", "w");
    if (fpSend == nullptr)
    {
        std::cout << "couldtn open alcie send pubKey file" << std::endl;
        return -1;
    }
    if (1 != PEM_write_PUBKEY(fpSend, dh_1))
    {
        std::cout << "couldtn write Alice PKey pair to pubKey file" << std::endl;
        return -1;
    }
    if (fclose(fpSend) != 0)
    {
        std::cout << "couldtn close alice pubKey file" << std::endl;
        return -1;
    }

    *dh_1RET = dh_1;
    if (NULL == (paramSend = fopen("tmp/paramsAlice", "w")))
    {
        std::cout << "couldtn open alice params file" << std::endl;
        return -1;
    }
    if (1 != PEM_write_DHparams(paramSend, dhParams))
    {
        std::cout << "couldnt write alice params file" << std::endl;
    }
    fclose(paramSend);

    EVP_PKEY_CTX_free(kctx1);

    return success;
}


int bobKeyGenAndDeriveSecretKey(unsigned char** bobSecretKeyRET)
{
    int success = 1;
    EVP_PKEY* dh_2 = NULL;
    EVP_PKEY_CTX* kctx2 = NULL;
    EVP_PKEY_CTX* dctx2 = NULL;
    unsigned char* skey2 = NULL;
    size_t skeylen2;
    EVP_PKEY* params = NULL; //to recerive params from alice to bob
    FILE* paramsFP = NULL;
    DH* DHrecv = nullptr;
    FILE* AlicePubKeyPEMfile = nullptr;
    FILE* BobPubKeyPEMfile = nullptr;
    EVP_PKEY* dh_1IN = nullptr;

    // Initalise Diffie Hellman parameter PKEY
    if (NULL == (params = EVP_PKEY_new())) {
        std::cout << "error 3" << std::endl;
        success = 0;
    }

    if (NULL == (paramsFP = fopen("tmp/paramsBob", "r")))
    {
        std::cout << "couldnt open paramsBob file" << std::endl;
        success = 0;
    }
    else
    {
        DHrecv = PEM_read_DHparams(paramsFP, NULL, NULL, NULL);
        fclose(paramsFP);
    }

    // Set Diffie Hellman paramerers to that of PEM file.
    if (1 != EVP_PKEY_assign(params, EVP_PKEY_DHX, DHrecv)) {
        std::cout << "error 4" << std::endl;
        success = 0;
    }



    // Initalise client 2 PKEY Context
    //if (!(kctx2 = EVP_PKEY_CTX_new(paramsIN, NULL))) {
    if (!(kctx2 = EVP_PKEY_CTX_new(params, NULL))) {
        std::cout << "error 6" << std::endl;
        int success = 0;
    }
    if (!kctx2) {
        std::cout << "error 8" << std::endl;
        int success = 0;
    }

    if (1 != EVP_PKEY_keygen_init(kctx2)) {
        std::cout << "error 10" << std::endl;
        int success = 0;
    }



    AlicePubKeyPEMfile = fopen("tmp/pubKeyRecvA2B", "r");
    if (AlicePubKeyPEMfile == nullptr) {
        std::cout << "error 5000" << std::endl;
        int success = 0;
    }

    dh_1IN = PEM_read_PUBKEY(AlicePubKeyPEMfile, nullptr, nullptr, nullptr);
    fclose(AlicePubKeyPEMfile);

    // Initalise Diffie Hellman PKEY for client 2
    if (NULL == (dh_2 = EVP_PKEY_new())) {
        std::cout << "error 2" << std::endl;
        int success = 0;
    }

    // Generate DH public and private keys for client 2
    if (1 != EVP_PKEY_keygen(kctx2, &dh_2)) {
        std::cout << "error 12" << std::endl;
        int success = 0;
    }





    //we have alicess pub key, and bob private eky and the params.
    //      we now derive shared secret in dctx

    dctx2 = EVP_PKEY_CTX_new(dh_2, NULL);
    // Initalise first key derivation context
    if (1 != EVP_PKEY_derive_init(dctx2)) {
        std::cout << "error 15" << std::endl;
        int success = 0;
    }

    if (1 != EVP_PKEY_check(dctx2)) {
        std::cout << "error 16" << std::endl;
        int success = 0;
    }

    if (1 != EVP_PKEY_param_check(dctx2)) {
        std::cout << "error 17" << std::endl;
        int success = 0;
    }

    // Set first key derivation context peer key to the second DH PKEY
    if (1 != EVP_PKEY_derive_set_peer(dctx2, dh_1IN)) {
        std::cout << "error 18" << std::endl;
        int success = 0;
    }

    if (1 != EVP_PKEY_public_check(dctx2)) {
        std::cout << "error 19" << std::endl;
        int success = 0;
    }

    // Determine buffer length
    if (EVP_PKEY_derive(dctx2, NULL, &skeylen2) <= 0) {
        int success = 0;
    }


    // Assign memory for shared secret variable
    skey2 = (unsigned char*)OPENSSL_malloc(skeylen2);

    int result = 1;
    if ((result = EVP_PKEY_derive(dctx2, skey2, &skeylen2)) > 0) {
        //std::cout << "Key: " << skey2 << std::endl;

        std::cout << std::endl << "bytes in BOBsharedSecret : " << std::dec << skeylen2 << std::endl << "HexOfBOBsharedSecret : " << std::endl;
        for (int i = 0; i < skeylen2; i++) {
            std::cout << std::hex << (unsigned int)skey2[i];
        }
        std::cout << std::endl << std::endl;
    }



    unsigned char* bobSharedSecretKey = nullptr;
    bobSharedSecretKey = (unsigned char*)malloc(32 * sizeof(unsigned char));
    if (bobSharedSecretKey == nullptr || (sizeof(unsigned char) != 1))
    {
        std::cout << "couldnt malloc for bob secret hash" << std::endl;
        return -1;
    }
    bobSharedSecretKey = SHA256(skey2, skeylen2, bobSharedSecretKey);
    if (bobSharedSecretKey == NULL)
    {
        std::cout << "bob sha256 failed" << std::endl;
        return -1;
    }

    *bobSecretKeyRET = bobSharedSecretKey;
    BobPubKeyPEMfile = fopen("tmp/pubKeySendB2A", "w");
    if (BobPubKeyPEMfile == nullptr) {
        std::cout << "error 5001" << std::endl;
        int success = 0;
    }
    if (1 != PEM_write_PUBKEY(BobPubKeyPEMfile, dh_2))
    {
        std::cout << "couldtn write bob PKey pair to pubKey file" << std::endl;
        return -1;
    }
    if (fclose(BobPubKeyPEMfile) != 0)
    {
        std::cout << "couldtn close bob pubKey file" << std::endl;
        return -1;
    }

    return success;
}


int AliceDeriveSecretKey(EVP_PKEY* dh_1IN, unsigned char** aliceSecretKeyRET)
{
    int success = 1;
    EVP_PKEY_CTX* dctx1 = NULL;
    unsigned char* skey1 = NULL;
    size_t skeylen1;
    FILE* fpB2Arcv = nullptr;
    EVP_PKEY* dh_2IN = nullptr;

    fpB2Arcv = fopen("tmp/pubKeyRecvB2A", "r");
    if (fpB2Arcv == nullptr) {
        std::cout << "error 5010" << std::endl;
        int success = 0;
    }

    if (fpB2Arcv == NULL)
    {
        std::cout << "error 5012" << std::endl;
        return -1;
    }

    dh_2IN = PEM_read_PUBKEY(fpB2Arcv, nullptr, nullptr, nullptr);
    fclose(fpB2Arcv);


    dctx1 = EVP_PKEY_CTX_new(dh_1IN, NULL);
    // Initalise first key derivation context
    if (1 != EVP_PKEY_derive_init(dctx1)) {
        std::cout << "error 15" << std::endl;
        int success = 0;
    }

    if (1 != EVP_PKEY_check(dctx1)) {
        std::cout << "error 16" << std::endl;
        int success = 0;
    }

    if (1 != EVP_PKEY_param_check(dctx1)) {
        std::cout << "error 17" << std::endl;
        int success = 0;
    }




    // Set first key derivation context peer key to the second DH PKEY
    if (1 != EVP_PKEY_derive_set_peer(dctx1, dh_2IN)) {
        std::cout << "error 18" << std::endl;
        int success = 0;
    }

    if (1 != EVP_PKEY_public_check(dctx1)) {
        std::cout << "error 19" << std::endl;
        int success = 0;
    }

    // Determine buffer length
    if (EVP_PKEY_derive(dctx1, NULL, &skeylen1) <= 0) {
        int success = 0;
    }


    // Assign memory for shared secret variable
    skey1 = (unsigned char*)OPENSSL_malloc(skeylen1);

    int result = 1;
    if ((result = EVP_PKEY_derive(dctx1, skey1, &skeylen1)) > 0) {
        //std::cout << "Key: " << skey2 << std::endl;

        std::cout << std::endl << "bytes in ALCsharedSecret : " << std::dec << skeylen1 << std::endl << "HexOfALCsharedSecret : " << std::endl;
        for (int i = 0; i < skeylen1; i++) {
            std::cout << std::hex << (unsigned int)skey1[i];
        }
        std::cout << std::endl << std::endl;
    }


    unsigned char* aliceSharedSecretKey = nullptr;
    aliceSharedSecretKey = (unsigned char*)malloc(32 * sizeof(unsigned char));
    if (aliceSharedSecretKey == nullptr || (sizeof(unsigned char) != 1))
    {
        std::cout << "couldnt malloc for bob secret hash" << std::endl;
        return -1;
    }
    aliceSharedSecretKey = SHA256(skey1, skeylen1, aliceSharedSecretKey);
    if (aliceSharedSecretKey == NULL)
    {
        std::cout << "aclice sha256 failed" << std::endl;
        return -1;
    }

    *aliceSecretKeyRET = aliceSharedSecretKey;


    return success;
}

long getFileSize(char* FPath)
{
    std::ifstream infile(FPath, std::ifstream::ate |
        std::ifstream::binary);
    return infile.tellg();
}

int EncryptFile(char* FPath, unsigned char* secretKey,
                unsigned char** ivRET, unsigned char** tagRET,
                unsigned char** cipherTextRET, int* ciphertext_len)
{
    int fail =0;
    long FileSize = 0;
    FileSize = getFileSize(FPath);
    unsigned char* plainText = nullptr;
    FILE* fp = nullptr;
    EVP_CIPHER_CTX* ctx = nullptr;
    *ciphertext_len = NULL;

    if (FileSize < 1)
    {
        std::cout << "file size less than 1" << std::endl;
        fail = 1;
    }
    else if (FileSize > MAX_FILE_SIZE)
    {
        std::cout << "file size too large" << std::endl;
        fail = 1;
    }

    //read in file.
    fp = fopen(FPath, "rb");
    if (NULL == (fp))
    {
        fail = 1;
        std::cout << "couldnt open file" << std::endl;
        return 1;
    }
    if (NULL==(plainText = (unsigned char*)malloc(FileSize * sizeof(unsigned char))))
    {
        std::cout << "couldnt malloc plaintext" << std::endl;
        return 1;
    }
    if (FileSize != ((fread(plainText, sizeof(unsigned char), FileSize, fp))))
    {
        std::cout << "read plaintext file" << std::endl;
        return 1;
    }
    fclose(fp);

    //make random IV
    *ivRET = (unsigned char*)malloc(12 * sizeof(unsigned char));
    if (*ivRET == NULL)
    {
        std::cout << "failed to malloc new IV" << std::endl;
        return 1;
    }
    for (int i = 0; i < 12; i++)
    {
        // Initialize a random device
        std::random_device rd;
        // Seed the generator
        std::mt19937 gen(rd());
        // Define the range [0, 255] for uint8_t
        std::uniform_int_distribution<int> distrib(0, 255);
        // Generate the random uint8_t number
        (*ivRET)[i] = static_cast<unsigned char>(distrib(gen));
    }
    // Create and initialise the context
    if (!(ctx = EVP_CIPHER_CTX_new()))
        return 1;

    // Initialise the encryption operation. //
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        return 1;

    //
     // Set IV length if default 12 bytes (96 bits) is not appropriate
     //
    //if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
      //  handleErrors();

    // Initialise key and IV //
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, secretKey, *ivRET))
        return 1;

    //
     // Provide any AAD data. This can be called zero or more times as
     // required
     //
    //if (1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
      //  return 1;

    //
     // Provide the message to be encrypted, and obtain the encrypted output.
    // EVP_EncryptUpdate can be called multiple times if necessary
     //
    int len = 0;
    *cipherTextRET = (unsigned char*)malloc(MAX_MALLOC_SIZE * sizeof(unsigned char));
    if (*cipherTextRET == NULL)
    {
        std::cout << "vbad" << std::endl;
    }
    if (1 != EVP_EncryptUpdate(ctx, *cipherTextRET, &len, plainText, FileSize))
        return 1;
    *ciphertext_len += len;

    //
     //Finalise the encryption. Normally ciphertext bytes may be written at
     // this stage, but this does not occur in GCM mode
     //
    if ((1 != EVP_EncryptFinal_ex(ctx, *cipherTextRET + len, &len)) || *cipherTextRET==NULL)
        return 1;
    *ciphertext_len += len;
    *cipherTextRET = (unsigned char*)realloc(*cipherTextRET, *ciphertext_len * sizeof(unsigned char));


    // Get the tag //
    *tagRET = (unsigned char*)malloc(GCM_TAG_SIZE * sizeof(unsigned char));
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, *tagRET))
        return 1;


    EVP_CIPHER_CTX_free(ctx);
    return fail;
}

int DecryptData(unsigned char* cipherText, int ciphertext_len, unsigned char* secretKey, unsigned char* iv, unsigned char* tag, unsigned char** plainTextRET, int* plaintext_lenRET)
{
    int fail=0;
    EVP_CIPHER_CTX* ctx;
    int len;

    // Create and initialise the context //
    if (!(ctx = EVP_CIPHER_CTX_new()))
        return -1;

    // Initialise the decryption operation. //
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        return -1;

    ///* Set IV length. Not necessary if this is 12 bytes (96 bits) //
    //if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
     //   return -1;

    // Initialise key and IV //
    if (!EVP_DecryptInit_ex(ctx, NULL, NULL, secretKey, iv))
        return -1;

    //
     // Provide any AAD data. This can be called zero or more times as
     // required
     //
    //if (!EVP_DecryptUpdate(ctx, NULL, &len, aad, aad_len))
      //  handleErrors();

    //
     // Provide the message to be decrypted, and obtain the plaintext output.
     // EVP_DecryptUpdate can be called multiple times if necessary
     //
    *plainTextRET = (unsigned char*)malloc(MAX_MALLOC_SIZE * sizeof(unsigned char));
    if (!EVP_DecryptUpdate(ctx, *plainTextRET, &len, cipherText, ciphertext_len))
        return -1;
    *plaintext_lenRET = len;

    // Set expected tag value. Works in OpenSSL 1.0.1d and later //
    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
        return -1;

    //
     // Finalise the decryption. A positive return value indicates success,
     // anything else is a failure - the plaintext is not trustworthy.
     //
    int ret = EVP_DecryptFinal_ex(ctx, *plainTextRET + len, &len);

    // Clean up //
    EVP_CIPHER_CTX_free(ctx);

    if (ret > 0 && fail==0) {
        // Success //
        std::cout << "sucessfully decrypted and verifed authenticity of message" << std::endl;
        *plaintext_lenRET += len;
        *plainTextRET = (unsigned char*)realloc(*plainTextRET, *plaintext_lenRET * sizeof(unsigned char));
        return 0;
    }
    else {
        // Verify failed //
        return -1;
    }


    return fail;
}


*/

