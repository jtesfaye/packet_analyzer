
#ifndef ONLINE_H
#define ONLINE_H

#include <capture/PacketCapture.h>
#include <cstring>
#include <sys/_types/_u_int8_t.h>

using namespace packet::frame;

namespace capture {

  class Online final : public PacketCapture {
  public:

    Online() = delete;
    Online(const Online&) = delete;
    Online& operator= (const Online&) = delete;

    Online(std::string&& device_name, int count, int capture_size, u_int8_t set, u_int8_t flags);

    ~Online() override;

  protected:

    void get_link_types() const;

  private:

    void initialize_handle(int capture_size);

    std::string _device;
    u_int8_t _settings;
    u_int8_t m_flags;
    
  };

}



#endif