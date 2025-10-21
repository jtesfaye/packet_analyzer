//
// Created by jeremiah tesfaye on 6/26/25.
//

#ifndef CAPTURECONTROLLER_H
#define CAPTURECONTROLLER_H

#include <string>
#include <sys/_types/_u_int8_t.h>
#include <capture/CaptureSession.h>
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

  void start_capture(const CaptureConfig&);

  void stop_online_capture(const std::shared_ptr<CaptureSession>& cap);

private:

  void init_start_btn(QPushButton* btn);

  void init_stop_btn(QPushButton* btn, const std::shared_ptr<CaptureSession>& ses);

  void connect_observer_to_model(const CaptureSession&);

  void start_session();

  void connect_session_form_to_main();

  void prompt_save() const;

  StartupWindow* m_window_view;

  std::shared_ptr<CaptureSession> current_session;
  QThread* consumer_thread;

};



#endif //CAPTURECONTROLLER_H
