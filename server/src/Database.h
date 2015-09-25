#ifndef DATABASE_H
#define DATABASE_H

#include "rocksdb/db.h"
#include "rocksdb/status.h"
#include "DBElement.h"

using namespace rocksdb;

class Database
{
    public:
        Database();
        virtual ~Database();

        Status config(const std::string& db_path);
        Status open();
        void close();
        Status clear_all();

        Status erase(DBElement &elem);
        Status put(DBElement &elem);
        Status get(DBElement &elem);

    protected:

    private:

        std::string db_path;
        DB* db;
};

#endif // DATABASE_H
