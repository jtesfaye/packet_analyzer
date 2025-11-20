//
// Created by jeremiah tesfaye on 9/9/25.
//

#ifndef PROTOCOLDATAUNIT_H
#define PROTOCOLDATAUNIT_H

#include <cstdint>
#include <string>
#include <utility>

struct Address {
  std::array<std::byte, 16> bytes{};
  u_int8_t size = 0;
};

struct ProtocolDataUnit {

  ProtocolDataUnit(const size_t hdr_len)
  : length(hdr_len) {}

  virtual ~ProtocolDataUnit() = default;

  virtual std::string make_info() const = 0;
  virtual std::string_view name() const = 0;
  virtual std::string address_to_string(const Address& addr) const {return {};};
  virtual Address src() const = 0;
  virtual Address dest() const = 0;

  size_t length;
  size_t stream_index = -1;

};

struct LinkPDU : ProtocolDataUnit {

  LinkPDU(const size_t len)
  : ProtocolDataUnit(len)
  {}

  ~LinkPDU() override = default;
};

struct NetworkPDU : ProtocolDataUnit {

  NetworkPDU(const size_t len)
  : ProtocolDataUnit(len)
  {}

  ~NetworkPDU() override = default;
};

struct TransportPDU : ProtocolDataUnit {

  TransportPDU(size_t len)
  : ProtocolDataUnit(len)
  {}

  ~TransportPDU() override = default;
};

#endif //PROTOCOLDATAUNIT_H
