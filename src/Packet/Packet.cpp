
#include "Packet.h"

using std::cout;

namespace packet {

  namespace frame {
  }

  Packet::Packet(layers::link_layer&& link) : l2(l2) {

    std::cout << "Packet constructed\n";
    
  }

}