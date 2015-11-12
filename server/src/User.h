#ifndef USER_H
#define USER_H

#include "DBElement.h"
#include "rocksdb/status.h"

using namespace rocksdb;

//!Clase que maneja al usuario.
/*!Esta clase hereda de DElement.
*/
class User : public DBElement
{
    public:
        //!Funcion que inicializa la clase.
        /*!Esta funcion emplea la misma inicializacion que DBElement.
        */
        User(Database* db, DatabaseWriteBatch* dbbatch = NULL);
        virtual ~User();

        //!Funcion que establece el username.
        /*!Recibe un string y lo asigna a username.
        */
        void setUsername(std::string usr) { this->username = usr; }

        //!Funcion que establece la password.
        /*!Recibe un string y lo asigna a password.
        */
        void setPassword(std::string pass) { this->password = pass; }

        //!Funcion que retorna el username.
        std::string getUsername() { return username; }

        //!Funcion que retorna la password.
        std::string getPassword() { return password; }

        //!Funcion que elimina al user de la base de datos.
        Status DBerase();

        //!Funcion que devuelve el estado de su busqueda en la base de datos.
        /*!Se busca a si mismo en la base de datos y devuelve el status resultante de eso.
        */
        Status DBget();

        //!Funcion que crea al usuario en la base de datos.
        /*!En caso de no existir previamente, crea un usuario con el username del objeto.
        */
        Status DBcreate();

    protected:
        //!Funcion que asigna username a la key.
        /*!De esta forma se puede manejar el elemento a traves de funciones definidas en la clase madre.
        */
        virtual void _setKey() { this->key = username; }

        //!Funcion que asigna password al value.
        /*!De esta forma se puede manejar el elemento a traves de funciones definidas en la clase madre.
        */
        virtual void _setValue() { this->value = password; }

        //!Funcion que asigna value a la password.
        /*!De esta forma se puede manejar el elemento a traves de funciones definidas en la clase madre.
        */
        virtual void _setValueVars() { this->password = value; }

    private:
        std::string username;
        std::string password;
};

#endif // USER_H
