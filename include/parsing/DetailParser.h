//
// Created by jeremiah tesfaye on 10/25/25.
//

#ifndef DETAILPARSER_H
#define DETAILPARSER_H

#include <parsing/ParseDispatcher.h>
#include <span>

class DetailParser {
public:

    DetailParser();

    std::vector<ProtocolDetails> detail_parse(
        const std::span<std::byte> &raw_data,
        const layer_offsets &offsets);

private:

    struct LayerJob {

        std::function<ProtocolDetails(
          const std::span<std::byte>&,
          parse_context&,
          const layer_offsets&)> detail_func;

    };

    std::vector<LayerJob> create_detail_parse_jobs();

    ParseDispatcher<ProtocolDetails> parser;

};

#endif //DETAILPARSER_H
