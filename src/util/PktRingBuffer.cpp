//
// Created by jeremiah tesfaye on 1/7/26.
//
#include <iostream>
#include <util/PktRingBuffer.h>

bool PktRingBuffer::add(packet_data&& ref) {
  //Check if full
  if (write_index - read_index >= Size) {
    std::unique_lock lk(lock);
    m_cv.wait(lk, [&] {
        return write_index - read_index <= Size;
    });
  }
  buffer[write_index % Size] = std::move(ref);
  ++write_index;
  return true;
}

const packet_data& PktRingBuffer::get(const size_t index) const {
  return buffer.at(index % Size);
}


size_t PktRingBuffer::size() const {
  return Size;
}

auto PktRingBuffer::get_packet_batch() -> std::optional<std::vector<packet_data>> {
  std::unique_lock lk(lock);

  // Nothing to read
  if (write_index == read_index) {
    return std::nullopt;
  }

  const size_t left = read_index;
  const size_t right = std::min(write_index, read_index + batch_size);

  std::vector<packet_data> batch;
  batch.reserve(right - left);

  for (size_t i = left; i < right; ++i) {
    batch.push_back(std::move(buffer[i % Size]));
  }

  read_index = right;

  // Notify potential producers that space is available
  m_cv.notify_one();

  return batch;
}

void PktRingBuffer::set_read_index(size_t read) {
  read_index = read;
}

void PktRingBuffer::set_write_index(size_t w) {
  write_index = w;
}

const packet_data& PktRingBuffer::cget(size_t pkt_id) {
  return buffer.at(pkt_id % Size);
}

size_t PktRingBuffer::Iterator::get_curr_index() const {
  return current_index;
}

PktRingBuffer::Iterator::const_reference PktRingBuffer::Iterator::operator*() const {
  return buffer->cget(current_index % buffer->size());
}

PktRingBuffer::Iterator::pointer PktRingBuffer::Iterator::operator->() const {
  return &buffer->cget(current_index % buffer->size());
}

PktRingBuffer::Iterator& PktRingBuffer::Iterator::operator++() {
  current_index++;
  return *this;
}

PktRingBuffer::Iterator PktRingBuffer::Iterator::operator++(int) {
  auto temp = *this;
  ++(*this);
  return temp;
}

bool PktRingBuffer::Iterator::operator==(const Iterator& other) const {
  return buffer == other.buffer && this->current_index == other.current_index;
}

bool PktRingBuffer::Iterator::operator!=(const Iterator& other) const{
  return !(*this == other);
}

PktRingBuffer::Iterator::Iterator(Iterator &&iter) noexcept {
  this->buffer = iter.buffer;
  iter.buffer = nullptr;
  this->current_index = iter.current_index;
}


