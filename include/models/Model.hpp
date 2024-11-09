#pragma once

#include <iostream>
using namespace std;

#include "nlohmann/json.hpp"

using json=nlohmann::json;

class Model {
    public:

        virtual string serialize() = 0;
        virtual void deserialize(string data) = 0;

        string get_id() {
            return id;
        }

        void set_id(string id) {
            this->id = id;
        }

        string get_id_string() {
            return id;
        }
    protected:
        string id;

    private:
        void generate_id(const string& previous_id="") {
            if (previous_id == "") {
                id = "1";
            } else {
                try {
                    id = to_string(stoi(previous_id) + 1);
                } catch (invalid_argument& e) {
                    id = "1";
                }
            }
        }
};
