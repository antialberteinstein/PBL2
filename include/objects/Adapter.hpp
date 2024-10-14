#ifndef PBL_ADAPTER_HPP
#define PBL_ADAPTER_HPP

#include "eins/tui_design/ftxui_includes.h"
#include "objects/Vector.hpp"
#include <memory>

class Adapter : public ftxui::ConstStringListRef::Adapter {
    public:
        static std::unique_ptr<Adapter> From(Vector<std::string>* entries) {
            return std::make_unique<Adapter>(entries);
        }
        explicit Adapter(Vector<std::string>* entries) : entries_(entries) {}
        ~Adapter() final = default;
        size_t size() const final { return entries_->size(); }
        std::string operator[](size_t index) const final { return entries_->at(index); }

    private:
        Vector<std::string>* entries_;
};

#endif