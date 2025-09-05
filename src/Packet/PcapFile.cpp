//
// Created by jeremiah tesfaye on 7/30/25.
//

#include "../../include/packet/PcapFile.h"

#include <mutex>

PcapFile::PcapFile(const char * file_name, pcap_t* handle, size_t buffer_size)
: m_file_name(file_name)
, m_dumper(pcap_dump_open(handle, file_name))
, m_file(file_name, std::ios::binary)
, header()
, m_buffer_size(buffer_size)
{
    m_file.read(reinterpret_cast<char*> (&header), sizeof(header));

    FILE* f = pcap_dump_file(m_dumper);
    setvbuf(f, nullptr, _IOFBF, buffer_size);

}

void PcapFile::map_file() {

    mapped_file.remap_file(*this);
}

const u_int8_t
*PcapFile::MappedPcapFile::read(size_t offset, size_t len) const {

    if (offset + len > m_region_size) {
        throw std::out_of_range("Reading outside mapped region");
    }

    return reinterpret_cast<const u_int8_t*> (m_base + offset);

}


















