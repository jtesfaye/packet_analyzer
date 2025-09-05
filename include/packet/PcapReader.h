//
// Created by jeremiah tesfaye on 8/27/25.
//

#ifndef PCAPREADER_H
#define PCAPREADER_H
#include <vector>
#include <sys/_types/_u_int8_t.h>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

class PcapReader {
public:
    virtual ~PcapReader() = default;

    virtual const std::vector<std::byte> read(size_t offset, size_t len) = 0;

};

class MmapPcapReader : public PcapReader {
public:

    explicit MmapPcapReader(const char* file_name)
        : m_filemap(file_name, boost::interprocess::read_only)
        , m_region(m_filemap, boost::interprocess::read_only)
        , m_base(static_cast<const u_int8_t*>(m_region.get_address()))
        , m_region_size(m_region.get_size()) {}

    const std::vector<std::byte> read(size_t offset, size_t len) override;

private:
    boost::interprocess::file_mapping m_filemap;
    boost::interprocess::mapped_region m_region;
    const u_int8_t* m_base;
    size_t m_region_size;
};

class IoPcapReader : public PcapReader {
public:

    explicit IoPcapReader(int fd) :
    m_fd(fd) {}

    const std::vector<std::byte> read(size_t offset, size_t len) override;

    int m_fd;

};

#endif //PCAPREADER_H
