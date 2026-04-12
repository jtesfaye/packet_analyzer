//
// Created by jeremiah tesfaye on 1/25/26.
//

#include <gtest/gtest.h>
#include <server/UnixDomainSocketServer.h>
#include <thread>
#include <filesystem>
#include <fstream>
class DomainSocketServerTest : public  testing::Test {
protected:

  using WorkGuard = boost::asio::executor_work_guard<io_context::basic_executor_type<std::allocator<void>, 0>>;

  DomainSocketServerTest()
    : client_socket(client_context) {
    std::filesystem::remove(path);
    endpoint = stream_protocol::endpoint(path);
    server = std::make_unique<UnixDomainSocketServer>(server_context, path);
    server_thread = std::move(std::thread{[&] { server_context.run(); }});
    client_socket.connect(endpoint);
  }

  ~DomainSocketServerTest() override{
    server_context.stop();
    client_context.stop();
    server->shutdown_server();
    server_work_guard.reset();
    client_work_guard.reset();

    if (server_thread.joinable()) {
      server_thread.join();
    }
    std::filesystem::remove(path);
  }

  const std::string path{"/Users/jt/projects/workspace/personal_projects/sniffer/tests/udsocket"};
  io_context client_context;
  io_context server_context;
  stream_protocol::endpoint endpoint;
  stream_protocol::socket client_socket;

  std::unique_ptr<UnixDomainSocketServer> server;
  WorkGuard client_work_guard = make_work_guard(client_context);
  WorkGuard server_work_guard = make_work_guard(server_context);
  std::thread server_thread;

};

static std::vector<std::byte> make_message(const boost::json::object& obj, CommandType type) {
  pktmsg::PktMsgHeader header{};
  const auto payload = serialize(obj);
  header.payload_length = htonl(payload.size());
  header.total_length   = htonl(sizeof(header) + payload.size());
  header.type           = htonl(static_cast<uint32_t>(type));
  std::vector<std::byte> buffer;
  buffer.resize(sizeof(header) + payload.size());
  std::memcpy(buffer.data(), &header, sizeof(header));
  std::memcpy(buffer.data() + sizeof(header), payload.data(), payload.size());
  return buffer;
}

TEST_F(DomainSocketServerTest, ServerReadTest) {

  pktmsg::PktMsgHeader header{};
  boost::json::object obj;
  boost::json::object args;
  args["file_path"] = "foobar.pcap";
  obj["args"] = args;

  client_socket.write_some(boost::asio::buffer(make_message(obj, CommandType::Save)));
  client_context.run_for(std::chrono::milliseconds(1000));
  SUCCEED();
}

TEST_F(DomainSocketServerTest, ServerSessionStart) {
  boost::json::object capture_config;
  boost::json::object settings;
  boost::json::object args;

  capture_config["mode"]         = static_cast<int>(CaptureMode::Online);
  capture_config["source"]       = "en10";
  capture_config["packet_count"] = 10;
  capture_config["capture_size"] = 255;
  settings["immediate"]          = true;
  settings["promisc"]            = true;
  settings["precision"]          = true;
  capture_config["settings"]     = settings;
  capture_config["filter"]       = nullptr;
  args["args"]                   = capture_config;

  auto buf = make_message(args, CommandType::Start);
  boost::asio::write(client_socket, boost::asio::buffer(buf));
  client_context.run_for(std::chrono::milliseconds(1000));
  auto cmd = server->get_queue().front();
  ASSERT_EQ(cmd.type, CommandType::Start);
  ASSERT_EQ(std::get<CaptureConfig>(cmd.arg).mode,CaptureMode::Online);
  ASSERT_EQ(std::get<CaptureConfig>(cmd.arg).source, "en10");
  ASSERT_EQ(std::get<CaptureConfig>(cmd.arg).packet_count, 10);
  ASSERT_EQ(std::get<CaptureConfig>(cmd.arg).capture_size, 255);
  ASSERT_EQ(std::get<CaptureConfig>(cmd.arg).filter, "n/a");
}

TEST_F(DomainSocketServerTest, ServerCanGetDetails) {
  boost::json::object obj;
  boost::json::object args;
  args["packet_id"] = 1;
  obj["args"] = args;
  auto buf = make_message(obj, CommandType::GetDetails);
  boost::asio::write(client_socket, boost::asio::buffer(buf));
  client_context.run_for(std::chrono::milliseconds(1000));
  auto cmd = server->get_queue().front();
  ASSERT_EQ(cmd.type, CommandType::GetDetails);
  ASSERT_EQ(std::get<size_t>(cmd.arg), 1);
}
