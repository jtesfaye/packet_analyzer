

#ifndef OFFLINE_H
#define OFFLINE_H

#include <capture/PacketCapture.h>

class Offline final : public PacketCapture {
public:

  Offline(pcap_t* handle, const std::shared_ptr<PcapFile>&);
  Offline() = delete;
  Offline(const Offline&) = delete;
  Offline& operator= (const Offline&) = delete;

  ~Offline() = default;

private:

  void capture_func() override;

  void stop_func() override;

  std::shared_ptr<PcapFile> m_file;
};




#endif