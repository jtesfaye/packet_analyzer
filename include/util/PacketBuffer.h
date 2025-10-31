//
// Created by jeremiah tesfaye on 8/14/25.
//

#ifndef PACKETBUFFER_H
#define PACKETBUFFER_H

#include <deque>
#include <packet/PacketUtil.h>
#include <util/IContainerType.h>
#include <optional>
#include <iostream>
#include <deque>

using namespace packet;

template<typename T>
class PacketBuffer : public IContainerType<T> {
public:

    explicit PacketBuffer(size_t capacity) :
    m_capacity(capacity) {

        buffer.resize(m_capacity);
    }

    ~PacketBuffer() override = default;

    void add(size_t index, T ref) override {

        if (index > buffer.size() - 1) {
            m_capacity *= 2;
            buffer.resize(m_capacity);
        }

        std::cout << "Buffer: Adding to index: " << std::to_string(index) << "\n";
        buffer[index] = std::move(ref);
    }

    std::optional<std::reference_wrapper<T>> poll(size_t index) override {

        if (index > buffer.size() - 1)
            throw std::runtime_error("PacketRefBuffer get_ref(): Access out of bounds on index " + std::to_string(index) + "\n");

        return std::ref(buffer[index].value());

    }

    std::optional<std::reference_wrapper<const T>> get(size_t key) override {

        return std::ref(buffer[key].value());

    }

    bool exists(size_t index) const override {

        if (index > buffer.size() - 1) {
            return false;
        }

        if (buffer[index] == std::nullopt)
            return false;

        return true;

    }

    size_t size() const override {
        return buffer.size();
    }

private:

    size_t m_capacity;
    std::deque<std::optional<T>> buffer;
    std::mutex m_lock;

};

#endif //PACKETBUFFER_H
