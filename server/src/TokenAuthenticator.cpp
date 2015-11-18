#include "TokenAuthenticator.h"
#include <ctime>

TokenAuthenticator::TokenAuthenticator()
{
    //ctor
}

TokenAuthenticator::~TokenAuthenticator()
{
    //dtor
}

bool TokenAuthenticator::isValidToken(std::string user, std::string token){
    if(active_tokens.find(user) == active_tokens.end()){
        // no existe el usuario
        return false;
    } else {
        std::string _token = active_tokens[user];
        if(token.compare(_token) != 0){
            // las tokens no son iguales
            return false;
        } else {
            return true;
        }
    }
}

std::string TokenAuthenticator::createToken(std::string user){
    std::string token = std::to_string(rand());
    if(active_tokens.find(user) != active_tokens.end()){
        this->removeToken(user);
    }
    active_tokens.emplace(user, token);
    return token;
}

bool TokenAuthenticator::removeToken(std::string user){
    // devuelve 1 (true) si borro, o 0 (false) si no existia la clave
    return active_tokens.erase(user);
}
