Documentation
======================

To build documentation for the projects in this repository,
you first need to install Sphynx, using the following commands: ::

   sudo apt install python3-sphinx
   pip3 install sphinx_rtd_theme

Once all dependencies have been installed, HTML documentation can be built
by calling the following commands: ::

   cd <path-to-repository>/docs
   make html