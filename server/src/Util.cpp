#include "Util.h"
#include <openssl/md5.h>

void get_md5_hash(const std::string str, std::string &result){
    unsigned char digest[MD5_DIGEST_LENGTH];

    MD5((unsigned char*)str.c_str(), str.size(), (unsigned char*)&digest);

    char mdString[33];
    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    result.assign(mdString);
}
