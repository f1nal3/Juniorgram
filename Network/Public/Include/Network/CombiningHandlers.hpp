#pragma once
#include"Handler.hpp"
#include "Network/CompressionHandler.hpp"
#include "Network/EncryptionHandler.hpp"
#include "Network/SerializationHandler.hpp"
namespace Network
{
	/** 
	* @class CombiningHandlers
	* @brief class that combines all handlers for message preprocessing.
	*/
	class CombiningHandlers
	{
		public:
		/**
		* @brief Method for serialization, compression and encryption of outcoming messages.
		* @param source      - buffer that contains data that should be serialized, compressed and encrypted.
		* @param destination - buffer that will contain serialized, compressed and encrypted body.
		*/
		static MessageProcessingState handleOutcomingMessage(const Message& source,yas::shared_buffer& destination)
		{
			static auto serializationHandler = std::make_unique<SerializationHandler>();
			static auto compressionHandler   = std::make_unique<CompressionHandler>();
			static auto encryptionHandler    = std::make_unique<EncryptionHandler>();

			serializationHandler->setNext(compressionHandler.get());
			compressionHandler-> setNext(encryptionHandler.get());

			return serializationHandler->handleOutcomingMessage(source, destination);
		}

		/**
		* @brief Method for decryption, decompression and deserialization of incoming message bodies.
		* @param destination - buffer that contains data that should be decrypted, decompressed and deserialized.
		* @param source      - variable that will contain decrypted, decompressed and deserialized message body.
		*/
		static MessageProcessingState handleIncomingMessage(const yas::shared_buffer destination, Message& source)
		{
			static auto encryptionHandler    = std::make_unique<EncryptionHandler>();
			static auto compressionHandler   = std::make_unique<CompressionHandler>();
			static auto serializationHandler = std::make_unique<SerializationHandler>();

			encryptionHandler->setNext(compressionHandler.get());
			compressionHandler->setNext(serializationHandler.get());

			return compressionHandler->handleIncomingMessageBody(destination, source);
		}
	};
}

