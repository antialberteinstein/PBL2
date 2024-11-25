#include "apps/MySpinner.hpp"
#include "viewmodel/my_view_model.hpp"

shared_ptr<SpinnerTreeNode> MySpinner::spin() {
    ModelProducer* room_db;
    try {
        room_db = ModelProducer::get_instance(ModelType::ROOM);
    } catch (const string& msg) {
        try {
            auto room_db = ModelProducer::get_instance(ModelType::ROOM);
        } catch (const string& msg) {
            return nullptr;
        }
    }
    if (!room_db) {
        return nullptr;
    }
    Vector<string> keys = room_db->get_all_keys();
    if (keys.size() == 0) {
        return nullptr;
    }
    auto root = SpinnerTreeSifu::create_tree("spinner");

    for (int i = 0; i < keys.size(); i++) {
        auto room = room_db->get_room(keys[i]);
        if (room != nullptr && room->get_status() == RoomStatus::AVAILABLE) {
            int index = SpinnerTreeSifu::add_child(root, room->get_block());
            auto block = root->children[index];
            index = SpinnerTreeSifu::add_child(block, to_string(room->get_floor()));
            auto floor = block->children[index];
            index = SpinnerTreeSifu::add_child(floor, to_string(room->get_room_number()));
        }
    }

    return move(root);
}

shared_ptr<SpinnerTreeNode> SpinnerTreeSifu::create_tree(const string& name) {
    shared_ptr<SpinnerTreeNode> root
            = make_shared<SpinnerTreeNode>();
    root->data = name;
    return root;
}

int SpinnerTreeSifu::add_child(shared_ptr<SpinnerTreeNode>& parent, const string& data) {
    for (int i = 0; i < parent->children.size(); i++) {
        if (parent->children[i]->data == data) {
            return i;
        }
    }
    shared_ptr<SpinnerTreeNode> child =
        make_shared<SpinnerTreeNode>();
    child->data = data;
    parent->children.push_back(move(child));
    return parent->children.size() - 1;
}

string Quarter::hash_to_id(
        const string& block,
        int floor,
        int room_number) {
    return block + to_string(floor * 100 + room_number);
    }
