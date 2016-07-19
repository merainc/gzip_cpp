#include <stdlib.h>

#include <gtest/gtest.h>

#include <list>
#include <utility>

#include "gzip_cpp.h"

const char* const SOURCE_STRING = "abcdefghijklmnopqrstuvwxyz";
gzip::DataList g_compressed_data;

TEST(gzip, compress) {
  gzip::Comp comp;
  EXPECT_TRUE(comp.IsSucc());
  g_compressed_data =
      comp.Process(SOURCE_STRING, strlen(SOURCE_STRING) + 1, true);
  EXPECT_TRUE(g_compressed_data.size() > 0);
}

TEST(gzip, decompress) {
  gzip::Decomp decomp;
  EXPECT_TRUE(decomp.IsSucc());
  bool succ;
  gzip::DataList out_data_list;
  for (const gzip::Data& data : g_compressed_data) {
    gzip::DataList this_out_data_list;
    std::tie(succ, this_out_data_list) = decomp.Process(data);
    EXPECT_TRUE(succ);
    std::copy(this_out_data_list.begin(), this_out_data_list.end(),
              std::back_inserter(out_data_list));
  }
  gzip::Data out_data = gzip::ExpandDataList(out_data_list);
  EXPECT_EQ(0, strcmp(out_data->ptr, SOURCE_STRING));
}

TEST(gzip, invalid_decompress) {
  gzip::Decomp decomp;
  EXPECT_TRUE(decomp.IsSucc());
  gzip::Data data = gzip::AllocateData(10);
  memcpy(data->ptr, "0123456789", 10);
  bool succ;
  std::tie(succ, std::ignore) = decomp.Process(data);
  EXPECT_TRUE(!succ);
}

const int MAX_BUFFER_SIZE = 1024 * 1024;
const int NUM_OF_COMPRESS_TIMES = 10;

gzip::Data g_random_data;
gzip::DataList g_random_compressed_data_list;

TEST(gzip, large_compress) {
  // Make random buffers
  srand(time(NULL));
  g_random_data = gzip::AllocateData(MAX_BUFFER_SIZE);
  for (int i = 0; i < MAX_BUFFER_SIZE; ++i) {
    g_random_data->ptr[i] = rand() % 256;
  }
  // Compress
  gzip::Comp comp(gzip::Comp::Level::Max);
  EXPECT_TRUE(comp.IsSucc());
  // Compress g_random_data 10 times.
  for (int i = 0; i < NUM_OF_COMPRESS_TIMES; ++i) {
    gzip::DataList compressed_data_list =
        comp.Process(g_random_data, i == NUM_OF_COMPRESS_TIMES - 1);
    std::copy(compressed_data_list.begin(), compressed_data_list.end(),
              std::back_inserter(g_random_compressed_data_list));
  }
}

TEST(gzip, large_decompress) {
  gzip::Decomp decomp;
  EXPECT_TRUE(decomp.IsSucc());
  bool succ;
  gzip::DataList out_data_list;
  for (const gzip::Data& data : g_random_compressed_data_list) {
    gzip::DataList this_out_data_list;
    std::tie(succ, this_out_data_list) = decomp.Process(data);
    EXPECT_TRUE(succ);
    std::copy(this_out_data_list.begin(), this_out_data_list.end(),
              std::back_inserter(out_data_list));
  }
  gzip::Data data = gzip::ExpandDataList(out_data_list);
  const char *p = data->ptr;
  for (int i = 0; i < NUM_OF_COMPRESS_TIMES; ++i) {
    EXPECT_EQ(0, memcmp(p, g_random_data->ptr, g_random_data->size));
    p += g_random_data->size;
  }
}
