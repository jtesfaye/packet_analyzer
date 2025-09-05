//
// Created by jeremiah tesfaye on 6/29/25.
//

#ifndef PACKETGETTER_H
#define PACKETGETTER_H

#include <QObject>
#include "PacketBuffer.h"

class PacketGetter : public QObject {
    Q_OBJECT

public:

    explicit PacketGetter(PacketBuffer* buffer, QObject* parent = nullptr);
    ~PacketGetter() override = default;

public slots:

    void get_packet();

    signals:

    void packet_ready(const proccessed_packet& pkt);

private:

    PacketBuffer* buffer;
};



#endif //PACKETGETTER_H
