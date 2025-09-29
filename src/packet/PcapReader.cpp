//
// Created by jeremiah tesfaye on 8/27/25.
//

#include <packet/PcapReader.h>

const std::vector<std::byte>
MmapPcapReader::read(size_t offset, size_t len) {

    if (offset + len > m_region_size) {
        throw std::out_of_range("mmap read out of range");
    }

    if (!m_base) {
        throw std::runtime_error("Memory not mapped");
    }

    auto start = reinterpret_cast<const std::byte*>(m_base + offset);
    auto end = start + len;

    return {start, end};
}

const std::vector<std::byte> IoPcapReader::read(size_t offset, size_t len) {

    std::vector<std::byte> buffer(len);

    ssize_t bytes_read = pread(m_fd, buffer.data(), len, offset);

    if (bytes_read <= 0) {

        throw std::runtime_error(std::string("pread failed: ") + strerror(errno));
    }


    buffer.resize(bytes_read);
    return buffer;

}
