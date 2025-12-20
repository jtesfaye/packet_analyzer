

#ifndef OFFLINE_H
#define OFFLINE_H

#include <capture/PacketCapture.h>

class Offline final : public PacketCapture {
public:
  Offline(CaptureInit init);
  Offline() = delete;
  Offline(const Offline&) = delete;
  Offline& operator= (const Offline&) = delete;
  ~Offline() override = default;

private:
  void capture_func() override;
};
#endif