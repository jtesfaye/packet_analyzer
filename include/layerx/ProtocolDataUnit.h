//
// Created by jeremiah tesfaye on 9/9/25.
//

#ifndef PROTOCOLDATAUNIT_H
#define PROTOCOLDATAUNIT_H

#include <cstdint>
#include <string>
#include <utility>

struct Address {
  std::array<std::byte, 16> bytes;
  u_int8_t size = 0;
};

struct ProtocolDataUnit {

  ProtocolDataUnit(const size_t len, u_int16_t s, u_int16_t d)
  : length(len) {
    std::memcpy(src.bytes.data(), &s, sizeof(s));
    std::memcpy(dest.bytes.data(), &d,  sizeof(d));
  }

  virtual ~ProtocolDataUnit() = default;

  virtual std::string make_info() const = 0;
  virtual std::string_view name() const = 0;

  Address src;
  Address dest;
  size_t length;

};

struct LinkPDU : ProtocolDataUnit {

  LinkPDU(const size_t len, const u_int16_t src, const u_int16_t dest)
  : ProtocolDataUnit(len, src, dest)
  {}

  ~LinkPDU() override = default;

};

struct NetworkPDU : ProtocolDataUnit {

  NetworkPDU(const size_t len, const u_int16_t src, const u_int16_t dest)
  : ProtocolDataUnit(len, src, dest)
  {}

  ~NetworkPDU() override = default;

};

struct TransportPDU : ProtocolDataUnit {

  TransportPDU(size_t len, u_int16_t src, u_int16_t dest)
  : ProtocolDataUnit(len, src, dest)
  {}

  ~TransportPDU() override = default;

  size_t stream_index = -1;

};



#endif //PROTOCOLDATAUNIT_H
