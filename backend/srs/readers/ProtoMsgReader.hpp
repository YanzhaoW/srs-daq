#pragma once

#include <memory>
#include <srs/data/SRSDataStructs.hpp>
#include <string_view>

namespace srs
{
    namespace process
    {
        class Proto2StructConverter;
        class ProtoDeserializer;
    } // namespace process

    class ProtoMsgReader
    {
      public:
        /**
         * Constructor allocates the memeory for member variables
         */
        ProtoMsgReader();

        //! Deleted copy constructor
        ProtoMsgReader(const ProtoMsgReader&) = delete;

        //! Deleted copy assignment
        ProtoMsgReader& operator=(const ProtoMsgReader&) = delete;

        //! Default move constructor
        ProtoMsgReader(ProtoMsgReader&&) = default;

        //! Default move assignment
        ProtoMsgReader& operator=(ProtoMsgReader&&) = default;

        //! Default destructor
        ~ProtoMsgReader();

        /**
         * Convert binary data to a struct (inout).
         *
         * @param msg The input binary data.
         * @param struct_data The struct data to store the deserialized binary data.
         */
        auto convert(std::string_view msg, StructData& struct_data);

        /**
         * Convert binary data to a struct, owned by ProtoMsgReader.
         *
         * Description.
         *
         * @param msg The input binary data.
         * @return The const reference to the internal struct data.
         *
         */
        auto convert(std::string_view msg) -> const StructData&;

      private:
        //! A converter to transform Protobuf struct to native C++ struct
        std::unique_ptr<process::Proto2StructConverter> proto_to_struct_converter_;

        //! A converter to transform binary data to Protobuf struct
        std::unique_ptr<process::ProtoDeserializer> proto_deserializer_;
    };
} // namespace srs
