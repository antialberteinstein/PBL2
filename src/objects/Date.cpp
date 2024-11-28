#include "objects/Date.hpp"

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {}

int Date::get_day() const {
    return day;
}

int Date::get_month() const {
    return month;
}

int Date::get_year() const {
    return year;
}

std::string Date::to_string() const {
    std::string day = ((this->day < 10) ? "0" : "") + std::to_string(this->day);
    std::string month = ((this->month < 10) ? "0" : "") + std::to_string(this->month);
    std::string year = std::to_string(this->year);
    return day + "/" + month + "/" + year;
}

Date Date::today() {
    std::time_t now = std::time(nullptr);

    std::tm local_time;
    if (!get_local_time(&local_time, &now)) {
        throw GettingLocalTimeException("Failed to get local time");
    }
    return Date(local_time.tm_mday, local_time.tm_mon + 1, local_time.tm_year + 1900);
}

Date DateConverter::from_string(const std::string& date) {
    char delimiter1, delimiter2;
    int day, month, year;
    std::stringstream ss(date);
    ss >> year >> delimiter1 >> month >> delimiter2 >> day;
    return Date(day, month, year);
}

std::string DateConverter::to_string(const Date& date) {
    std::string day = ((date.get_day() < 10) ? "0" : "") + std::to_string(date.get_day());
    std::string month = ((date.get_month() < 10) ? "0" : "") + std::to_string(date.get_month());
    std::string year = std::to_string(date.get_year());
    return year + "-" + month + "-" + day;
}

Date DateConverter::from_grenadian_string(const std::string& date) {
    char delimiter1, delimiter2;
    int day, month, year;
    std::stringstream ss(date);
    ss >> day >> delimiter1 >> month >> delimiter2 >> year;
    return Date(day, month, year);
}

std::string DateConverter::to_grenadian_string(const Date& date) {
    std::string day = ((date.get_day() < 10) ? "0" : "") + std::to_string(date.get_day());
    std::string month = ((date.get_month() < 10) ? "0" : "") + std::to_string(date.get_month());
    std::string year = std::to_string(date.get_year());
    return day + "/" + month + "/" + year;
}

Date& Date::operator++() {
    day++;
    if ((month < 8 && month&1 && day > 31)
        || (month >= 8 && !(month&1) && day > 31)) {
        day = 1;
        month++;
    } else if ((month >= 8 && month&1 && day > 30)
            || (month < 8 && !(month&1) && day > 30)) {
        day = 1;
        month++;
    } else if (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month == 2 && day > 29) {
        day = 1;
        month++;
    } else if (((year % 4 != 0 || year % 100 == 0) && year % 400 != 0) && month == 2 && day > 28) {
        day = 1;
        month++;
    }

    if (month > 12) {
        month = 1;
        year++;
    }
    return *this;
}

Date Date::operator++(int) {
    Date temp = *this;
    ++(*this);
    return temp;
}

Date& Date::operator+=(int days) {
    for (int i = 0; i < days; i++) {
        ++(*this);
    }
    return *this;
}

Date Date::operator+(int days) const {
    Date temp = *this;
    temp += days;
    return temp;
}

bool Date::operator==(const Date& other) const {
    return day == other.day
        && month == other.month
        && year == other.year;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year < other.year) {
        return true;
    } else if (year > other.year) {
        return false;
    }

    if (month < other.month) {
        return true;
    } else if (month > other.month) {
        return false;
    }

    return day < other.day;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}


bool Date::operator>(const Date& other) const {
    return !(*this <= other);
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}
