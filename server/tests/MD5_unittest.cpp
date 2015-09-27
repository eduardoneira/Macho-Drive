
#include <openssl/md5.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include "Util.h"

int main(){
    unsigned char digest[MD5_DIGEST_LENGTH];
    std::string str = "happy";

    MD5((unsigned char*)str.c_str(), str.size(), (unsigned char*)&digest);

    char mdString[33];

    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    std::cout << "md5 digest: " << mdString << std::endl;

    std::string res = "";
    get_md5_hash(str, res);
    std::cout << "md5 in str: " << res << std::endl;

    return 0;
}
