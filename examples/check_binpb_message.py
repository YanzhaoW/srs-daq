#!/usr/bin/env python3

import argparse
import gzip

import delimited_protobuf as dp
import message_pb2


class ProtobufFile:
    def __init__(self, filename: str):
        self.filename = filename
        self.struct_data = message_pb2.Data()

    def print(self):
        with gzip.open(self.filename, "rb") as file:
            while True:
                self.struct_data = dp.read(file, message_pb2.Data)
                if self.struct_data is not None:
                    self.__print_message()
                else:
                    break

    def __print_message(self):
        if self.struct_data is None:
            return
        header = self.struct_data.header
        print("---------------------------------------------")
        print(f"hit size: {len(self.struct_data.hit_data)}")
        print(f"header: {header}")
        for marker_data in self.struct_data.marker_data:
            print(f"markder data: [vmm_id: {marker_data.vmm_id}, srs_timestamp: {marker_data.srs_timestamp}]")
        for hit_data in self.struct_data.hit_data:
            print(
                f"hit data: [ot: {hit_data.is_over_threshold}, "
                f"cn: {hit_data.channel_num}, tdc: {hit_data.is_over_threshold}, "
                f"off: {hit_data.offset}, vid: {hit_data.vmm_id}, "
                f"adc: {hit_data.adc}, bid: {hit_data.bc_id}]"
            )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="check_binpb_message", description="Check the data output from a protobuf binary file"
    )
    parser.add_argument("-f", "--filename", type=str, required=True, help="set the binary file name")
    args = parser.parse_args()

    proto_file = ProtobufFile(args.filename)
    proto_file.print()
