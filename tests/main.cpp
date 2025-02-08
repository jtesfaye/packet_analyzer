
#include "../src/capture/Online.h"
#include <stdexcept>
#include <iostream>

using namespace capture;

int main(int argc, char** argv) {

  try {

    Online session("en0", 10, 0x1, 0x80);
    session.get_link_types();
    session.get_devices();
    session.start_capture();

  } catch (std::runtime_error &e) {
    std::cerr << e.what();
  }

  return 0;
}