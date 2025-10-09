
#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H

#include <pcap/pcap.h>
#include <parsing/PacketParse.h>
#include <string>
#include <packet/PcapFile.h>
#include <util/PacketRefBuffer.h>
#include <util/ThreadPool.h>
#include <util/PacketObserver.h>

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

class PacketCapture {
public:

  static std::unique_ptr<PacketCapture> createOnlineCapture(
    std::string& device_name,
    int count,
    int capture_size,
    u_int8_t settings,
    u_int8_t flags);

  static std::unique_ptr<PacketCapture> createOfflineCapture(std::string& path_name);

  std::shared_ptr<PacketRefBuffer> get_buffer();

  std::shared_ptr<PacketObserver> get_observer();

  static std::vector<std::string> get_devices();

  int get_datalink() const;

  void start_capture();

  void stop_capture();

  PacketCapture(const PacketCapture&) = delete;
  PacketCapture& operator=(const PacketCapture&) = delete;
  virtual ~PacketCapture();

protected:

  explicit PacketCapture();

  virtual void capture_func() = 0;

  static void free_devices(pcap_if_t* devlist);

  static void close_handle(pcap_t* handle);

  pcap_t* handle() const;

  pcap_if_t* devices() const;

  void set_data_link(int dlt);

  void set_buffer(const std::shared_ptr<PacketRefBuffer>&);

  void set_observer(const std::shared_ptr<PacketObserver>&);

  std::unique_ptr<pcap_t, decltype(&close_handle)> _handle;

  std::unique_ptr<pcap_if_t, decltype(&free_devices)> _device_list;

  int _packets_to_capture;

  int m_data_link;

  char errbuf[PCAP_ERRBUF_SIZE]{};

  std::shared_ptr<PacketRefBuffer> _buffer;
  std::shared_ptr<PacketObserver> observer;

};




#endif