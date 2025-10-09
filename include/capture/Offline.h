

#ifndef OFFLINE_H
#define OFFLINE_H

#include <capture/PacketCapture.h>
#include <sys/_types/_u_int8_t.h>

#include <utility>



class Offline final : public PacketCapture {
public:

  Offline(std::string&& path_name);
  Offline() = delete;
  Offline(const Offline&) = delete;
  Offline& operator= (const Offline&) = delete;

  ~Offline() = default;

private:

  void initialize_handle();

  void capture_func() override;

  std::string m_file_path;

  struct capture_obj {

    PcapFile file;
    PacketParse parser;
    std::shared_ptr<PacketRefBuffer> buffer;

    capture_obj(std::string file_path, int dlt, std::shared_ptr<PacketRefBuffer> b)
    : file(file_path.data())
    , parser(dlt, 0x1111)
    , buffer(b) {}
  };

};




#endif