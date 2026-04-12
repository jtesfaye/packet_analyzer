//
// Created by jeremiah tesfaye on 12/20/25.
//

#ifndef IEVENTSINK_H
#define IEVENTSINK_H

#include <stream/StreamStatistics.h>
#include <queue>
#include <util/PktRingBuffer.h>
struct IEventSink {
  virtual void on_packet_batch(std::vector<packet_data>&) = 0;
  virtual void on_detail_request(std::shared_ptr<const std::vector<ProtocolDetails>>) = 0;
  virtual void on_stream_update(const StreamStatistics& stats) = 0;
  virtual ~IEventSink() = default;
};



#endif //IEVENTSINK_H
