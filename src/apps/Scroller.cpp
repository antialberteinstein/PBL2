#include "apps/Scroller.hpp"

int max_of(Vector<int>& arr) {
    int max = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int min_of(Vector<int>& arr) {
    int min = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

string regex_search(const string& s) {
    size_t n = s.size();
    string res = "";
    for (size_t i = 0; i < n; ++i) {
        if (s[i] == ' ') {
            continue;
        }
        if (s[i] >= 'A' && s[i] <= 'Z') {
            res += s[i] - 'A' + 'a';
        } else {
            res += s[i];
        }
    }
    return res;
}

bool all(Vector<bool>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (!arr[i]) {
            return false;
        }
    }
    return true;
}

bool _any(Vector<bool>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i]) {
            return true;
        }
    }
    return false;
}