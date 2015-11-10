#ifndef USERMETADATA_H
#define USERMETADATA_H

#include "DBElement.h"
#include <vector>
#include <string>
#include <algorithm>
#include "rocksdb/status.h"

class User;
class FileData;

class UserMetadata : public DBElement
{
    public:
        friend class User;
        friend class FileData;

        UserMetadata(Database* db, DatabaseWriteBatch* dbbatch = NULL);
        virtual ~UserMetadata();

        void setUsername(std::string name) { this->username = name; }
        void addMyFile(std::string name);
        void removeMyFile(std::string name);
        void addSharedFile(std::string name, std::string user);
        void removeSharedFile(std::string name, std::string user);
        void setJoinDate(std::string date) { this->join_date = date; }
        void setCuotaMax(double c) { this->cuota_max = c; }
        void changeEmail(std::string n_email) { this->email = n_email; }
        void changeUltimaUbicacion(std::string u) { this->ultima_ubicacion = u; }
        void add_to_cuota(double c) { this->cuota_actual += c; }
        void remove_from_cuota(double c) { this->cuota_actual -= c; }
        void setCuotaActual(double c) { this->cuota_actual = c; }

        std::vector<std::string>* const getMy_files() { return &my_files; }
        std::vector< std::pair<std::string, std::string> >* const getShared_files() { return &shared_files; }
        std::vector<std::string> getAll_files();
        std::string getJoinDate() { return join_date; }
        std::string getUsername() { return username; }

        std::vector<std::string> search_files_by_tag(std::string word);
        std::vector<std::string> search_files_by_extension(std::string word);
        std::vector<std::string> search_files_by_owner(std::string word);
        std::vector<std::string> search_files_by_name(std::string word);

        std::string getFileTreeJson();

        bool DBisMyFile(std::string filename);
        std::pair<std::string, std::string> DBisSharedFile(std::string filename);
        Status DBget();
        Status DBchange_email(std::string n_email);

        std::string getRecycleBin();
        bool recoverFileRecycleBin(std::string filename);
        std::vector<std::string> getAllFilesBin(){return recycle_bin;}
    protected:

        virtual void _setKey();
        virtual void _setValue();
        virtual void _setValueVars();

    private:
        std::string username;

        std::vector<std::string> my_files;
        std::vector< std::pair<std::string, std::string> > shared_files;
        std::vector<std::string> recycle_bin;
        std::string join_date;
        std::string email;
        double cuota_max;
        double cuota_actual;
        std::string ultima_ubicacion;
        // agregar lo que haga falta


        Status DBerase();
        Status DBcreate();

        Status DBadd_my_file(std::string filename/*, double file_size, std::string u*/);

        Status DBchange_cuota_max(double n_cuota_max);
        Status DBadd_shared_file(std::string user, std::string filename);
        Status DBremove_shared_file(std::string user, std::string filename);
        Status DBremove_my_file(std::string filename, double file_size);
        Status DBhas_enough_cuota(double file_size, bool &result);
        Status DBchange_shared_filename(std::string old_filename, std::string new_filename);
        Status DBmodif_file(double dif_cuota);
        Status DBchange_my_filename(std::string old_filename, std::string new_filename);
        Status DBchange_ultima_ubicacion(std::string u);
};

#endif // USERMETADATA_H
