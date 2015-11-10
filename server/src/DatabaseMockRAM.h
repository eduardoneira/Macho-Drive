#ifndef DATABASEMOCKRAM_H
#define DATABASEMOCKRAM_H

#include "Database.h"
#include <map>
#include <string>

class DatabaseMockRAM : public Database
{
    public:
        DatabaseMockRAM();
        virtual ~DatabaseMockRAM();

        Status config(const std::string& db_path, bool);
        Status open();
        void close();
        Status clear_all();

        Status erase(DBElement &elem);
        Status erase(std::string key);
        Status put(DBElement &elem);
        Status put(std::string key, std::string value);
        Status get(DBElement &elem);
        Status writeBatch(DatabaseWriteBatch *batch);

    protected:
    private:

        std::map<std::string, std::string> db;
};

#endif // DATABASEMOCKRAM_H
