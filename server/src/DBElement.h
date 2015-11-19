#ifndef DBELEMENT_H
#define DBELEMENT_H

#include "rocksdb/slice.h"
#include "rocksdb/status.h"
#include <string>

class Database;
class DatabaseWriteBatch;

using namespace rocksdb;

//! Clase que representa a los registros de la base de datos.
/*! Esta clase se encarga de obtener elementos o modificarlos en la base de datos usando rocksdb\
De esta clase heredan luego los distintos tipos de registros que pueden darse.
*/

class DBElement
{
    public:

        //! Funcion que inicializa DBElement.
        /*! Se inicializa los valores de key y value con "" y db con el argumento que se pasa.
        */
        DBElement(Database* db, DatabaseWriteBatch* dbbatch = NULL);
        virtual ~DBElement();

        //!Funcion que devuelve el valor key.
        /*!Esta funcion se emplea para poder obtener la key de las clases hijas sin perder la abstraccion.
        */
        std::string getKey();

        //!Funcion que devuelve el valor value.
        /*!Esta funcion se emplea para poder obtener el value de las clases hijas sin perder la abstraccion.
        */
        std::string getValue();

        //!Funcion que devuelve el valor key.
        /*!Simplemente devuelve la key sin hacer ningun cambio.
        */
        std::string getKeyToString() { return key/*.ToString()*/; }

        //!Funcion que devuelve el valor vaue.
        /*!Simplemente devuelve el valor value sin hacer ningun cambio.
        */
        std::string getValueToString() { return value/*.ToString()*/; }

        //!Funcion que establece la key.
        /*!La funcion recibe un string y lo establece como key.
        */
        void setKey(std::string key) { this->key = key; }

        //!Funcion que establece el value.
        /*!Recibe un string de la base de datos y lo guarda en el valor value\
        Ademas de esto emplea _setValueVars para que cada clase hija maneje estos datos como deba.
        */
        void setValue(std::string value);

        //!Funcion que devuelve su estado en la base de datos.
        /*!Se busca en la base de datos y devuelve si se encontro o no.
        */
        Status get();

        //!Funcion que agrega al dbelement a la base de datos.
        /*!Agrega al bash la accion de agregar el elemento para que luego cuando se ejecute se agregue a la base de datos.
        */
        Status put();

        //!Funcion que elimina el dbelement de la base de datos.
        /*!Agrega al bash la accion de eliminar el elemento para que luego cuando se ejecute se elimine de la base de datos.
        */
        Status erase();

        //!Funcion que inicializa el batch.
        //!Con el puntero db crea un batch y lo asigna a la variable batch.
        void startBatch();

        //!Funcion que procesa el batch y lo elimina.
        /*!Procesa el batch y luego lo elimina, asignando NULL a la variable batch.
        */
        Status endBatch();

        //!Funcion que fuerza la variable synced a false.
        /*!Se emplea cuando se quiere forzar un get a pesar de que synced sea true.
        */
        void reSync();

    protected:

        //!Funcion que establece la key.
        /*!Las clases hijas generan a partir de sus variables la key para DBElements.
        */
        virtual void _setKey() = 0;

        //!Funcion que establece el value.
        /*!Las clases hijas generan a partir de sus variables el value para DBElements.
        */
        virtual void _setValue() = 0;

        //!Funcion que establece varias variables.
        /*!Las clases hijas procesan el valor value y a partir de esto establecen sus propias variables.
        */
        virtual void _setValueVars() = 0;

        //!Variable key.
        /*!Esta variable se emplea para interactuar con la base de datos.
        */
        std::string key;

        //!Variable value.
        /*!Contiene los valores que se encuentran en la base de datos en un string con formato json.
        */
        std::string value;

        //!Variable db.
        /*!Es el puntero a la base de datos.
        */
        Database *db;

        //!Variable batch.
        /*!Es el puntero al batch.
        */
        DatabaseWriteBatch *batch;

        // se fija si ya se hizo get de la base de datos (si es true el get no hace nada)
        // para forzar un get despues de que synced es true, hay que llamar a reSync() antes
        //!Variable bool synced.
        /*!se fija si ya se hizo get de la base de datos
        */
        bool synced;

    private:

};

#endif // DBELEMENT_H
