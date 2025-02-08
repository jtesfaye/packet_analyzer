

#ifndef OFFLINE_H
#define OFFLINE_H

#include "PacketCapture.h"
#include <sys/_types/_u_int8_t.h>

namespace capture {

  class Offline : public PacketCapture {
  public: 

    Offline(std::string path_name, int count, u_int8_t flags);

  };

}


#endif