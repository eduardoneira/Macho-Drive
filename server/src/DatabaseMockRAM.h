#ifndef DATABASEMOCKRAM_H
#define DATABASEMOCKRAM_H

#include "Database.h"
#include <map>
#include <string>

//!Clase mock que maneja la base de datos.
/*!Esta clase emplea como interface Database y sirve de mock para subir a travis\
Utiliza para esto la clase map de C++
*/
class DatabaseMockRAM : public Database
{
    public:
        //!Funcion que inicializa la clase.
        DatabaseMockRAM();
        virtual ~DatabaseMockRAM();

        //!Funcion que establece el path a la base de datos.
        /*!Devuelve status OK.
        */
        Status config(const std::string& db_path, bool);

        //!Funcion que abre la base de datos para ser leida o modificada.
        /*!Devuelve status OK.
        */
        Status open();

        //!Funcion que cierra la base de datos.
        void close();

        //!Funcion que elimina todos los elementos de la base de datos.
        /*!Utiliza la funcion clear de la clase map.
        */
        Status clear_all();

        //!Funcion que elimina un elemento de la base de datos.
        /*!Recibe un elemento, lo busca a partir de su key y lo elimina.
        */
        Status erase(DBElement &elem);

        //!Funcion que elimina un elemento de la base de datos.
        /*!Recibe una key de un elemento, lo busca y lo elimina.
        */
        Status erase(std::string key);

        //!Funcion que agrega un elemento a la base de datos.
        /*!Recibe un elemento y lo introduce a la base de datos.
        */
        Status put(DBElement &elem);

        //!Funcion que agrega un elemento a la base de datos.
        /*!Recibe una key de un elemento, lo busca y lo agrega junto con su value.
        */
        Status put(std::string key, std::string value);

        //!Funcion que obtiene un elemento de la base de datos.
        /*!Recibe un elemento, lo busca a partir de su key y lo obtiene.
        */
        Status get(DBElement &elem);

        //!Funcion que procesa el batch.
        /*!Recibe un Databasewritebatch y lo procesa para aplicarlo en la base de datos.
        */
        Status writeBatch(DatabaseWriteBatch *batch);

    protected:
    private:
        //!Variable db.
        /*!Es la clase map que utiliza para tratar como base de datos.
        */
        std::map<std::string, std::string> db;
};

#endif // DATABASEMOCKRAM_H
