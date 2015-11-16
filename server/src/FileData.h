#ifndef FILEDATA_H
#define FILEDATA_H

#include "DBElement.h"
#include <string>
#include <vector>
#include <algorithm>
#include "rocksdb/status.h"

class UserMetadata;

//!Clase que maneja la informacion del archivo.
/*!Hereda de DBElement.
*/
class FileData : public DBElement
{
    public:
        friend class UserMetadata;
        //!Funcion que inicializa la clase.
        /*!Utiliza el mismo constructor que DBElement.
        */
        FileData(Database* db, DatabaseWriteBatch* databbatch = NULL);
        virtual ~FileData();

        //!Funcion que devuelve el ultimo elemento del contenido.
        std::string getContent() { if(content.size() < 1) return ""; return content[content.size()-1]; }
        std::vector<std::string>* getVersions() { return &content; }
        //!Funcion que devuelve filename.
        std::string getFilename() { return filename; }
        //!Funcion que devuelve la extension.
        std::string getExtension() { return extension; }
        //!Funcion que devuelve el username del duenio.
        std::string getOwnerUsername() { return owner_username; }
        //!Funcion que devuelve la key del duenio.
        std::string getOwnerKey() { return owner_key; }
        //!Funcion que devuelve lista de usuarions con permiso de lectura.
        std::vector<std::string>* getUsersWithReadPermission() { return &users_with_read_permission; }
        //!Funcion que devuelve lista de usuarions con permiso de escritura.
        std::vector<std::string>* getUsersWithWritePermission() { return &users_with_write_permission; }
        //!Funcion que devuelve ultima fecha de modificacion.
        std::string getDateLastModified() { return date_last_modified; }
        //!Funcion que devuelve ultimo usuario en modificar archivo.
        std::string getUserWhoLastModified() { return user_who_modified; }
        //!Funcion que devuelve lista de tags.
        std::vector<std::string>* getTags() { return &tags; }

        //!Funcion que establece el contenido.
        void setContent(std::string n_content);
        //!Funcion que establece el nombre del archivo.
        void setFilename(std::string n_filename) { filename = n_filename; }
        //!Funcion que establece la extension.
        void setExtension(std::string n_extension) { extension = n_extension; }
        //!Funcion que establece el username.
        void setOwnerUsername(std::string n_owner_username) { owner_username = n_owner_username; }
        //!Funcion que establece la key del duenio.
        void setOwnerKey(std::string n_owner_key) { owner_key = n_owner_key; }
        //!Funcion que agrega usuario con permiso de lectura.
        void addUserWithReadPermission(std::string user_key);
        //!Funcion que agrega usuario con permiso de escritura.
        void addUserWithWritePermission(std::string user_key);
        //!Funcion que remueve usuario con permiso de lectura.
        void removeUserWithReadPermission(std::string user_key);
        //!Funcion que remueve usuario con permiso de escritura.
        void removeUserWithWritePermission(std::string user_key);
        //!Funcion que establece la ultima fecha de modificacion.
        void setDateLastModified(std::string date) { date_last_modified = date; }
        //!Funcion que establece el ultimo usuario en modificarlo.
        void setUserWhoLastModified(std::string user_key) { user_who_modified = user_key; }
        //!Funcion que agrega tag a la lista de tags.
        void addTag(std::string tag_key);
        //!Funcion que remueve un tag de la lista.
        void removeTag(std::string tag_key) { tags.erase(std::remove(tags.begin(), tags.end(), tag_key), tags.end()); }
        //!Funcion que verifica permiso de lectura.
        bool check_read_permission(std::string username);
        //!Funcion que verifica permiso de escritura.
        bool check_write_permission(std::string username);

        //!Funcion que verifica si el archivo tiene un tag.
        bool check_if_tag_belongs(std::string t) { return (std::find(tags.begin(),tags.end(),t) != tags.end());}
        //!Funcion que verifica la extension.
        bool check_extension(std::string e) {return extension == e;}

        //!Funcion que crea el archivo en la base de datos.
        /*!Recibe el contenido y crea un batch que luego ejecuta
        con todas las instrucciones para llenar los campos en la base de datos.
        */
        Status DBcreate(std::string content, std::string ubicacion);
        //!Funcion que verifica archivo para lectura.
        /*!Recibe un username y verifica que el archivo exista y el usuario tenga permiso de lectura.
        */
        Status DBget_for_read(std::string username);
        //!Funcion que elimina el archivo.
        /*!Elimina el archivo de las listas
        de archivos compartidos de todos los usuarios con acceso a el\
        */
        Status DBerase();
        //!Función que elimina archivo sin fijarse usarios con read permission
        /*!Elimina el archivo de la base de datos, asumiendo que ya se han eliminado
        las referencias de los otros usuarios al archivo
        */
        Status DBdelete_file();
        //!Funcion que modifica archivo.
        /*!Modifica los distintos campos del archivo\
        Los cambios en los permisos solo los puede hacer el duenio\
        No se puede modificar contenido y eliminar versiones al mismo tiempo.
        */
        Status DBmodify(std::string username, std::string n_filename, std::string ubicacion, std::string n_content, std::vector<std::string> &users_read_add,
                        std::vector<std::string> &users_read_remove, std::vector<std::string> &users_write_add, std::vector<std::string> &users_write_remove,
                        std::vector<std::string> &tags_add, std::vector<std::string> &tags_remove, std::vector<int> delete_versions_except_last_n);

    protected:
        //!Funcion que establece la key.
        /*!Une el username del duenio con el nombre del archivo.
        */
        virtual void _setKey();
        //!Funcion que establece el value.
        /*!A partir de todas sus variables genera un json y lo asigna a value.
        */
        virtual void _setValue();
        //!Funcion que establece variables del objeto.
        /*!Toma el json en value y establece las variables que contiene.
        */
        virtual void _setValueVars();

    private:
        //!Variable con las versiones del contenido.
        std::vector<std::string> content;
        //!Variable con el nombre del archivo.
        std::string filename;
        //!Variable con la extension.
        std::string extension;
        //!Variable con el username del duenio.
        std::string owner_username;
        //!Variable con la key del duenio.
        std::string owner_key; // si es el MD5 del username no hace falta tenerlo

        //!Variable con los usuarios con permiso de lectura.
        std::vector<std::string> users_with_read_permission; // tal vez deberian ser sets para evitar duplicados
        //!Variable con los usuarios con permiso de escritura.
        std::vector<std::string> users_with_write_permission;

        //!Variable con la ultima fecha de modificacion.
        std::string date_last_modified;
        //!Variable con el ultimo usuario en modificarlo.
        std::string user_who_modified;

        //!Variable con los tags.
        std::vector<std::string> tags; // tal vez deberia ser set

        //!Funcion que cambia la fecha de ultima modificacion y el usuario.
        Status _DBchangeModified(std::string username);
        //!Funcion que cambia la extension.
        Status _DBsetExtension(std::string new_extension);

        int _contentSize();
        //!Funcion que remueve tag.
        Status _DBremoveTag(std::string tag);
        //!Funcion que remueve version.
        /*!Busca la version pedida y si la encuentra la elimina\
        Tambien resta el peso de esta version a la cuota del usuario.
        */
        Status _DBeraseVersion(int v, UserMetadata* user_metadata);
        //!Funcion que verifica archivo para modificar.
        /*!Recibe un username y verifica que el archivo exista y el usuario tenga permiso de escritura.
        */
        Status _DBget_for_modify(std::string username);
        //!Funcion que modifica el nombre del archivo.
        /*!Modifica el nombre del archivo y el nombre en las listas
        de archivos compartidos de todos los usuarios con acceso a el.
        */
        Status _DBsetFilename(std::string name, UserMetadata* user_metadata);
        Status _DBchangeFilename(std::string name, UserMetadata* user_metadata);
        //!Funcion que modifica el contenido.
        /*!Se fija primero si el usuario tiene suficiente espacio en la cuota
        para el nuevo contenido y si lo tiene lo agrega.
        */
        Status _DBsetContent(std::string content, UserMetadata* usr);
        //!Funcion que remueve usuario con permiso de lectura en la base de datos.
        Status _DBremoveUserWithReadPermission(std::string user_key);
        //!Funcion que remueve usuario con permiso de escritura.
        Status _DBremoveUserWithWritePermission(std::string user_key);
        //!Funcion que agrega un usuarion con permiso de lectura en la base de datos.
        Status _DBaddUserWithReadPermission(std::string user_key);
        //!Funcion que agrega un usuarion con permiso de escritura en la base de datos.
        Status _DBaddUserWithWritePermission(std::string user_key);
        //!Funcion que agrega un tag en la base de datos.
        Status _DBaddTag(std::string tag);
};

#endif // FILEDATA_H
