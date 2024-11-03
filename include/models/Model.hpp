#pragma once

#include <iostream>
using namespace std;

#include "nlohmann/json.hpp"

using json=nlohmann::json;

class Model {
    public:

        virtual string serialize() = 0;
        virtual void deserialize(string data) = 0;

        int get_id() {
            return id;
        }

        void set_id(int id) {
            this->id = id;
        }

        string get_id_string() {
            return to_string(id);
        }
    protected:
        int id;
};
