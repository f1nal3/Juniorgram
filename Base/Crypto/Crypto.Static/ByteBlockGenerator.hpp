#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

namespace Base::Generators
{
	using CryptoPP::SecByteBlock;
	using CryptoPP::AutoSeededRandomPool;

/** @class ByteBlockGenerator
* @brief Random generator of CryptoPP::SecByteBlock
* @details Class uses AutoSeededRandomPool, because:
* https://stackoverflow.com/questions/55889143/when-should-i-use-autoseededrandompool-and-not-nonblockingrng
*/
class ByteBlockGenerator
{
    /** @brief Method for generating block of bytes with
    * @param blockSize size of byte block, in bytes
    */
    SecByteBlock generateBlock(size_t blockSize)
    {
        SecByteBlock block(blockSize);
        _generator.GenerateBlock(block, blockSize);
        return block;
    };

private:
    AutoSeededRandomPool _generator;
};
};  // namespace Base::Generators
