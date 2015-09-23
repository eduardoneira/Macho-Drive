#ifndef DATABASE_H
#define DATABASE_H

#include "rocksdb/db.h"
#include "rocksdb/status.h"

using namespace rocksdb;

class Database
{
    public:
        Database();
        virtual ~Database();

        Status config(const std::string& db_path);
        Status create();

    protected:

    private:

        std::string db_path;
        DB* db;
};

#endif // DATABASE_H
