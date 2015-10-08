#ifndef FILEQUERY_H
#define FILEQUERY_H

#include <vector>
#include <string>
#include "Database.h"
#include "rocksdb/status.h"
#include "UserMetadata.h"

class FileQuery
{
    public:
        FileQuery();
        FileQuery(std::string,std::string,std::string,Database* db);

        void search_files();

        std::string getResponse();

        virtual ~FileQuery();

    protected:
    private:
        std::string username;
        std::string metadata;
        std::string word;
        std::vector<std::string> valid_files;
        Database* db;
};

#endif // FILEQUERY_H
