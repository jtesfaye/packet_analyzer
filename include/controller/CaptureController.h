//
// Created by jeremiah tesfaye on 6/26/25.
//

#ifndef CAPTURECONTROLLER_H
#define CAPTURECONTROLLER_H

#include <session/CaptureSession.h>
#include <model/RowModel.h>
#include <view/StartupWindow.h>
#include <model/Models.h>
#include <QThread>
#include <model/DetailModel.h>
#include <view/ViewComponents.h>

class CaptureController : public QObject {

  Q_OBJECT

public:

  CaptureController();
  ~CaptureController() override = default;

  void start_capture(const CaptureConfig&, ViewComponents&);
  void stop_capture();
  void end_capture();

private:
  void prompt_save() const;
  bool stopped = false;

  std::shared_ptr<CaptureSession> current_session;
  std::shared_ptr<RowModel> m_row_model;
  std::shared_ptr<DetailModel> m_detail_model;

  QThread* consumer_thread;

};



#endif //CAPTURECONTROLLER_H
