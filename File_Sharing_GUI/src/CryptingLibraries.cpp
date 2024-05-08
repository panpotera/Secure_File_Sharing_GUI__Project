#pragma unmanaged

#include "CryptingLibraries.h"
#pragma warning(disable: 4996)
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>
#include <ctime>
#include <openssl/applink.c>
#include <openssl/dh.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#define SHA256OUTPUTSIZE    32
#define MAX_FILE_SIZE       100000000 //Max file size is 100 MB
#define MAX_MALLOC_SIZE     1000000000
#define GCM_TAG_SIZE        16


struct ALICE2BOB_MESSAGE1
{//params and alice pubkey
    int MSG_ID = 1;
    int pubKeySize=0;
    unsigned char params[769];
    unsigned char AlicePubKey[2048];//right size?
};

struct BOB2ALICE_MESSAGE1
{
    int MSG_ID = 2;
    int pubKeySize=0;
    unsigned char BobPubKey[2048];//right size?
};

struct ALICE2BOB_MESSAGE2
{
    int MSG_ID = 3;
    unsigned char IV[12];
    unsigned char tag[16];
    int ciphertext_size;
};





AliceFileCrypt::~AliceFileCrypt()
{

}

BobFileCrypt::~BobFileCrypt()
{

}

int connectToBob(char* bobIP, short port,  SOCKET* sockRET)//alice calls this function
{
    //create socket
    struct sockaddr_in bobAddr;
    memset((void*)&bobAddr, 0, sizeof(bobAddr));
    bobAddr.sin_family = AF_INET;
    bobAddr.sin_addr.S_un.S_addr = inet_addr(bobIP);
    bobAddr.sin_port = htons(port);
    *sockRET = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*sockRET == INVALID_SOCKET) {
        std::cerr << "Error creating socket: "<< std::endl;
        return -1;
    }
    if (connect(*sockRET, reinterpret_cast<SOCKADDR*>(&bobAddr), sizeof(bobAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server: " << std::endl;
        closesocket(*sockRET);
        WSACleanup();
        return -1;
    }



    std::time_t starttime = std::time(nullptr);
    int receivedAck = 0;
    //while (receivedAck==0)
    //{
    //    //send hello
        if (sizeof("helloThere") != (send(*sockRET, "helloThere", sizeof("helloThere"), 0)))
        {
            std::cerr << "error sending helloThere " << std::endl;
        }
    //    while (1)
    //    {
            //poll/listen for ack response, set the var 1 if we got it.
            char refStr[] = "ackHEllo";
            char inBuf[] = "tttttttt";
            if (SOCKET_ERROR == recv(*sockRET, inBuf, (int)sizeof("ackHEllo"), 0))
            {
                std::cerr << "error recv ack " << std::endl; \
                    std::cout << "error code: " << WSAGetLastError() << std::endl;
            }
            for (int i = 0; i < (int)sizeof("ackHEllo"); i++)
            {
                if (inBuf[i] != refStr[i])
                {
                    break;
                }
                if (i == (int)sizeof("ackHEllo")-1)
                {
                    receivedAck = 1;
                    break;
                }
                
            }
            //if (std::time(nullptr) - starttime > 6)//6 second timeout
                //break;
     //  / }
    //}

    //send ackAck back to bob
    if (sizeof("ackAck") != (send(*sockRET, "ackAck", sizeof("ackAck"), 0)))
    {
        std::cerr << "error sending ackack " << std::endl;
    }


    return 1;//1 sucess
}

int connectToAlice(char* AliceIP, short port, SOCKET* sockRET)//bob calls this function
{
    int receivedAckAck = 0;
    int receivedAck = 0;
    char refStr[] = "helloThere";
    char tmpBuf[] = "tttttttrtt";
    char refStr2[] = "ackAck";
    char tmpBuf2[] = "tttrtt";

    //set up socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in localAddress;
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(port);  // or whatever port you'd like to listen to
    localAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (SOCKADDR*)&localAddress, sizeof(localAddress)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Bob listening on port ..." <<port<< std::endl;


    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Accept incoming connections
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    *sockRET = accept(serverSocket, reinterpret_cast<SOCKADDR*>(&clientAddr), &clientAddrSize);
    if (*sockRET == INVALID_SOCKET) {
        std::cerr << "Accept failed: " <<std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    closesocket(serverSocket);
    std::cout << "Client connected!" << std::endl;
    

    
    //while (receivedAck == 0)
    //{
        //poll/listen for alices ping, then  break
        if (recv(*sockRET, tmpBuf, sizeof("helloThere"), 0) != (int)sizeof("helloThere"))
        {
            //
            std::cout << "error receriving bytes" << std::endl;
            std::cout << "sock error code" << WSAGetLastError() << std::endl;
            //break;
            goto done;
        }
        for (int i = 0; i < (int)sizeof("helloThere"); i++)
        {
            if (tmpBuf[i] != refStr[i])
            {
                std::cout << "ping string dont match" << std::endl;
                goto done;
            }
            if (i == (int)sizeof("helloThere") - 1)
            {
                receivedAck = 1;
                //break;  //fall through, continue
                std::cout << "matched ping str" << std::endl;
            }

        }
    //}


    //send pong to alice
    if (sizeof("ackHEllo") != (send(*sockRET, "ackHEllo", sizeof("ackHEllo"), 0)))
    {
        std::cout << "error sending ack" << std::endl;
        goto  done;
    }

    //wait for ack ack. then break
    if (sizeof("ackAck") != recv(*sockRET, tmpBuf2, sizeof("ackAck"), 0))
    {
        std::cout << "error recv ackack" << std::endl;
        goto done;
    }
    for (int i = 0; i < (int)sizeof("ackAck"); i++)
    {
        if (tmpBuf2[i] != refStr2[i])
        {
            std::cout << "ackAck string dont match" << std::endl;
            goto done;
        }
        if (i == (int)sizeof("ackAck") - 1)
        {
            receivedAckAck = 1;
            std::cout << "matched ackack str" << std::endl;
            goto done;
        }

    }

    done:
    if (receivedAckAck)
    {
        return 1;
        std::cout << "got ackack" << std::endl;
    }
    else
    {
        return -1;
    }
}

int sendData(SOCKET* sock, char* outBuf, int bufSize)
{
    //returns number of bytes written
    //send to connected socket
    int numBytesWritten = 0;
    numBytesWritten = send(*sock, outBuf, bufSize, 0);
    if (numBytesWritten == SOCKET_ERROR)
    {
        std::cout << "error writing data to socket. error code: " << std::dec<<WSAGetLastError() << std::endl;
    }

    return numBytesWritten;
}

int recvMsg(SOCKET* sock, char* inBuf, int bufSize)
{
    //rcv from target an shit
    int numBytesRecv = 0;
    numBytesRecv = recv(*sock, inBuf, bufSize, 0);
    if (numBytesRecv == SOCKET_ERROR)
    {
        std::cout << "error recv data from socket. error code: " << std::dec<<WSAGetLastError() << std::endl;
        return 0;
    }

    return 1;
}

int saveBufToFile(unsigned char* bufToSave, int bufSize, char* FPath)
{
    FILE* fp = nullptr;

    if (bufSize < 1)
    {
        std::cout << "bad bufSize" << std::endl;
        return -1;
    }

    fp = fopen(FPath, "wb");
    if (fp == nullptr)
    {
        std::cout << "couldnt open file: " << FPath << " to save buf" << std::endl;
        return -1;
    }
    if (bufSize != fwrite(bufToSave, sizeof(char), bufSize, fp))
    {
        std::cout << "errror writing contents to file: " << FPath << " to save buf" << std::endl;
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 1;
}

int readBufFromFile(char* FPath, unsigned char* buf, int bufSize)
{
    std::cout << "bufSize: " << bufSize << std::endl;
    FILE* fp = nullptr;
    if (bufSize == 0)
    {
        std::cout << "bad buf size" << std::endl;
        return -1;
    }

    fp = fopen(FPath, "rb");
    if (fp == nullptr)
    {
        std::cout << "couldnt open file: " << FPath << " to read buf" << std::endl;
        return -1;
    }

    if (bufSize != fread(buf, sizeof(char), bufSize, fp))
    {
        std::cout << "errror reading contents from file: " << FPath << " to get buf" << std::endl;
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 1;
}






long getFileSize(char* FPath)
{
    std::ifstream infile(FPath, std::ifstream::ate |
        std::ifstream::binary);
    return infile.tellg();
}

int aliceKeyGen(void** dh_1RET)
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
    fpSend = fopen("tmp/pubKeySendA2B", "wb");
    if (fpSend == nullptr)
    {
        std::cout << "couldtn open alcie send pubKey file" << std::endl;
        return -1;
    }
    if (1 != PEM_write_PUBKEY(fpSend, dh_1))
    {
        std::cout << "couldtn write Alice PKey pair to pubKey file" << std::endl;
        fclose(fpSend);
        return -1;
    }
    fclose(fpSend);

    *dh_1RET = dh_1;

    if (NULL == (paramSend = fopen("tmp/paramsAlice", "w")))
    {
        std::cout << "couldtn open alice params file" << std::endl;
        return -1;
    }
    if (1 != PEM_write_DHparams(paramSend, dhParams))
    {
        std::cout << "couldnt write alice params file" << std::endl;
        fclose(paramSend);
        return -1;
    }
    fclose(paramSend);

    EVP_PKEY_CTX_free(kctx1);

    return success;
}


int AliceDeriveSecretKey(void* dh_1IN, unsigned char** aliceSecretKeyRET)
{
    int success = 1;
    EVP_PKEY_CTX* dctx1 = NULL;
    unsigned char* skey1 = NULL;
    size_t skeylen1;
    FILE* fpB2Arcv = nullptr;
    EVP_PKEY* dh_2IN = nullptr;

    fpB2Arcv = fopen("tmp/pubKeyRecvB2A", "rb");
    if (fpB2Arcv == nullptr) {
        std::cout << "error 5010" << std::endl;
        int success = 0;
        return -1;
    }


    dh_2IN = PEM_read_PUBKEY(fpB2Arcv, nullptr, nullptr, nullptr);
    fclose(fpB2Arcv);


    dctx1 = EVP_PKEY_CTX_new((EVP_PKEY*)dh_1IN, NULL);
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

    /* Determine buffer length */
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

int EncryptFileNPot(char* FPath, unsigned char* secretKey,
    unsigned char* ivRET, unsigned char* tagRET,
    unsigned char** cipherTextRET, int* ciphertext_len)
{
    int success = 1;
    long FileSize = 0;
    FileSize = getFileSize(FPath);
    unsigned char* plainText = nullptr;
    FILE* fp = nullptr;
    EVP_CIPHER_CTX* ctx = nullptr;
    *ciphertext_len = 0;

    if (FileSize < 1)
    {
        std::cout << "file size less than 1" << std::endl;
        return -1;
    }
    else if (FileSize > MAX_FILE_SIZE)
    {
        std::cout << "file size too large" << std::endl;
        return -1;
    }

    //read in file.
    fp = fopen(FPath, "rb");
    if (NULL == (fp))
    {
        success = 0;
        std::cout << "couldnt open file" << std::endl;
        return -1;
    }
    if (NULL == (plainText = (unsigned char*)malloc(FileSize * sizeof(unsigned char))))
    {
        std::cout << "couldnt malloc plaintext" << std::endl;
        fclose(fp);
        return -1;
    }
    if (FileSize != ((fread(plainText, sizeof(unsigned char), FileSize, fp))))
    {
        std::cout << "read plaintext file" << std::endl;
        fclose(fp);
        return -1;
    }
    fclose(fp);

    //make random IV
    for (int i = 0; i < 12; i++)
    {
        // Initialize a random device
        std::random_device rd;
        // Seed the generator
        std::mt19937 gen(rd());
        // Define the range [0, 255] for uint8_t
        std::uniform_int_distribution<int> distrib(0, 255);
        // Generate the random uint8_t number
        (ivRET)[i] = static_cast<unsigned char>(distrib(gen));
    }
    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        return -1;

    /* Initialise the encryption operation. */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        return -1;

    /*
     * Set IV length if default 12 bytes (96 bits) is not appropriate
     */
     //if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
       //  handleErrors();

     /* Initialise key and IV */
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, secretKey, ivRET))
        return -1;

    /*
     * Provide any AAD data. This can be called zero or more times as
     * required
     */
     //if (1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
       //  return 1;

     /*
      * Provide the message to be encrypted, and obtain the encrypted output.
      * EVP_EncryptUpdate can be called multiple times if necessary
      */
    int len = 0;
    *cipherTextRET = (unsigned char*)malloc(MAX_MALLOC_SIZE * sizeof(unsigned char));
    if (*cipherTextRET == NULL)
    {
        std::cout << "vbad" << std::endl;
        success = 0;
        return -1;
    }
    if (1 != EVP_EncryptUpdate(ctx, *cipherTextRET, &len, plainText, FileSize))
        return -1;
    *ciphertext_len += len;

    /*
     * Finalise the encryption. Normally ciphertext bytes may be written at
     * this stage, but this does not occur in GCM mode
     */
    if (1 != EVP_EncryptFinal_ex(ctx, *cipherTextRET + len, &len))
        return -1;
    *ciphertext_len += len;
    *cipherTextRET = (unsigned char*)realloc(*cipherTextRET, *ciphertext_len * sizeof(unsigned char));


    /* Get the tag */
    //*tagRET = (unsigned char*)malloc(GCM_TAG_SIZE * sizeof(unsigned char));
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tagRET))
        return -1;


    EVP_CIPHER_CTX_free(ctx);
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

    if (NULL == (paramsFP = fopen("tmp/paramsBob", "rb")))
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



    AlicePubKeyPEMfile = fopen("tmp/pubKeyRecvA2B", "rb");
    if (AlicePubKeyPEMfile == nullptr) {
        std::cout << "error 5000" << std::endl;
        return -1;
    }

    dh_1IN = PEM_read_PUBKEY(AlicePubKeyPEMfile, nullptr, nullptr, nullptr);
    fclose(AlicePubKeyPEMfile);
    if (dh_1IN == nullptr)
    {
        std::cout << "couldnt read alice public key file" << std::endl;
        return -1;
    }

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

int DecryptData(unsigned char* cipherText, int ciphertext_len, unsigned char* secretKey, unsigned char* iv, unsigned char* tag, unsigned char** plainTextRET, int* plaintext_lenRET)
{
    int success = 1;
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

    if (success != 1)
    {
        std::cout << "encryption failure" << std::endl;
        return -1;
    }

    if (ret > 0) {
        // Success //
        std::cout << "sucessfully decrypted and verifed authenticity of message" << std::endl;
        *plaintext_lenRET += len;
        *plainTextRET = (unsigned char*)realloc(*plainTextRET, *plaintext_lenRET * sizeof(unsigned char));
        return 1;
    }
    else {
        // Verify failed //
        std::cout << "could not verify authenticity of message" << std::endl;
        return -1;
    }


    return success;
}


BobFileCrypt::BobFileCrypt(char* FilePath, char* aliceIP, short port)
{
    DWORD ftyp;
    FileReceived = -10000;
    unsigned char* bobSecretKey = nullptr;
    unsigned char* ciphertext = nullptr;
    unsigned char* plainText = nullptr;
    int plaintext_size = 0;
    FILE* pt_output = nullptr;
    struct ALICE2BOB_MESSAGE1 A2B_msg1;
    struct BOB2ALICE_MESSAGE1 B2A_msg1;
    struct ALICE2BOB_MESSAGE2 A2B_msg2;
    A2B_msg2.ciphertext_size = 0;
    SOCKET bobSock;
    WSADATA wsaData;
    int result  = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        goto done;
    }

    printf("hey\n");


    //if tmp file DNE, make one.
    ftyp = GetFileAttributesA("tmp\\");
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        // Try to create directory
        if (CreateDirectoryA("tmp\\", NULL)) {
            //std::cout << "Directory created successfully.\n";
        }
        else {
            std::cerr << "Error creating directory!\n";
            goto done;
        }
    }
    else if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        //std::cout << "Directory already exists.\n";
    }
    else {
        std::cerr << "tmp exists but is not a directory!\n";
        goto done;
    }

    if (1 != connectToAlice(aliceIP, port, &bobSock))
    {
        //cant connect
        FileReceived = -1;
        goto done;
    }
    FileReceived = 0;

    printf("hey\n");

    //while (1)
    //{
        if (1 != recvMsg(&bobSock, (char*)(&A2B_msg1), sizeof(A2B_msg1)))
        {
            //bad received message
            FileReceived = -2;
            goto done;
        }
        //if (A2B_msg1.MSG_ID == 1)//default ALICE2BOB_MESSAGE1.MSG_ID)
        //{
      //      break;
        //}
    //}

    if (1 != saveBufToFile(A2B_msg1.params, sizeof(A2B_msg1.params), (char*)"tmp/paramsBob"))
    {
        FileReceived = -11;
        goto done;
    }

    std::cout << A2B_msg1.pubKeySize << std::endl << A2B_msg1.AlicePubKey << std::endl;

    if (1 != saveBufToFile(A2B_msg1.AlicePubKey, A2B_msg1.pubKeySize, (char*)"tmp/pubKeyRecvA2B"))
    {
        FileReceived = -12;
        goto done;
    }



    if (1 != bobKeyGenAndDeriveSecretKey(&bobSecretKey))
    {
        //couldnt do bob keygen or secret key derive
        FileReceived = -3;
        std::cout << A2B_msg1.AlicePubKey << std::endl;
        goto done;
    }

    B2A_msg1.pubKeySize = (int)getFileSize((char*)"tmp/pubKeySendB2A");

    if (1 != readBufFromFile((char*)"tmp/pubKeySendB2A", B2A_msg1.BobPubKey, B2A_msg1.pubKeySize))
    {
        FileReceived = -4;
        goto done;
    }

    if (sizeof(BOB2ALICE_MESSAGE1) != sendData(&bobSock, (char*)(&B2A_msg1), sizeof(B2A_msg1)))
    {
        //couldnt send b2a message
        FileReceived = -5;
        goto done;
    }

    if (1 != recvMsg(&bobSock, (char*)(&A2B_msg2), sizeof(A2B_msg2)))
    {
        //bad received message
        FileReceived = -6;
        goto done;
    }

    //malloc for size of incoming ciphertext
    ciphertext = (unsigned char*)malloc(sizeof(char) * A2B_msg2.ciphertext_size);

    if ((ciphertext==nullptr)  ||  (1 != recvMsg(&bobSock, (char*)(ciphertext), A2B_msg2.ciphertext_size)))
    {
        FileReceived = -7;
        goto done;
    }

    //std::cout << std::endl << std::dec<<A2B_msg2.IV << std::endl << std::dec << A2B_msg2.tag << std::endl<< std::dec << A2B_msg2.ciphertext_size << std::endl;
    for (int i = 0; i < 12; i++)
    {
        printf("%hhu ", A2B_msg2.IV[i]);
    }
    std::cout << std::endl;
    for (int i = 0; i < 16; i++)
    {
        printf("%hhu ", A2B_msg2.tag[i]);
    }
    std::cout << std::endl;


    if (1 != DecryptData(ciphertext, A2B_msg2.ciphertext_size,
        bobSecretKey, A2B_msg2.IV,
        A2B_msg2.tag, &plainText,
        &plaintext_size))
    {
        FileReceived = -8;
        goto done;
    }

    pt_output = fopen(FilePath, "wb");
    fwrite(plainText, sizeof(unsigned char), plaintext_size, pt_output);
    fclose(pt_output);


done:
    closesocket(bobSock);
    WSACleanup();
    if (FileReceived == 0)
        FileReceived = 1;
    else
        std::cout << "FileReceived: " << FileReceived << std::endl;
}


AliceFileCrypt::AliceFileCrypt(char* FilePath, char* bobIP, short port)
{
    DWORD ftyp;
    FileSent = -1000;
    void* dh_1_EVP_PKEY = nullptr;
    int bob2aBufSize = 0;
    unsigned char* aliceSecretKey = nullptr;
    unsigned char* cipherText = nullptr;
    struct ALICE2BOB_MESSAGE1 A2B_msg1;
    struct BOB2ALICE_MESSAGE1 B2A_msg1;
    struct ALICE2BOB_MESSAGE2 A2B_msg2;
    A2B_msg2.ciphertext_size = 0;
    SOCKET aliceSock;
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        goto done;
    }


    ftyp = GetFileAttributesA("tmp\\");
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        // Try to create directory
        if (CreateDirectoryA("tmp\\", NULL)) {
            //std::cout << "Directory created successfully.\n";
        }
        else {
            std::cerr << "Error creating directory!\n";
            goto done;
        }
    }
    else if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        //std::cout << "Directory already exists.\n";
    }
    else {
        std::cerr << "tmp exists but is not a directory!\n";
        goto done;
    }


    if (1 != aliceKeyGen(&dh_1_EVP_PKEY))
    {
        FileSent = -1;
        //couldnt gen key
        goto done;
    }


    FileSent = 34;//connecting to bob
    if (1 != connectToBob(bobIP, port, &aliceSock))
    {
        FileSent = -2;
        //couldnt connect to bob
        goto done;
    }
    FileSent = 0;

    if (1 != readBufFromFile((char*)"tmp/paramsAlice", A2B_msg1.params, sizeof(A2B_msg1.params)))
    {
        FileSent = -2;
        goto done;
    }

    A2B_msg1.pubKeySize = (int)getFileSize((char*)"tmp/pubKeySendA2B");

    if (1 != readBufFromFile((char*)"tmp/pubKeySendA2B", A2B_msg1.AlicePubKey, A2B_msg1.pubKeySize))
    {
        FileSent = -2;
        goto done;
    }

    std::cout << A2B_msg1.pubKeySize << std::endl << A2B_msg1.AlicePubKey << std::endl;

    if (sizeof(ALICE2BOB_MESSAGE1) != sendData(&aliceSock, (char*)(&A2B_msg1), sizeof(A2B_msg1)))
    {
        FileSent = -3;
        //coudlnt send message
        goto done;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    if (1 != recvMsg(&aliceSock, (char*)(&B2A_msg1), sizeof(B2A_msg1)))
    {
        FileSent = -4;
        //std::cout << A2B_msg1.pubKeySize << std::endl<< A2B_msg1.AlicePubKey << std::endl;
        //couldnt recv msg
        goto done;
    }
    if (1 != saveBufToFile(B2A_msg1.BobPubKey, B2A_msg1.pubKeySize, (char*)"tmp/pubKeyRecvB2A"))
    {
        FileSent = -44;
        goto done;
    }
    

    if (1 != AliceDeriveSecretKey(dh_1_EVP_PKEY, &aliceSecretKey))
    {
        FileSent = -5;
        //couldnt derive secret key
        goto done;
    }

    if (1 != EncryptFileNPot(FilePath, aliceSecretKey,
        A2B_msg2.IV, A2B_msg2.tag,
        &cipherText, &A2B_msg2.ciphertext_size))
    {
        FileSent = -6;
        //couldnt encrypt file
        goto done;
    }

    //std::cout << std::endl << std::dec << A2B_msg2.IV << std::endl << std::dec << A2B_msg2.tag << std::endl << std::dec << A2B_msg2.ciphertext_size << std::endl;
    for (int i = 0; i < 12; i++)
    {
        printf("%hhu ",A2B_msg2.IV[i]);
    }
    std::cout << std::endl;
    for (int i = 0; i < 16; i++)
    {
        printf("%hhu ", A2B_msg2.tag[i]);
    }
    std::cout << std::endl;

    if (sizeof(ALICE2BOB_MESSAGE2) != sendData(&aliceSock, (char*)(&A2B_msg2), sizeof(A2B_msg2)))
    {
        FileSent = -61;
        goto done;
    }

    //delay 1second
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


    if (A2B_msg2.ciphertext_size != sendData(&aliceSock, (char*)cipherText, A2B_msg2.ciphertext_size))
    {
        FileSent = -7;
        //couldnt send cipherText
        goto done;
    }

done:
    closesocket(aliceSock);
    WSACleanup();
    if (FileSent == 0)
    {
        FileSent = 1;//if we made it through, we sent the file!
        std::cout << "cipher file was sent successfully" << std::endl;
    }
    else
    {
        std::cout << "FileSent: " << std::dec<<FileSent << std::endl;
    }
}
