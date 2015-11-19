#ifndef DBELEMENTMOCK_H
#define DBELEMENTMOCK_H

#include "DBElement.h"

//! Clase mock que representa a los registros de la base de datos.
/*! Esta clase se encarga de obtener elementos o modificarlos en la base de datos.
*/
class DBElementMock : public DBElement
{
    public:
        //! Funcion que inicializa DBElement.
        /*! Se inicializa los valores de mock_clave y mock_value con "" ,dbbatch con NULL y db con el argumento que se pasa.
        */
        DBElementMock(Database* db, std::string c = "", std::string v = "", DatabaseWriteBatch* dbbatch = NULL);
        virtual ~DBElementMock();

        //!Funcion que establece mockclave.
        void setMockClave(std::string s) { this->mock_clave = s; }
        //!Funcion que establece mockvalue.
        void setMockValue(std::string s) { this->mock_value = s; }

        //!Funcion que devuelve mockclave.
        std::string getMockClave() { return this->mock_clave; }
        //!Funcion que devuelve mockvalue
        std::string getMockValue() { return this->mock_value; }
    protected:
    private:
        //!Funcion que asigna mockclave a la key.
        /*!De esta forma se puede manejar el elemento a traves de funciones definidas en la clase madre.
        */
        void _setKey() { key = mock_clave; }
        //!Funcion que asigna mockvalue al value.
        /*!De esta forma se puede manejar el elemento a traves de funciones definidas en la clase madre.
        */
        void _setValue() { value = mock_value; }
        //!Funcion que asigna value al mockvalue.
        /*!De esta forma se puede manejar el elemento a traves de funciones definidas en la clase madre.
        */
        void _setValueVars() { mock_value = value; }

        //!Variable string con la clave.
        std::string mock_clave;
        //!Variable string con el valor.
        std::string mock_value;
};

#endif // DBELEMENTMOCK_H
