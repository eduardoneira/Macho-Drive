#ifndef FILEEXTENSION_H
#define FILEEXTENSION_H

#include "FileSearchRegister.h"

//!Clase que maneja la extension de los archivos.
/*!Hereda de FileSearchRegister.
*/
class FileExtension : public FileSearchRegister
{
    public:
        //!Funcion que inicializa DBElement.
        /*!Se inicializa db con el puntero a Database que se pasa.
        */
        FileExtension(Database* db);
        virtual ~FileExtension();

        //!Devuelve la extension.
        std::string getExtension() { return extension; }

        //!Establece la extension.
        void setExtension(std::string ext) { extension = ext; }

    protected:
        //!Establece la key.
        /*!Toma la variable username y le agrega "." y el tag.
        */
        virtual void _setKey();

    private:
        //!Variable string extension.
        std::string extension;

};

#endif // FILEEXTENSION_H
