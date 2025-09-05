//
// Created by jeremiah tesfaye on 7/30/25.
//

#ifndef PCAPFILE_H
#define PCAPFILE_H

#include <fstream>
#include <pcap/pcap.h>
#include <packet/PcapReader.h>
#include <packet/PcapArray.h>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

/**
 * Serves as an interface with the .pcap that gets written to or being read from
 */

class PcapFile {
public:

    enum class ReaderType {Mmap, IO, None};

    static constexpr size_t m_global_header_size = 24;

    //Use this constructor when capturing live packets and want to write to file
    explicit PcapFile(const char* file_name, pcap_t* handle, size_t buffer_size);

    //use this for preexisting .pcap files,
    explicit PcapFile(const char* file_path);

    std::vector<std::byte> read(size_t index) const;

    size_t write(const pcap_pkthdr* header, const u_char* data) const;

    size_t get_buffer_size() const {return m_buffer_size;}

    size_t get_file_size() const {return m_file_size;}


private:

    std::unique_ptr<PcapReader> create_reader(ReaderType type);

    std::string m_file_name;
    std::ifstream m_file;
    size_t m_file_size;
    pcap_dumper_t* m_dumper;
    size_t m_buffer_size;
    pcap_file_header m_header;
    int m_pcap_fd;
    std::unique_ptr<PcapReader> m_reader;
    std::shared_ptr<PcapArray> m_array;
};



#endif //PCAPFILE_H
