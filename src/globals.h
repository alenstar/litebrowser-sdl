#pragma once

#include <fstream>
#include <string>
#include <cerrno>
#include <clocale>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../litehtml/include/litehtml.h"
//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include <fontconfig/fontconfig.h>
#include <curl/curl.h>
// #include <Poco/URI.h>

extern std::string urljoin(const std::string &base, const std::string &relative);

extern std::string loadfile(const char *file);
