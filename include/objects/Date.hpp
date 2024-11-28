#ifndef PBL_DATE_HPP
#define PBL_DATE_HPP

#include <string>
#include <chrono>
#include <ctime>
#include <sstream>

inline int get_local_time(std::tm* localtime, const std::time_t* now) {
#ifdef _WIN32
    return localtime_s(localtime, now) == 0;
#else
    return localtime_r(now, localtime) != nullptr;
#endif
}


// Date class support Gregorian date format.
class Date {
    public:
        Date(int day, int month, int year);
        int get_day() const;
        int get_month() const;
        int get_year() const;

        Date& operator++();
        Date operator++(int);

        Date& operator+=(int days);
        Date operator+(int days) const;

        bool operator==(const Date& other) const;
        bool operator!=(const Date& other) const;
        bool operator<(const Date& other) const;
        bool operator<=(const Date& other) const;
        bool operator>(const Date& other) const;
        bool operator>=(const Date& other) const;

        // Gregorian date format: DD/MM/YYYY
        std::string to_string() const;

        static Date today();
    private:
        int day;
        int month;
        int year;
};

// Convert from string to Date object following the format.
class DateConverter {
    public:
        // Convert from standard date format (sqlite3 support): YYYY-MM-DD
        static Date from_string(const std::string& date);

        // Convert to standard date format (sqlite3 support): YYYY-MM-DD
        static std::string to_string(const Date& date);

        // Convert from Gregorian date format: DD/MM/YYYY
        static Date from_grenadian_string(const std::string& date);

        // Convert to Gregorian date format: DD/MM/YYYY
        static std::string to_grenadian_string(const Date& date);

};

class GettingLocalTimeException : public std::exception {
    public:
        GettingLocalTimeException(const std::string& message="") : message(message) {}

        std::string get_message() {
            return "Error getting local time: " + message + "!!";
        }
    private:
        std::string message;
};

#endif
