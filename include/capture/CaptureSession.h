//
// Created by jeremiah tesfaye on 10/9/25.
//

#ifndef CAPTURESESSION_H
#define CAPTURESESSION_H

#include <pcap/pcap.h>
#include <packet/PcapFile.h>
#include <util/PacketRefBuffer.h>
#include <capture/PacketCapture.h>
#include <capture/CaptureConfig.h>

enum class CommandType {
  Start,
  Stop,
  Save,
  End
};

struct SessionCommand {

  CommandType type;
  std::variant<std::monostate, std::string> cmd_data;

  static SessionCommand start() { return {CommandType::Start, {}}; }
  static SessionCommand stop() { return {CommandType::Stop,{}}; }
  static SessionCommand save(const std::string& path) { return {CommandType::Save, {path}}; }
  static SessionCommand end() { return {CommandType::End, {}}; }

};

class CaptureSession {
public:

  explicit CaptureSession(const CaptureConfig& config);
  ~CaptureSession() = default;

  void send_command(const SessionCommand& cmd);

  void start_session();

  std::shared_ptr<PacketRefBuffer> get_buffer() const;
  std::shared_ptr<PacketObserver> get_observer() const;

private:

  void process_cmd(const SessionCommand& cmd);

  void start_capture() const;
  void stop_capture() const;
  bool save_capture(const std::string& path) const;

  void initialize_online_handle(const std::string& device_name, u_int8_t settings, int capture_size);
  void initialize_offline_handle(const std::string& path);

  static void close_handle(pcap_t* handle);

  std::queue<SessionCommand> commands;

  std::atomic<bool> running;

  std::mutex lock;
  std::condition_variable cv;

  std::unique_ptr<PacketCapture> capture;
  std::unique_ptr<pcap_t, decltype(&close_handle)> m_handle;

  std::shared_ptr<PcapFile> m_pcap_file;
};

#endif //CAPTURESESSION_H
