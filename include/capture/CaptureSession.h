//
// Created by jeremiah tesfaye on 10/9/25.
//

#ifndef CAPTURESESSION_H
#define CAPTURESESSION_H

#include <pcap/pcap.h>
#include <packet/PcapFile.h>
#include <util/PacketBuffer.h>
#include <util/LRUCache.h>
#include <capture/PacketCapture.h>
#include <capture/CaptureConfig.h>


enum class CommandType {
  Start,
  Stop,
  Save,
  GetDetails,
  End
};

struct SessionCommand {

  CommandType type;
  std::variant<std::monostate, std::string, int> cmd_data;

  static SessionCommand start() { return {CommandType::Start, {}}; }
  static SessionCommand stop() { return {CommandType::Stop,{}}; }
  static SessionCommand save(const std::string& path) { return {CommandType::Save, {path}}; }
  static SessionCommand get_details(int index) {return {CommandType::GetDetails, {index}};}
  static SessionCommand end() { return {CommandType::End, {}}; }

};

class CaptureSession {
public:

  explicit CaptureSession(const CaptureConfig& config);
  ~CaptureSession() = default;

  void send_command(const SessionCommand& cmd);

  void start_session();

  std::shared_ptr<IContainerType<packet_ref>> get_buffer() const;
  std::shared_ptr<PacketObserver> get_observer() const;

private:

  void process_cmd(const SessionCommand& cmd);

  void start_capture() const;
  void stop_capture() const;
  bool save_capture(const std::string& path) const;
  void get_details(int index);

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

  std::mutex lock;
  std::condition_variable cv;

  std::shared_ptr<InitialParser> m_initial_parser;
  std::shared_ptr<DetailParser> m_detail_parser;

  std::unique_ptr<PacketCapture> capture;
  std::shared_ptr<LRUCache<ProtocolDetails>> m_cache;
  std::shared_ptr<IContainerType<packet_ref>> m_buffer;
  std::shared_ptr<PacketObserver> m_observer;
  std::shared_ptr<PcapFile> m_pcap_file;
  std::shared_ptr<ThreadPool> m_pool;

  std::unique_ptr<pcap_t, decltype(&close_handle)> m_handle;
  std::unique_ptr<bpf_program, decltype(&free_bpf_program)> m_bpf_program;

};

#endif //CAPTURESESSION_H
