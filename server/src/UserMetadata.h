#ifndef USERMETADATA_H
#define USERMETADATA_H

#include "DBElement.h"
#include <vector>
#include <string>
#include <algorithm>
#include "rocksdb/status.h"

class UserMetadata : public DBElement
{
    public:
        UserMetadata(Database* db);
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
        std::string getJoinDate() { return join_date; }
        std::string getUsername() { return username; }

        std::string getFileTreeJson();

        Status DBerase();
        Status DBcreate();
        Status DBget();
        Status DBremove_my_file(std::string filename, double file_size);
        Status DBremove_shared_file(std::string user, std::string filename);
        Status DBadd_my_file(std::string filename/*, double file_size, std::string u*/);
        Status DBadd_shared_file(std::string user, std::string filename);
        Status DBchange_email(std::string n_email);
        Status DBchange_cuota_max(double n_cuota_max);
        Status DBchange_ultima_ubicacion(std::string u);
        Status DBhas_enough_cuota(double file_size, bool &result);
        Status DBmodif_file(double dif_cuota, std::string u);

    protected:

        virtual void _setKey();
        virtual void _setValue();
        virtual void _setValueVars();

    private:
        std::string username;

        std::vector<std::string> my_files;
        std::vector< std::pair<std::string, std::string> > shared_files;
        std::string join_date;
        std::string email;
        double cuota_max;
        double cuota_actual;
        std::string ultima_ubicacion;
        // agregar lo que haga falta
};

#endif // USERMETADATA_H
