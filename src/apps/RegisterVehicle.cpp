#include "apps/RegisterVehicle.hpp"
#include "objects/Vector.hpp"
#include "viewmodel/my_view_model.hpp"
#include "apps/StudentDetail.hpp"
#include "apps/MainMenu.hpp"

RegisterVehicle::RegisterVehicle(App* parent, string student_id) {
    error_message = "";
    this->student_id = student_id;
    this->parent = parent;

    masoTf = {"Mã số sinh viên", this->student_id, Input(&masoTf.value, "")};

    Vector<string> loaiXe;
    loaiXe.push_back(VehicleType::MOTORBIKE);
    loaiXe.push_back(VehicleType::CAR);
    loaiXe.push_back(VehicleType::BICYCLE);

    loaiXeTf = {
        "Loại xe",
        loaiXe,
        0,
        Dropdown(StringAdapter::From(&loaiXeTf.values), &loaiXeTf.selected)
    };

    vm_option = MenuOption::Vertical();
    vm_option.selected = &vm_selected;
    vm_option.focused_entry = &vm_selected;

    vehicles_menu = Menu(StringAdapter::From(&vm_list), &vm_selected, vm_option);

    confirm_btn = Button("Xác nhận", [&] {
        if (this->vm_list.size() >= 1) {
            error_message = "Hiện tại chỉ cho phép đăng ký tối đa 1 xe / 1 sinh viên.";
            return;
        }
        try {
            auto vehicle_db = ModelProducer::get_instance(ModelType::VEHICLE);
            if (vehicle_db) {
                auto vehicle = make_unique<Vehicle>();
                vehicle->set_type(loaiXeTf.values[loaiXeTf.selected]);
                vehicle->set_student_id(this->student_id);
                vehicle->flag_creating();
                vehicle_db->add(vehicle.get());
                if (this->parent != nullptr) {

                    this->parent->run();
                } else {
                    main_menu::show();
                }
            }
        } catch (const string& msg) {
            error_message = msg;
        }
    }, ButtonOption::Animated(Color::Green3));

    return_btn = Button("Quay lại", [&] {
        if (this->parent != nullptr) {
            this->parent->run();
        } else {
            main_menu::show();
        }
    }, ButtonOption::Animated(Color::Orange1));

    delete_btn = Button("Xoá phương tiện này (danh sách bên trái)", [&] {
        try {
            auto vehicle_db = ModelProducer::get_instance(ModelType::VEHICLE);
            if (vehicle_db) {
                string the_id = this->student_id;
                for (int i = 1; i <= vm_selected; ++i) {
                    the_id += "@" + this->student_id;
                }
                string next_id;
                auto this_vehicle = vehicle_db->get_vehicle(the_id);
                while (true) {
                    next_id = the_id + "@" + this->student_id;
                    auto next_vehicle = vehicle_db->get_vehicle(next_id);
                    if (this_vehicle == nullptr) {
                        break;
                    }

                    if (next_vehicle == nullptr) {
                        vehicle_db->remove(this_vehicle.get());
                        break;
                    }
                    vehicle_db->modify(the_id, next_vehicle.get());
                    this_vehicle = move(next_vehicle);
                    the_id = next_id;
                }
            }
        } catch (const string& msg) {
            error_message = msg;
        }
    }, ButtonOption::Animated(Color::Red1));
}

Element RegisterVehicle::create_element() {
    auto title_box = get_title().get_doc() | color(TITLE_COLOR);

    auto temp = text("Không có thông tin sinh viên");
    try {
        auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
        if (student_db) {
            auto student = student_db->get_student(this->student_id);
            Vector<Vehicle> vehicles = Vehicle::get_vehicles(this->student_id);
            vm_list.clear();
            for (int i = 0; i < vehicles.size(); i++) {
                vm_list.push_back(
                    "   " + to_string(i + 1) + "/. " + vehicles[i].get_type()
                );
            }
            if (student) {
                temp = vbox({
                    vbox({
                        text("Họ và tên: " + student->get_name()),
                        text("Phòng: " + student->get_room()),
                        text("Ngày sinh: " + student->get_dob()),
                    }) | border | flex,
                    vbox({
                        text("Phương tiện sinh viên này đang sử dụng:"),
                        vehicles_menu->Render(),
                    }) | border | flex,
                });
            }
        }

    } catch (const string& msg) {
        error_message = msg;
    }

    return vbox({
        title_box,
        separator(),
        text(error_message) | center | color(ERROR_COLOR),
        hbox({
            vbox({
                get_doc(masoTf) | flex,
                temp | border | flex,
            }) | flex,
            vbox({
                get_doc(loaiXeTf) | flex,
                confirm_btn->Render() ,
                return_btn->Render(),
                delete_btn->Render(),
            }) | flex,
        }),
    });
}

bool RegisterVehicle::event(Event event) {


    return confirm_btn->OnEvent(event)
        || return_btn->OnEvent(event)
        || loaiXeTf.com->OnEvent(event)
        || masoTf.com->OnEvent(event)
        || vehicles_menu->OnEvent(event)
        || delete_btn->OnEvent(event);
}