#pragma once
#include <string>

class EVPCipher
{
private:
    static void handleErrors(void)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

public:
    struct Key
    {
        std::string key;
        std::string iv;
    };

    static Key generateKey()
    {
        unsigned char keyChars[EVP_MAX_KEY_LENGTH];
        RAND_bytes(keyChars, EVP_MAX_KEY_LENGTH);
        unsigned char ivChars[EVP_MAX_BLOCK_LENGTH];
        RAND_bytes(ivChars, EVP_MAX_BLOCK_LENGTH);

        Key key;
        key.key = std::string(keyChars, keyChars + EVP_MAX_KEY_LENGTH);
        key.iv  = std::string(ivChars, ivChars + EVP_MAX_BLOCK_LENGTH);

        return key;
    }

    static std::string encrypt(const std::string& data, const Key& cipherKey)
    {
        auto key = reinterpret_cast<const unsigned char*>(cipherKey.key.c_str());
        auto iv  = reinterpret_cast<const unsigned char*>(cipherKey.iv.c_str());
        auto plainText =
            const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(data.c_str()));

        int plainTextLength = static_cast<int>(data.size());

        unsigned char* cipherData = new unsigned char[cipherKey.key.size()];
        int len;
        int cipherDataLength;

        EVP_CIPHER_CTX* ctx;
        if (!(ctx = EVP_CIPHER_CTX_new()))
        {
            handleErrors();
        }
        if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        {
            handleErrors();
        }
        if (1 != EVP_EncryptUpdate(ctx, cipherData, &len, plainText, plainTextLength))
        {
            handleErrors();
        };
        plainTextLength = len;
        if (1 != EVP_EncryptFinal_ex(ctx, cipherData + len, &len))
        {
            handleErrors();
        };
        plainTextLength += len;

        EVP_CIPHER_CTX_free(ctx);
        return std::string(cipherData, cipherData + plainTextLength);
    }

    static std::string decrypt(const std::string& cipherData, const Key& cipherKey)
    {
        auto key = reinterpret_cast<const unsigned char*>(cipherKey.key.c_str());
        auto iv  = reinterpret_cast<const unsigned char*>(cipherKey.iv.c_str());
        auto cipherText =
            const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(cipherData.c_str()));

        int cipherTextLength = static_cast<int>(cipherData.size());

        unsigned char* decryptedText = new unsigned char[cipherTextLength];
        int len;

        EVP_CIPHER_CTX* ctx;
        if (!(ctx = EVP_CIPHER_CTX_new()))
        {
            handleErrors();
        };
        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        {
            handleErrors();
        };
        if (1 != EVP_DecryptUpdate(ctx, decryptedText, &len, cipherText, cipherTextLength))
        {
            handleErrors();
        };
        cipherTextLength = len;
        if (1 != EVP_DecryptFinal_ex(ctx, decryptedText + len, &len))
        {
            handleErrors();
        };
        cipherTextLength += len;

        EVP_CIPHER_CTX_free(ctx);
        return std::string(decryptedText, decryptedText + cipherTextLength);
    }
};

bool registerUser(const std::string& email, const std::string& login, const std::string& password,
                  std::string& token, std::string& refreshToken)
{
    return 0;
}

struct Crypto
{
    using namespace CryptoPP;

    struct CipherKey
    {
        std::string key;
        std::string iv;
    };

    CipherKey generateKey()
    {
        auto getRandomHexBy = [&](const std::int8_t aesEnumer) -> std::string {
            AutoSeededRandomPool rnd;

            SecByteBlock bytes(0x0, aesEnumer);
            rnd.GenerateBlock(bytes, bytes.size());

            std::string bytesHex{reinterpret_cast<const char*>(bytes.BytePtr()), bytes.size()};

            return bytesHex;
        };

        CipherKey cipherKey;
        cipherKey.key = getRandomHexBy(AES::DEFAULT_KEYLENGTH);
        cipherKey.iv  = getRandomHexBy(AES::BLOCKSIZE);

        return cipherKey;
    }

    std::string encrypt(const std::string& data, const CipherKey& cKey)
    {
        CFB_Mode<AES>::Encryption encryptor;

        const byte* pKey = reinterpret_cast<const byte*>(&cKey.key[0]);
        const byte* pIV  = reinterpret_cast<const byte*>(&cKey.iv[0]);

        encryptor.SetKeyWithIV(pKey, cKey.key.size(), pIV);

        std::string encryptedData;

        StringSource(data, true,
                     new StreamTransformationFilter(encryptor,
                                                    new HexEncoder(new StringSink(encryptedData))));

        return encryptedData;
    }

    std::string decrypt(const std::string& cipherData, const CipherKey& cKey)
    {
        CFB_Mode<AES>::Decryption decryptor;

        const byte* pKey = reinterpret_cast<const byte*>(&cKey.key[0]);
        const byte* pIV  = reinterpret_cast<const byte*>(&cKey.iv[0]);

        decryptor.SetKeyWithIV(pKey, cKey.key.size(), pIV);

        std::string decryptedData;

        StringSource(cipherData, true,
                     new HexDecoder(
                         new StreamTransformationFilter(decryptor, new StringSink(decryptedData))));

        return decryptedData;
    }
}