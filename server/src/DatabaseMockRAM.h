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

        Status config(const std::string& db_path);
        Status open();
        void close();
        Status clear_all();

        Status erase(DBElement &elem);
        Status put(DBElement &elem);
        Status get(DBElement &elem);

    protected:
    private:

        std::map<std::string, std::string> db;
};

#endif // DATABASEMOCKRAM_H
