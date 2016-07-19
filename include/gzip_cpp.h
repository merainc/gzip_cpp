#ifndef __GZIP_CPP_H__
#define __GZIP_CPP_H__

#include <zlib.h>

#include <list>
#include <memory>
#include <tuple>

namespace gzip {

struct DataBlock {
  char *ptr;
  std::size_t size;
};

typedef std::shared_ptr<DataBlock> Data;
typedef std::list<Data> DataList;

Data ExpandDataList(const DataList &data_list);

/// Compress processor.
class Comp {
 public:
  /// Construct a compressor.
  Comp();

  /// Destructor, will release z_stream.
  ~Comp();

  /// Returns true if compressor initialize successfully.
  bool IsSucc() const;

  /// Compress incoming buffer to DataBlock list.
  DataList Process(const char *buffer, std::size_t size,
                   bool last_block = false);

 private:
  z_stream zs_;
  bool init_ok_;
};

/// Decompress processor.
class Decomp {
 public:
  /// Construct a decompressor.
  Decomp();

  /// Destructor, will release z_stream.
  ~Decomp();

  /// Returns true if decompressor initialize successfully.
  bool IsSucc() const;

  /// Decompress incoming buffer to DataBlock list.
  std::tuple<bool, DataList> Process(const Data &compressed_data);

 private:
  z_stream zs_;
  bool init_ok_;
};

}  // namespace gzip

#endif
