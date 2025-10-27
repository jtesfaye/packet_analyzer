//
// Created by jeremiah tesfaye on 10/25/25.
//

#ifndef DETAILPARSER_H
#define DETAILPARSER_H

#include <QObject>
#include <util/IContainerType.h>
#include <util/PacketObserver.h>
#include <parsing/ParseDispatcher.h>

class DetailParser : QObject {

    Q_OBJECT

public:

    DetailParser(
        const std::shared_ptr<IContainerType<packet_ref>>& buffer,
        const std::shared_ptr<IContainerType<ProtocolDetails>>& cache,
        const std::shared_ptr<PacketObserver>& observer);

public slots:

    std::vector<ProtocolDetails> process_packets(size_t start, size_t end);

private:

    void cache_result(size_t key, ProtocolDetails item);

    ParseDispatcher<ProtocolDetails> parser;
    std::shared_ptr<IContainerType<ProtocolDetails>> cache;
    std::shared_ptr<IContainerType<packet_ref>> buffer;

};

#endif //DETAILPARSER_H
