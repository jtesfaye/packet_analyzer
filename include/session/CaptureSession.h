//
// Created by jeremiah tesfaye on 10/9/25.
//

#ifndef CAPTURESESSION_H
#define CAPTURESESSION_H

#include <packet/PcapFile.h>
#include <util/PacketUtil.h>
#include <util/SparsePacketBuffer.h>
#include <util/LRUCache.h>
#include <capture/PacketCapture.h>
#include <capture/CaptureConfig.h>
#include <queue>

enum class CommandType {
  Start,
  Stop,
  Save,
  End,
};

struct SessionCommand {
  CommandType type;
  std::variant<std::monostate, std::string, int, size_t, CaptureConfig> cmd_data;
  static SessionCommand start(const CaptureConfig config) {
    return { CommandType::Start, {config} };
  }
  static SessionCommand stop() {
    return { CommandType::Stop,{} };
  }
  static SessionCommand save(const std::string& path) {
    return { CommandType::Save, {path} };
  }
  static SessionCommand end() { return {
    CommandType::End, {}};
  }
};

using InitialParseBuffer = SparsePacketBuffer<packet_ref>;
using DetailParseCache = LRUCache<std::vector<ProtocolDetails>>;

class CaptureSession {
public:

  CaptureSession();
  ~CaptureSession();

  void send_command(const SessionCommand& cmd);
  void start_session();

  InitialParseBuffer& get_buffer() const;
  DetailParseCache& get_cache() const;
  PacketObserver& get_observer() const;

private:
  void process_cmd(const SessionCommand& cmd);
  void start_capture(const CaptureConfig& config);
  void stop_capture() const;
  bool save_capture(const std::string& path) const;

  void initialize_online_handle(
    const std::string& device_name,
    u_int8_t settings,
    int capture_size,
    const std::string& filter
    );

  void initialize_offline_handle(const std::string& path);
  void apply_filter(const std::string& device_name, const std::string& filter);
  static void close_handle(pcap_t* handle);
  static void free_bpf_program(bpf_program* program);

  std::queue<SessionCommand> commands;
  std::atomic<bool> running;
  std::atomic<bool> capture_on;
  std::mutex lock;
  std::condition_variable cv;

  struct Engine {

    raw_pkt_queue m_raw_pkt_queue;
    InitialParser m_initial_parser;
    DetailParser m_detail_parser;
    InitialParseBuffer m_pkt_ref_buffer;
    DetailParseCache m_details_cache;
    StreamTable m_table;
    PacketObserver m_observer;
    ParsingEngine m_engine;

    Engine(const int data_link_type, const u_int8_t flags, const size_t buffer_size)
    : m_initial_parser(pcap_dlt_to_ieee.at(data_link_type), flags)
    , m_pkt_ref_buffer(buffer_size)
    , m_details_cache(buffer_size)
    , m_observer(ObserverInit{m_pkt_ref_buffer, m_details_cache, m_table})
    , m_engine(EngineInit{
      m_initial_parser,
      m_detail_parser,
      m_pkt_ref_buffer,
      m_details_cache,
      m_observer,
      m_raw_pkt_queue,
      std::thread::hardware_concurrency(),
      m_table}) {
    }

    ~Engine() {
      m_engine.shutdown();
      m_observer.set_done();
    }
  };

  std::unique_ptr<Engine> m_engine;
  std::unique_ptr<PacketCapture> capture;
  std::shared_ptr<PcapFile> m_pcap_file;
  std::unique_ptr<pcap_t, decltype(&close_handle)> m_handle;
  std::unique_ptr<bpf_program, decltype(&free_bpf_program)> m_bpf_program;
};

#endif //CAPTURESESSION_H
