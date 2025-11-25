//
// Created by jeremiah tesfaye on 6/26/25.
//

#ifndef CAPTURECONTROLLER_H
#define CAPTURECONTROLLER_H

#include <session/CaptureSession.h>
#include <model/RowModel.h>
#include <view/StartupWindow.h>

#include <view/SessionForm.h>
#include <QThread>
#include <model/DetailModel.h>

class CaptureController : public QObject {

  Q_OBJECT

public:

  CaptureController();
  ~CaptureController() override = default;

  void start_capture(const CaptureConfig&, QTableView &table, QTreeView &tree);
  void stop_capture();
  void end_capture();

public slots:

  void recieve_row(std::deque<packet_ref>::iterator, std::deque<packet_ref>::iterator) const;
  void receive_details(const std::vector<ProtocolDetails>&);
  void recieve_row_index(const QModelIndex&);

  signals:
  void send_row_to_model(std::deque<packet_ref>::iterator, std::deque<packet_ref>::iterator) const;
  void forward_detail_request(size_t index);
  void prompt_save() const;

private:

  void connect_observer_to_this(const CaptureSession&, QTableView&, QTreeView&);
  void connect_session_to_table_view(const QTableView&);

  bool stopped = false;

  std::shared_ptr<CaptureSession> current_session;
  std::shared_ptr<RowModel> m_row_model;
  std::shared_ptr<DetailModel> m_detail_model;

  QThread* consumer_thread;

};



#endif //CAPTURECONTROLLER_H
