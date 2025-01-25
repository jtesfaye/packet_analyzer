
#include <iostream>
#include <gtest/gtest.h>

#include "../src/capture/PacketCapture.h"
#include "../src/capture/Online.h"

using namespace capture;

int main(int argc, char** argv) {

  Online session("bridge0", 10, 0x1);
  session.get_devices();
  

  return 0;
}