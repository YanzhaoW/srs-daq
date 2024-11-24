################################################
Library usage
################################################

Use CMake to link the library
#################################

Link the SRS library with CMake `find_package`:

.. code-block:: cmake

  find_package(srs REQUIRED)
  add_executable(main PRIVATE srs::srs)

Please make sure add the directory path of the installation folder ``srs-daq`` to ``CMAKE_PREFIX_PATH``:

.. code-block:: bash

  cmake -DCMAKE_PREFIX_PATH=[...]/srs-daq ..

Available APIs
##################################

Deserialization of the UDP binary data
=============================================

The class :cpp:expr:`srs::ProtoMsgReader` is responsible for the deserialization of the UDP binary data, which converts the Protobuf binary data sent by a UDP socket to a C++ data structure. All memory allocation of this class is done in its constructor. If the conversion are repeated many times, the object should be kept alive to reduce the memory allocation.

The full example can be found in this `script <https://github.com/YanzhaoW/srs-daq/blob/master/examples/readUDP/main.cpp>`_ .

**Minimum example:**

.. code-block:: cpp
  :linenos:

  #include <srs/srs.hpp>

  auto main() -> int
  {
    auto msg_reader = srs::ProtoMsgReader{};

    std::string_view proto_binary = get_proto_msg();

    const auto& data_struct = msg_reader.convert(proto_binary);

    return 0;
  }


