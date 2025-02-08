
#ifndef ONLINE_H
#define ONLINE_H

#include "PacketCapture.h"

#include <cstring>
#include <sys/_types/_u_int8_t.h>

using packet::Packet;
using namespace packet::frame;

namespace capture {

  class Online : public PacketCapture {
  public:

    Online() = delete;
    Online(std::string device_name, int cnt, const u_int8_t set, u_int8_t flags);

    Online(Online&) = delete;
    Online operator= (Online&) = delete;

    ~Online() override;

    void get_devices() override;

    void get_link_types() override;

  private:

    void set_data_link(int dlt) {
      m_data_link = dlt;
    }

    int m_data_link;
    
  };

}



#endif