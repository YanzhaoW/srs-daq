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

Download the project
=================================================

Please visit the `release page <https://github.com/YanzhaoW/srs-daq/releases>`_ and download the latest release for your operating system:

.. code-block:: bash

  wget [download-link]
  tar -xvzf [download-file]

After unzipping the downloaded file, a new folder ``srs-daq`` will be put in the current folder.

If your operating system is not in the download link list. Please either :doc:`build the project from source </build_source>` or create an issue to make the request.

.. important::
  If the ROOT support is needed, please install the same ROOT version used in the download link.

srs_control - The main program
=================================================

Go to ``srs-daq/bin`` directory and run

.. code-block:: bash

  ./srs_control [-p DATA_PRINT_OPTION] [-v LOG_LEVEL] [-h]

Run-time options
-------------------------------------------

- ``-h`` or ``--help``: print the help message.

- ``-v`` or ``--version``: show the current version.

- ``--root-version``: show the ROOT version if used.

- ``-l`` or ``--log-level``: set the verbose level. Available options: "critical", "error", "warn", "info" (default), "debug", "trace", "off".

- ``-p`` or ``--print-mode``: set the data printing mode. Available options:

  - speed (default): print the reading rate of received data.

  - header: print the header message of received data.

  - raw: print the received raw bytes.

  - all: print all data, including header, hit and marker data, but no raw data.
    
- ``-o`` or ``--output-files``: set the file outputs (more detail below).

Data output to multiple files
-------------------------------------------

``srs_control`` can output received data into multiple files with different types at the same time. Currently, following output types are available (or planned):

* **binary**

  * raw data if ``.lmd`` or ``.bin``

  * Protobuf data if ``.binpb``

* **json**. File extensions: ``.json`` (NOTE: JSON file could be very large)

* **root**. File extensions: ``.root`` (require ROOT library)

* **UDP socket** (Protobuf + gzip). Input format: ``[ip]:[port]``

Users have to use the correct file extensions to enable the corresponding output types.

To output the same data to multiple different output types at the same time:

.. code-block:: bash

  ./srs_control -o "output1.root" -o "output2.root" \
                -o "output.bin" -o "output.binpb" \
                -o "output.json" -o "localhost:9999"

Usage of other executables
=================================================

``srs_check_binpb``
-------------------------------------------

This is used for checking the contents of a Protobuf binary file.

.. code-block:: bash

  ./srs_check_binpb -f filename.binpb

``srs_check_udp``
-------------------------------------------

The executable checks the data output from a UDP socket.

.. code-block:: bash

  ./srs_check_udp --port [port number] --ip "localhost"

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
