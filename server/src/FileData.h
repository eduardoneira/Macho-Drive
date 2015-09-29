#ifndef FILEDATA_H
#define FILEDATA_H

#include "DBElement.h"
#include <string>
#include <vector>

class FileData : public DBElement
{
    public:
        FileData();
        virtual ~FileData();

        std::string getContent() { return content; }
        std::string getFilename() { return filename; }
        std::string getExtension() { return extension; }
        std::string getOwnerUsername() { return owner_username; }
        std::string getOwnerKey() { return owner_key; }
        std::vector<std::string>* getUsersWithReadPermission() { return &users_with_read_permission; }
        std::vector<std::string>* getUsersWithWritePermission() { return &users_with_write_permission; }
        std::string getDateLastModified() { return date_last_modified; }
        std::string getUserWhoLastModified() { return user_who_modified; }
        std::vector<std::string>* getTags() { return &tags; }

        void setContent(std::string n_content) { content = n_content; }
        void setFilename(std::string n_filename) { filename = n_filename; }
        void setExtension(std::string n_extension) { extension = n_extension; }
        void setOwnerUsername(std::string n_owner_username) { owner_username = n_owner_username; }
        void setOwnerKey(std::string n_owner_key) { owner_key = n_owner_key; }
        void addUserWithReadPermission(std::string user_key);
        void addUserWithWritePermission(std::string user_key);
        void setDateLastModified(std::string date) { date_last_modified = date; }
        void setUserWhoLastModified(std::string user_key) { user_who_modified = user_key; }
        void addTag(std::string tag_key) { tags.push_back(tag_key); }

    protected:
        virtual void _setKey();
        virtual void _setValue();
        virtual void _setValueVars();

    private:
        std::string content;
        std::string filename;
        std::string extension;
        std::string owner_username;
        std::string owner_key; // si es el MD5 del username no hace falta tenerlo

        std::vector<std::string> users_with_read_permission; // tal vez deberian ser sets para evitar duplicados
        std::vector<std::string> users_with_write_permission;

        std::string date_last_modified;
        std::string user_who_modified;

        std::vector<std::string> tags; // tal vez deberia ser set
};

#endif // FILEDATA_H
