#ifndef FILETAG_H
#define FILETAG_H

#include "FileSearchRegister.h"

class FileTag : public FileSearchRegister
{
    public:
        FileTag(Database* db);
        virtual ~FileTag();

        std::string getTag() { return tag; }
        void setTag(std::string t) { tag = t; }

    protected:
        virtual void _setKey();

    private:
        std::string tag;
};

#endif // FILETAG_H
