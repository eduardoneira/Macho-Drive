#ifndef DATABASE_H
#define DATABASE_H

//#include "rocksdb/db.h"
#include "rocksdb/status.h"
#include "DBElement.h"
class DatabaseWriteBatch;

using namespace rocksdb;

//!Clase abstracta como interface para base de datos.
/*!Esta clase sirve como interface para las clases \
DatabaseRocksDB y DatabaseMockRAM.
*/

class Database
{
    public:

        //!Funcion que inicializa la clase.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        Database() {}
        virtual ~Database() {}

        //!Funcion que establece el path a la base de datos.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        virtual Status config(const std::string& db_path, bool) = 0;

        //!Funcion que abre la base de datos para ser leida o modificada.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        virtual Status open() = 0;

        //!Funcion que cierra la base de datos.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        virtual void close() = 0;

        //!Funcion que elimina todos los elementos de la base de datos.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        virtual Status clear_all() = 0;

        //!Funcion que elimina un elemento de la base de datos.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        virtual Status erase(DBElement &elem) = 0;

        //!Funcion que agrega un elemento a la base de datos.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        virtual Status put(DBElement &elem) = 0;

        //!Funcion que obtiene un elemento de la base de datos.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        virtual Status get(DBElement &elem) = 0;

        //!Funcion que procesa el batch.
        /*!Esta funcion es redefinida en las clases hijas.
        */
        virtual Status writeBatch(DatabaseWriteBatch *batch) = 0;

    protected:

    private:

};

#endif // DATABASE_H
