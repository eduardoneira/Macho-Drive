#ifndef DATABASEWRITEBATCH_H_INCLUDED
#define DATABASEWRITEBATCH_H_INCLUDED

#include <vector>
#include <string>

class DBElement;
class Database;

class DatabaseWriteBatch
{
    public:
        DatabaseWriteBatch(Database *_db);
        virtual ~DatabaseWriteBatch();

        void Erase(DBElement *elem);
        void Put(DBElement *elem);

        std::vector<std::string>* getKeys() { return &this->keys; }
        std::vector<std::string>* getValues() { return &this->values; }
        std::vector<std::string>* getOperations() { return &this->operations; }

    protected:
        Database *db;
        std::vector<std::string> keys;
        std::vector<std::string> values;
        std::vector<std::string> operations;

    private:
};

#endif // DATABASEWRITEBATCH_H_INCLUDED
