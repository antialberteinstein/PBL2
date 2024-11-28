#pragma once


// Use for handling triggers in the database
class Trigger {
    public:

        // This is a form of polymorphism

        virtual bool onPrepareToAdd() {
            if (prepare_added_flag) {
                // Do something.

                prepare_added_flag = false;
            }
            return true;
        }

        virtual void on_add() {
            if (added_flag) {
                // Do something

                added_flag = false;
            }
        }

        virtual void on_remove() {
            if (removed_flag) {
                // Do something

                removed_flag = false;
            }
        }

        virtual void on_modify() {
            if (modified_flag) {
                // Do something

                modified_flag = false;
            }
        }

        Trigger() {
            added_flag = false;
            removed_flag = false;
            modified_flag = false;
            prepare_added_flag = false;
        }
    
    protected:
        bool added_flag;
        bool removed_flag;
        bool modified_flag;
        bool prepare_added_flag;

};