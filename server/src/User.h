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

        std::string getUsername() { return username; }
        std::string getPassword() { return password; }

    protected:
        virtual void _setKey() { this->key = username; }
        virtual void _setValue() { this->value = password; }
        virtual void _setValueVars() { this->password = value; }

    private:
        std::string username;
        std::string password;
};

#endif // USER_H
