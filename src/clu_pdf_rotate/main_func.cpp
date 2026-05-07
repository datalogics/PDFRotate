#include "CommonUtils.h"
#include "rotatorlib.h"

int main(int argc, char *argv[]) {
#if defined(WIN_ENV)
  argv = getUTF8CommandLine(argc);
#endif
  return Rotator_Main(argc, argv);
}
