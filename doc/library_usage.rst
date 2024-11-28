################################################
Library usage
################################################

Use CMake to link the library
#################################

Link the SRS library with CMake `find_package`:

.. code-block:: cmake

  find_package(srs REQUIRED)
  add_executable(main PRIVATE srs::srs)

Please make sure add the directory path of the installation folder ``srs-install`` to ``CMAKE_PREFIX_PATH``:

.. code-block:: bash

  cmake -DCMAKE_PREFIX_PATH=[...]/srs-install ..

Available APIs
##################################

.. toctree::
   :maxdepth: 1

   stubs/library_apis
