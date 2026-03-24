#include <iostream>

#include "windows.h"

int main(int argc, const char** argv) {
  return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(),
                 SW_SHOWDEFAULT);
}
