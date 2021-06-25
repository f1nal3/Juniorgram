#pragma once

#include "lz4.h"

#include "Handler.hpp"

namespace Network
{
/** @class CompressionHandler
 *  @brief handler class for messages compression.
 */
class CompressionHandler : public AbstractHandler
{
public:
    /**
     * @brief Method for compression of outcoming messages.
     * @param message - buffer that contains data that should be compressed.
     * @param headerBuffer - buffer that will contain compressed header.
     * @param bodyBuffer - buffer that will contain compressed body.
     */

    std::pair<std::unique_ptr<char[]>, size_t> compress(const void* data, size_t dataSize)
    {
        if (dataSize == 0) return std::make_pair(std::unique_ptr<char[]>(), 0);

        int csizeBound = LZ4_compressBound((int)dataSize);

        std::unique_ptr<char[]> cdata(new char[csizeBound]);

            LZ4_compress_default(static_cast<const char*>(data), cdata.get(), (int)dataSize, csizeBound);

        // assert(csize >= 0 && csize <= csizeBound);

        return std::make_pair(std::move(cdata), strlen(cdata.get()));
    }

    void decompress(void* dest, size_t destSize, const void* source, size_t sourceSize)
    {
        if (sourceSize == 0 && destSize == 0) return;

       /* int result = */LZ4_decompress_safe(static_cast<const char*>(source), static_cast<char*>(dest),
                                         (int)sourceSize, (int)destSize);

        //

        //assert(result >= 0);
        //assert(/*static_cast<size_t>(*/result/*)*/ == destSize);
    }

    MessageProcessingState handleOutcomingMessage(const Message& message,
                                yas::shared_buffer& bodyBuffer) override
    {
        // Message::MessageHeader messageHeader = message.mHeader;
        // body compression
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header compression
         std::string kka = "";

           std::generate_n(std::back_inserter(kka), bodyBuffer.size,
                         [i = 0, &bodyBuffer]() mutable { return bodyBuffer.data.get()[i++]; });
   
        std::string ssa = std::string((char*)bodyBuffer.data.get(), bodyBuffer.size);
       
        std::string mma =
            "worjghe86t37694t49376tr8eggr8^&%$#@$%^&&&&&&&*(((^&gtyfg u  uyguyg     gyuwfgwf;";

        //kka.reserve(255);

        
        std::string ksska = "";
        ksska.reserve(255);

          //int olen = LZ4_COMPRESSBOUND((int)kka.size());
         std::string decompr = "";

         std::string compr = "";

         compr.reserve(255);

         decompr.resize(255);

         //ksska.resize(olen);

         std::pair<std::unique_ptr<char[]>, size_t> pair = compress(mma.data(), mma.size());

        /* decompress(decompr.data(), 255, pair.first.get(), pair.second);*/

         
         //std::unique_ptr<char[]> uptr(new char[150]);

         //char* eee = new char[255];

         //       LZ4_decompress_safe((const char*)(&new_compr.data()), ddd, 255, 255);

         //      LZ4_compress_default(mma.c_str(), compr.data(), (int)mma.size(),
         //                     255);
         //
         //      std::string new_compr(compr.data(), ((int)strlen(compr.data())));

         //      //std::generate_n(std::back_inserter(ksska), (int)strlen(compr.data()),
         //      //          [i = 0, &compr]() mutable { return compr.data()[i++]; });

         //      char* ddd = new char[255];

                  
        /*       std::generate_n(std::back_inserter(ksska), pair.second,
                         [i = 0, &pair]() mutable { return pair.first.get()[i++]; });*/


               std::string decr = "";

               decr.resize(255);

               //char* ddd = new char[255];
               
               
               std::unique_ptr<char[]> uptr(new char);



               LZ4_decompress_safe((char*)pair.first.get(), uptr.get(), 255, 255);

               //LZ4_decompress_safe_partial(eee, ddd, 255, 255, 255);

               //std::cout << "\n\n\n" << strlen(ddd);






               
               /*std::unique_ptr<char[]> uptr(new char[1]);

               LZ4_decompress_safe(ksska.data(), uptr.get(), 255, 255);*/


          // LZ4_decompress_safe(ksska.data(), smdka.data(), (int)ksska.size(), 255);

         /*  compress(std::string((char*)bodyBuffer.data.get(), bodyBuffer.size).data(), bodyBuffer.size);*/

        //LZ4_compress_default(kka.c_str(), ksska.data(), (int)kka.size(), olen);

        ksska.shrink_to_fit();

    /*    LZ4_compress_default(kka.c_str(), pchNewCompressed, (int)ssa.size(), 255);*/


     /*   for (size_t i = 0; i < 255; i++)
        {
            std::cout << pchCompressed[i];
        }*/

     /*    char* deCompressed = new char[255];*/

        //std::string smdka = "";

        //smdka.reserve(255);

        //int k = (int)strlen(pchCompressed);

        //std::cout << k; 

       

        //vc.clear();

        //for (size_t i = 0; i < bodyBuffer.size; i++)
        //{
        //    vc.push_back(bodyBuffer.data.get()[i]);
        //    std::cout << bodyBuffer.data.get()[i];
        //}
        //l = 11;

        //std::cout << "\n";


        if (this->nextHandler)
        {
           return this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }

    
    /**
     * @brief Method for decompression of incoming message bodies.
     * @param buffer - buffer that contains data that should be decompressed.
     * @param messageHeader - variable that will contain decompressed message body.
     */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer buffer,
                                                Message& message) override
    {
        std::vector<char> vc;
        int l = 10;

        for (size_t i = 0; i < buffer.size; i++)
        {
            vc.push_back(buffer.data.get()[i]);
            std::cout << buffer.data.get()[i];
        }
        l = 11;

        std::cout << "\n";

        char* pchCompressed = new char[255];

        // body decompression
        LZ4_decompress_safe((const char*)(&*buffer.data), pchCompressed, (int)buffer.size,
                            255);

        vc.clear();
       
        for (size_t i = 0; i < buffer.size; i++)
        {
              vc.push_back(buffer.data.get()[i]);
            std::cout << buffer.data.get()[i];
        }
        l = 11;

        std::cout << "\n";

        if (this->nextHandler)
        {
          return this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
        
    }
};
}  // namespace Network
