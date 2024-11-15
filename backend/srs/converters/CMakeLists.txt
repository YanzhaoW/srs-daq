target_link_libraries(srscpp PRIVATE $<BUILD_LOCAL_INTERFACE:zpp_bits::zpp_bits>)

target_sources(srscpp PRIVATE StructDeserializer.cpp)

target_sources(
    srscpp
    PRIVATE FILE_SET
            privateHeaders
            FILES
            DataDeserializeOptions.hpp
            ProtoDelimSerializer.hpp
            ProtoSerializer.hpp
            ProtoSerializerBase.hpp
            SerializableBuffer.hpp
            StructDeserializer.hpp
            StructToProtoConverter.hpp)
