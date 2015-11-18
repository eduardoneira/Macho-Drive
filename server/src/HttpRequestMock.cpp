#include "HttpRequestMock.h"

HttpRequestMock::HttpRequestMock()
{
    //ctor
}

HttpRequestMock::~HttpRequestMock()
{
    //dtor
}

std::string HttpRequestMock::getCampo(std::string campo){
    if(campo == "filename"){
        return this->mock_filename;
    } else if(campo == "ubicacion"){
        return this->mock_ubicacion;
    } else if(campo == "content"){
        return this->mock_content;
    }
    return "";
}

std::string HttpRequestMock::getCampoDeArray(std::string campo, int index){
    std::vector<std::string>* vec = NULL;
    if(campo == "tags"){
        vec = &this->mock_add_tags;
    } else if(campo == "users_with_read_permission"){
        vec = &this->mock_add_read_perm;
    } else if(campo == "users_with_write_permission"){
        vec = &this->mock_add_write_perm;
    }

    if(vec == NULL || index < 0 || index >= vec->size()){
        return "";
    }
    return vec->at(index);
}
