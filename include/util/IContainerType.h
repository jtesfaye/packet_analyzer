//
// Created by jeremiah tesfaye on 10/25/25.
//

#ifndef ICONTAINERTYPE_H
#define ICONTAINERTYPE_H

#include <optional>

template<typename Type>
struct IContainerType {
    virtual ~IContainerType() = default;
    virtual std::optional<Type> poll(size_t key) = 0;
    virtual Type get(size_t key) = 0;
    virtual void add(size_t key, Type item) = 0;
    virtual size_t size() const = 0;
    virtual bool exists(size_t key) const = 0;
};

#endif //ICONTAINERTYPE_H
