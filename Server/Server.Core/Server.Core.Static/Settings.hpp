#pragma once

#include "ArgParser.hpp"

class Settings
{
public:
    explicit Settings(const ArgParser& parser)
    {
        _serverPort     = parser.getPair("--serverport").second;
        _hostAddress    = parser.getPair("--hostaddr").second;
        _dbName         = parser.getPair("--dbname").second;
        _dbPort         = parser.getPair("--port").second;
        _dbUser         = parser.getPair("--user").second;
        _dbPassword     = parser.getPair("--password").second;
    }

    std::string ServerPort() const  { return _serverPort; }
    std::string HostAddress() const { return _hostAddress; }
    std::string DBName() const      { return _dbName; }
    std::string DBPort() const      { return _dbPort; }
    std::string DBUser() const      { return _dbUser; }
    std::string DBPassword() const  { return _dbPassword; }

private:
    std::string         _serverPort;
    std::string         _hostAddress;
    std::string         _dbName;
    std::string         _dbPort;
    std::string         _dbUser;
    std::string         _dbPassword;
};