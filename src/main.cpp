//
// Created by jeremiah tesfaye on 7/31/25.
//
#include <sniffer_api.h>
#include <session/CaptureSession.h>
int main(int argc, char** argv) {
  PythonSink sink;
  CaptureSession session(sink);
  CaptureConfig config;
  config.capture_size = 255;
  config.flags = 1;
  config.filter = "";
  config.mode = CaptureMode::Online;
  config.settings = 0xff;
  config.packet_count = 5;
  config.source = "en0";

  session.start_session();
  auto cmd = SessionCommand::start(config);
  session.send_command(std::move(cmd));
  int count{};
  while (count < 5)
  {
    auto msg = sink.poll();
    if (msg.type == PythonSink::Packet)
    {
      count += 1;
      std::cout << msg.msg << std::endl;
    }
  }
  cmd = SessionCommand::stop();
  session.send_command(std::move(cmd));
  cmd = SessionCommand::end();
  session.send_command(std::move(cmd));
}