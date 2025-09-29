//
// Created by jeremiah tesfaye on 6/26/25.
//

#ifndef CAPTURECONTROLLER_H
#define CAPTURECONTROLLER_H

#include <string>
#include <sys/_types/_u_int8_t.h>
#include <capture/PacketCapture.h>
#include <model/DisplayModel.h>
#include <view/StartupWindow.h>
#include <util/PacketRefBuffer.h>
#include <view/SessionForm.h>
#include <QThread>
#include <thread>

class CaptureController : public QObject {

  Q_OBJECT

public:

  CaptureController();
  ~CaptureController() override = default;

  void start_online_capture(std::string, int, int, u_int8_t, u_int8_t);

private:

  void init_menubar();

  void start_session();

  void connect_session_form_to_main();

  StartupWindow* m_window_view;

  QThread* consumer_thread;

  std::unique_ptr<PacketCapture> capture;

};



#endif //CAPTURECONTROLLER_H
