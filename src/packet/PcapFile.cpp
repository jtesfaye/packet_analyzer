//
// Created by jeremiah tesfaye on 7/30/25.
//

#include <iostream>
#include <packet/PcapFile.h>
#include <print>

PcapFile::PcapFile(const char* file_name, pcap_t* handle, size_t buffer_size)
: m_file_name(file_name)
, m_file(file_name, std::ios::binary)
, m_dumper(pcap_dump_open(handle, file_name))
, m_buffer_size(buffer_size)
, m_header()
, m_array(std::make_unique<PcapArray>(m_file_size, m_global_header_size))
{

    if (!m_dumper) {
        throw std::runtime_error("PcapFile: pcap_dump_open failed");
    }

    m_file_size = -1;

    FILE* f = pcap_dump_file(m_dumper);

    m_pcap_fd = f->_file;

    setvbuf(f, nullptr, _IOFBF, buffer_size);

    m_file.read(reinterpret_cast<char*> (&m_header), sizeof(m_header));

    m_reader = create_reader(ReaderType::IO);

}

PcapFile::PcapFile(const char *file_path)
: m_file_name(file_path)
, m_file(m_file_name, std::ios::binary | std::ios::in)
, m_dumper(nullptr)
, m_buffer_size(-1)
, m_header()
, m_pcap_fd(-1)
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
        std::println("Failed to open file: {}", file_path);
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



std::unique_ptr<PcapReader> PcapFile::create_reader(const ReaderType type) {

    switch (type) {

        case ReaderType::Mmap:
            return std::make_unique<MmapPcapReader>(m_file_name.c_str());

        case ReaderType::IO:
            return std::make_unique<IoPcapReader>(m_pcap_fd);

        default:
            return nullptr;
    }
}






