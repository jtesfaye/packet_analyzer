//
// Created by jeremiah tesfaye on 6/26/25.
//

#ifndef CAPTURECONTROLLER_H
#define CAPTURECONTROLLER_H

#include <capture/CaptureSession.h>
#include <model/DisplayModel.h>
#include <view/StartupWindow.h>

#include <view/SessionForm.h>
#include <QThread>

class CaptureController : public QObject {

  Q_OBJECT

public:

  CaptureController();
  ~CaptureController() override = default;

  void start_capture(const CaptureConfig&);

  void stop_online_capture(const std::shared_ptr<CaptureSession>& cap);

public slots:
  void recieve_row(std::deque<packet::packet_ref>::iterator, std::deque<packet::packet_ref>::iterator) const;
  void receive_details(const std::vector<ProtocolDetails>&);
  void recieve_row_index(const QModelIndex&);

  signals:
  void send_row_to_model(std::deque<packet_ref>::iterator, std::deque<packet_ref>::iterator) const;
  void forward_detail_request(size_t index);

private:

  void init_start_btn(QPushButton* btn);
  void init_stop_btn(QPushButton* btn, const std::shared_ptr<CaptureSession>& ses);
  void connect_observer_to_this(const CaptureSession&);
  void connect_session_to_table_view(const QTableView&);


  void start_session();

  void connect_session_form_to_main();

  void prompt_save() const;

  StartupWindow* m_window_view;
  std::shared_ptr<CaptureSession> current_session;
  std::shared_ptr<DisplayModel> m_model;

  QThread* consumer_thread;

};



#endif //CAPTURECONTROLLER_H
