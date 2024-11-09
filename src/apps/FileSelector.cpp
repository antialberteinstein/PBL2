#include "apps/FileSelector.hpp"
#include <fstream>

string get_save_file_path() {
    if (!WINDOWS && system("chmod u+x ./res/bin/folderdialog")) {
        throw "There is an error at allowing the program to execute.";
    } else {
        int status = 0;
        if (WINDOWS)  status = system(".\\res\\bin\\folderdialog");
        else  status = system("./res/bin/folderdialog");

        if (status) {
            throw "There is an error at executing the program.";
        } else {
            ifstream file("./folderdialog.txt");

            if (file.is_open()) {
                string path;
                getline(file, path);
                file.close();
                return path;
            } else {
                throw "There is an error at opening the file.";
            }
        }
    }
}