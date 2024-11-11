target_link_libraries(srscpp PRIVATE zpp_bits::zpp_bits)

target_sources(
    srscpp
    PRIVATE FILE_SET
            privateHeaders
            FILES
            DataDeserializeOptions.hpp
            ProtoDelimDeserializer.hpp
            ProtoDeserializer.hpp
            ProtoDeserializerBase.hpp
            SerializableBuffer.hpp
            StructDeserializer.hpp
            StructToProtoConverter.hpp)
