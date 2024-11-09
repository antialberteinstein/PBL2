#ifndef PBL_SET_HPP
#define PBL_SET_HPP

#include "objects/Vector.hpp"

class Set {
    public:

        template <typename T>
        static Vector<T> union_(const Vector<T>& a, const Vector<T>& b) {
            Vector<T> result = a;
            for (size_t i = 0; i < b.size(); ++i) {
                if (!result.contains(b[i])) {
                    result.push_back(b[i]);
                }
            }
        }

        template <typename T>
        static Vector<T> intercept_(const Vector<T>& a, const Vector<T>& b) {
            Vector<T> result;
            for (size_t i = 0; i < a.size(); ++i) {
                if (b.contains(a[i])) {
                    result.push_back(a[i]);
                }
            }
            return result;
        }

        template <typename T>
        static Vector<T> except_(const Vector<T>& a, const Vector<T>& b) {
            Vector<T> result;
            for (size_t i = 0; i < a.size(); ++i) {
                if (!b.contains(a[i])) {
                    result.push_back(a[i]);
                }
            }
            return result;
        }
};

#endif