

#ifndef DISPATCH_TABLE
#define DISPATCH_TABLE

#include <functional>
#include <unordered_map>


template<typename Signature, std::size_t Size>
class DispatchTable {
public: 

  using function = std::function<Signature>;
  using key_pair  = std::pair <int, function>;

  explicit DispatchTable(const std::array<key_pair, Size>& init) {

    for (const auto& [key, func] : init) {

      table[key] = func;

    }

  }

  DispatchTable(const DispatchTable&) = delete;
  DispatchTable operator= (const DispatchTable&) = delete;
  DispatchTable& operator= (DispatchTable&&) = delete;

  ~DispatchTable() {};

  function operator[] (int key) const {

      auto it = table.find(key);

      if (it == table.end()) {
        throw std::out_of_range("Key not found in dispatch table\n");
      }

      return it->second;

  }

private:

  std::unordered_map<int, function> table;

};

#endif