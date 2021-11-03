## Working with pgAdmin:
1. Go to pgAdmin after copying the database code.
 it can be found here:
 https://github.com/f1nal3/Juniorgram/blob/master/Utility/SQL/Database_Scheme.sql
2. Right-click on Databases and then Create / Database ..
3. Create a table with an arbitrary name.
4. Next, click on the created base and select the Query Tool
5. Paste the previously copied code and run F5


## Work in the editor:
1. In ServerInfo.cpp, specify the address of the local host
   
    ```c++
      constexpr std :: string_view address = "127.0.0.1"
      //and
      constexpr std::uint16_t port = test_server;
    ```

2. Change the parameters in DataBaseOptions.hpp
"dbname = your_bdname user = postgres hostaddr = 127.0.0.1 port = 5432 password = your_password"

3. Then build the project.


## Work in the console:
1. Go to the \ Juniorgram \ Server \ bin \ Server \ Debug folder and run cmd
2. Run the command Server.Exec.exe -p 65001
3. Start the client.
4. Done, the server is running locally.
