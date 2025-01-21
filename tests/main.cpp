
#include <iostream>
#include <gtest/gtest.h>

#include "../src/capture/PacketCapture.h"
#include "../src/capture/Online.h"

using namespace capture;

int main(int argc, char** argv) {

  Online("any", 10, 0x11);

  return 0;
}