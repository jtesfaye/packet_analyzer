
#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H

#include <pcap/pcap.h>
#include <string>
#include <packet/PcapFile.h>
#include <util/PacketRefBuffer.h>
#include <util/ThreadPool.h>
#include <util/PacketObserver.h>
#include <parsing/PacketParse.h>

namespace capture {

  constexpr int FULL = 65535;
  constexpr int DEFAULT = 256;
  constexpr int BASIC = 128;

  constexpr u_int8_t FULL_CAP = 0x80;
  constexpr u_int8_t BASIC_CAP = 0x40;
  constexpr u_int8_t PROMISC = 0x20;
  constexpr u_int8_t IMMEDIATE = 0x10;
  constexpr u_int8_t MONITOR = 0x08;
  constexpr u_int8_t PRECISION = 0x04;
  constexpr u_int8_t HIGH_TRAFF = 0x02;

}

enum class CaptureState {
  Idle,
  Running,
  Paused
};

class PacketCapture {
public:

  void start_capture();

  void stop_capture();

  static std::unique_ptr<PacketCapture> createOnlineCapture(
    pcap_t* handle,
    int packet_count,
    size_t layer_flags,
    const std::shared_ptr<PcapFile> &file
    );

  static std::unique_ptr<PacketCapture> createOfflineCapture(
    pcap_t* handle,
    const std::shared_ptr<PcapFile> &file
    );

  std::shared_ptr<PacketRefBuffer> get_buffer();

  std::shared_ptr<PacketObserver> get_observer();

  static std::vector<std::string> get_devices();

  int get_datalink() const;

  PacketCapture(const PacketCapture&) = delete;
  PacketCapture& operator=(const PacketCapture&) = delete;
  virtual ~PacketCapture();

protected:

  explicit PacketCapture(pcap_t* h);

  virtual void capture_func() = 0;

  virtual void stop_func() = 0;

  pcap_t* handle() const;

  void set_data_link(int dlt);

  void set_buffer(const std::shared_ptr<PacketRefBuffer>&);

  void set_observer(const std::shared_ptr<PacketObserver>&);

  pcap_t* _handle;

  int m_data_link;

  char errbuf[PCAP_ERRBUF_SIZE]{};

  std::shared_ptr<PacketRefBuffer> _buffer;
  std::shared_ptr<PacketObserver> observer;

};




#endif