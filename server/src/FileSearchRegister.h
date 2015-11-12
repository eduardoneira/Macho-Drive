#ifndef FILESEARCHREGISTER_H
#define FILESEARCHREGISTER_H

#include "DBElement.h"
#include <string>
#include <vector>

//!Clase que se emplea para realizar busquedas.
/*!Hereda de DBElement.
*/
class FileSearchRegister : public DBElement
{
    public:
        //!Funcion que inicializa DBElement.
        /*!Se inicializa db con el puntero a Database que se pasa.
        */
        FileSearchRegister(Database* db);
        virtual ~FileSearchRegister();

        //!Funcion que devuelve lista de archivos.
        std::vector<std::string>* getFiles() { return &files; }

        //!Funcion que agrega token a lista de archivos.
        void addFileToken(std::string token) { files.push_back(token); }

        //!Funcion que remueve archivo de la lista.
        void removeFileToken(std::string token);

        //!Funcion que establece username.
        /*!Recibe un string con el nombre de usuario y lo asigna a username.
        */
        void setUsername(std::string u) { username = u; }

        //!Funcion que devuelve username.
        std::string getUsername() { return username; }

    protected:
        //!Funcion que establece el value.
        /*!A partir de la lista de archivos genera un json y lo asigna a value.*/
        virtual void _setValue();

        //!Funcion que establece lista de archivos.
        /*!Toma la variable value y de ella obtiene el vector files.
        */
        virtual void _setValueVars();

        //!Variable string username.
        std::string username;

    private:
        //!Variable vector files.
        /*!Contiene un vector con la lista de tokens de los archivos.
        */
        std::vector<std::string> files;

};

#endif // FILESEARCHREGISTER_H
