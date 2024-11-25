#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "objects/Vector.hpp"
#include "objects/List.hpp"

using namespace std;

struct CSVCreatorMap {
    string name;  // Title.
    Vector<string> list_;

    CSVCreatorMap() {
        list_ = Vector<string>();
    }

    void push_back(const string& s) {
        list_.push_back(s);
    }

    void clear() {
        list_.clear();
    }
};

class CSVCreator {
    public:
        CSVCreator() {
            maps_ = List<CSVCreatorMap>(20);
        }

        void add_map(const string& name) {
            maps_.add(CSVCreatorMap());
            maps_[maps_.size() - 1].name = name;
        }

        void add_record(Vector<string> arr) {
            if (arr.size() != maps_.size()) {
                throw "Record size is not equal to the number of maps.";
            }
            for (size_t i = 0; i < maps_.size(); ++i) {
                maps_[i].push_back(arr[i]);
            }
        }

        size_t size() {
            if (maps_.size() <= 0) {
                return 0;
            } else {
                return maps_[0].list_.size();
            }
        }

        void dump_to_file(const string& file_path) {
            ofstream file(file_path);
            if (!file.is_open()) {
                throw "Cannot open file " + file_path;
            }

            for (size_t i = 0; i < maps_.size(); ++i) {
                file << maps_[i].name;
                if (i < maps_.size() - 1) {
                    file << ",";
                }
            }
            file << endl;

            for (size_t i = 0; i < size(); ++i) {
                for (size_t j = 0; j < maps_.size(); ++j) {
                    file << maps_[j].list_[i];
                    if (j < maps_.size() - 1) {
                        file << ",";
                    }
                }
                file << endl;
            }

            file.close();
        }
    private:
        List<CSVCreatorMap> maps_;
};