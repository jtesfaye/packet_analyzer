
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

  class PacketCapture {
  protected:

    int _packets_to_capture;

    int m_data_link;

    char errbuf[PCAP_ERRBUF_SIZE]{};

    static void free_devices(pcap_if_t* devlist);

    static void close_handle(pcap_t* handle);

    std::shared_ptr<PacketRefBuffer> _buffer;

    std::unique_ptr<pcap_t, decltype(&close_handle)> _handle;

    std::unique_ptr<pcap_if_t, decltype(&free_devices)> _device_list;

    [[nodiscard]] pcap_t* handle() const;

    [[nodiscard]] pcap_if_t* devices() const;

    void set_data_link(int dlt);

    void set_buffer(const std::shared_ptr<PacketRefBuffer> &);

    explicit PacketCapture();

  public:

    std::shared_ptr<PacketRefBuffer> get_buffer();

    static std::unique_ptr<PacketCapture> createOnlineCapture
    (std::string& device_name, int count, int capture_size, u_int8_t settings, u_int8_t flags);

    static std::unique_ptr<PacketCapture> createOfflineCapture
    (std::string& path_name,int count);

    int get_datalink() const;

    static std::vector<std::string> get_devices();

    void start_capture() const;

    void stop_capture();

    PacketCapture(const PacketCapture&) = delete;
    PacketCapture& operator=(const PacketCapture&) = delete;

    virtual ~PacketCapture();

  private:

    static void pcap_loop_callback(u_char* data, const pcap_pkthdr* header, const u_char* packet);

    struct capture_objects {

      static capture_objects setup_capture(
        const char* file_path,
        parse::PacketParse& parser,
        const std::shared_ptr<PacketRefBuffer>&,
        int thread_count,
        pcap_t* handle
        );

      static capture_objects make(parse::PacketParse& p,
        PacketRefBuffer* b,
        ThreadPool& tp,
        PacketObserver& o,
        PcapFile& f);

      parse::PacketParse& parser;
      PacketRefBuffer* pktref_buffer;
      ThreadPool& tpool;
      PacketObserver& pkt_observer;
      PcapFile& file;

      capture_objects(parse::PacketParse& p,
        PacketRefBuffer* b,
        ThreadPool& tp,
        PacketObserver& o,
        PcapFile& f);
    };

  };

}



#endif