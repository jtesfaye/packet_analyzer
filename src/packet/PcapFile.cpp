//
// Created by jeremiah tesfaye on 7/30/25.
//

#include <iostream>
#include <packet/PcapFile.h>
#include <print>
#include <filesystem>
#include <utility>

PcapFile::PcapFile(const std::filesystem::path& file_name, pcap_t* handle)
: m_abs_file_path(file_name)
, m_file(m_abs_file_path, std::ios::binary)
, m_header()
, m_array(std::make_unique<PcapArray>(m_file_size, m_global_header_size))
{

    auto f = m_abs_file_path;

    FILE* pcap_file = fopen(f.c_str(), "w");

    if (!pcap_file) {
        throw std::runtime_error(std::string("Failed to open dumper file: ") + strerror(errno));
    }

    m_pcap_fd = pcap_file->_file;

    m_dumper = pcap_dump_fopen(handle, pcap_file);

    if (!m_dumper) {
        throw std::runtime_error("PcapFile: pcap_dump_open failed");
    }

    m_pcap_fd = open(f.c_str(), O_RDONLY);
    if (m_pcap_fd < 0) {
        throw std::runtime_error(std::string("Failed to open fd for reader: ") + strerror(errno));
    }

    m_file.read(reinterpret_cast<char*>(&m_header), sizeof(m_header));

    m_reader = create_reader(ReaderType::IO);

}

PcapFile::PcapFile(const std::filesystem::path& file_path)
: m_abs_file_path(file_path)
, m_file(m_abs_file_path, std::ios::binary | std::ios::in)
, m_dumper(nullptr)
, m_header()
{

    if (m_file) {

        m_file.seekg(0, std::ios::end);

        if (!m_file) {
            std::println("seekg failed");
        }

        m_file_size = m_file.tellg();

        if (m_file_size == -1) {
            std::println("tellg failed");
        }

    } else {
        std::println("Failed to open file: {}", file_path.c_str());
        std::cout << std::strerror(errno) << "\n";
    }

    m_file.read(reinterpret_cast<char*> (&m_header), sizeof(m_header));

    m_array  = std::make_unique<PcapArray>(\
        create_reader(ReaderType::Mmap),
        m_file_size,
        m_global_header_size
        );

    m_reader = create_reader(ReaderType::Mmap);

}

PcapFile::~PcapFile() {

    if (m_dumper) {
        flush();
        close_dumper();
    }

}

size_t
PcapFile::write(const pcap_pkthdr *header, const u_char *data) const {

    pcap_dump(reinterpret_cast<u_char *>(m_dumper), header, data);

    size_t pkt_size = header->caplen;

    m_array->add_packet_index(pkt_size);

    return m_array->size() - 1;

}

std::vector<std::byte> PcapFile::read(size_t index) const {

    auto meta = m_array->get(index);
    return m_reader->read(meta.offset, meta.pkt_size);

}

bool PcapFile::save_file(std::string file_path) {

    flush();
    close_dumper();

    std::cout << file_path << std::endl;

    std::filesystem::copy(
        m_abs_file_path,
        file_path,
        std::filesystem::copy_options::overwrite_existing
        );

    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* handle = pcap_open_offline(file_path.c_str(), errbuf);

    if (!handle) {
        std::println("File copy exists but failed to open with pcap: {}", errbuf);
        return false;
    }

    pcap_close(handle);
    return true;

}


void PcapFile::flush() const {

    pcap_dump_flush(m_dumper);
}

void PcapFile::close_dumper() {

    pcap_dump_close(m_dumper);
    m_dumper = nullptr;
}


std::unique_ptr<PcapReader> PcapFile::create_reader(const ReaderType type) {

    switch (type) {

        case ReaderType::Mmap:
            return std::make_unique<MmapPcapReader>(m_abs_file_path.c_str());

        case ReaderType::IO:
            return std::make_unique<IoPcapReader>(m_pcap_fd);

        default:
            return nullptr;
    }
}






