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
		* @param message    - buffer that contains data that should be serialized, compressed and encrypted.
		* @param bodyBuffer - buffer that will contain serialized, compressed and encrypted body.
		*/
		MessageProcessingState operator()(const Message& message,yas::shared_buffer& bodyBuffer) const
		{
			auto serializationHandler = std::make_unique<SerializationHandler>();
			auto compressionHandler   = std::make_unique<CompressionHandler>();
			auto encryptionHandler    = std::make_unique<EncryptionHandler>();

			serializationHandler->setNext(compressionHandler.get());
			compressionHandler-> setNext(encryptionHandler.get());

			return serializationHandler->handleOutcomingMessage(message, bodyBuffer);
		}

		/**
		* @brief Method for decryption, decompression and deserialization of incoming message bodies.
		* @param buffer - buffer that contains data that should be decrypted, decompressed and deserialized.
		* @param message - variable that will contain decrypted, decompressed and deserialized message body.
		*/
		MessageProcessingState operator()(const yas::shared_buffer buffer, Message& message) const
		{
			auto encryptionHandler    = std::make_unique<EncryptionHandler>();
			auto compressionHandler   = std::make_unique<CompressionHandler>();
			auto serializationHandler = std::make_unique<SerializationHandler>();

			encryptionHandler->setNext(compressionHandler.get());
			compressionHandler->setNext(serializationHandler.get());

			return compressionHandler->handleIncomingMessageBody(buffer, message);
		}
	};
}

