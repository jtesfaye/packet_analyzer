
#include <iostream>
#include <parsing/PacketParse.h>



PacketParse::PacketParse(int dlt, u_int8_t flags)
: m_dlt{dlt}
, m_flags{flags}
, m_inital_time()
, first_parse_dispatcher(Layer::get_first_parse_functions())
, detail_parse_dispatcher(Layer::get_detail_parse_functions())
{}

packet_ref PacketParse::start_extract(
  const std::vector<std::byte> &raw_data,
  const size_t index) {

  packet_ref pkt_ref{};
  layer_offsets offsets{};
  parse_context context{};

  pkt_ref.index = index;
  pkt_ref.length = 0;
  std::memcpy(&context.header, raw_data.data(), sizeof(pcaprec_hdr_t));
  const parse_time time {context.header.ts_sec, context.header.ts_usec};

  //find time relative to first capture
  pkt_ref.time = set_relative_time(time);

  const std::vector<LayerJob> jobs = create_first_parse_jobs();

  for (const auto& job: jobs) {

    //if there's an error parsing we come across an unsupported type, return false and terminate loop
    if (bool keep_going = job.func(pkt_ref, raw_data, context, offsets); !keep_going)

      break;
  }

  pkt_ref.data = offsets;

  return pkt_ref;

}

std::vector<ProtocolDetails> PacketParse::detail_parse(
  const std::vector<std::byte> &raw_data,
  const layer_offsets &offsets) {

  std::vector<ProtocolDetails> details_arr;
  const std::vector<LayerJob> jobs = create_detail_parse_jobs();
  parse_context cxt{};

  for (const auto& job : jobs) {

    ProtocolDetails details{std::move(job.detail_func(raw_data, cxt, offsets))};

    if (details.name == "N/A") {
      break;
    }

    details_arr.push_back(details);
  }

  return details_arr;
}

std::vector<PacketParse::LayerJob> PacketParse::create_first_parse_jobs() {

  std::vector<LayerJob> jobs;

  auto layer2_job = [&](
    packet_ref& pkt,
    const std::vector<std::byte>& data,
    parse_context& context,
    layer_offsets& offsets) {

    context.offset = sizeof(pcaprec_hdr_t);

    pkt.layer2 = first_parse_dispatcher(m_dlt, data, context);

    if (!pkt.layer2) {

      offsets.l2.length = -1;
      offsets.l2.offset = -1;
      offsets.l2.protocol_type = -1;
      return false;

    }

    offsets.l2.length = context.curr_length;
    offsets.l2.offset = context.offset;
    offsets.l2.protocol_type = m_dlt;

    context.prev_length = context.curr_length;

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

      pkt.layer3 = first_parse_dispatcher(context.next_type, data, context);

      if (!pkt.layer3) {

        offsets.l3.length = -1;
        offsets.l3.offset = -1;
        offsets.l3.protocol_type = -1;
        return false;
      }

      offsets.l3.length = context.curr_length;
      offsets.l3.offset = context.offset;

      context.prev_length = context.curr_length;

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

      pkt.layer4 = first_parse_dispatcher(context.next_type, data, context);

      if (!pkt.layer4) {

        offsets.l3.length = -1;
        offsets.l3.offset = -1;
        offsets.l3.protocol_type = -1;
        return false;

      }

      offsets.l4.length = context.curr_length;
      offsets.l4.offset = context.offset;

      context.prev_length = context.curr_length;

      pkt.length += context.curr_length;

      return true;

    };

    jobs.push_back({layer4_job});

  }

  return jobs;

}

std::vector<PacketParse::LayerJob> PacketParse::create_detail_parse_jobs() {

  auto layer2 = [&] (
    const std::vector<std::byte>& raw_data,
    parse_context& cxt,
    const layer_offsets& offsets) {

    cxt.offset = offsets.l2.offset;

    if (cxt.offset == -1) {
      return ProtocolDetails{"N/A", {}};
    }

    return detail_parse_dispatcher(offsets.l2.protocol_type, raw_data, cxt);

  };

  auto layer3 = [&] (
    const std::vector<std::byte>& raw_data,
    parse_context& cxt,
    const layer_offsets& offsets) {

    cxt.offset = offsets.l3.offset;

    if (cxt.offset == -1) {
      return ProtocolDetails{"N/A", {}};
    }

    return detail_parse_dispatcher(offsets.l3.protocol_type, raw_data, cxt);

  };

  auto layer4 = [&] (
    const std::vector<std::byte>& raw_data,
    parse_context& cxt,
    const layer_offsets& offsets) {

    cxt.offset = offsets.l4.offset;

    if (cxt.offset == -1) {
      return ProtocolDetails{"N/A", {}};
    }

    return detail_parse_dispatcher(offsets.l4.protocol_type, raw_data, cxt);

  };

  return {
    {nullptr,layer2},
    {nullptr, layer3},
    {nullptr, layer4}};

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
