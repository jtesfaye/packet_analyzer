

#ifndef OFFLINE_H
#define OFFLINE_H

#include <capture/PacketCapture.h>
#include <sys/_types/_u_int8_t.h>



class Offline final : public PacketCapture {
public:
  Offline(std::string&& path_name, int count);

protected:

  void
  initialize_handle();

private:

  std::string m_file_path;

};




#endif