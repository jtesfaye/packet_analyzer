//
// Created by jeremiah tesfaye on 3/11/26.
//

#ifndef SNIFFER_API_H
#define SNIFFER_API_H
#include <capture/CaptureConfig.h>
#include <interfaces/IEventSink.h>
#include <util/PktRingBuffer.h>

class PythonSink final : public IEventSink
{
public:

  enum EventMsgType
  {
    Empty, Packet, Detail, Stat
  };

  struct EventMsg
  {
    EventMsgType type;
    std::string msg;
  };

  PythonSink() = default;
  void on_packet_batch(std::vector<packet_data>&) override;
  void on_detail_request(std::shared_ptr<const std::vector<ProtocolDetails>>) override;
  void on_stream_update(const StreamStatistics &stats) override;
  EventMsg poll();
  ~PythonSink() override = default;

private:

  std::mutex mutex;
  std::queue<EventMsg> msg_queue;

};
#endif //SNIFFER_API_H
