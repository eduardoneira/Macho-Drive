#include "TokenAuthenticator.h"

TokenAuthenticator::TokenAuthenticator()
{
    //ctor
}

TokenAuthenticator::~TokenAuthenticator()
{
    //dtor
}

bool TokenAuthenticator::isValidToken(std::string user, std::string token){
    return true;
}

std::string TokenAuthenticator::createToken(std::string user){
    std::string token = "token"; // cambiar por token random
    active_tokens.emplace(user, token);
    return token;
}

bool TokenAuthenticator::removeToken(std::string user){
    // devuelve 1 (true) si borro, o 0 (false) si no existia la clave
    return active_tokens.erase(user);
}
