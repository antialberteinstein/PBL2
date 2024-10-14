#include "apps/student_list.hpp"
#include <vector>
#include <ftxui/component/component_options.hpp>

#include "objects/Vector.hpp"
#include "objects/List.hpp"

typedef Vector<string> StringVector;

struct Foo {
    int a;
};


class N_Adapter : public ftxui::ConstStringListRef::Adapter {
    public:
        static std::unique_ptr<N_Adapter> From(StringVector* entries) {
            return make_unique<N_Adapter>(entries);
        }
        explicit N_Adapter(StringVector* entries) : entries_(entries) {}
        ~N_Adapter() final = default;
        size_t size() const final { return entries_->size(); }
        string operator[](size_t index) const final { return entries_->at(index); }

    private:
        StringVector* entries_;
};


namespace student_list {
    Component container;
    ftxui::MenuOption menu_option;
    Component menu;
    int selected = 0;
    StringVector entries;


    Element create_element() {
        /* auto title_box = get_title().get_doc() | color(TITLE_COLOR);
        return vbox({
            title_box,
            text("Student list") | flex | border,
        }); */
        return container->Render();
    }

    bool check_event(Event event) {
        return container->OnEvent(event);
    }

    void action() {
        /* vector<string> entries = {
            "Le Nha Phuong",
            "Tran Nhat Nguyen",
        }; */

        entries.push_back("Lê Nhã  Phương");
        entries.push_back("Tran Nhat Nguyen");

        menu = Dropdown(N_Adapter::From(&entries), &selected);

        container = Container::Vertical({
            menu | border | flex,
        });
        add_component_tree(container);
    }
}