
#include <parsing/PacketParse.h>
#include <layerx/ProtocolTypes.h>

namespace parse {

  PacketParse::PacketParse(int dlt, u_int8_t flags)
  : m_dlt{dlt}
  , m_flags{flags}
  , link_parser(m_dlt, Layer2::get_all_functions())
  , net_parser(Layer3::get_all_functions())
  , transport_parser(Layer4::get_all_functions())
  , m_inital_time()
  {}

  std::pair<row_entry,packet_ref>
  PacketParse::start_extract(const std::vector<std::byte> &raw_data, const size_t index) {

    packet_ref pkt_ref;
    layer_offsets offsets{};
    row_entry entry {};
    parse_context context{};

    timeval& packet_ts = context.header.ts;

    //find time relative to first capture
    entry.index = index;

    entry.time = set_relative_time(packet_ts);

    const std::vector<LayerJob> jobs = create_jobs(m_flags);

    for (auto& job : jobs) {

      job.func(pkt_ref, raw_data, context, offsets);

    }

    pkt_ref.data = offsets;

    return {entry, pkt_ref};

  }


  std::vector<PacketParse::LayerJob>
  PacketParse::create_jobs(u_int8_t flags) {

    std::vector<LayerJob> jobs;

    auto layer2_job = [&](packet_ref& pkt, auto& data, auto& context, auto& offsets) {

      context.offset = sizeof(pcap_pkthdr);

      pkt.layer2 = link_parser(data, context);

      offsets.l2.length = context.length;
      offsets.l2.offset = context.offset;

    };

    jobs.emplace_back(layer2_job);

    if (m_flags & DO_LAYER3) {

      auto layer3_job = [&](auto& pkt, auto& data, auto& context, auto& offsets) {

        context.offset = context.length;

        pkt.layer3 = net_parser(context.next_type, data, context);

        offsets.l3.length = context.length;
        offsets.l3.offset = context.offset;

      };

      jobs.emplace_back(layer3_job);

    }

    if (m_flags & DO_LAYER4) {

      auto layer4_job = [&](auto& pkt, auto& data, auto& context, auto& offsets) {

        context.offset = context.length;

        pkt.layer4 = transport_parser(context.next_type, data, context);

        offsets.l4.length = context.length;
        offsets.l4.offset = context.offset;

      };

      jobs.emplace_back(layer4_job);

    }

    return std::move(jobs);

  }

  void PacketParse::set_inital_time(const timeval &time) {

    std::call_once(time_init_flag, [&]() {
      m_inital_time = time;
    });

  }

  double PacketParse::set_relative_time(const timeval &time) {

    set_inital_time(time);

    return static_cast<double>(time.tv_sec - m_inital_time.tv_sec) +
      (time.tv_usec - m_inital_time.tv_usec) / 1e6;

  }


}