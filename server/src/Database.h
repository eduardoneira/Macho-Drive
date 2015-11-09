#ifndef DATABASE_H
#define DATABASE_H

#include "rocksdb/db.h"
#include "rocksdb/status.h"
#include "DBElement.h"

using namespace rocksdb;

class Database
{
    public:
        Database() {}
        virtual ~Database() {}

        virtual Status config(const std::string& db_path, bool) = 0;
        virtual Status open() = 0;
        virtual void close() = 0;
        virtual Status clear_all() = 0;

        virtual Status erase(DBElement &elem) = 0;
        virtual Status put(DBElement &elem) = 0;
        virtual Status get(DBElement &elem) = 0;

    protected:

    private:

};

#endif // DATABASE_H
