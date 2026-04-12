//
// Created by jeremiah tesfaye on 8/14/25.
//

#ifndef PKTRINGBUFFER_H
#define PKTRINGBUFFER_H

#include <util/PacketUtil.h>

using namespace packet;

class PktRingBuffer {
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = packet_data;
        using difference_type = std::ptrdiff_t;
        using pointer = const packet_data*;
        using const_reference = const packet_data&;
    private:
        PktRingBuffer* buffer;
        size_t current_index;

    public:
        Iterator() = default;
        Iterator(PktRingBuffer* b, size_t logical_index)
        : buffer(b)
        , current_index(logical_index) {}

        Iterator(Iterator&& iter) noexcept;
        Iterator(const Iterator&) = default;
        Iterator& operator=(const Iterator&) = default;
        Iterator& operator=(Iterator&&) = default;

        const_reference operator*() const;
        pointer operator->() const;
        Iterator& operator++();
        Iterator operator++(int);
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        size_t get_curr_index() const;
    };
public:

    friend class PktView;

    explicit PktRingBuffer(size_t size, size_t batch_size)
    : batch_size(batch_size)
    , Size(size) {
        if (batch_size > size) {
            throw std::runtime_error("PktRingBuffer::PktRingBuffer: batch size cannot be larger than buffer size");
        }
        buffer.resize(size);
    }

    ~PktRingBuffer() = default;
    bool add(packet_data&& ref);
    [[nodiscard]] size_t size() const;
    const packet_data& get(size_t pkt_id) const;

    class PktView {
    public:
        Iterator begin_iter;
        Iterator end_iter;
        PktView(PktRingBuffer& buf, const Iterator& b, const Iterator& e)
        : begin_iter(b)
        , end_iter(e)
        , buffer(buf)
        {}

        PktView(const PktView&) = default;
        PktView& operator= (const PktView&) = delete;
        PktView(PktView&&) noexcept = default;
        ~PktView() = default;

        Iterator begin() const { return begin_iter; }
        Iterator end() const { return end_iter; }

        void commit()
        {
            buffer.set_read_index(end_iter.get_curr_index());
        }

    private:
        PktRingBuffer& buffer;
    };

    std::optional<std::vector<packet_data>> get_packet_batch();

private:

    void set_read_index(size_t read);
    void set_write_index(size_t w);
    const packet_data& cget(size_t pkt_id);


    const size_t batch_size;
    const size_t Size;
    size_t read_index{};
    size_t write_index{};
    std::vector<packet_data> buffer;
    std::condition_variable m_cv;
    std::mutex lock;
};

#endif //PKTRINGBUFFER_H
