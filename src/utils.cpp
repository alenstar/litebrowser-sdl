#include "globals.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <streambuf>
#include <string>

std::string urljoin(const std::string &base, const std::string &relative) {
    return "";
}

std::string loadfile(const char *file) {
    std::ifstream t(file);
    if (t) {
        // std::string str((std::istreambuf_iterator<char>(t)),
        //                std::istreambuf_iterator<char>());
        std::string str;

        t.seekg(0, std::ios::end);
        str.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());
        return std::move(str);
    }
    return "";
}

std::unique_ptr<char[]> loadfile2(const char *file) {

    std::ifstream is(file, std::ifstream::binary);
    if (is) {
        // get length of file:
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        char *buffer = new char[length];
        std::unique_ptr<char[]> pb(buffer);
        // read data as a block:
        is.read(buffer, length);

        if (is)
            std::cout << "all characters read successfully.";
        else
            std::cout << "error: only " << is.gcount() << " could be read";
        is.close();
        return pb;
    }
    return nullptr;
}
