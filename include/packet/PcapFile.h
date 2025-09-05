//
// Created by jeremiah tesfaye on 7/30/25.
//

#ifndef PCAPFILE_H
#define PCAPFILE_H
#include <fstream>
#include <pcap/pcap.h>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <sys/_types/_u_int16_t.h>
#include <sys/_types/_u_int32_t.h>

using namespace boost::interprocess;

class PcapFile {
public:

    static constexpr size_t m_global_header_size = 24;

    explicit PcapFile(const char* file_name, pcap_t* handle, size_t buffer_size);

    [[nodiscard]] pcap_dumper_t* get_dumper() const {return m_dumper;}

    void map_file();

private:

    const char* m_file_name;

    pcap_dumper_t* m_dumper;

    std::ifstream m_file;

    size_t m_buffer_size;

    bool mapped = false; //after first flush from dumper, map the region

    struct glb_header {

        u_int32_t magic_num;
        u_int16_t version_major;
        u_int16_t version_minor;
        u_int32_t this_zone;
        u_int32_t sigfigs;
        u_int32_t snaplen;
        u_int32_t link_type;

    };

    glb_header header;

    class MappedPcapFile {
    public:

        explicit MappedPcapFile(const char* file_name)
            : m_filemap(file_name, read_only)
            , m_region(m_filemap, read_only)
            , m_base(static_cast<const char*> (m_region.get_address()))
            , m_region_size(m_region.get_size()) {}

        MappedPcapFile() = default;

        void remap_file(PcapFile& file) {

            m_region = mapped_region(m_filemap, read_only);

        }

        [[nodiscard]] const u_int8_t* read(size_t offset, size_t len) const;

    private:

        file_mapping m_filemap;
        mapped_region m_region;
        const char* m_base{};
        size_t m_region_size{};

    };

    MappedPcapFile mapped_file;



public:

    MappedPcapFile& get_mapped_file() {return mapped_file;}
    u_int32_t get_magic_num() const {return header.magic_num;}
    u_int16_t get_version_major() const {return header.version_major;}
    u_int16_t get_version_minor() const {return header.version_minor;}
    u_int32_t get_this_zone() const {return header.this_zone;}
    u_int32_t get_sigfigs() const {return header.sigfigs;}
    u_int32_t get_snaplen() const {return header.snaplen;}
    u_int32_t get_link_type() const {return header.link_type;}
    bool isMapped() const {return mapped;}
    size_t get_buffer_size() const {return m_buffer_size;}

    void setMapped() {mapped = !mapped;}


};



#endif //PCAPFILE_H
