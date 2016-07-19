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
