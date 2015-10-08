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

        std::vector<std::string> search_files();

        virtual ~FileQuery();

    protected:
    private:
        std::string username;
        std::string metadata;
        std::string word;
        Database* db;
};

#endif // FILEQUERY_H
