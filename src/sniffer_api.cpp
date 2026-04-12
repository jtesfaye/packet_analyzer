//
// Created by jeremiah tesfaye on 3/11/26.
//
#include <sniffer_api.h>
#include <util/PktRingBuffer.h>
#include <util/PacketUtil.h>
#include <mutex>
#include <queue>

void PythonSink::on_packet_batch(std::vector<packet_data>& view)
{
  std::lock_guard<std::mutex> l(mutex);
  for (auto & iter : view)
  {
    //std::cout << iter->index << std::endl;
    auto json = packet::to_json(iter);
    msg_queue.push(EventMsg{Packet, std::move(boost::json::serialize(json))});
  }
}

void PythonSink::on_stream_update(const StreamStatistics &stats)
{
    std::lock_guard<std::mutex> l(mutex);
    msg_queue.push(EventMsg{Stat, serialize(to_json(stats))});
}

void PythonSink::on_detail_request(std::shared_ptr<const std::vector<ProtocolDetails>> details)
{
  std::lock_guard<std::mutex> l(mutex);
  boost::json::array detail_arr;
  auto pdetails = *details;
  for (const auto& d : pdetails)
  {
    const auto msg = to_json(d);
    detail_arr.push_back(msg);
  }
  msg_queue.push(EventMsg{Detail, boost::json::serialize(detail_arr)});
}

PythonSink::EventMsg PythonSink::poll()
{
  std::lock_guard<std::mutex> l(mutex);
  if (msg_queue.empty())
  {
    return {None, ""};
  }
  auto msg = msg_queue.front();
  msg_queue.pop();
  return msg;
}




