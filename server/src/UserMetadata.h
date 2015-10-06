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

        std::vector<std::string>* const getMy_files() { return &my_files; }
        std::vector< std::pair<std::string, std::string> >* const getShared_files() { return &shared_files; }
        std::string getJoinDate() { return join_date; }
        std::string getUsername() { return username; }

        std::string getFileTreeJson();

        Status DBerase();
        Status DBcreate();
        Status DBremove_my_file(std::string filename);
        Status DBremove_shared_file(std::string user, std::string filename);
        Status DBadd_my_file(std::string filename);
        Status DBadd_shared_file(std::string user, std::string filename);

    protected:

        virtual void _setKey();
        virtual void _setValue();
        virtual void _setValueVars();

    private:
        std::string username;

        std::vector<std::string> my_files;
        std::vector< std::pair<std::string, std::string> > shared_files;
        std::string join_date;
        // agregar lo que haga falta
};

#endif // USERMETADATA_H
