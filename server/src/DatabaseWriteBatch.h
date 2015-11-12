#ifndef DATABASEWRITEBATCH_H_INCLUDED
#define DATABASEWRITEBATCH_H_INCLUDED

#include <vector>
#include <string>

class DBElement;
class Database;

//!Clase que maneja el batch.
/*!Se encarga de administrar las funciones que se pueden colocar en el batch.
*/
class DatabaseWriteBatch
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe un puntero a una clase Database y lo guarda en su variable db.
        */
        DatabaseWriteBatch(Database *_db);
        virtual ~DatabaseWriteBatch();

        //!Funcion que guarda la accion de eliminar un elemento.
        /*!Recibe un elemento y lo agrega a las variables keys y values, asi como agrega erase a operations.
        */
        void Erase(DBElement *elem);

        //!Funcion que guarda la accion de agregar un elemento.
        /*!Recibe un elemento y lo agrega a las variables keys y values, asi como agrega put a operations.
        */
        void Put(DBElement *elem);

        //!Funcion que devuelve el vector keys.
        /*!Devuelve el vector con las keys de los elementos que va a tratar el batch.
        */
        std::vector<std::string>* getKeys() { return &this->keys; }

        //!Funcion que devuelve el vector values.
        /*!Devuelve el vector con los values de los elementos que va a tratar el batch.
        */
        std::vector<std::string>* getValues() { return &this->values; }

        //!Funcion que devuelve el vector operations.
        /*!Devuelve el vector que tiene las acciones a tomar con los elementos almacenados en las otras variables.
        */
        std::vector<std::string>* getOperations() { return &this->operations; }

    protected:
        //!Variable puntero db.
        /*!Apunta a una Database.
        */
        Database *db;

        //!Variable vector keys.
        /*!Contiene las keys de los elementos que procesara el batch.
        */
        std::vector<std::string> keys;

        //!Variable vector values.
        /*!Contiene los values de los elementos que procesara el batch.
        */
        std::vector<std::string> values;

        //!Variable vector operations.
        /*!Contiene las operaciones que procesara el batch.
        */
        std::vector<std::string> operations;

    private:
};

#endif // DATABASEWRITEBATCH_H_INCLUDED
