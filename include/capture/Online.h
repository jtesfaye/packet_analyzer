
#ifndef ONLINE_H
#define ONLINE_H

#include <capture/PacketCapture.h>
#include <cstring>
#include <sys/_types/_u_int8_t.h>

using namespace packet::frame;

class Online final : public PacketCapture {
public:

  Online() = delete;
  Online(const Online&) = delete;
  Online& operator= (const Online&) = delete;

  Online(std::string&& device_name, int count, int capture_size, u_int8_t set, u_int8_t flags);

  ~Online() override;

protected:

  void get_link_types() const;

private:

  static void pcap_loop_callback(u_char* data, const pcap_pkthdr* header, const u_char* packet);

  void capture_func() override;

  struct capture_objects {

    static capture_objects setup_capture(
      const char* file_path,
      PacketParse& parser,
      const std::shared_ptr<PacketRefBuffer>&,
      int thread_count,
      pcap_t* handle,
      PacketObserver&
      );

    static capture_objects make(PacketParse& p,
      PacketRefBuffer* b,
      std::shared_ptr<ThreadPool> tp,
      PacketObserver& o,
      std::shared_ptr<PcapFile> f);

    PacketParse& parser;
    PacketRefBuffer* pktref_buffer;
    std::shared_ptr<ThreadPool> tpool;
    PacketObserver& pkt_observer;
    std::shared_ptr<PcapFile> file;

    capture_objects(PacketParse& p,
      PacketRefBuffer* b,
      std::shared_ptr<ThreadPool> tp,
      PacketObserver& o,
      std::shared_ptr<PcapFile> f);
  };

  void initialize_handle(int capture_size);

  std::string _device;
  u_int8_t _settings;
  u_int8_t m_flags;

};





#endif