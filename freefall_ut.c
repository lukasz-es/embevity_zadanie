#include "utest.h"

UTEST_STATE();

UTEST(foo, bar) {
  ASSERT_TRUE(1);
}

int main(int argc, const char *const argv[]) {
  // do your own thing
  return utest_main(argc, argv);
}