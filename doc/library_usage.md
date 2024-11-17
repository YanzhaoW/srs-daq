## Usage of SRS library

Link the SRS library with CMake `find_package`:

```cmake
find_package(srs REQUIRED)

add_executable(main PRIVATE srs::srs)
```

Please make sure add the directory path of the program folder `srs-install` to `CMAKE_PREFIX_PATH`:

```bash
cmake -DCMAKE_PREFIX_PATH=[...]/srs-install ..
```

### Convert the proto binary data to data structure

The full example can be found in this [script](../examples/readUDP/main.cpp).

The class `srs::ProtoMsgReader` converts the proto binary data to the data structure. The memory allocation is done in the constructor and no allocation happens during the conversion. If multiple conversions needs to be done, the object should be kept alive to reduce the memory allocation.

Minimum example:

```c++
#include <srs/srs.hpp>

auto main() -> int
{
  auto msg_reader = srs::ProtoMsgReader{};

  std::string_view proto_binary = get_proto_msg();

  const auto& data_struct = msg_reader.convert(proto_binary);

  return 0;
}
```
