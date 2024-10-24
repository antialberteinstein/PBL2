#ifndef PBL_DATE_HPP
#define PBL_DATE_HPP

#include <iostream>
#include <string>

// Date class support Gregorian date format.
class Date {
    public:
        Date(int day, int month, int year);
        int get_day() const;
        int get_month() const;
        int get_year() const;

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