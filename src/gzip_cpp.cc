#include "gzip_cpp.h"

#include <cstring>

namespace gzip {

const int MAX_CHUNK_SIZE = 1024 * 16;

/// Allocate memory to DataBlock and assign to a shared_ptr object.
Data AllocateData(std::size_t size) {
  Data data(new DataBlock, [](DataBlock *p) { delete[] p->ptr; });
  data->ptr = new char[size];
  data->size = size;
  return data;
}

Data ExpandDataList(const DataList &data_list) {
  std::size_t total_size = 0;
  for (const Data &this_data : data_list) {
    total_size += this_data->size;
  }
  Data out_data = AllocateData(total_size);
  char *this_ptr = out_data->ptr;
  for (const Data &this_data : data_list) {
    memcpy(this_ptr, this_data->ptr, this_data->size);
    this_ptr += this_data->size;
  }
  return out_data;
}

Comp::Comp(Level level) : level_(level) {
  memset(&zs_, 0, sizeof(zs_));
  int ret = deflateInit(&zs_, static_cast<int>(level_));
  init_ok_ = ret == Z_OK;
}

Comp::~Comp() { deflateEnd(&zs_); }

bool Comp::IsSucc() const { return init_ok_; }

DataList Comp::Process(const char *buffer, std::size_t size, bool last_block) {
  DataList out_data_list;
  // Prepare output buffer memory.
  unsigned char out_buffer[MAX_CHUNK_SIZE];
  zs_.next_in = reinterpret_cast<unsigned char *>(const_cast<char *>(buffer));
  zs_.avail_in = size;
  do {
    // Reset output buffer position and size.
    zs_.avail_out = MAX_CHUNK_SIZE;
    zs_.next_out = out_buffer;
    // Do compress.
    deflate(&zs_, last_block ? Z_FINISH : Z_NO_FLUSH);
    // Allocate output memory.
    std::size_t out_size = MAX_CHUNK_SIZE - zs_.avail_out;
    Data out_data = AllocateData(out_size);
    // Copy and add to output data list.
    memcpy(out_data->ptr, out_buffer, out_size);
    out_data_list.push_back(std::move(out_data));
  } while (zs_.avail_out == 0);
  // Done.
  return out_data_list;
}

DataList Comp::Process(const Data &data, bool last_block) {
  return Process(data->ptr, data->size, last_block);
}

Decomp::Decomp() {
  memset(&zs_, 0, sizeof(zs_));
  int ret = inflateInit(&zs_);
  init_ok_ = ret == Z_OK;
}

Decomp::~Decomp() { inflateEnd(&zs_); }

bool Decomp::IsSucc() const { return init_ok_; }

std::tuple<bool, DataList> Decomp::Process(const Data &compressed_data) {
  DataList out_data_list;
  unsigned char out_buffer[MAX_CHUNK_SIZE];
  // Incoming buffer.
  zs_.avail_in = compressed_data->size;
  zs_.next_in = reinterpret_cast<unsigned char *>(
      const_cast<char *>(compressed_data->ptr));
  int ret;
  do {
    // Prepare outcoming buffer and size.
    zs_.avail_out = MAX_CHUNK_SIZE;
    zs_.next_out = out_buffer;
    // Decompress data.
    ret = inflate(&zs_, Z_NO_FLUSH);
    switch (ret) {
      case Z_NEED_DICT:
        // Incoming data is invalid.
        return std::make_tuple(false, std::move(out_data_list));
      case Z_DATA_ERROR:
      case Z_MEM_ERROR:
        // Critical error.
        return std::make_tuple(false, std::move(out_data_list));
    }
    // Outcome size.
    std::size_t out_size = MAX_CHUNK_SIZE - zs_.avail_out;
    // Allocate outcome buffer.
    Data out_data = AllocateData(out_size);
    memcpy(out_data->ptr, out_buffer, out_size);
    out_data_list.push_back(std::move(out_data));
  } while (zs_.avail_out == 0);
  return std::make_tuple(true, std::move(out_data_list));
}

}  // namespace gzip
