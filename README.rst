======================================================================
SRS-DAQ - A data acquisition program for SRS FEC & VMM3
======================================================================

|codacyBadge| |ciPipeline| |githubReleases| |license|

.. |codacyBadge| image:: https://app.codacy.com/project/badge/Grade/7e8c956af1bc46c7836524f1ace32c11
   :alt: Codacy badge
   :target: https://app.codacy.com/gh/YanzhaoW/srs-daq/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade
.. |ciPipeline| image:: https://github.com/YanzhaoW/srs-daq/actions/workflows/ci.yml/badge.svg?branch=dev
   :alt: ci pipeline
   :target: https://github.com/YanzhaoW/srs-daq/actions?query=branch%3Adev
.. |githubReleases| image:: https://img.shields.io/github/release/YanzhaoW/srs-daq.svg
   :alt: github releases
   :target: https://github.com/YanzhaoW/srs-daq/releases
.. |license| image:: https://img.shields.io/badge/License-MIT-yellow.svg
   :alt: License: MIT
   :target: https://opensource.org/licenses/MIT

Introduction
=================================================

srs-daq is an asynchronous data IO program for SRS system. It communicates with the SRS system through the UDP socket with different commands to, for example, start or stop the data transferring from the system. Data sent from the SRS system is then analyzed in the program in different stages, such as the deserialization, monitoring, data conversion and writing to different files or sockets. There are 4 types of output formats supported: binary file, root file (require ROOT), JSON file and UDP socket. Binary file can store either **as-is** data sent from the SRS system or the Google's Protobuf binary. UDP socket can only send the Google's Protobuf binary due to its easy deserialization.

Technical information about classes and functions can be found in this `Doxygen documentation <https://yanzhaow.github.io/srs-daq/>`_.



Usage of other executables
=================================================


Usage of SRS library
=================================================

The program also has APIs to provide following functionality:

- Convert binary ``string_view`` to the data structure ``srs::StructData``.

For more information, please check the :doc:`library usage </library_usage>`.

Custom configuration
-------------------------------------------

To be added ...

Acknowledgments
=================================================

- A lot of information was used from the existing codebase of the VMM slow control software `vmmsc <https://gitlab.cern.ch/rd51-slow-control/vmmsc.git>`_.

TODO list
=================================================

- unit tests
- Control/monitoring for SRS FEC & VMM3a hybrids
- Graphical user interface (typescript + react + websocket).
