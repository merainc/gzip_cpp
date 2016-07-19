A zlib c++ wrap library
=======================

This is a c++ library which will be able to compress/decompress your data buffers. It is a bunch of wrap classes of C library zlib. So you need to install zlib developer files in your system before you compile this library it.

Compile and install instructions
================================

Install zlib developer files first
----------------------------------

### MacOSX

Please install Xcode to your OSX system and zlib can be found.

### Debian

Use apt-get to Install `zlib1g-dev` on your system.

```
sudo apt-get install zlib1g-dev
```

Compile and install library
---------------------------

Clone source

```
git clone https://git.meradev.com/mera_server/zlib_cpp.git
```

Compile source

```
cmake . && make
```

Install library

```
make install
```

How to use?
===========

Add header files
----------------

```
#include <gzip_cpp/gzip_cpp.h>
```

Memory management
-----------------

### Structure to stores memory

gzip::DataBlock is a structures stores memory buffer.

```
struct DataBlock {
    char *ptr;          // buffer pointer
    std::size_t size;   // size of the buffer
};

typedef std::shared_ptr<DataBlock> Data;
```

### Allocate memory

Invoke `gzip::AllocateMemory(std::size_t size)` to allocate data buffer and stores to `gzip::Data`. `gzip::Data` is a smart pointer it will release buffer automatically, please do not use `delete[] Data()->ptr` to delete memory.

### Memory list

Compressor or decompressor will return a variable of type `gzip::DataList` to hold processed data. It's a container (std::list) of `gzip::Data` so you can visit each data block one by one. But you also can use `gzip::ExpandDataList(const DataList &data_list)` to copy all blocks to one.

Compress data
-------------

### Construct a compressor

You can construct a default compressor.

```
gzip::Comp comp;
```

Also you should check the object is good or not.

```
if (!comp.IsSucc()) {
  // Failed to create compressor.
  return;
}
```

### Compress data

Make a `gzip::Data` object and pass it to `gzip::Comp::Process` to compress data buffer. If you are compressing the last data block, please pass `true` for the latest parameter.

```
gzip::DataList out_data_list = comp.Process(source_data, source_data_size);
```

Decompress data
---------------

### Construct a decompressor

You can construct a default decompressor.

```
gzip::Decomp decomp;
```

Also you should check the object is good or not.

```
if (!decomp.IsSucc()) {
  // Failed to create compressor.
  return;
}
```

### Decompress data

To decompress data by passing a `gzip::Data` object to method `gzip::Decomp::Process`, the method returns `std::tuple<bool, gzip::DataList>` as outcome value. The first `boolean` value is telling you the uncompress process succeed or not, second `gzip::DataList` value holds the processed data if succeed.

```
gzip::Data incoming_data;

// TODO Reading incoming data and store to incoming_data.

boolean succ;
gzip::DataList out_data_list;
std::tie(succ, out_data_list) = decomp.Process(incoming_data);
if (!succ) {
  // Failed to decompress data.
  return;
}

// TODO Processing out_data_list.
```

Get more information from test.cc
---------------------------------

I have built a test case (by using Google Test) and you can run the test after you compiled the source code.

```
./gzip_cpp_test
```

Test result looks like below

```
Running main() from gtest_main.cc
[==========] Running 5 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 5 tests from gzip
[ RUN      ] gzip.compress
[       OK ] gzip.compress (0 ms)
[ RUN      ] gzip.decompress
[       OK ] gzip.decompress (0 ms)
[ RUN      ] gzip.invalid_decompress
[       OK ] gzip.invalid_decompress (0 ms)
[ RUN      ] gzip.large_compress
[       OK ] gzip.large_compress (318 ms)
[ RUN      ] gzip.large_decompress
[       OK ] gzip.large_decompress (37 ms)
[----------] 5 tests from gzip (355 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 1 test case ran. (355 ms total)
[  PASSED  ] 5 tests.
```

Review `test.cc` to get more help there or you can just send me a message if you have any question about this library.
