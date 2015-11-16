#ifndef USERMETADATA_H
#define USERMETADATA_H

#include "DBElement.h"
#include <vector>
#include <string>
#include <algorithm>
#include "rocksdb/status.h"

class User;
class FileData;

//!Clase que contiene la informacion del usuario.
/*!Tiene los datos del usuario y se encarga de manejarlos\
Hereda de DBElement.
*/
class UserMetadata : public DBElement
{
    public:
        friend class User;
        friend class FileData;

        //!Funcion que inicializa la clase.
        /*!Realiza la misma inicializacion que DBElement, pero ademas sus variables particulares
        las inicia en "", 0 o en el caso de couta_max, en 10GB.
        */
        UserMetadata(Database* db, DatabaseWriteBatch* dbbatch = NULL);
        virtual ~UserMetadata();

        //!Funcion que establece el username.
        /*!Recibe un string y se lo asigna a username.
        */
        void setUsername(std::string name) { this->username = name; }

        //!Funcion que agrega un archivo a la lista.
        /*!Recibe el nombre de un archivo y lo agrega a la lista de archivos.
        */
        void addMyFile(std::string name);

        //!Funcion que elimina un archivo de la lista.
        /*!Recibe el nombre de un archivo y lo elimina de la lista de archivos.
        */
        void removeMyFile(std::string name);

        //!Funcion que agrega un archivo compartido a la lista.
        /*!Recibe el nombre de un archivo y el usuario con el que lo comparte
        y lo agrega a la lista de archivos compartidos.
        */
        void addSharedFile(std::string name, std::string user);
        //!Funcion que agrega archivos a papelera
        /*!Recibe el nombre del archivo y lo agrega a la papelera
        */
        void addRecycleBinFile(std::string name);

        //!Funcion que elimina un archivo compartido de la lista.
        /*!Recibe el nombre de un archivo y el usuario con el que lo comparte
        y lo elimina de la lista de archivos compartidos.
        */
        void removeSharedFile(std::string name, std::string user);

        //!Funcion que establece la fecha de inscripcion.
        /*!Recibe un string y lo establece como la fecha.
        */
        void setJoinDate(std::string date) { this->join_date = date; }

        //!Funcion que establece la cuota maxima.
        /*!Recibe un double y se lo asigna a couta_max.
        */
        void setCuotaMax(double c) { this->cuota_max = c; }

        //!Funcion que cambia el mail por uno nuevo.
        /*!Recibe como string el nuevo mail y lo asigna a la variable.
        */
        void changeEmail(std::string n_email) { this->email = n_email; }

        //!Funcion que cambia la ultima ubicacion del usuario.
        /*!Recibe como string la nueva ubicacion y la asigna a la variable.
        */
        void changeUltimaUbicacion(std::string u) { this->ultima_ubicacion = u; }

        //!Funcion que aumenta la cuota actual.
        /*!Recibe un double y lo suma a la cuota_actual ya presente.
        */
        void add_to_cuota(double c) { this->cuota_actual += c; }

        //!Funcion que disminuye la cuota actual.
        /*!Recibe un double y lo resta a la cuota_actual ya presente.
        */
        void remove_from_cuota(double c) { this->cuota_actual -= c; }

        //!Funcion que establece la cuota actual.
        /*!Recibe un double y lo asigna a la variable cuota_actual.
        */
        void setCuotaActual(double c) { this->cuota_actual = c; }

        //!Funcion que devuelve vector de archivos.
        /*!Devuelve un puntero al vector con los nombres de los archivos.
        */
        std::vector<std::string>* const getMy_files() { return &my_files; }

        //!Funcion que devuelve vector de archivos compartidos.
        /*!Devuelve un puntero al vector con los nombres de los archivos compartidos.
        */
        std::vector< std::pair<std::string, std::string> >* const getShared_files() { return &shared_files; }

        //!Funcion que devuelve lista de todos los archivos.
        /*!Devuelve un vector con los nombres de todos los archivos, tanto simples como compartidos.
        */
        std::vector<std::string> getAll_files();

        //!Funcion que devuelve la fecha de inscripcion.
        std::string getJoinDate() { return join_date; }

        //!Funcion que devuelve el username.
        std::string getUsername() { return username; }

        //!Funcion que busca en los archivos por tag.
        /*!Recibe un string con el tag y devuelve un vector con los archivos encontrados.
        */
        std::vector<std::string> search_files_by_tag(std::string word);

        //!Funcion que busca en los archivos por extension.
        /*!Recibe un string con la extension y devuelve un vector con los archivos encontrados.
        */
        std::vector<std::string> search_files_by_extension(std::string word);

        //!Funcion que busca en los archivos por duenio.
        /*!Recibe un string con el duenio y devuelve un vector con los archivos encontrados.
        */
        std::vector<std::string> search_files_by_owner(std::string word);

        //!Funcion que busca en los archivos por nombre.
        /*!Recibe un string con el nombre y devuelve un vector con los archivos encontrados.
        */
        std::vector<std::string> search_files_by_name(std::string word);

        //!Funcion que devuelve la lista de todos los archivos en json.
        /*!Devuelve en formato json la lista de los archivos, tanto los normales como los compartidos.
        */
        std::string getFileTreeJson();

        //!Funcion que chequea si existe un archivo.
        /*!Busca el nombre del archivo en l lista y si lo encuentra devuelve TRUE.
        */
        bool DBisMyFile(std::string filename);

        //!Funcion que chequea si existe un archivo compartido.
        /*!Busca el nombre del archivo en l lista y si lo encuentra devuelve el nombre y el usuario.
        */
        std::pair<std::string, std::string> DBisSharedFile(std::string filename);

        //!Funcion que obtiene la metadata del usuario de la base de datos.
        Status DBget();

        //!Funcion que cambia mail en la base de datos.
        Status DBchange_email(std::string n_email);
        //!Funcion que manda un archivo al bin
        Status DB_move_to_bin(std::string filename);
        //!Funcion que pasa la papelera de reciclaje a json.
        /*!Transforma la variable de la papelera de reciclaje a string formato json.
        */
        std::string getRecycleBin();

        //!Funcion que devuelve un archivo de la papelera.
        /*!Devuelve un archivo de la papelera de reciclaje a la lista de archivos y lo elimina de la papelera.
        */
        bool recoverFileRecycleBin(std::string filename);

        //!Funcion que devuelve los archivos de la papelera.
        /*!Devuelve el vector con los nombres de todos los archivos de la papelera de reciclaje.
        */
        std::vector<std::string> getAllFilesBin(){return recycle_bin;}
    protected:

        //!Funcion que establece la key.
        /*!Toma la variable username y la establece como key, luego de agrgarle ".usertoken".
        */
        virtual void _setKey();

        //!Funcion que establece el value.
        /*!Toma las variables join_date, cuota_max, cuota_actual, email, ultima_ubicacion my_files y shared_files y forma un json.
        */
        virtual void _setValue();

        //!Funcion que establece varias variables.
        /*!Toma la variable value y a partir de ella obtiene join_date, cuota_max, cuota_actual, email, ultima_ubicacion my_files y shared_files .
        */
        virtual void _setValueVars();

    private:
        //!Variable string que contiene username.
        std::string username;

        //!Variable con los nombres de los archivos.
        /*!Contiene un vector con los nombres de los archivos del usuario.
        */
        std::vector<std::string> my_files;

        //!Variable con los nombres de los archivos compartidos.
        /*!Contiene un vector con los nombres de los archivos compartidos del usuario.
        */
        std::vector< std::pair<std::string, std::string> > shared_files;

        //!Variable con los nombres de los archivos en la papelera.
        /*!Contiene un vector con los nombres de los archivos que se encuentran en la papelera de reciclaje.
        */
        std::vector<std::string> recycle_bin;

        //!Variable con la fecha de inscripcion.
        /*!Contiene la fecha en que el usuario creo su cuenta.
        */
        std::string join_date;

        //!Variable con el mail del usuario.
        std::string email;

        //!Variable con la cuota maxima.
        /*!Es la cuouta maxima que el usuario puede tener.
        */
        double cuota_max;

        //!Variable con la cuota actual.
        /*!Es la cuota que presenta en el momento el usuario.
        */
        double cuota_actual;

        //!Variable con la ultima ubicacion del usuario.
        std::string ultima_ubicacion;
        // agregar lo que haga falta


        //!Funcion que elimina usuario de la base de datos.
        Status DBerase();

        //!Funcion que crea al usuario en la base de datos.
        /*!Si no existe ya el usuario, lo crea y registra la fecha de inscripcion.
        */
        Status DBcreate();

        //!Funcion que agrega un archivo a la lista en la base de datos.
        Status DBadd_my_file(std::string filename/*, double file_size, std::string u*/);

        //!Funcion que cambia cuota maxima en la base de datos.
        Status DBchange_cuota_max(double n_cuota_max);

        //!Funcion que agrega un archivo compartido a la lista en la base de datos.
        Status DBadd_shared_file(std::string user, std::string filename);

        //!Funcion que elimina archivo compartido de la base de datos.
        /*!Recibe el nombre y usuario del archivo y lo elimina de la lista.
        */
        Status DBremove_shared_file(std::string user, std::string filename);

        //!Funcion que elimina archivo de la base de datos.
        /*!Recibe el nombre y tamaño del archivo y lo elimina de la lista y de la cuota actual.
        */
        Status DBremove_my_file(std::string filename, double file_size);


        //!Funcion que evalua si hay suficiente cuota.
        /*!Recibe el tamaño del archivo a introducir y se fija si la diferencia entre la cuota
        maxima y la cuota actual es mayor a este.
        */
        Status DBhas_enough_cuota(double file_size, bool &result);

        //!Funcion que cambia nombre de archivo compartido en la base de datos.
        Status DBchange_shared_filename(std::string old_filename, std::string new_filename);

        //!Funcion que modifica el tamaño del archivo.
        /*!Recibe la diferencia que el cambio produce en la cuota y la modifica.
        */
        Status DBmodif_file(double dif_cuota);

        //!Funcion que cambia nombre de archivo en la base de datos.
        Status DBchange_my_filename(std::string old_filename, std::string new_filename);

        //!Funcion que cambia ultima ubicacion en la base de datos.
        Status DBchange_ultima_ubicacion(std::string u);
};

#endif // USERMETADATA_H
