//
// Created by jeremiah tesfaye on 8/14/25.
//

#ifndef SPARSEPACKETBUFFER_H
#define SPARSEPACKETBUFFER_H

#include <deque>
#include <packet/PacketUtil.h>
#include <util/IContainerType.h>
#include <optional>
#include <iostream>
#include <deque>

using namespace packet;

template<typename T>
class SparsePacketBuffer : public IContainerType<T> {
public:

    explicit SparsePacketBuffer(size_t capacity) :
    m_capacity(capacity) {

        buffer.resize(m_capacity);
        m_is_present.resize(m_capacity);
    }

    ~SparsePacketBuffer() override = default;

    void add(size_t index, T ref) override {

        if (index > buffer.size() - 1) {

            m_capacity *= 2;
            buffer.resize(m_capacity);
            m_is_present.resize(m_capacity);

        }

        buffer[index] = std::move(ref);
        m_is_present[index] = true;

    }

    std::optional<std::reference_wrapper<T>> poll(size_t index) override {

        if (index > buffer.size() - 1)
            throw std::runtime_error("PacketRefBuffer get_ref(): Access out of bounds on index " + std::to_string(index) + "\n");

        return std::optional{std::ref(buffer[index])};

    }

    const T& get(size_t key) override {

        if (!m_is_present[key]) {
            throw std::runtime_error("PacketBuffer: unavailable index accessed");
        }

        return buffer[key];
    }

    bool exists(size_t index) const override {

        if (index > buffer.size() - 1) {

            return false;
        }

        return m_is_present[index];
    }

    size_t size() const override {
        return buffer.size();
    }

    auto begin() {
        return buffer.begin();
    }

    auto begin_at(size_t index){
        return buffer.begin() + index;
    }

    auto end(){
        return buffer.end();
    }

private:

    size_t m_capacity;
    std::vector<bool> m_is_present;
    std::deque<T> buffer;

};

#endif //SPARSEPACKETBUFFER_H
