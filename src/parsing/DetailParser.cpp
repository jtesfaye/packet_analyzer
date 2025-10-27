//
// Created by jeremiah tesfaye on 10/25/25.
//

#include <parsing/DetailParser.h>
#include <layerx/Layer.h>

DetailParser::DetailParser(
    const std::shared_ptr<IContainerType<packet_ref>> &buffer,
    const std::shared_ptr<IContainerType<ProtocolDetails> >&cache,
    const std::shared_ptr<PacketObserver>& observer)
: parser(Layer::get_detail_parse_functions())
, cache(cache)
, buffer(buffer)
{
    //When the observer sends a signal, we then start parsing for details and caching result
    connect(
        observer.get(),
        &PacketObserver::emit_packets_ready,
        this,
        &DetailParser::process_packets,
        Qt::QueuedConnection
        );
}

std::vector<ProtocolDetails> DetailParser::process_packets(size_t start, size_t end) {

    std::vector<ProtocolDetails> details;
    details.reserve(end - start);

    for (size_t i = start; i <= end; i++) {
        packet_ref ref = buffer->get(i);
        details.emplace_back(parser.detail_parse(ref.))

    }


}

void DetailParser::cache_result(size_t key, ProtocolDetails item) {


}




