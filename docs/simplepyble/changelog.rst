Changelog
=========

All notable changes to this project will be documented in this file.

The format is based on `Keep a Changelog`_, and this project adheres to
`Semantic Versioning`_.

[0.0.6] - 2022-XX-XX
--------------------

**Added**

-

**Changed**

-  Updated native implementation to SimpleBLE v0.5.0.


[0.0.5] - 2022-06-12
--------------------

**Added**

-  Python's Global Interpreter Lock (GIL) will be released during ``Peripheral.connect()``.
-  Keep-alive policies for function objects passed into SimplePyBLE.

**Changed**

-  Updated native implementation to SimpleBLE v0.4.0.


[0.0.4] - 2022-02-13
--------------------

**Changed**

-  Updated native implementation to SimpleBLE v0.2.0.


[0.0.3] - 2022-01-22
--------------------

**Fixed**

-  ``write_request`` and ``write_command`` functions would accept strings instead of bytes as payloads. *(Thanks kaedenbrinkman!)*


[0.0.2] - 2022-01-14
--------------------

**Added**

-  All functionality presented by SimpleBLE is now available.


[0.0.1] - 2022-01-09
--------------------

This is a proof-of-concept release, to show all steps in the build and deployment process work.

.. _Keep a Changelog: https://keepachangelog.com/en/1.0.0/
.. _Semantic Versioning: https://semver.org/spec/v2.0.0.html