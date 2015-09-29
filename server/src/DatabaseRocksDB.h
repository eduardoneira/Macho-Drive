#ifndef DATABASEROCKSDB_H
#define DATABASEROCKSDB_H

#include "rocksdb/db.h"
#include "rocksdb/status.h"
#include "DBElement.h"
#include "Database.h"

using namespace rocksdb;

class DatabaseRocksDB : public Database
{
    public:
        DatabaseRocksDB();
        virtual ~DatabaseRocksDB();

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

#endif // DATABASEROCKSDB_H
