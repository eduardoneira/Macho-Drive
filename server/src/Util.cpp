#include "Util.h"
#include <openssl/md5.h>
#include <vector>
#include <sstream>
#include <ctime>

void get_md5_hash(const std::string str, std::string &result){
    unsigned char digest[MD5_DIGEST_LENGTH];

    MD5((unsigned char*)str.c_str(), str.size(), (unsigned char*)&digest);

    char mdString[33];
    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    result.assign(mdString);
}

void parse_html_uri(const std::string& s, std::vector<std::string>& parsed){
    std::stringstream input;
    input << s;

    std::string temp = "";
    std::string token = "";
    while(getline(input, temp, '/')){
        token.append(temp);
        if(token.compare("") == 0){
            continue;
        }
        parsed.push_back(token);
        token = "";
    }
}

std::string get_longest_extension_from_filename(const std::string s){
    std::stringstream input;
    input << s;

    std::string temp = "";
    std::string token = "";
    getline(input, temp, '.');
    temp = "";
    getline(input, temp);
    //std::cout << s << std::endl;
    //std::cout << temp << std::endl;
    return temp;
}

std::string get_name_wo_extension_from_filename(const std::string s){
    std::stringstream input;
    input << s;

    std::string temp = "";
    std::string token = "";
    getline(input, temp, '.');
    return temp;
}

std::string get_date_and_time(){
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );

    std::string str = asctime(now);
    /*str.append(std::to_string(now->tm_year + 1900));
    str.append("-");
    str.append(std::to_string(now->tm_mon + 1));
    str.append("-");
    str.append(std::to_string(now->tm_mday));*/
    return str;
}
