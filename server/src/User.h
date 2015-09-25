#ifndef USER_H
#define USER_H

#include "DBElement.h"

class User : public DBElement
{
    public:
        User();
        virtual ~User();

        void setUsername(std::string usr) { this->username = usr; }
        void setPassword(std::string pass) { this->password = pass; }

    protected:
        virtual void _setKey() { this->key = username; }
        virtual void _setValue() { this->value = password; }

    private:
        std::string username;
        std::string password;
};

#endif // USER_H
