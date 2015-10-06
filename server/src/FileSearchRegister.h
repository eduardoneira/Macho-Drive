#ifndef FILESEARCHREGISTER_H
#define FILESEARCHREGISTER_H

#include "DBElement.h"
#include <string>
#include <vector>

class FileSearchRegister : public DBElement
{
    public:
        FileSearchRegister(Database* db);
        virtual ~FileSearchRegister();

        std::vector<std::string>* getFiles() { return &files; }
        void addFileToken(std::string token) { files.push_back(token); }
        void removeFileToken(std::string token);
        void setUsername(std::string u) { username = u; }
        std::string getUsername() { return username; }

    protected:
        virtual void _setValue();
        virtual void _setValueVars();
        std::string username;

    private:
        std::vector<std::string> files;

};

#endif // FILESEARCHREGISTER_H
