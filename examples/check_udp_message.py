#!/usr/bin/env python3

import argparse
import socket

import message_pb2


class UDPConnection:
    def __init__(self, _ip: str, _port: int):
        self.ip = _ip
        self.port = _port
        self.buffer_size = 100000
        self.test_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.test_remote = None
        self.struct_data = message_pb2.Data()

    @property
    def buffer_size(self):
        return self._buffer_size

    @buffer_size.setter
    def buffer_size(self, size: int):
        self._buffer_size = size

    def start_listen(self):
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as udp_socket:
            udp_socket.bind((self.ip, self.port))
            udp_socket.settimeout(0.5)

            while True:
                try:
                    message, remote = udp_socket.recvfrom(self.buffer_size)
                    self.test_remote = remote
                    self.__deserialize(message)
                except socket.timeout:
                    if self.__test_udp_open():
                        continue
                    return

    def __deserialize(self, message: bytes):
        self.struct_data.ParseFromString(message)
        header = self.struct_data.header
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

    def __test_udp_open(self) -> bool:
        if self.test_remote is not None:
            res = self.test_socket.connect_ex(self.test_remote)
            if res != 0:
                return True
            print("Remote udp port is closed")
            return False
        return True


if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog="check_udp_message", description="Check the data output from the udp socket")
    parser.add_argument("--port", type=int, required=True, help="set the port number")
    parser.add_argument("--ip", type=str, required=True, help="set the ip address")
    args = parser.parse_args()

    connection = UDPConnection(args.ip, args.port)
    connection.start_listen()
