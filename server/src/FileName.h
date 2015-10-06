#ifndef FILENAME_H
#define FILENAME_H

#include "FileSearchRegister.h"

class FileName : public FileSearchRegister
{
    public:
        FileName(Database* db);
        virtual ~FileName();

        std::string getName() { return name; }
        void setName(std::string n) { name = n; }

    protected:
        virtual void _setKey();

    private:
        std::string name;

};

#endif // FILENAME_H
