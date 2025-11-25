//
// Created by jeremiah tesfaye on 10/25/25.
//

#include <parsing/DetailParser.h>
#include <layerx/Registry.h>

DetailParser::DetailParser()
: parser(registry::get_detail_parse_functions())
{
}


std::vector<ProtocolDetails> DetailParser::detail_parse(
  const std::span<std::byte> &raw_data,
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

std::vector<DetailParser::LayerJob> DetailParser::create_detail_parse_jobs() {

    auto layer2 = [&] (
      const std::span<std::byte>& raw_data,
      parse_context& cxt,
      const layer_offsets& offsets) {

        cxt.offset = offsets.l2.offset;
        cxt.curr_length = offsets.l2.length;

        if (cxt.offset == -1) {
            return ProtocolDetails{"N/A", {}};
        }

        return parser(offsets.l2.protocol_type, {raw_data.begin(), raw_data.end()}, cxt);
    };

    auto layer3 = [&] (
      const std::span<std::byte>& raw_data,
      parse_context& cxt,
      const layer_offsets& offsets) {

        cxt.offset = offsets.l3.offset;
        cxt.curr_length = offsets.l3.length;

        if (cxt.offset == -1) {
            return ProtocolDetails{"N/A", {}};
        }

        return parser(offsets.l3.protocol_type, {raw_data.begin(), raw_data.end()}, cxt);

    };

    auto layer4 = [&] (
      const std::span<std::byte>& raw_data,
      parse_context& cxt,
      const layer_offsets& offsets) {

        cxt.offset = offsets.l4.offset;
        cxt.curr_length = offsets.l4.length;

        if (cxt.offset == -1) {
            return ProtocolDetails{"N/A", {}};
        }

        return parser(offsets.l4.protocol_type, {raw_data.begin(), raw_data.end()}, cxt);

    };

    return {
        {layer2},
        {layer3},
        {layer4}
    };
}


