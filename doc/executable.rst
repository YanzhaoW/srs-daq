##############################
Command line tools
##############################

srs_control
#####################

Synopsis
==================

.. code-block:: bash

  ./srs_control [-p DATA_PRINT_OPTION] [-v LOG_LEVEL] [-h]

Description
==================

:program:`srs_control` is the main program that controls the communications with SRS system, the data analysis and data writing.


Options
==================

.. program:: srs_control

.. option:: -h, --help

   Print the help message

.. option:: -v, --version

   Show the current version

.. option:: --root-version

   Show the ROOT version if used

.. option:: -l, --log-level

   Set the verbose level. Available options: "critical", "error", "warn", "info" (default), "debug", "trace", "off".

.. option:: -p, --print-mode

   Set the data printing mode. 

   Available options:

    - speed (default): print the reading rate of received data.

    - header: print the header message of received data.

    - raw: print the received raw bytes.

    - all: print all data, including header, hit and marker data, but no raw data.

.. option:: -o, --output-files

   Set the file outputs (more details below).

Data output to multiple files
--------------------------------

``srs_control`` can output received data into multiple files with different types at the same time. Currently, following output types are available:

* **binary**

  * raw data if ``.lmd`` or ``.bin``

  * Protobuf data if ``.binpb``

* **json**. File extensions: ``.json`` (NOTE: JSON file could be very large)

* **root**. File extensions: ``.root`` (require ROOT library)

* **UDP socket** (Protobuf + gzip). Input format: ``[ip]:[port]``

Users have to use the correct file extensions to enable the corresponding outputs.

Example
====================

To output the same data to multiple different output types at the same time:

.. code-block:: bash

  ./srs_control -o "output1.root" -o "output2.root" \
                -o "output.bin" -o "output.binpb" \
                -o "output.json" -o "localhost:9999"

Other tools
#############################

.. toctree::
   :maxdepth: 1

   stubs/check_binpb_message
   stubs/check_udp_message

