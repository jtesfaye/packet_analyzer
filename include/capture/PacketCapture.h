
#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H

#include <pcap/pcap.h>
#include <string>
#include <packet/PcapFile.h>
#include <util/PktRingBuffer.h>
#include <functional>
#include <queue>
#include <util/PacketUtil.h>

using onRawPkt = std::function<void()>;
struct CaptureInit {
  pcap_t* handle;
  const std::shared_ptr<PcapFile> &file;
  raw_packet_queue& queue;
};



class PacketCapture {
public:

  void start_capture();
  void stop_capture();
  void set_callback(onRawPkt&& fn);
  raw_packet_queue& get_queue() const;
  static std::unique_ptr<PacketCapture> createOnlineCapture(int packet_count, size_t layer_flags, CaptureInit init);
  static std::unique_ptr<PacketCapture> createOfflineCapture(CaptureInit init);
  static std::vector<std::string> get_devices();


  PacketCapture(const PacketCapture&) = delete;
  PacketCapture& operator=(const PacketCapture&) = delete;
  virtual ~PacketCapture();

protected:
  onRawPkt on_pkt_fn;
  explicit PacketCapture(CaptureInit init);
  virtual void capture_func() = 0;
  virtual void stop_func() {};

  pcap_t* handle() const;
  pcap_t* _handle;
  char errbuf[PCAP_ERRBUF_SIZE]{};
  std::shared_ptr<PcapFile> file;
  raw_packet_queue& queue;


};

#endif