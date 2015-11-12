#ifndef FILETAG_H
#define FILETAG_H

#include "FileSearchRegister.h"

//!Clase que maneja el tag en los archivos.
/*!Hereda de FileSearchRegister.
*/
class FileTag : public FileSearchRegister
{
    public:
        //!Funcion que inicializa DBElement.
        /*!Se inicializa db con el puntero a Database que se pasa.
        */
        FileTag(Database* db);
        virtual ~FileTag();

        //!Devuelve el tag.
        std::string getTag() { return tag; }
        //!Establece el tag.
        void setTag(std::string t) { tag = t; }

    protected:
        //!Establece la key.
        /*!Toma la variable username y le agrega "." y la extension.
        */
        virtual void _setKey();

    private:
        //!Variable string tag.
        std::string tag;
};

#endif // FILETAG_H
