//
// Created by jeremiah tesfaye on 10/9/25.
//

#ifndef CAPTURESESSION_H
#define CAPTURESESSION_H

#include <queue>
#include <packet/PcapFile.h>
#include <capture/CaptureConfig.h>
#include <session/SessionCommand.h>
#include <interfaces/IEventSink.h>

class CaptureEngine;

class CaptureSession {
public:

  const size_t RING_BUFFER_SIZE {1000};
  const size_t CACHE_SIZE {200};

  explicit CaptureSession(IEventSink& sink);
  ~CaptureSession();

  void send_command(SessionCommand&& cmd);
  void start_session();

private:

  void process_cmd(SessionCommand& cmd);
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

  IEventSink& sink;

  std::queue<SessionCommand> commands;
  std::atomic<bool> running;
  std::atomic<bool> capture_on;
  std::mutex lock;
  std::condition_variable cv;

  raw_packet_queue raw_pkt_queue;
  std::unique_ptr<CaptureEngine> m_engine;
  std::shared_ptr<PcapFile> m_pcap_file;
  std::unique_ptr<pcap_t, decltype(&close_handle)> m_handle;
  std::unique_ptr<bpf_program, decltype(&free_bpf_program)> m_bpf_program;
};

#endif //CAPTURESESSION_H
