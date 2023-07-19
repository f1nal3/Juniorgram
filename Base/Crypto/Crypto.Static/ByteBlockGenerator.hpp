#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

namespace Base::Generators
{
	using CryptoPP::SecByteBlock;
	using CryptoPP::AutoSeededRandomPool;

/** @class ByteBlockGenerator
* @brief Random generator of CryptoPP::SecByteBlock. This is singleton.
*/
class ByteBlockGenerator
{
public:
    /// @brief Method to get singleton object
    static ByteBlockGenerator& Instance()
    {
        static ByteBlockGenerator singleton;
        return singleton;
    }

    /** @brief Method for generating block of bytes
    * @param blockSize size of byte block, in bytes. Default value (12) is used in enryption; can be increased
    */
    SecByteBlock generateBlock(size_t blockSize = 12)
    {
        SecByteBlock block(blockSize);
        _generator.GenerateBlock(block, blockSize);
        return block;
    };

private:
    AutoSeededRandomPool _generator;
};
}  // namespace Base::Generators
