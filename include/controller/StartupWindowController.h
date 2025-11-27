//
// Created by jeremiah tesfaye on 11/24/25.
//

#ifndef STARTUPWINDOWCONTROLLER_H
#define STARTUPWINDOWCONTROLLER_H

#include <QObject>
#include <session/CaptureSession.h>
#include <view/StartupWindow.h>
#include <controller/CaptureController.h>

class StartupWindowController : public QObject {
    Q_OBJECT

public:

    StartupWindowController();
    ~StartupWindowController() override = default;

private:

    void init_start_btn(QPushButton* btn);
    void init_stop_btn(QPushButton* btn);
    void connect_session_form_to_main();

    StartupWindow* main_window;
    CaptureController capture_controller{};
    std::weak_ptr<CaptureSession> current_session;

};

#endif //STARTUPWINDOWCONTROLLER_H
