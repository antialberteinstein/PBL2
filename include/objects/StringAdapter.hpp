#ifndef PBL_ADAPTER_HPP
#define PBL_ADAPTER_HPP

#include "eins/tui_support.h"
#include "objects/Vector.hpp"
#include <memory>

class StringAdapter : public ftxui::ConstStringListRef::Adapter {
    public:
        static std::unique_ptr<StringAdapter> From(Vector<std::string>* entries) {
            return std::make_unique<StringAdapter>(entries);
        }
        explicit StringAdapter(Vector<std::string>* entries) : entries_(entries) {}
        ~StringAdapter() final = default;
        size_t size() const final { return entries_->size(); }
        std::string operator[](size_t index) const final { return entries_->at(index); }

    private:
        Vector<std::string>* entries_;
};

#endif
