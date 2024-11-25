#pragma once

#include <iostream>
#include "models/Trigger.hpp"

using namespace std;

#include "nlohmann/json.hpp"

using json=nlohmann::json;

class Model : public Trigger {
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
        
        // Hash function to generate id
        virtual string hash_to_id() = 0;
        
        virtual string generate_id() {
            string _id = hash_to_id();
            this->id = _id;
            return _id;
        };
        
        
    protected:
        string id;

};
