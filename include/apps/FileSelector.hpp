#ifndef PBL_FILE_SELECTOR_HPP
#define PBL_FILE_SELECTOR_HPP

#include <iostream>
#include <string>
#include <exception>

#ifdef _WIN32
#define WINDOWS 1
#else
#define WINDOWS 0
#endif

using namespace std;

string get_save_file_path();

#endif