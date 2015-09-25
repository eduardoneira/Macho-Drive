#ifndef DBELEMENT_H
#define DBELEMENT_H

#include "rocksdb/slice.h"
#include <string>

using namespace rocksdb;

class DBElement
{
    public:
        DBElement();
        virtual ~DBElement();

        std::string getKey();
        std::string getValue();

        std::string getKeyToString() { return key/*.ToString()*/; }
        std::string getValueToString() { return value/*.ToString()*/; }

        void setKey(std::string key) { this->key = key; }
        void setValue(std::string value) { this->value = value; }

    protected:

        virtual void _setKey() = 0;
        virtual void _setValue() = 0;

        std::string key;
        std::string value;

    private:

};

#endif // DBELEMENT_H
