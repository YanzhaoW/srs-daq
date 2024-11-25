# srs-daq - A data acquisition program for SRS FEC & VMM3

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/7e8c956af1bc46c7836524f1ace32c11)](https://app.codacy.com/gh/YanzhaoW/srs-daq/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![CI pipeline](https://github.com/YanzhaoW/srs-daq/actions/workflows/ci.yml/badge.svg?branch=dev)](https://github.com/YanzhaoW/srs-daq/actions?query=branch%3Adev)
[![Github Releases](https://img.shields.io/github/release/YanzhaoW/srs-daq.svg)](https://github.com/YanzhaoW/srs-daq/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

SRS-DAQ is a project containing a command line tool `srs_control` and additional library APIs that can be used to communicate with SRS system and interpret the received data in your own program.

Please check the full documentation of this project: [SRS-DAQ documentation](<https://yanzhaow.github.io/srs-daq/>).

## `srs_control` preview

![Imgur](doc/media/srs_control_preview_v1.gif)

## Acknowledgments

- A lot of information was used from the existing codebase of the VMM slow control software [vmmsc](https://gitlab.cern.ch/rd51-slow-control/vmmsc.git).
- The understanding of the communication protocol was helped from [srslib](https://github.com/bl0x/srslib).
