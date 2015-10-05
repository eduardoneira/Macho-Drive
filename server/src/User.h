#ifndef USER_H
#define USER_H

#include "DBElement.h"
#include "rocksdb/status.h"

using namespace rocksdb;

class User : public DBElement
{
    public:
        User(Database* db);
        virtual ~User();

        void setUsername(std::string usr) { this->username = usr; }
        void setPassword(std::string pass) { this->password = pass; }

        std::string getUsername() { return username; }
        std::string getPassword() { return password; }

        Status DBerase();
        Status DBget();
        Status DBcreate();

    protected:
        virtual void _setKey() { this->key = username; }
        virtual void _setValue() { this->value = password; }
        virtual void _setValueVars() { this->password = value; }

    private:
        std::string username;
        std::string password;
};

#endif // USER_H
