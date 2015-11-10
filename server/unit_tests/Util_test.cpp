
#include <openssl/md5.h>
#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include "Util.h"
#include <fstream>

TEST(UtilTests, MD5Test){
    unsigned char digest[MD5_DIGEST_LENGTH];
    std::string str = "happy";

    MD5((unsigned char*)str.c_str(), str.size(), (unsigned char*)&digest);

    char mdString[33];

    std::ofstream log;
    log.open("Util_test_LOG.txt", std::ios::app);
    if(!log){
        log.open("Util_test_LOG.txt", std::ios::trunc);
    }

    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    log << "md5 digest: " << mdString << std::endl;

    std::string res = "";
    get_md5_hash(str, res);

    log << "md5 in str: " << res << std::endl;

    log.close();
}

TEST(UtilTests, parse_html_uriTest){
    std::ofstream log;
    log.open("Util_test_LOG.txt", std::ios::app);
    if(!log){
        log.open("Util_test_LOG.txt", std::ios::trunc);
    }

    std::string uri1 = "/usuarios";
    log << "uri: " << uri1 << std::endl;
    std::vector<std::string> uri1_parsed;
    parse_html_uri(uri1, uri1_parsed);
    for(std::vector<std::string>::iterator it = uri1_parsed.begin(); it != uri1_parsed.end(); ++it){
        log << "token: " << *it << ", ";
    }
    log << std::endl << std::endl;

    std::string uri2 = "/archivos/gabriel/test.txt";
    log << "uri: " << uri2 << std::endl;
    std::vector<std::string> uri2_parsed;
    parse_html_uri(uri2, uri2_parsed);
    for(std::vector<std::string>::iterator it = uri2_parsed.begin(); it != uri2_parsed.end(); ++it){
        log << "token: " << *it << ", ";
    }
    log << std::endl << std::endl;

    std::string uri3 = "/";
    log << "uri: " << uri3 << std::endl;
    std::vector<std::string> uri3_parsed;
    parse_html_uri(uri3, uri3_parsed);
    for(std::vector<std::string>::iterator it = uri3_parsed.begin(); it != uri3_parsed.end(); ++it){
        log << "token: " << *it << ", ";
    }
    log << std::endl << std::endl;

    log.close();
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
