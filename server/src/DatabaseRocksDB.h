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
        Si ya estaba abierta no puede ser reconfigurada.
        */
        Status config(const std::string& db_path);

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

        Status erase(DBElement &elem);
        Status put(DBElement &elem);
        Status get(DBElement &elem);

    protected:

    private:

        std::string db_path;
        DB* db;
};

#endif // DATABASEROCKSDB_H
