#pragma once

#include "apps/App.hh"
#include "objects/Vector.hpp"

class ModelProducer;

enum class SpinnerGender {
    MALE,
    FEMALE,
    OTHER,
};

struct SpinnerPair {
    string key;
    SpinnerGender value;
};

struct SpinnerTreeNode {
    string data;
    Vector<shared_ptr<SpinnerTreeNode>> children;
};

namespace SpinnerTreeSifu {
    shared_ptr<SpinnerTreeNode> create_tree(const string& name);

    int add_child(shared_ptr<SpinnerTreeNode>& parent, const string& data);
}

namespace Quarter {

    string hash_to_id(
            const string& block,
            int floor,
            int room_number);
    

}

class MySpinner {
    public:
        static shared_ptr<SpinnerTreeNode> spin();
};


