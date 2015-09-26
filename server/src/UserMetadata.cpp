#include "UserMetadata.h"
#include "JsonSerializer.h"
#include <string>

UserMetadata::UserMetadata()
{
    //ctor
}

UserMetadata::~UserMetadata()
{
    //dtor
}

void UserMetadata::_setValue(){
    JsonSerializer serializer;

    std::string array_my_file_tokens = "";
    serializer.turnVectorToArray(my_file_tokens, "my_file_tokens", array_my_file_tokens);
    std::string array_shared_file_tokens = "";
    serializer.turnVectorToArray(shared_file_tokens, "shared_file_tokens", array_my_file_tokens);
    std::string val_date_joined = "";
    serializer.addValueToObjectList(val_date_joined, "join_date", join_date);

    std::string val_json = "";
    serializer.joinValueIntoList(val_json, array_my_file_tokens);
    serializer.joinValueIntoList(val_json, array_shared_file_tokens);
    serializer.joinValueIntoList(val_json, val_date_joined);
    serializer.turnObjectListToObject(val_json);
    this->value = val_json;
}
