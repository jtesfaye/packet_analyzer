//
// Created by jeremiah tesfaye on 11/24/25.
//

#include <controller/StartupWindowController.h>
#include <QPushButton>

StartupWindowController::StartupWindowController()
: main_window(new StartupWindow())
{
    init_start_btn(main_window->get_start_session_btn());
    init_stop_btn(main_window->get_stop_session_btn());
    connect_session_form_to_main();

    main_window->show();
}

void StartupWindowController::connect_session_form_to_main() {

    const auto form = main_window->get_form();

    connect(form->get_start_session_button(), &QPushButton::clicked, this, [this, form] () {

        const auto config = form->get_config();

        capture_controller->start_capture(
            config,
            *main_window->get_table_view(),
            *main_window->get_tree_view()
            );

        main_window->get_form()->hide();
    });

}

void StartupWindowController::init_start_btn(QPushButton *btn) {

    connect(btn, &QPushButton::clicked, this, [=]() {

        if (current_session.lock()) {
            capture_controller->end_capture();
        }

        main_window->get_form()->show();
    });
}

void StartupWindowController::init_stop_btn(QPushButton *btn) {

    connect(btn, &QPushButton::clicked, this, [=]() {
        capture_controller->stop_capture();
    });
}


