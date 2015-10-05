#ifndef FILEEXTENSION_H
#define FILEEXTENSION_H

#include "FileSearchRegister.h"

class FileExtension : public FileSearchRegister
{
    public:
        FileExtension();
        virtual ~FileExtension();

        std::string getExtension() { return extension; }
        void setExtension(std::string ext) { extension = ext; }

    protected:
        virtual void _setKey();

    private:
        std::string extension;

};

#endif // FILEEXTENSION_H
