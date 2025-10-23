
#include <iostream>
#include <parsing/PacketParse.h>



PacketParse::PacketParse(int dlt, u_int8_t flags)
: m_dlt{dlt}
, m_flags{flags}
, m_inital_time()
, link_parser(m_dlt, Layer2::get_all_functions())
, net_parser(Layer3::get_all_functions())
, transport_parser(Layer4::get_all_functions())
{}

packet_ref
PacketParse::start_extract(const std::vector<std::byte> &raw_data, const size_t index) {

  packet_ref pkt_ref{};
  layer_offsets offsets{};
  parse_context context{};

  pkt_ref.index = index;
  pkt_ref.length = 0;

  std::memcpy(&context.header, raw_data.data(), sizeof(pcaprec_hdr_t));
  std::cout << context.header.ts_sec << "\n";
  std::cout << context.header.ts_usec << "\n";

  const parse_time time {context.header.ts_sec, context.header.ts_usec};

  //find time relative to first capture
  pkt_ref.time = set_relative_time(time);

  const std::vector<LayerJob> jobs = create_jobs();

  for (const auto&[func] : jobs) {

    //if there's an error parsing we come across an unsupported type, return false and terminate loop
    if (bool keep_going = func(pkt_ref, raw_data, context, offsets); !keep_going)

      break;

  }

  pkt_ref.data = offsets;

  return pkt_ref;

}

std::vector<PacketParse::LayerJob>
PacketParse::create_jobs() {

  std::vector<LayerJob> jobs;

  auto layer2_job = [&](
    packet_ref& pkt,
    const std::vector<std::byte>& data,
    parse_context& context,
    layer_offsets& offsets) {

    context.offset = sizeof(pcaprec_hdr_t);

    pkt.layer2 = link_parser(data, context);

    offsets.l2.length = context.curr_length;
    offsets.l2.offset = context.offset;
    offsets.l2.protocol_type = m_dlt;

    context.prev_length = context.curr_length;

    if (!pkt.layer2)
      return false;

    pkt.length += context.curr_length;

    return true;

  };

  jobs.push_back({layer2_job});

  if (m_flags & parse::DO_LAYER3) {

    auto layer3_job = [&](
    packet_ref& pkt,
    const std::vector<std::byte>& data,
    parse_context& context,
    layer_offsets& offsets) {

      context.offset += context.prev_length;

      offsets.l3.protocol_type = context.next_type;

      pkt.layer3 = net_parser(context.next_type, data, context);

      offsets.l3.length = context.curr_length;
      offsets.l3.offset = context.offset;

      context.prev_length = context.curr_length;

      if (!pkt.layer3) {
        return false;
      }

      if (context.is_fragmented) {
        return false;
      }

      pkt.length += context.curr_length;

      return true;

    };

    jobs.push_back({layer3_job});

  }

  if (m_flags & parse::DO_LAYER4) {

    auto layer4_job = [&](
    packet_ref& pkt,
    const std::vector<std::byte>& data,
    parse_context& context,
    layer_offsets& offsets) {

      context.offset += context.prev_length;

      offsets.l4.protocol_type = context.next_type;

      pkt.layer4 = transport_parser(context.next_type, data, context);

      offsets.l4.length = context.curr_length;
      offsets.l4.offset = context.offset;

      context.prev_length = context.curr_length;

      if (!pkt.layer4)
        return false;

      pkt.length += context.curr_length;

      return true;

    };

    jobs.push_back({layer4_job});

  }

  return jobs;

}

void PacketParse::set_initial_time(const parse_time &time) {

  std::call_once(time_init_flag, [&]() {
    m_inital_time = time;
  });

}

double PacketParse::set_relative_time(const parse_time &time) {

  set_initial_time(time);

  return (time.ts_sec - m_inital_time.ts_sec) +
    (time.ts_usec - m_inital_time.ts_usec) / 1e6;

}
