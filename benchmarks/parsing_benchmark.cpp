//
// Created by jeremiah tesfaye on 11/15/25.
//


#include <benchmark/benchmark.h>
#include <parsing/InitialParser.h>
#include <layerx/layer2/Ethernet.h>
#include <packet/PcapFile.h>
#include <pcap/pcap.h>
#include <packet/PacketUtil.h>
#include <span>

static std::vector<std::vector<std::byte>> setup() {

    const auto file_name = "/Users/jt/Desktop/pcap_files/mycap.pcap";
    PcapFile file(file_name);
    std::vector<std::vector<std::byte>> all_data;
    all_data.reserve(file.get_packet_count());

    for (int i = 0; i < file.get_packet_count(); i++) {
        std::vector<std::byte> data = file.read(i);
        all_data.push_back(data);

    }

    return all_data;

}

static void BM_initial_parser_baseline(benchmark::State& state) {

    auto d = setup();

    std::vector<RawPacket> all_data;

    for (int i = 0; i < d.size(); i++) {
        auto data = d[i];
        RawPacket pkt{};
        int copy_len = data.size() ? data.size() < 1500 : 1500;

        std::memcpy(pkt.packet, data.data(), copy_len);
        pkt.index = i;
        all_data.push_back(pkt);

    }

    InitialParser parser{220, 0xff};

    for (auto _ : state) {

        for (int i = 0; i < d.size(); i++) {

            const auto sp = std::span<std::byte>(all_data[i].packet);

            auto ignore = parser.start_extract(sp, i);

        }
    }
}

BENCHMARK(BM_initial_parser_baseline);

BENCHMARK_MAIN();