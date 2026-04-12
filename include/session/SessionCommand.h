//
// Created by jeremiah tesfaye on 1/4/26.
//

#ifndef SESSIONCOMMAND_H
#define SESSIONCOMMAND_H

#include <util/PktRingBuffer.h>
#include <capture/CaptureConfig.h>

enum class CommandType : u_int32_t {
  Start = 100,
  Stop = 101,
  Save = 102,
  End = 103,
  SendPackets = 104,
  GetDetails = 105,
  GetStats = 106
};

using CmdArgs = std::variant<
  std::monostate,
  std::string,
  int,
  size_t,
  CaptureConfig,
  std::vector<packet_data>
>;

struct SessionCommand {
  CommandType type;
  CmdArgs arg;
  static SessionCommand start(CaptureConfig config) {
    return { CommandType::Start, {config} };
  }
  static SessionCommand stop() {
    return { CommandType::Stop,{} };
  }
  static SessionCommand save(const std::string& path) {
    return { CommandType::Save, {path} };
  }
  static SessionCommand end() {
    return {CommandType::End, {}};
  }
  static SessionCommand send_packets(std::vector<packet_data> v) {
    return {CommandType::SendPackets, std::move(v)};
  }
  static SessionCommand get_details(size_t pkt_id) {
    return {CommandType::GetDetails, {pkt_id}};
  }
  static SessionCommand get_statistics(size_t pkt_id) {
    return {CommandType::GetStats, {pkt_id}};
  }
};

#endif //SESSIONCOMMAND_H
