#ifndef USERMETADATA_H
#define USERMETADATA_H

#include "DBElement.h"
#include <vector>
#include <string>

class UserMetadata : public DBElement
{
    public:
        UserMetadata();
        virtual ~UserMetadata();

        void setUserToken(std::string token) { this->user_token = token; }
        void addMyFileToken(std::string token) { this->my_file_tokens.push_back(token); }
        void addSharedFileToken(std::string token) { this->shared_file_tokens.push_back(token); }
        void setJoinDate(std::string date) { this->join_date = date; }

        std::vector<std::string>* const getMy_file_tokens() { return &my_file_tokens; }
        std::vector<std::string>* const getShared_file_tokens() { return &shared_file_tokens; }
        std::string getJoinDate() { return join_date; }

    protected:

        virtual void _setKey() { this->key = this->user_token; }
        virtual void _setValue();

    private:
        std::string user_token;

        std::vector<std::string> my_file_tokens;
        std::vector<std::string> shared_file_tokens;
        std::string join_date;
        // agregar lo que haga falta
};

#endif // USERMETADATA_H
