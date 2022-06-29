<h1>I/O operations.<br>Boost.Asio</h1>

<h4>CONTENT </h4>
 

[1. I/O operations](#IOoperations) <br>
[2. Data integrity](#dataIntegrity) <br>
[3. Error and exception handling](#errorHandling) <br>
[4. Big file transmissions](#bigFileTransmissions) <br>
[5. Usage in Open Source projects](#bigProjects) <br>
[6. To read](#toRead) <br>

<a name="IOoperations"><h2>[I/O operations](#1)</h2></a>

<i>I will consider everything with the example of asynchronous networking over TCP/IP. <br> </i>

There are several functions for writing and reading operations:

Basis functions that perform the I/O on the socket:
* ```async_read_some(buffer, handler)```
* ```async_write_some(buffer)```

Basis functions that read from or write to a stream:
* ```async_read(stream, buffer [, completion], handler)```
* ```async_write(stream, buffer [, completion], handler)```

First, note that the first argument is a stream. This includes sockets but is not limited. 
For instance, instead of a socket, you can use a Windows file handle.

Each read or write operation will end when one of these conditions occur:
* The supplied buffer is full (for read) or all the data in the buffer has been written (for write)
* The completion function returns 0 (if you supplied one such function)
* An error occurs

<a name="comletion"><h3> The completion </h3></a>

Signature: 
```c++ 
size_t completion(const boost::system::error_code& err, size_t bytes_transfered);
```

A completion condition is a function object that is used with the algorithms ```read```, ```async_read```, ```write```, and 
```async_write``` to determine when the algorithm has completed transferring data.

You can optionally specify a completion function. It is called after each successful read, and tells Boost.Asio 
if the ```async_read``` operation is complete (if not, it will continue to read). 

When this completion function returns 0, we consider the read operation complete. 

If it returns a non-zero value, it indicates the maximum number of bytes to be read on the next call to 
the stream's ```async_read_some``` operation. 

Boost.Asio comes with a few helper completion functors as well:
* transfer_at_least(n)
* transfer_exactly(n)
* transfer_all()

<a name="handler"><h3> The handler </h3></a>

Signature: 
```c++
void handler(const boost::system::error_code& err, size_t bytes_transfered);
```

Each asynchronous operation has a handler, a function that is called when the operation has completed.

For example, the ```asio::async_read()``` function initiates the operation that reads the data from the socket until 
the buffer passed to it as an argument is full. In this case, the callback (in our case it’s called handler) 
is called when the amount of data read is equal to the size of the provided buffer or when an error occurs. 

You can specify your own handler implementation and bind it with async_read.

```c++
// Alias for the error handler functor
const auto myHandler = [this](std::error_code error) {
       // realization
};

asio::async_read(stream, socket,
                      bind(myHandler, std::placeholders::_1)); 
```

<a name="reading"><h3> [Reading from a TCP socket asynchronously](#1) </h3></a>

<a name="async_read_some"><h4> async_read_some() </h4></a>

The most basic tool used to asynchronously read data from a TCP socket provided by the Boost.Asio library is 
the ```async_read_some()``` method of the ```asio::ip::tcp::socket``` class. 

Here is one of the method's overloads:
```c++
template<
    typename MutableBufferSequence,
    typename ReadHandler>
void async_read_some(
    const MutableBufferSequence & buffers,
    ReadHandler handler);
```

The ```async_read_some()``` method initiates an operation that is intended to read some amount of data from 
the socket to the buffer. This method guarantees that <b>at least one byte</b> will be read during the corresponding 
asynchronous operation if an error does not occur. This means that, in a general case, in order to read all 
the data from the socket, we may need to perform this asynchronous operation several times.

<h5> Algorithm </h5>

The following algorithm describes the steps required to implement an application, which reads data from 
a socket asynchronously. Note that this algorithm provides a possible way to implement such an application.

1. Define a data structure that contains a pointer to a socket object, a buffer, a variable 
that defines the size of the buffer, and a variable used as a counter of bytes read.
2. Define a callback function that will be called when an asynchronous reading 
operation is completed.
3. In a client application, allocate and open an active TCP socket, and then, connect it to 
a remote application. In a server application, obtain a connected active TCP socket by 
accepting a connection request.
4. Allocate a buffer big enough for the expected message to fit in.
5. Initiate an asynchronous reading operation by calling the socket's async_read_
some() method, specifying a function defined in step 2 as a callback.
6. Call the run() method on an object of the asio::io_service class.
7. In a callback, increase the counter of bytes read. If the number of bytes read is less 
than the total amount of bytes to be read (that is, the size of an expected message), 
initiate a new asynchronous reading operation to read the next portion of data.

<h4> Code example </h4>

```c++
#include <asio.hpp>
#include <iostream>

// Step 1.
// Keeps objects we need in a callback to identify whether all data has been read from the socket and to
// initiate next async reading operation if needed.
struct Session 
{
      std::shared_ptr<asio::ip::tcp::socket> sock;
      std::unique_ptr<char[]> buf;
      std::size_t total_bytes_read;
      unsigned int buf_size;
};

// Step 2.
// Function used as a callback for asynchronous reading operation. Check if all data has been read from the
// socket and initiates new reading operation if needed.
void callback(const asio::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> s) 
{
      if (ec.value() != 0) 
      {
        logging::get()->error("Error occurred! Error code = {}. Message: {}", ec.value(), ec.message());
        return;
      }

      // Step 7. Increasing the counter  of bytes read.
      s->total_bytes_read += bytes_transferred;
      if (s->total_bytes_read == s->buf_size) 
      {
        logging::get()->info("Total Bytes Read = {}", s->total_bytes_read);
        return;
      }

      s->sock->async_read_some(
          asio::buffer(s->buf.get() + s->total_bytes_read, s->buf_size - s->total_bytes_read),
          std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

void readFromSocket(std::shared_ptr<asio::ip::tcp::socket> sock) 
{
      auto s = std::make_shared<Session>();

      // Step 4. Allocating the buffer;
      const unsigned int MESSAGE_SIZE = 7;

      s->buf.reset(new char[MESSAGE_SIZE]);
      s->total_bytes_read = 0;
      s->sock = sock;
      s->buf_size = MESSAGE_SIZE;

      // Step 5. Initiating asynchronous reading operation.
      s->sock->async_read_some(asio::buffer(s->buf.get(), s->buf_size),
                               std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

int main() 
{
      auto console = logging::setup();

      std::string raw_ip_address = "127.0.0.1";
      unsigned short port_num = 3333;

      try 
      {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_service ios;

        // Step 3. Allocating, opening and connecting a socket.
        auto sock = std::make_shared<asio::ip::tcp::socket>(ios, ep.protocol());
        sock->connect(ep);

        readFromSocket(sock);

        // Step 6.
        ios.run();

      } 
      catch (asio::system_error& e) 
      {
        console->error("Error occurred! Error code = {}. Message: {}", e.code(), e.what());
        return e.code().value();
      }

      return 0;
}
```

The read operation ```async_read_some``` may not read all of the requested number of bytes. Consider using 
the ```async_read``` function if you need to ensure that the requested amount of data is read before the asynchronous 
operation completes.

In last boost standards this function is <b>deduced</b>.
More information you can see in [Boost Documentation](https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/reference/basic_stream_socket/async_read_some.html).

<a name="async_read"><h4> async_read() </h4></a>

Although the async_read_some() method allows asynchronously reading data from the socket or the stream, the solution 
based on it is somewhat complex and error-prone. Fortunately, Boost.Asio provides a more convenient way to asynchronously 
read data from a socket: the free function ```asio::async_read()```. 

Use the ```async_read``` function if you need to ensure that the requested amount of data is read before the asynchronous 
operation completes.

Let's consider one of its overloads:
```c++
template<
    typename AsyncReadStream,
    typename MutableBufferSequence,
    typename ReadHandler>
void async_read(
    AsyncReadStream & s,
    const MutableBufferSequence & buffers,
    ReadHandler handler);
```

More overloads you can see in [Boost Documentation](https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/reference/async_read.html).

<h4> Algorithm </h4>

The asynchronous reading algorithm is the same as when using the function ```async_read_some``` except for item 7 of 
the algorithm. It doesn't exists.

<h4> Code example </h4>

```c++
#include <asio.hpp>
#include <iostream>

// Step 1.
// Keeps objects we need in a callback to identify whether all data has been read from the socket and to
// initiate next async reading operation if needed.
struct Session 
{
      std::shared_ptr<asio::ip::tcp::socket> sock;
      std::unique_ptr<char[]> buf;
      unsigned int buf_size;
};


// Step 2.
// Function used as a callback for asynchronous reading operation. Checks if all data has been read from the
// socket and initiates new readnig operation if needed.
void callback(const asio::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> s) 
{
      if (ec.value() != 0) 
      {
        logging::get()->error("Error occurred! Error code = {}. Message: {}", ec.value(), ec.message());
        return;
      }

      // Here we know that the reading has completed successfully and the buffer is full with data read from the
      // socket.
      logging::get()->info("Total Bytes Read = {}", bytes_transferred);
      logging::get()->info("Message: {}", std::string(s->buf.get()));
}

void readFromSocket(std::shared_ptr<asio::ip::tcp::socket> sock) 
{
      auto s = std::make_shared<Session>();

      const unsigned int MESSAGE_SIZE = 7;

      // Step 4. Allocating the buffer.
      s->buf.reset(new char[MESSAGE_SIZE]);
      s->sock = sock;
      s->buf_size = MESSAGE_SIZE;

      // Step 5. Initiating asynchronous reading opration.
      asio::async_read(*sock, asio::buffer(s->buf.get(), s->buf_size),
                       std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

int main() 
{
      auto console = logging::setup();

      std::string raw_ip_address = "127.0.0.1";
      unsigned short port_num = 3333;

      try 
      {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_service ios;

        // Step 3. Allocating, opening and connecting a socket.
        auto sock = std::make_shared<asio::ip::tcp::socket>(ios, ep.protocol());
        sock->connect(ep);

        readFromSocket(sock);

        // Step 6.
        ios.run();
      } 
      catch (asio::system_error& e) 
      {
        console->error("Error occurred! Error code = {}. Message: {}", e.code(), e.what());
        return e.code().value();
      }

      return 0;
}
```

<a name="writting"><h3> Writing to a TCP socket asynchronously </h3></a>

<a name="async_write_some"><h4> async_write_some() </h4></a>

The most basic tool used to asynchronously write data to the socket provided by the Boost.
Asio library is the ```async_write_some()``` method of the ```asio::ip::tcp::socket``` class. 
Let's take a look at one of the method's overloads:

```c++ 
template<
    typename ConstBufferSequence,
    typename WriteHandler>
void async_write_some(
    const ConstBufferSequence & buffers,
    WriteHandler handler);
```

The ```async_write_some()``` method initiates an operation that 
is intended to write some amount of data from the buffer to the socket. This method 
guarantees that at least one byte will be written during the corresponding asynchronous 
operation if an error does not occur. This means that, in a general case, in order to write all 
the data available in the buffer to the socket, we may need to perform this asynchronous 
operation several times.

<h5> Algorithm </h5>

The following algorithm describes the steps required to perform and implement an 
application, which writes data to a TCP socket asynchronously. Note that this algorithm 
provides a possible way to implement such an application. 

1. Define a data structure that contains a pointer to a socket object, a buffer, and a 
variable used as a counter of bytes written.
2. Define a callback function that will be called when the asynchronous writing 
operation is completed.
3. In a client application, allocate and open an active TCP socket and connect it to a 
remote application. In a server application, obtain a connected active TCP socket by 
accepting a connection request.
4. Allocate a buffer and fill it with data that is to be written to the socket.
5. Initiate an asynchronous writing operation by calling the socket's async_write_
some() method. Specify a function defined in step 2 as a callback.
6. Call the run() method on an object of the asio::io_service class.
7. In a callback, increase the counter of bytes written. If the number of bytes written is 
less than the total amount of bytes to be written, initiate a new asynchronous writing 
operation to write the next portion of the data.

<h5> Code example </h5>

```c++
#include <asio.hpp>
#include <iostream>
#include <memory>

// Step 1.
// Keeps objects we need in a callback to identify whether all data has been written to the socket and to
// initiate next async writing operation if needed.
struct Session {
      std::shared_ptr<asio::ip::tcp::socket> sock;
      std::string buf;
      std::size_t total_bytes_written;
};

// Step 2.
// Function used as a callback for asynchronous writing operation. Checks if all data from the buffer has been
// written to the socket and initiates new asynchronous writing operation if needed.
void callback(const asio::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> s) 
{
      if (ec.value() != 0) 
      {
        logging::get()->error("Error occurred! Error code = {}. Message: {}", ec.value(), ec.message());
        return;
      }

      // Step 7. Increasing y=the counter of bytes written.
      s->total_bytes_written += bytes_transferred;
      if (s->total_bytes_written == s->buf.length()) 
      {
        return;
      }

      s->sock->async_write_some(
          asio::buffer(s->buf.c_str() + s->total_bytes_written, s->buf.length() - s->total_bytes_written),
          std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

void writeToSocket(std::shared_ptr<asio::ip::tcp::socket> sock) 
{
      // We need the Session object allocated it in the free memory and not on the stack; it must live until the
      // callback function is called.
      auto s = std::make_shared<Session>();

      // Step 4. Allocating and filling the buffer.
      s->buf = std::string("Hello\n");
      s->total_bytes_written = 0;
      s->sock = sock;

      // Step 5. Initiating asynchronous write operation.
      s->sock->async_write_some(asio::buffer(s->buf),
                                std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

int main() {
      auto console = logging::setup();

      std::string raw_ip_address = "127.0.0.1";
      unsigned short port_num = 3333;

      try 
      {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_service ios;

        // Step 3. Allocating, opening and connecting a socket.
        auto sock = std::make_shared<asio::ip::tcp::socket>(ios, ep.protocol());
        sock->connect(ep);

        writeToSocket(sock);

        // Step 6.
        // The run() method blocks, as long as, at least one pending asynchronous operation. When the last
        // callback of the last pending asynchronous operation is completed, this method returns.
        ios.run();
      } 
      catch (asio::system_error& e) 
      {
        console->error("Error occurred! Error code = {}. Message: {}", e.code().value(), e.what());
        return e.code().value();
      }

      return 0;
}
```
The write operation ```async_write_some``` may not write all of the requested number of bytes. Consider using 
the ```async_write``` function if you need to ensure that the requested amount of data is written before the asynchronous 
operation completes.

In last boost standards this function is <b>deduced</b>.
More information you can see in [Boost Documentation](https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/reference/basic_stream_socket/async_write_some.html).

<a name="async_write"><h4> async_write() </h4></a>

Although the ```async_write_some()``` method allows asynchronously writing data to the socket, the solution based on 
it is somewhat complex and error-prone. Fortunately, Boost.Asio provides a more convenient way to asynchronously 
write data to a socket using the free function asio::async_write(). 

Let's consider one of its overloads:

```c++
template<
    typename AsyncWriteStream,
    typename ConstBufferSequence,
    typename WriteHandler>
void async_write(
    AsyncWriteStream & s,
    const ConstBufferSequence & buffers,
    WriteHandler handler);
```

<h4> Algorithm </h4>

The asynchronous write algorithm is the same as when using the function ```async_write_some``` except for item 7 of 
the algorithm. It doesn't exists.

<h4> Code example </h4>

```c++
#include <asio.hpp>
#include <iostream>

using namespace boost;

// Step 1.
// Keeps objects we need in a callback to
// identify whether all data has been written
// to the socket and to initiate next async
// writing operatino if needed.
struct Session 
{
      std::shared_ptr<asio::ip::tcp::socket> sock;
      std::string buf;
};

// Step 2.
// Function used as a callback for asynchronous writing operation. Checks if all data from the buffer has been
// written to the socket and initiates new writting operation if needed.
void callback(const asio::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> s) 
{
      if (ec.value() != 0) 
      {
        logging::get()->error("Error occurred! Error code = {}. Message: {}", ec.value(), ec.message());
        return;
      }

      // Here we know that all the data has been written to the socket.
      logging::get()->info("Bytes read: {}", bytes_transferred);
      logging::get()->info("Message: {}", s->buf);
}

void writeToSocket(std::shared_ptr<asio::ip::tcp::socket> sock)
{
      std::shared_ptr<Session> s(new Session);

      // Step 4. Allocating and filling the buffer.
      s->buf = std::string("Hello\n");
      s->sock = sock;

      // Step 5. Initiating asynchronous write operation.
      asio::async_write(*sock, asio::buffer(s->buf),
                        std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

int main() 
{
      auto console = logging::setup();

      std::string raw_ip_address = "127.0.0.1";
      unsigned short port_num = 3333;

      try 
      {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_service ios;

        // Step 3. Allocating, opening and connecting a socket.
        std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));
        sock->connect(ep);

        writeToSocket(sock);

        // Step 6.
        ios.run();
      } 
      catch (asio::system_error& e) 
      {
        console->error("Error occurred! Error code = {}. Message: {}", e.code().value(), e.what());
        return e.code().value();
      }

      return 0;
}
```

<a name="dataIntegrity"><h2> Data integrity </h2></a>

If talking about data integrity functions such as ```async_read```, ```async_write``` guarantees that all of the requested number
of bytes will be read to the stream or written from the stream. This functions will wait all bytes and only after receiving/sending all 
of them the handler will be called. But if we specify the completion function, reading/writing transfering may be stopped and 
handler called when the completion function returns 0.

<a name="errorHandling"><h2> [Error and exception handling](#3) </h2></a>

Boost::asio gives 2 ways of handling errors and exceptions:
* using try-cath block
* using error codes

All the <b>synchronous functions</b> have overloads that either throw in case of error or can return an error code. In case 
the function throws, it will always throw a ```boost::system::system_error``` error.

In case you're using <b>asynchronous functions</b>, they all return an error code, which you can examine in your callback. 
Asynchronous functions never throw an exception, as it would make no sense to do so.

In your synchronous functions, you can use exceptions or error codes (whatever you wish), but do it consistently. 
Mixing them up can cause problems and most of the time crashes (when you forget to handle a thrown exception, by mistake). 
If your code is complex (socket read/write function calls), you should probably prefer exceptions and embody your reads/writes 
in the try {} catch block of a function.

All Boost.Asio error codes are in namespace ```boost::asio::error```.

<h4> Code example </h4>

```c++
// throwing exceptions
try 
{
    sock.connect(ep);
} 
catch(boost::system::system_error e) 
{
    std::cout << e.code() << std::endl;
}

// using error codes
boost::system::error_code err;
sock.connect(ep, err);
if ( err) 
{
    std::cout << err << std::endl;
}
```

<a><h3> [Delayed processing exceptions](#2)</h3></a>

Processing exceptions is not always trivial and may take a lot of time. Consider the situation where an exception must be 
serialized and sent by the network. This may take milliseconds and a few thousand lines of code. After the exception is caught is not 
always the best time and place to process it.

The Boost.Exception library provides an ability to store and rethrow exceptions. The ```boost::current_exception()``` method 
must be called from inside the catch() block, and it returns an object of the type ```boost::exception_ptr```.

The only way to restore the exception type from ```boost::exception_ptr``` is to rethrow it using 
```boost::rethrow_exception(exc)``` and then catch it by specifying the exception type.

Usually, exception_ptr is used to pass exceptions between threads. For example:

```c++
void run_throw(boost::exception_ptr& ptr) 
{
    try 
    {
        // A lot of code goes here
    } 
    catch (...) 
    {
        ptr = boost::current_exception();
    }
}

int main ()     
{
     boost::exception_ptr ptr;

     // Do some work in parallel
     boost::thread t(boost::bind(
         &run_throw, 
         boost::ref(ptr)
     ));

     // Some code goes here
     // …
     t.join();

     // Checking for exception
     if (ptr) 
     {
         // Exception occured in thread
         boost::rethrow_exception(ptr);
     }
}
```

<a name="bigFileTransmissions"><h2>Big file transmissions</h2></a>

<h3>Algorithm</h3>

1. Create file if not exists.
2. Open file in reading/writing mode.
3. Transfer the file data over network / Get data and write it into the file.
4. Close file.

What we can use?

<b> 1. Transferring file using ```async_read_some```, ```async_write_some``` functions </b>

The way how to transfer data using ```async_read_some```, ```async_write_some``` was described before in chapters 
about asynchronous [reading](#async_read_some) and [writing](#async_write_some).

But there may be cases when data size is too big for creating a buffer for it.

<b> 2. Transferring [composite buffers](#1) using ```async_read```, ```async_write``` functions </b>

A composite buffer is basically a complex buffer that consists of two or more simple buffers (contiguous blocks of memory) distributed over the process' address space. Such buffers become especially handy in two situations. 

* need to store message in a buffer, but message is so huge as program may fail due to the process' address space fragmentation. 
In this case, allocating multiple smaller buffers, whose sizes when summed would be enough to store the data, and combining them 
in a single composite buffer is a good solution to the problem. 

* due to specificity of the design of the application, the message to be sent to the remote application is broken into several parts 
and stored in different buffers, or if the message to be received from the remote application needs to be broken into several parts, 
each of which should be stored in a separate buffer for further processing.

<h3> Algorithm </h3>

<h4> 1. Preparing a composite buffer for gather output operations </h4>

The following is the algorithm and corresponding code sample that describe how to prepare the composite buffer that is to be used 
with the socket's method that performs output operations such as ```asio::ip::tcp::socket::send()``` or a free function such as 
```asio::write()```:
 
1. Allocate as many memory buffers as needed to perform the task at hand. Note that this step does not involve any functionality 
or data types from Boost.Asio. 
2. Fill the buffers with data to be output. 
3. Create an instance of a class that satisfies the ConstBufferSequence or MultipleBufferSequence concept's requirements, 
representing a composite buffer.
4. Add simple buffers to the composite buffer. Each simple buffer should be represented as an instance of the 
```asio::const_buffer``` or ```asio::mutable_buffer``` classes. 
5. The composite buffer is ready to be used with Boost.Asio output functions.

<h5> Code Example </h5>

```c++
#include <boost/asio.hpp>
using namespace boost;

int main() 
{ 
    // Steps 1 and 2. Create and fill simple buffers. 
    const char* part1 = "Hello ";
    const char* part2 = "my "; 
    const char* part3 = "friend!"; 

    // Step 3. Create an object representing a composite buffer. 
    std::vector composite_buffer; 

    // Step 4. Add simple buffers to the composite buffer. 
    composite_buffer.push_back(asio::const_buffer(part1, 6));   
    composite_buffer.push_back(asio::const_buffer(part2, 3)); 
    composite_buffer.push_back(asio::const_buffer(part3, 7)); 

    // Step 5. Now composite_buffer can be used with Boost.Asio 
    // output operations as if it was a simple buffer represented 
    // by contiguous block of memory. 

    return 0; 
}
```

<h4> 2. Preparing a composite buffer for an input operation </h4>

The following is the algorithm and corresponding code sample that describe how to prepare the composite buffer that is to be used 
with the socket's method that performs an input operation such as ```asio::ip::tcp::socket::receive()``` or a free function such as 
```asio::read()```: 

1. Allocate as many memory buffers as required to perform the task at hand. 
The sum of the sizes of the buffers must be equal to or greater than the size of the expected message to be received in these buffers. 
Note that this step does not involve any functionalities or data types from Boost.Asio. 
2. Create an instance of a class that satisfies the MutableBufferSequence concept's requirements that represents a composite buffer. 
3. Add simple buffers to the composite buffer. Each simple buffer should be represented as an instance of 
the ```asio::mutable_buffer``` class. 
4. The composite buffer is ready to be used with Boost.Asio input operations.

<h5> Code Example </h5>

Let's imagine a hypothetical situation, where we want to receive 16 bytes long messages from the server. However, we do not have 
a buffer that can fit the entire message. Instead, we have three buffers: 6, 3, and 7 bytes long. To create a buffer in which we 
can receive 16 bytes of data, we can join our three small buffers into a composite one. This is how we do it in the following code: 

```c++
#include <boost/asio.hpp>
using namespace boost;

int main() 
{ 
    // Step 1. Allocate simple buffers. 
    char part1[6]; 
    char part2[3]; 
    char part3[7]; 

    // Step 2. Create an object representing a composite buffer. 
    std::vector composite_buffer; 

    // Step 3. Add simple buffers to the composite buffer object. 
    composite_buffer.push_back(asio::mutable_buffer(part1, sizeof(part1))); 
    composite_buffer.push_back(asio::mutable_buffer(part2, sizeof(part2))); 
    composite_buffer.push_back(asio::mutable_buffer(part3, sizeof(part3))); 

    // Step 4. Now composite_buffer can be used with Boost.Asio 
    // input operation as if it was a simple buffer 
    // represented by contiguous block of memory. 

    return 0; 
}
```

<a name="bigProjects"><h2>Usage in Open Source projects</h2></a>
There are some libraries such that are based on Boost.Asio.

* [libpion](https://github.com/splunk/pion) - C++ framework for building lightweight HTTP interfaces
* [cpp-netlib](https://sourceforge.net/projects/cpp-netlib/) - a collection of network-related routines/implementations 
geared towards providing a robust cross-platform networking library. <br>
Cpp-netlib offers the following implementations:
  * Common Message Type - a generic message type which can be used to encapsulate and store message-related 
  information, used by all network implementations as the primary means of data exchange.
  * Network protocol message parsers - a collection of parsers which generate message objects from strings.
  * Adapters and Wrappers - a collection of Adapters and wrappers aimed towards making the message type STL 
  friendly.
  * Network protocol client and server implementations - a collection of network protocol implementations 
  that include embeddable client and server types.

* [libtorrent](https://www.rasterbar.com/products/libtorrent/) -  a feature complete C++ bittorrent implementation 
focusing on efficiency and scalability. It runs on embedded devices as well as desktops. It comes with a simple bittorrent client demonstrating the use of the library.

<a name="toRead"><h2>To read</h2></a>

<a name="1">1. Dmytro Radchuk. Boost.Asio C++ Network Programming Cookbook. [repo](https://github.com/ct-clmsn/asio-network-programming-cookbook/tree/master/src/ch02)</a><br>
<a name="2">2. Antony Polukhin. Boost C++ Application Development Cookbook [(Chapter 6)](https://github.com/apolukhin/Boost-Cookbook/tree/second_edition/Chapter06).</a><br>
<a name="3">3. John Torjo. Boost.Asio C++ Network Programming.</a><br>
<a name="4">4. [Boost Documentation](https://www.boost.org)</a><br>
5. [Presentation](https://docs.google.com/presentation/d/16wfo8neJ2Y-g9KLuwEDjb6MxgsnPrpVa9FnWXQKSYAs/edit?usp=sharing)