#pragma once 

#include <cryptopp/hmac.h>
#include <cryptopp/osrng.h>
#include <cryptopp/eccrypto.h>

#include <cryptopp/oids.h>
#include <cryptopp/dh2.h>
#include <cryptopp/modes.h>

#include <cstdlib>

//#define ATEXIT_FIXED important moment!
namespace Utility
{
    class KeyHolder
    {
    public:

        static KeyHolder& Instance() 
        { 
            if (!mpInstance)
            {
                if (mDestroyed)
                {
                    onDeadReference();
                }
                else
                {
                    create();
                }
            }
            return *mpInstance;
        }

        virtual ~KeyHolder()
        { 
            mpInstance = nullptr;
            mDestroyed = true;
        }

        inline static void killKeyHolder()
        { 
           mpInstance->~KeyHolder();
        }

        inline std::string getPublicClientKey()
        {
            return {reinterpret_cast<const char*>(mPublicClient.data()), mPublicClient.size()};
        }

        inline void setPublicServerKey(const std::string& publicServerKey)
        {
            mPublicServer = CryptoPP::SecByteBlock(
                reinterpret_cast<const CryptoPP::byte*>(publicServerKey.data()),
                publicServerKey.size());
        }

        inline void calculateSharedSecret()
        {
            mSharedSecret.Assign(CryptoPP::SecByteBlock(mDHClient.AgreedValueLength()));

            if (!mDHClient.Agree(mSharedSecret, mPrivClient, mPublicServer))
                throw std::runtime_error("Failed to reach shared secret!");

             Network::EncryptionHandler::calculateDigest(mSharedSecret);
        }

        inline static bool generateKey() 
        {
            using namespace CryptoPP;


            //CryptoPP::AutoSeededRandomPool Rng{};
            //CryptoPP::OID EllipticCurve{CryptoPP::ASN1::secp256r1()};
            //CryptoPP::ECDH<CryptoPP::ECP>::Domain DHClient{mEllipticCurve};
            //CryptoPP::ECDH<CryptoPP::ECP>::Domain DHServer{mEllipticCurve};
            //CryptoPP::SecByteBlock publicClient{DHClient.PublicKeyLength()};
            //CryptoPP::SecByteBlock privClient{DHClient.PrivateKeyLength()};
            //SecByteBlock privServ(DHClient.PrivateKeyLength()), pubServ(DHClient.PublicKeyLength());

            //DHClient.GenerateKeyPair(Rng, privClient, pubServ);
            //DHServer.GenerateKeyPair(Rng, privServ, publicClient);

            //if (DHClient.AgreedValueLength() != DHServer.AgreedValueLength())
            //    throw std::runtime_error("Shared shared size mismatch");

            // SecByteBlock sharedClient(DHClient.AgreedValueLength()),
            //    sharedServer(DHServer.AgreedValueLength());

            // if (!DHClient.Agree(sharedClient, privClient, pubServ))
            //     throw std::runtime_error("Failed to reach shared secret (A)");

            // if (!DHServer.Agree(sharedServer, privServ, publicClient))
            //     throw std::runtime_error("Failed to reach shared secret (B)");

            // 
            //int aesKeyLength = SHA256::DIGESTSIZE;  // 32 bytes = 256 bit key
            // int defBlockSize = AES::BLOCKSIZE;

            // // Calculate a SHA-256 hash over the Diffie-Hellman session key
            // SecByteBlock key(SHA256::DIGESTSIZE);
            // SHA256().CalculateDigest(key, privClient, privClient.size());

            // // Generate a random IV
            // byte iv[AES::BLOCKSIZE];
            // Rng.GenerateBlock(iv, defBlockSize /*AES::BLOCKSIZE*/);


            // SecByteBlock keyss;

            // std::string kbnm = "asd";
            // iv[0]            = '2';
            // 
            // 

            // //(byte*)iv, iv.size()

            // char message[] = "Hello! How are you.";
            // int messageLen = (int)strlen(message) + 1;

            // CFB_Mode<AES>::Encryption cfbEncryption(key, aesKeyLength, iv);
            // cfbEncryption.ProcessData((byte*)message, (byte*)message, messageLen);

            // //////////////////////////////////////////////////////////////////////////
            // // Decrypt

            // byte iv1[AES::BLOCKSIZE];
            // Rng.GenerateBlock(iv1, defBlockSize /*AES::BLOCKSIZE*/);

            // CFB_Mode<AES>::Decryption cfbDecryption(key, aesKeyLength, iv1);
            // cfbDecryption.ProcessData((byte*)message, (byte*)message, messageLen);

            // int k = 10;

            // k = 55 + 38;
          //   OID CURVE = CryptoPP::ASN1::secp256r1();

          //   AutoSeededRandomPool rng;

          //   ECDH<ECP>::Domain dhA(CURVE), dhB(CURVE);

          //   SecByteBlock privA(dhA.PrivateKeyLength()), pubA(dhA.PublicKeyLength());


          ///*     std::string token2(reinterpret_cast<const char*>(pubA.data()),
          //                      pub.size());*/


          //   SecByteBlock privB(dhB.PrivateKeyLength()), pubB(dhB.PublicKeyLength());

          //   dhA.GenerateKeyPair(rng, privA, pubA);

          //   dhB.GenerateKeyPair(rng, privB, pubB);

          //   if (dhA.AgreedValueLength() != dhB.AgreedValueLength())
          //       throw std::runtime_error("Shared shared size mismatch");

          //   SecByteBlock sharedA(dhA.AgreedValueLength()), sharedB(dhB.AgreedValueLength());

          //   if (!dhA.Agree(sharedA, privA, pubB))
          //       throw std::runtime_error("Failed to reach shared secret (A)");

          //   if (!dhB.Agree(sharedB, privB, pubA))
          //       throw std::runtime_error("Failed to reach shared secret (B)");

          //   Integer ssa, ssb;

          //   ssa.Decode(sharedA.BytePtr(), sharedA.SizeInBytes());
          //   std::cout << "(A): " << std::hex << ssa << std::endl;

          //   ssb.Decode(sharedB.BytePtr(), sharedB.SizeInBytes());
          //   std::cout << "(B): " << std::hex << ssb << std::endl;

          //   if (ssa != ssb) throw std::runtime_error("Failed to reach shared secret (C)");

          //       std::cout << "Agreed to shared secret" << std::endl;



          //  ////////////////////////////////////////////////////////////////////////
          ///*   Alice*/

          //   //Initialize the Diffie-Hellman class with a random prime and base
          //  AutoSeededRandomPool rngA;
          //  DH dhA;
          //  dhA.AccessGroupParameters().Initialize(rngA, 128);

          //  // Extract the prime and base. These values could also have been hard coded
          //  // in the application
          //  Integer iPrime     = dhA.GetGroupParameters().GetModulus();
          //  Integer iGenerator = dhA.GetGroupParameters().GetSubgroupGenerator();

          //  SecByteBlock privA(dhA.PrivateKeyLength());
          //  SecByteBlock pubA(dhA.PublicKeyLength());
          //  SecByteBlock secretKeyA(dhA.AgreedValueLength());

          //  // Generate a pair of integers for Alice. The public integer is forwarded to Bob.
          //  dhA.GenerateKeyPair(rngA, privA, pubA);

          //  //////////////////////////////////////////////////////////////////////////
          //  // Bob

          //  AutoSeededRandomPool rngB;
          //  // Initialize the Diffie-Hellman class with the prime and base that Alice generated.
          //  DH dhB(iPrime, iGenerator);

          //  SecByteBlock privB(dhB.PrivateKeyLength());
          //  SecByteBlock pubB(dhB.PublicKeyLength());
          //  SecByteBlock secretKeyB(dhB.AgreedValueLength());

          //  // Generate a pair of integers for Bob. The public integer is forwarded to Alice.
          //  dhB.GenerateKeyPair(rngB, privB, pubB);

          //  std::string token(reinterpret_cast<const char*>(pubB.data()), pubB.size());

          //  SecByteBlock sbb(reinterpret_cast<const byte*>(token.data()), token.size());

          //  if (pubB == sbb)
          //  {
          //      std::cout << "Wow!";
          //  }

          //  //////////////////////////////////////////////////////////////////////////
          //  // Agreement

          //  // Alice calculates the secret key based on her private integer as well as the
          //  // public integer she received from Bob.
          //  if (!dhA.Agree(secretKeyA, privA, pubB)) return false;

          //  // Bob calculates the secret key based on his private integer as well as the
          //  // public integer he received from Alice.
          //  if (!dhB.Agree(secretKeyB, privB, pubA)) return false;

          //  // Just a validation check. Did Alice and Bob agree on the same secret key?
          //  if (memcmp(secretKeyA.begin(), secretKeyB.begin(), dhA.AgreedValueLength()))
          //      return false;

          //  //------------------------------------------------------------------------------------

          //  Integer ssa, ssb;

          //  ssa.Decode(secretKeyA.BytePtr(), secretKeyA.SizeInBytes());
          //  std::cout << "(A): " << std::hex << ssa << std::endl;

          //  ssb.Decode(secretKeyB.BytePtr(), secretKeyB.SizeInBytes());
          //  std::cout << "(B): " << std::hex << ssb << std::endl;

          //  int aesKeyLength = SHA256::DIGESTSIZE;  // 32 bytes = 256 bit key
          //  int defBlockSize = AES::BLOCKSIZE;

          //  // Calculate a SHA-256 hash over the Diffie-Hellman session key
          //  SecByteBlock key(SHA256::DIGESTSIZE);
          //  SHA256().CalculateDigest(key, secretKeyA, secretKeyA.size());

          //  // Generate a random IV
          //  byte iv[AES::BLOCKSIZE];
          //  rngA.GenerateBlock(iv, defBlockSize /*AES::BLOCKSIZE*/);

          //  char message[] = "Hello! How are you.";
          //  int messageLen = (int)strlen(message) + 1;

          //  //////////////////////////////////////////////////////////////////////////
          //  // Encrypt

          //  CFB_Mode<AES>::Encryption cfbEncryption(key, aesKeyLength, iv);
          //  cfbEncryption.ProcessData((byte*)message, (byte*)message, messageLen);

          //  //////////////////////////////////////////////////////////////////////////
          //  // Decrypt

          //  CFB_Mode<AES>::Decryption cfbDecryption(key, aesKeyLength, iv);
          //  cfbDecryption.ProcessData((byte*)message, (byte*)message, messageLen);

            return 1;
        }

    private:
        
        inline static void create()
        {
            static KeyHolder theInstance;
            mDHClient.GenerateKeyPair(mRng, mPrivClient, mPublicClient);
            mpInstance = &theInstance;
        }

        inline static void onDeadReference()
        {
            create();
            new (mpInstance) KeyHolder;
            #ifdef ATEXIT_FIXED
            std::atexit(killKeyHolder);
            #endif
            mDestroyed = false;
        }

        inline static KeyHolder* mpInstance = nullptr;
        inline static bool mDestroyed       = false;
      
        inline static CryptoPP::AutoSeededRandomPool mRng{};
        inline static CryptoPP::OID mEllipticCurve{CryptoPP::ASN1::secp256r1()};
        inline static CryptoPP::ECDH<CryptoPP::ECP>::Domain mDHClient{mEllipticCurve};
        inline static CryptoPP::SecByteBlock mPublicClient{mDHClient.PublicKeyLength()};
        inline static CryptoPP::SecByteBlock mPrivClient{mDHClient.PrivateKeyLength()};

        inline static CryptoPP::SecByteBlock mPublicServer{};

        inline static CryptoPP::SecByteBlock mSharedSecret{};
    };

}
