
#include <parsing/PacketParse.h>
#include <layer2_protocols/Layer2.h>
#include <layer3_protocols/Layer3.h>
#include <layer4_protocols/Layer4.h>

namespace parse {

  PacketParse::PacketParse(int dlt, u_int8_t flags)
  : m_dlt{dlt}
  , m_flags{flags}
  , link_parser(m_dlt, Layer2::get_all_functions())
  , net_parser(Layer3::get_all_functions())
  , transport_parser(Layer4::get_all_functions())
  , m_inital_time(-1, -1)
  {}


  //TODO: Refactor variants to hold object with human readable data
  std::pair<row_entry,packet_ref>
  PacketParse::start_extract(const std::vector<std::byte> &raw_data) {

    /**
     * 1.Determine what layers exist
     * 2. find highest existing layer
     * 3. store highest layer for description/ protocol/
     */

    packet_ref pkt_ref;
    layer_offsets offsets{};
    parse_context context{};
    row_entry& entry = context.entry;

    std::memcpy(&context.header, raw_data.data(), sizeof(pcap_pkthdr));

    entry.length = std::to_string(context.header.caplen);

    timeval& packet_ts = context.header.ts;

    //find time relative to first capture
    if (init_time_has_value()) {

      entry.time = set_relative_time(packet_ts);

    } else {

      set_inital_time(packet_ts);
      entry.time = 0;

    }

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

  void PacketParse::set_inital_time(timeval &time) {

    m_inital_time = time;

  }

  bool PacketParse::init_time_has_value() const {

    if (m_inital_time.tv_sec == -1)
      return false;


    return true;

  }

  double PacketParse::set_relative_time(const timeval &time) const {

    return static_cast<double>(time.tv_sec - m_inital_time.tv_sec) +
      (time.tv_usec - m_inital_time.tv_usec) / 1e6;

  }


}