#include <gtest/gtest.h>

#include "gzip_cpp.h"

const char* const SOURCE_STRING = "abcdefghijklmnopqrstuvwxyz";
gzip::Data g_compressed_data;

TEST(gzip, compress) {
  gzip::Comp comp;
  g_compressed_data = comp.process(SOURCE_STRING, strlen(SOURCE_STRING) + 1);
  EXPECT_TRUE(compressed_data);
}

TEST(gzip, decompress) {
  gzip::Decomp decomp;
  gzip::Data data = decomp.process(g_compressed_data);
  EXPECT_TRUE(data);
  EXPECT_TRUE(strcmp(SOURCE_STRING, data.get()) == 0);
}
