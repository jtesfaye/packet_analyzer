
#include "Offline.h"
#include <sys/_types/_u_int8_t.h>
#include <iostream>


namespace capture {

  Offline::Offline
  (
    std::string path_name,
    int count,
    u_int8_t flags
  ) 
  : PacketCapture(path_name, count, flags) 
  {
    if (!handle()) {

      std::cerr << "error with handle" << errbuf << "\n";

    }
  }

}

