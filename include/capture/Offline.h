

#ifndef OFFLINE_H
#define OFFLINE_H

#include <capture/PacketCapture.h>

class Offline final : public PacketCapture {
public:

  Offline(pcap_t* handle, int dlt, const std::shared_ptr<PcapFile>&, const std::shared_ptr<ThreadPool>&);
  Offline() = delete;
  Offline(const Offline&) = delete;
  Offline& operator= (const Offline&) = delete;

  ~Offline() = default;

private:

  void capture_func() override;

  void stop_func() override;
};




#endif