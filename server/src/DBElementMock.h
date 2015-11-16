#ifndef DBELEMENTMOCK_H
#define DBELEMENTMOCK_H

#include "DBElement.h"

class DBElementMock : public DBElement
{
    public:
        DBElementMock(Database* db, std::string c = "", std::string v = "", DatabaseWriteBatch* dbbatch = NULL);
        virtual ~DBElementMock();

        void setMockClave(std::string s) { this->mock_clave = s; }
        void setMockValue(std::string s) { this->mock_value = s; }

        std::string getMockClave() { return this->mock_clave; }
        std::string getMockValue() { return this->mock_value; }
    protected:
    private:
        void _setKey() { key = mock_clave; }
        void _setValue() { value = mock_value; }
        void _setValueVars() { mock_value = value; }

        std::string mock_clave;
        std::string mock_value;
};

#endif // DBELEMENTMOCK_H
