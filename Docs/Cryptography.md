# Cryptography system
### Authentication

The main idea of the architecture is the ability to configure the connection at the stage of its creation.

All subsequent statements work for the following cryptography configuration:
- Encryption: AES GCM

- Key exchange protocol: ECDH


Also, this architecture will work for any symmetric encryption algorithm if the generated shared key is used as the key encryption key.

The diagram below shows how the parts of the cryptographic system interact with each other and with the server.

![crypto_one](./images/crypto_authentication.svg "Crypto authentication")

The dependencies shown in the diagram make it possible to configure the security elements separately: the encryption method, the key creation protocol, the key confirmation protocol, the connection verification algorithm - which allows you to configure the connection in a variety of ways.

### Connection

The diagram below shows how the application sides will be connected.

![crypto_two](./images/crypto_connection.svg "Crypto connection")

Connection stages:

1. Setting up an anonymous connection.
2. Connection verification. If it fails, the server cannot be sure that it is working with a real client.
3. If the client has confirmed that he is the one the server knows, a shared key is generated (used for symmetric encryption).
4. As a result of generation, both the client and the server have a key, but they cannot be sure that the other side has exactly the same one. To do this, the client sends an encrypted message with known content. If the server decrypted and received the correct values, it is assumed that both parties received the same key.
