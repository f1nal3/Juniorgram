# Setting up server on a local machine {#localserversetup}

In order to launch a server locally and connect to it you have to go over these 3 steps:

## Setup database
This is a generic algorithm, a more specific guide on pgAdmin is at the bottom of the document.
1. Open the PostgreSQL Tool for you machine.(e.g. pgAdmin for Windows)
2. Create a database with an arbitrary name.
3. Go to (Scheme script)[https://github.com/f1nal3/Juniorgram/blob/master/Utility/SQL/Database_Scheme.sql]
4. Execute the script from previous point on the created database. 

## Setup server
1. Change the parameters in DataBaseOptions.hpp
  ```c++
    "dbname = your_bdname user = postgres hostaddr = 127.0.0.1 port = 5432 password = your_password";
  ```
2. Then build the project.
3. Go to the \ Juniorgram \ Server \ bin \ Server \ Debug folder and run cmd
4. Run the command in the console: Server.Exec.exe -p 65001

You can also launch it from the Visual Studio(or any other IDE) as long as you specify the arguments.
The server will indicate that it's running correctly in the console.

## Setup client
1. In ServerInfo.cpp, specify the address of the local host
   
    ```c++
      constexpr std::string_view address = "127.0.0.1"
      //and
      constexpr std::uint16_t port = test_server;
    ```
2. Build the Client
3. Launch Client

That's it, if the server is launched and setup correctly, the client will connect.

## Working with pgAdmin:
1. Go to pgAdmin after copying the (Scheme script)[https://github.com/f1nal3/Juniorgram/blob/master/Utility/SQL/Database_Scheme.sql].
2. Right-click on Databases and then Create / Database .
3. Create a table with an arbitrary name.
4. Next, click on the created base and select the Query Tool.
5. Paste the previously copied code and run F5.
