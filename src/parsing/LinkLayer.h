
#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "PacketParsing.h"
#include <netinet/if_ether.h>

namespace Parse {

  class LinkLayer : public PacketParse {

    LinkLayer() = delete;
    
    ether_header m_header;

  };
}


#endif
