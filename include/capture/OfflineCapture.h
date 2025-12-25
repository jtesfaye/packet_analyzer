

#ifndef OFFLINECAPTURE_H
#define OFFLINECAPTURE_H

#include <capture/PacketCapture.h>

class OfflineCapture final : public PacketCapture {
public:
  OfflineCapture(CaptureInit init);
  OfflineCapture() = delete;
  OfflineCapture(const OfflineCapture&) = delete;
  OfflineCapture& operator= (const OfflineCapture&) = delete;
  ~OfflineCapture() override = default;

private:
  void capture_func() override;
};
#endif