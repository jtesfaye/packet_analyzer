//
// Created by jeremiah tesfaye on 10/9/25.
//

#ifndef CAPTURESESSION_H
#define CAPTURESESSION_H

#include <QObject>
#include <packet/PcapFile.h>
#include <packet/PacketUtil.h>
#include <util/SparsePacketBuffer.h>
#include <util/LRUCache.h>
#include <capture/PacketCapture.h>
#include <capture/CaptureConfig.h>
#include <queue>

enum class CommandType {
  Start,
  Stop,
  Save,
  End
};

struct SessionCommand {

  CommandType type;
  std::variant<std::monostate, std::string, int> cmd_data;

  static SessionCommand start() { return { CommandType::Start, {} }; }
  static SessionCommand stop() { return { CommandType::Stop,{} }; }
  static SessionCommand save(const std::string& path) { return { CommandType::Save, {path} }; }
  static SessionCommand end() { return { CommandType::End, {}}; }

};

using InitialParseBuffer = SparsePacketBuffer<packet_ref>;
using DetailParseCache = LRUCache<std::vector<ProtocolDetails>>;

class CaptureSession {
public:

  explicit CaptureSession(const CaptureConfig& config);

  ~CaptureSession();

  void send_command(const SessionCommand& cmd);

  void start_session();

  InitialParseBuffer& get_buffer() const;
  DetailParseCache& get_cache() const;
  std::shared_ptr<PacketObserver> get_observer() const;

private:

  void process_cmd(const SessionCommand& cmd);

  void start_capture();
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

  raw_pkt_queue m_raw_pkt_queue;

  std::shared_ptr<DetailParseCache> m_details_cache;
  std::shared_ptr<InitialParseBuffer> m_pkt_ref_buffer;

  std::shared_ptr<InitialParser> m_initial_parser;
  std::shared_ptr<DetailParser> m_detail_parser;

  std::unique_ptr<PacketCapture> capture;

  std::shared_ptr<PacketObserver> m_observer;
  std::shared_ptr<PcapFile> m_pcap_file;
  std::shared_ptr<ParsingEngine> m_pool;
  StreamTable table;

  std::unique_ptr<pcap_t, decltype(&close_handle)> m_handle;
  std::unique_ptr<bpf_program, decltype(&free_bpf_program)> m_bpf_program;



};

#endif //CAPTURESESSION_H
