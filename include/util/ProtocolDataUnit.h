//
// Created by jeremiah tesfaye on 9/9/25.
//

#ifndef PROTOCOLDATAUNIT_H
#define PROTOCOLDATAUNIT_H

#include <cstdint>
#include <string>
#include <utility>

struct ProtocolDataUnit {

  ProtocolDataUnit(const size_t len, std::string s, std::string d)
  : length(len)
  , src(std::move(s))
  , dest(std::move(d)) {}

  size_t length;
  std::string src;
  std::string dest;

};

#endif //PROTOCOLDATAUNIT_H
