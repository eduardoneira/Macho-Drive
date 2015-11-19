#include "TokenAuthenticator.h"
#include <ctime>
#include "Logger.h"

TokenAuthenticator::TokenAuthenticator()
{
    //ctor
}

TokenAuthenticator::~TokenAuthenticator()
{
    //dtor
}

bool TokenAuthenticator::isValidToken(std::string user, std::string token){
    Server_Logger* log = Server_Logger::getInstance();
    ///log->Log("username : ", user, TRACE);
    ///log->Log("token : %s", token, TRACE);
    log->Log("Se corrobora si existe el usuario",INFO);
    if(active_tokens.find(user) == active_tokens.end()){
        // no existe el usuario
        log->Log("El usuario no existe",WARNING);
        return false;
    } else {
        log->Log("El usuario existe",INFO);
        log->Log("Se corrobora si el token de autentificacion es el correcto",INFO);
        std::string _token = active_tokens[user];
        log->Log("token valido: "+_token, DEBUG);
        if(token.compare(_token) != 0){
            // las tokens no son iguales
            log->Log("El token es incorrecto",WARNING);
            return false;
        } else {
            log->Log("El token es el correcto",INFO);
            return true;
        }
    }
}

std::string TokenAuthenticator::createToken(std::string user){
    std::string token = std::to_string(rand());
    Server_Logger* log = Server_Logger::getInstance();
    //log->Log("user : %s", user, TRACE);
    if(active_tokens.find(user) != active_tokens.end()){
        this->removeToken(user);
    }
    active_tokens.emplace(user, token);
    log->Log("Se crea el token de autentificacion para el usuario",INFO);
    return token;
}

bool TokenAuthenticator::removeToken(std::string user){
    // devuelve 1 (true) si borro, o 0 (false) si no existia la clave
    return active_tokens.erase(user);
}
