#pragma once
#include "apps/App.hh"

#define DEFAULT_SHOWED 17
#include "objects/Set.hpp"

struct ScrollerMap {
    string name;
    string display_name;
    Vector<string>_list;
    Component _menu;
    Vector<string> complete_list;
    Vector<string> _backup_list;
    bool sorted_flag;

    ScrollerMap() {
        _list = Vector<string>();
        complete_list = Vector<string>();
        _backup_list = Vector<string>();
        sorted_flag = false;
    }

    void create_menu(int& selected, MenuOption& option) {
        _menu = Menu(
            StringAdapter::From(&_list),
            &selected,
            option
        );
    }

    void update(int index) {
        if (index < complete_list.size()) {
            _list.push_back(complete_list[index]);
        } else {
            _list.push_back("");
        }
    }

    void push_back(const string& s) {
        complete_list.push_back(s);
        _backup_list.push_back(s);
        sorted_flag = false;
    }

    void clear() {
        _list.clear();
    }

    Element Render(const string& title) {
        return vbox ({
            text(title) | bold,
            separator(),
            _menu->Render() | flex,
        }) | flex;
    }

    bool OnEvent(Event event) {
        return _menu->OnEvent(event);
    }

    size_t size() {
        return complete_list.size();
    }

    void filter_reset() {
        complete_list = _backup_list;
    }


};


// Lower and remove all spaces.
string regex_search(const string& s);

// Max of an array.
int max_of(Vector<int>& arr);

// Min of an array.
int min_of(Vector<int>& arr);

// Return true if all elements are true.
bool all(Vector<bool>& arr);

// Return true if _any elements are true.
bool _any(Vector<bool>& arr);

class Scroller {
    public:

        Scroller(const int SHOWED=DEFAULT_SHOWED) : SHOWED(SHOWED) {
            _maps = List<ScrollerMap>(20);
            _start = 0;
            _end = SHOWED - 1;
            selected = 0;

            option = MenuOption::Vertical();
            option.selected = &selected;
            option.focused_entry = &selected;
        }

        void add_map(ScrollerMap map, const string& name, const string& display_name) {
            _maps.add(map);
            _maps[_maps.size() - 1].create_menu(selected, option);
            _maps[_maps.size() - 1].name = name;
            _maps[_maps.size() - 1].display_name = display_name;
        }

        void add_record(Vector<string> arr) {
            if (arr.size() != _maps.size()) {
                throw "Record size is not equal to the number of maps.";
            }
            for (size_t i = 0; i < _maps.size(); ++i) {
                _maps[i].push_back(arr[i]);
            }
        }

        void push_back(int index, const string& s) {
            _maps[index].push_back(s);
            for (size_t i = 0; i < _maps.size(); ++i) {
                if (i != index) {
                    _maps[i].push_back("");
                }
            }
        }

        void push_back(string name, const string& s) {
            for (size_t i = 0; i < _maps.size(); ++i) {
                if (_maps[i].name == name) {
                    _maps[i].push_back(s);
                } else {
                    _maps[i].push_back("");
                }
            }
        }

        Element Render() {
            Elements elements;
            for (size_t i = 0; i < _maps.size(); ++i) {
                elements.push_back(_maps[i].Render(_maps[i].display_name));
                elements.push_back(separator());
            }

            return hbox(elements) | border | flex;
        }


        // Return the maximum number of items.
        size_t size() {
            if (_maps.size() <= 0) {
                return 0;
            } else {
                return _maps[0].size();
            }
        }

        bool OnMouseEvent(Event event) {
            Vector<bool> arr;
            for (size_t i = 0; i < _maps.size(); ++i) {
                arr.push_back(_maps[i].OnEvent(event));
            }

            return _any(arr);
        }

        bool OnEvent(Event event) {
            if (event == Event::ArrowDown) {
                if (selected == SHOWED - 1) {
                    slide_down();
                }
            }
            if (event == Event::ArrowUp) {
                if (selected == 0) {
                    slide_up();
                }
            }

            if (_maps.size() <= 0) {
                return false;
            } else {
                return _maps[0].OnEvent(event);
            }
        }

        void update_visible_list() {
            for (size_t i = 0; i < _maps.size(); ++i) {
                _maps[i].clear();
            }

            for (int i = _start; i <= _end; ++i) {
                for (size_t j = 0; j < _maps.size(); ++j) {
                    _maps[j].update(i);
                }
            }
        }

        int start() {
            return _start;
        }

        int end() {
            return _end;
        }

        int selected_index() {
            return selected;
        }

        int get_showed() {
            return SHOWED;
        }

        void filter_reset() {
            for (size_t i = 0; i < _maps.size(); ++i) {
                _maps[i].filter_reset();
            }

            update_visible_list();
        }

        void filter_linear(string name, const string& s) {
            for (size_t i = 0; i < _maps.size(); ++i) {
                if (_maps[i].name == name) {
                    filter_linear(i, s);
                    break;
                }
            }
        }

        void filter_linear(int index, const string& s) {
            for (size_t i = 0; i < _maps.size(); ++i) {
                _maps[i].complete_list.clear();
            }
            

            _start = 0;
            _end = SHOWED - 1;

            for (int i = 0; i < _maps[index]._backup_list.size(); ++i) {
                if (regex_search(_maps[index]._backup_list[i]).find(regex_search(s)) != string::npos) {
                    for (size_t j = 0; j < _maps.size(); ++j) {
                        _maps[j].complete_list.push_back(_maps[j]._backup_list[i]);
                    }
                }
            }

            update_visible_list();
            selected = 0;
        }

        string get_current_item(int index) {
            return _maps[index]._list[selected];
        }

        string get_current_item(string name) {
            for (size_t i = 0; i < _maps.size(); ++i) {
                if (_maps[i].name == name) {
                    return get_current_item(i);
                }
            }
            return "";
        }

        Vector<string> get_current_item() {
            Vector<string> arr;
            for (size_t i = 0; i < _maps.size(); ++i) {
                arr.push_back(_maps[i]._list[selected]);
            }
            return arr;
        }

        void clear() {
            for (size_t i = 0; i < _maps.size(); ++i) {
                _maps[i].clear();
                _maps[i].complete_list.clear();
                _maps[i]._backup_list.clear();
            }
        }


    private:
        List<ScrollerMap> _maps;
        int _start;
        int _end;
        int selected;
        const int SHOWED;

        MenuOption option;

        void slide_down() {
            if (_end < size() - 1) {
                _start++;
                _end++;
                update_visible_list();
            }
        }

        void slide_up() {
            if (_start > 0) {
                _start--;
                _end--;
                update_visible_list();
            }
        }

};
