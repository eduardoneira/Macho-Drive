#ifndef TOKENAUTHENTICATOR_H
#define TOKENAUTHENTICATOR_H

#include <string>
#include <unordered_map>

class TokenAuthenticator
{
    public:
        TokenAuthenticator();
        virtual ~TokenAuthenticator();

        bool isValidToken(std::string user, std::string token);
        std::string createToken(std::string user);
        bool removeToken(std::string user);
    protected:
    private:
        std::unordered_map<std::string, std::string> active_tokens;
};

#endif // TOKENAUTHENTICATOR_H
