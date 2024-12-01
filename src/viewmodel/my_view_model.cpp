#include "viewmodel/my_view_model.hpp"
#include <csignal>

void debug(const string& s) {
    std::ofstream out("temp/debug.txt", std::ios::app);
    out << s << std::endl;
    out.close();
}


ModelProducer* ModelProducer::student_instance_ = nullptr;
ModelProducer* ModelProducer::room_instance_ = nullptr;
ModelProducer* ModelProducer::vehicle_instance_ = nullptr;
ModelProducer* ModelProducer::room_fee_payment_instance_ = nullptr;
ModelProducer* ModelProducer::electricity_payment_instance_ = nullptr;

leveldb::Options ModelProducer::options_ = leveldb::Options();

ModelProducer* ModelProducer::get_instance(ModelType type) {
    if (!options_.create_if_missing) {
        options_.create_if_missing = true;
    }

    switch (type) {
        case ModelType::STUDENT: {
            if (student_instance_ == nullptr) {
                student_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::STUDENT),
                    &student_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            student_instance_->type_ = ModelType::STUDENT;
            // return student_instance_.get();
            return student_instance_;
        }
        case ModelType::ROOM: {
            if (room_instance_ == nullptr) {
                room_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ROOM),
                    &room_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            room_instance_->type_ = ModelType::ROOM;
            // return room_instance_.get();
            return room_instance_;
        }
        case ModelType::VEHICLE: {
            if (vehicle_instance_ == nullptr) {
                vehicle_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::VEHICLE),
                    &vehicle_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            vehicle_instance_->type_ = ModelType::VEHICLE;
            // return vehicle_instance_.get();
            return vehicle_instance_;
        }
        case ModelType::ROOM_FEE_PAYMENT: {
            if (room_fee_payment_instance_ == nullptr) {
                room_fee_payment_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ROOM_FEE_PAYMENT),
                    &room_fee_payment_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            room_fee_payment_instance_->type_ = ModelType::ROOM_FEE_PAYMENT;
            // return room_fee_payment_instance_.get();
            return room_fee_payment_instance_;
        }
        case ModelType::ELECTRICITY_PAYMENT: {
            if (electricity_payment_instance_ == nullptr) {
                electricity_payment_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ELECTRICITY_PAYMENT),
                    &electricity_payment_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            electricity_payment_instance_->type_ = ModelType::ELECTRICITY_PAYMENT;
            // return electricity_payment_instance_.get();
            return electricity_payment_instance_;
        }
    }
    return nullptr;
}

void signal_handler(int signum) {
    debug("Interrupt signal (" + to_string(signum) + ") received.");

    ModelProducer::cleanup();

    exit(signum);
}

void ModelProducer::init() {
    try {
        atexit(ModelProducer::cleanup);

        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
        signal(SIGABRT, signal_handler);
        signal(SIGSEGV, signal_handler);
        signal(SIGILL, signal_handler);
        signal(SIGFPE, signal_handler);  // Linux only, floating point exception
        signal(SIGBUS, signal_handler);  // Linux only, bus error
    } catch (const std::exception& e) {
        debug(e.what());
    } catch (...) {
        debug("Unknown error");
    }
}

void ModelProducer::cleanup() {
    if (student_instance_ != nullptr) {
        delete student_instance_;
    }

    if (room_instance_ != nullptr) {
        delete room_instance_;
    }

    if (vehicle_instance_ != nullptr) {
        delete vehicle_instance_;
    }

    if (room_fee_payment_instance_ != nullptr) {
        delete room_fee_payment_instance_;
    }

    if (electricity_payment_instance_ != nullptr) {
        delete electricity_payment_instance_;
    }

    student_instance_ = nullptr;
    room_instance_ = nullptr;
    vehicle_instance_ = nullptr;
    room_fee_payment_instance_ = nullptr;
    electricity_payment_instance_ = nullptr;
}

// Regexing Pattern

int compare_by_dictionary(const string& a, const string& b) {
    int a_size = a.size();
    int b_size = b.size();
    int _min = (a_size < b_size) ? a_size : b_size;
    for (int i = 0; i < _min; ++i) {
        if (a[i] > b[i]) {
            return MORE_THAN;
        } else if (a[i] < b[i]) {
            return LESS_THAN;
        }
    }
    return EQUAL;
}

bool operator==(const RegexingPattern& lhs, const RegexingPattern& rhs) {
    int _check = compare_by_dictionary(lhs.first_name, rhs.first_name);
    if (_check != EQUAL) {
        return false;
    }

    _check = compare_by_dictionary(lhs.last_name, rhs.last_name);
    return _check == EQUAL;
}

bool operator>(const RegexingPattern& lhs, const RegexingPattern& rhs) {
    int _check = compare_by_dictionary(lhs.first_name, rhs.first_name);
    if (_check == EQUAL) {
        return (compare_by_dictionary(lhs.last_name, rhs.last_name) == MORE_THAN);
    }
    return _check == MORE_THAN;
}

bool operator>=(const RegexingPattern& lhs, const RegexingPattern& rhs) {
    return lhs > rhs || lhs == rhs;
}

bool operator<(const RegexingPattern& lhs, const RegexingPattern& rhs) {
    return !(lhs >= rhs);
}

bool operator<=(const RegexingPattern& lhs, const RegexingPattern& rhs) {
    return lhs < rhs || lhs == rhs;
}

RegexingPattern regex_name(const string& name) {
    string first_name = "", last_name = "";
    istringstream iss(name);
    Vector<string> regexes;
    string temp;
    while (iss >> temp) {
        regexes.push_back(temp);
    }

    if (regexes.size() == 1) {
        first_name = regexes[0];
    } else {
        first_name = regexes[regexes.size() - 1];
        for (int i = 0; i < regexes.size() - 1; ++i) {
            last_name += regexes[i] + " ";
        }
        last_name.pop_back();
    }


    // Convert to lower case

    for (int i = 0; i < first_name.size(); ++i) {
        first_name[i] = tolower(first_name[i]);
    }

    for (int i = 0; i < last_name.size(); ++i) {
        last_name[i] = tolower(last_name[i]);
    }

    return {first_name, last_name};
}



Vector<RegexingPattern> regex_name(Vector<Student>& students) {
    Vector<RegexingPattern> result;
    Vector<string> regexes;
    for (int i = 0; i < students.size(); ++i) {
        result.push_back(regex_name(students[i].get_name()));
    }
    return result;
}

void insertion_sort(Vector<string>& keys, Vector<RegexingPattern>& regexes) {
    for (int i = 1; i < keys.size(); ++i) {
        string key = keys[i];
        RegexingPattern regex = regexes[i];
        int j = i - 1;
        while (j >= 0 && regex < regexes[j]) {
            keys[j + 1] = keys[j];
            regexes[j + 1] = regexes[j];
            j = j - 1;
        }
        keys[j + 1] = key;
        regexes[j + 1] = regex;
    }
}

int binary_search(RegexingPattern& name, Vector<RegexingPattern>& regexes) {
    int low = 0;
    int high = regexes.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (regexes[mid] == name) {
            debug(regexes[mid].first_name + " == " + name.first_name);
            return mid;
        }

        if (regexes[mid] < name) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return NOT_FOUND;
}
