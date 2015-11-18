#ifndef DATABASEROCKSDB_H
#define DATABASEROCKSDB_H

#include "rocksdb/db.h"
#include "rocksdb/status.h"
#include "Database.h"

//!Clase que maneja la base de datos con rocksdb.
/*!Esta clase emplea como interface Database y utiliza rocksdb para cumplir sus funciones.
*/
class DBElement;

using namespace rocksdb;

class DatabaseRocksDB : public Database
{
    public:

        //!Funcion que inicializa la clase.
        /*!Inicializa db en NULL y deb_path en "".
        */
        DatabaseRocksDB();
        virtual ~DatabaseRocksDB();

        //!Funcion que establece el path a la base de datos.
        /*!Recibe un string con el path donde debe establecer la base de datos\
        Si ya estaba abierta no puede ser reconfigurada\
        Tambien recibe un bool y lo asigna a create_if_missing.
        */
        Status config(const std::string& db_path, bool create_if_missing);

        //!Funcion que abre la base de datos para ser leida o modificada.
        /*!Si el db no es NULL o si el db_path es "" devuelve error\
        Si no ubica el db con el db_path.
        */
        Status open();

        //!Funcion que cierra la base de datos.
        /*!Si db no es NULL, lo convierte en NULL.
        */
        void close();

        //!Funcion que elimina todos los elementos de la base de datos.
        /*!Si db no es NULL elimina los elementos de la base de datos.
        */
        Status clear_all();

        //!Funcion que elimina un elemento de la base de datos.
        /*!Recibe un elemento, lo busca a partir de su key y lo elimina.
        */
        Status erase(DBElement &elem);

        //!Funcion que agrega un elemento a la base de datos.
        /*!Recibe un elemento y lo introduce a la base de datos.
        */
        Status put(DBElement &elem);

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

        //!Variable db_path.
        /*!Esta variable contiene el path a la base de datos.
        */
        std::string db_path;

        //!Variable create_if_missing.
        /*!Es un booleno utilizado por la funcion open.
        */
        bool create_if_missing;

        //!Variable db.
        /*!Es el puntero a la base de datos.
        */
        DB* db;
};

#endif // DATABASEROCKSDB_H
