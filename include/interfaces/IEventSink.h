//
// Created by jeremiah tesfaye on 12/20/25.
//

#ifndef IEVENTSINK_H
#define IEVENTSINK_H

struct IEventSink {
  virtual void on_packet_batch(size_t index_first, size_t index_last) = 0;
  virtual void on_stream_update(size_t packet_index) = 0;
  virtual ~IEventSink() = default;
};

#endif //IEVENTSINK_H
