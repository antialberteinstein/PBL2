#include "apps/RoomDetail.hpp"
#include "models/Student.hpp"

#include "viewmodel/my_view_model.hpp"
#include "apps/RoomList.hpp"
#include "apps/RoomReservation.hpp"
#include "viewmodel/fee_calculator.hpp"

unique_ptr<RoomReservation> room_reservation = nullptr;

RoomDetail::RoomDetail(App* parent, unique_ptr<Room> room)
        : room(move(room)) {
    this->parent = parent;
    error_message = "";

    register_btn = Button("Đăng ký bao phòng", [&] {
        if (this->room != nullptr) {
            room_reservation = make_unique<RoomReservation>(this->parent, this->room->get_id());
            if (room_reservation != nullptr) {
                room_reservation->run();
            }
        }
    }, ButtonOption::Animated(Color::Blue));


    payment_btn = Button("Thanh toán tiền điện", [&] {
        try {
            auto ef_cal = FeeCalculator::get_instance(FeeType::ELECTRICITY_FEE);
            if (ef_cal) {
                ef_cal->pay_by(this->room.get());
            }
        } catch (const string& msg) {
            error_message = msg;
        } catch (...) {
            error_message = "Không co hoa don tiền điện!!";
        }
    }, ButtonOption::Animated(Color::Blue1));
    
    if (this->room != nullptr && this->room->get_status() == RoomStatus::MAINTENANCE) {
        notify_maintain_btn = Button("TB Phong da sua chua", [&] {
            if (this->room != nullptr) {
                this->room->set_status(RoomStatus::AVAILABLE);
                try {
                    auto room_db = ModelProducer::get_instance(ModelType::ROOM);
                    if (room_db) {
                        room_db->modify(this->room->get_id(), this->room.get());
                        this->parent->run();
                    } else {
                        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
                    }
                } catch (const string& msg) {
                    error_message = "Lỗi kết nối cơ sở dữ liệu!!";
                }
                this->parent->run();
            }
        }, ButtonOption::Animated(Color::SkyBlue3));
    } else {
        notify_maintain_btn = Button("Thong bao sua chua", [&] {
            if (this->room != nullptr) {
                this->room->set_status(RoomStatus::MAINTENANCE);
                try {
                    auto room_db = ModelProducer::get_instance(ModelType::ROOM);
                    if (room_db) {
                        room_db->modify(this->room->get_id(), this->room.get());
                        
                        // Reload database.
                        auto temp = dynamic_cast<RoomList*>(this->parent);
                        if (temp) {
                            temp->init_db();
                        }

                        this->parent->run();
                    } else {
                        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
                    }
                } catch (const string& msg) {
                    error_message = "Lỗi kết nối cơ sở dữ liệu!!";
                }
                this->parent->run();
            }
        }, ButtonOption::Animated(Color::SkyBlue1));
    }

    return_btn = Button("Quay lại", [&] {
        this->parent->run();
    }, ButtonOption::Animated(Color::LightSkyBlue1));

}

Element RoomDetail::create_element() {
    Element detail_box = text("Không có thông tin phòng");
    Elements texts;
    if (room != nullptr) {
        texts.push_back(text("Mã phòng: " + room->get_id()));
        texts.push_back(text("Tình trạng: " + room->get_status()));
        if (room->get_status() != RoomStatus::MAINTENANCE) {
            Elements _students;

            Vector<Student> students = Student::get_students_living_in(room->get_id());
            Student::fit_room(room->get_id());
            for (int i = 0; i < students.size(); i++) {
                string amount = "Không nợ tiền phòng";
                try {
                    auto rf_cal = FeeCalculator::get_instance(FeeType::ROOM_FEE);
                    if (rf_cal) {
                        auto payment = rf_cal->get_payment(&students[i]);
                        if (payment != nullptr && payment->get_status() != PaymentStatus::PAID) {
                            if (payment->get_status() == PaymentStatus::OVERDUE) {
                                amount = to_string(payment->get_amount()) + " VND (Quá hạn)";
                            } else {
                                amount = to_string(payment->get_amount()) + " VND (Chưa thanh toán)";
                            }
                        }
                    }
                } catch (const string& msg) {
                    error_message = msg;
                } catch (...) {
                    error_message = "Lỗi không xác định!!";
                }
                _students.push_back(text(
                    "   "
                    + to_string(i + 1) + "/. "
                    + students[i].get_name()
                    + " - " + amount));
            }

            string fee = "Không có";
            try {
                auto ef_cal = FeeCalculator::get_instance(FeeType::ELECTRICITY_FEE);
                if (ef_cal) {
                    auto ef = ef_cal->get_payment(room.get());
                    if (ef != nullptr) {
                        fee = to_string(ef->get_amount()) + " VND";
                    }
                }
            } catch (...) {
                error_message = "Lỗi không xác định khi kiểm tra tiền điện!!";
            }

            texts.push_back(text("Tiền điện còn thiếu: " + fee));

            texts.push_back(text("Sức chứa: " + to_string(room->get_capacity())));
            texts.push_back(text("Số sinh viên hiện tại: " + to_string(room->get_current_number())));
            texts.push_back(vbox(_students));
        }
    detail_box = vbox(texts);
    }

    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        hbox({
            detail_box | border | flex,
            vbox({
                register_btn->Render() | flex,
                payment_btn->Render() | flex,
                notify_maintain_btn->Render() | flex,
                return_btn->Render() | flex,
            }) | flex,

        }),
        text(error_message) | center | color(ERROR_COLOR),
    });
}

bool RoomDetail::event(Event event) {
    return payment_btn->OnEvent(event)
        || notify_maintain_btn->OnEvent(event)
        || return_btn->OnEvent(event)
        || register_btn->OnEvent(event);
}
