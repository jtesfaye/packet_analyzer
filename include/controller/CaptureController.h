//
// Created by jeremiah tesfaye on 6/26/25.
//

#ifndef CAPTURECONTROLLER_H
#define CAPTURECONTROLLER_H

#include <string>
#include <sys/_types/_u_int8_t.h>
#include "../../include/capture/PacketCapture.h"
#include "../../include/util/PacketBuffer.h"
#include "../model/DisplayModel.h"
#include "../view/MainWindow.h"

#include <QThread>
#include <thread>

Q_DECLARE_METATYPE(proccessed_packet)

class CaptureController : public QObject {

  Q_OBJECT

public:
  CaptureController();
  ~CaptureController() override = default;

  void start_online_capture(std::string, int, int, u_int8_t, u_int8_t);

  DisplayModel* get_model();

  signals:

  void update_model(const proccessed_packet&);

  void display_details(std::string& s);

  public slots:

  void get_packet_details(int row);

  void send_to_model(const proccessed_packet&);

private:

  void init_menubar();

  void start_session();

  MainWindow* m_window_view;

  DisplayModel* m_model;

  QThread* consumer_thread;

  std::unique_ptr<capture::PacketCapture> capture;

  std::shared_ptr<PacketBuffer> buffer;

};



#endif //CAPTURECONTROLLER_H
