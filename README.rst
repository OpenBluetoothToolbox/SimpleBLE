SimpleBLE
==========

The ultimate fully-fledged cross-platform library and bindings for Bluetooth Low Energy (BLE).

|Latest Documentation Status|

Overview
--------

The SimpleBLE project aims to provide fully cross-platform BLE libraries and bindings
for Python, Rust and C++, designed for simplicity and ease of use with a licencing scheme
chosen to be friendly towards commercial use. All specific operating system quirks
are handled internally to provide a consistent behavior across all platforms. The
libraries also provide first-class support for vendorization of all third-party
dependencies, allowing for easy integration into existing projects.

**NOTICE: Since February 20, 2024 the license terms of SimpleBLE have changed. Please make sure to read and understand the details below.**

* **SimpleBLE:** C++ cross-platform BLE library.
* **SimplePyBLE:** Python bindings for SimpleBLE. See the `SimplePyBLE`_ PyPI page for more details.
* **SimpleRsBLE:** Rust bindings for SimpleBLE. See the `SimpleRsBLE`_ Crates.io page for more details.
* **SimpleBluez:** C++ abstraction layer for BlueZ over DBus. (Linux only)
* **SimpleDBus:** C++ wrapper for libdbus-1 with convenience classes to handle DBus object hierarchies effectively. (Linux only)

If you want to use SimpleBLE and need help. **Please do not hesitate to reach out!**

* Visit our `ReadTheDocs`_ page.
* Join our `Discord`_ server.
* Contact me: ``kevin at dewald dot me`` (Dedicated consulting services available)

Are you using SimpleBLE on your own project and would like to see it featured here?
Reach out and I'll add a link to it below!

Supported platforms
-------------------
=========== ============= =================================== =====
Windows     Linux         MacOS                               iOS
=========== ============= =================================== =====
Windows 10+ Ubuntu 20.04+ 10.15+ (except 12.0, 12.1 and 12.2) 15.0+
=========== ============= =================================== =====

**NOTE:** WSL does not support Bluetooth.

Projects using SimpleBLE
------------------------
Don't forget to check out the following projects using SimpleBLE:

* `BrainFlow`_
* `InsideBlue`_
* `NodeWebBluetooth`_

Contributing
------------
Pull requests are welcome. For major changes, please open an issue first to discuss
what you would like to change.

License
=======

Since February 15th 2024, SimpleBLE is now available under the GNU General Public License
version 3 (GPLv3), with the option for a commercial license without the GPLv3 restrictions
available for a fee.

**More information on pricing and commercial terms of service will be available soon.**

To enquire about a commercial license, please contact us at ``contact at simpleble dot org``.

**NOTICE:** The following projects are allowed to use SimpleBLE under a GNU Lesser General Public
License version 3 (LGPLv3), due to their past contributions: `BrainFlow`_, `InsideBlue`_, `NodeWebBluetooth`_.

Likewise, if you are using SimpleBLE in an open-source project and would like to request
a free commercial license or if you have any other questions, please reach out at ``contact at simpleble dot org``.

Licensing FAQ
-------------

Why are you making this change?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

So far, SimpleBLE has been a labor of passion with over 4000 hours invested in multiple iterations.
The decision to transition SimpleBLE to a dual-licensing model is mainly driven by the kind
of products that have been built around it, in particular around notable names in the medical
and industrial sectors, which has been both surprising and encouraging. Providing robust support for
these diverse and critical use cases is a resource-intensive endeavor which can't be achieved on
goodwill alone, especially so when the underlying APIs are also evolving and life having its own
plans. By introducing a commercial license, I'm opening a pathway to dedicate more resources to
enhance SimpleBLE. Some of the things on the roadmap include:
- Bindings into more languages and frameworks.
- Hardware-in-the-loop test infrastructure.
- Offering bounties and revenue sharing with other developers who contribute.
- Providing more comprehensive documentation and tutorials.

Despite this transition, I remain firmly committed to the open-source philosophy. SimpleBLE was grown
a lot thanks to the feedback of the open-source community, and that foundation will always be a part
of the project. The GPLv3 license option ensures continued accessibility for open-source projects,
and I pledge to actively contribute to and collaborate with the community whenever possible.

Ultimately, the success of SimpleBLE has been fueled by its open nature, and I believe this
dual-licensing model strengthens that success by enabling both community-driven growth and
targeted enhancements that benefit everyone.


I'm already using SimpleBLE. What happens to my project?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Please reach out at ``contact at simpleble dot org`` and we can discuss the specifics of your
situation. It is my intention to make this transition as smooth as possible for existing users,
and I'm open to finding a solution that works for everyone.


What do you consider "commercial use"?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Commercial use in the context of SimpleBLE refers to any usage of the SimpleBLE or any of its
language bindings in a commercial setting where it contributes, either directly or indirectly,
to financial gain. This includes, but is not limited to, incorporating SimpleBLE into commercial
software products, using it in the development of services offered for a fee, or deploying it
within internal tools or software in a for-profit organization where it adds business value.
The key factor is the use of the library as a component in a revenue-generating or
business-advancing context, irrespective of whether the library itself is sold or offered as
a standalone product.


What does the GPLv3 license imply for my commercial project?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The GPLv3 license ensures that end users have the freedom to run, study, share, and modify the software.
It requires that any modified versions of SimpleBLE, or any software incorporating it, also be
distributed under the GPLv3. Essentially, if your project incorporates SimpleBLE and is distributed,
the entire codebase must be open-source under the GPLv3.

You can find the full text of the GPLv3 license at https://www.gnu.org/licenses/gpl-3.0.html.

.. Links

.. _SimplePyBLE: https://pypi.org/project/simplepyble/

.. _SimpleRsBLE: https://crates.io/crates/simplersble

.. _Discord: https://discord.gg/N9HqNEcvP3

.. _ReadTheDocs: https://simpleble.readthedocs.io/en/latest/

.. |Latest Documentation Status| image:: https://readthedocs.org/projects/simpleble/badge?version=latest
   :target: http://simpleble.readthedocs.org/en/latest

.. Other projects using SimpleBLE

.. _GDSimpleBLE: https://github.com/jferdelyi/GDSimpleBLE
.. _BrainFlow: https://github.com/brainflow-dev/brainflow
.. _InsideBlue: https://github.com/eriklins/InsideBlue-BLE-Tool
.. _NodeWebBluetooth: https://github.com/thegecko/webbluetooth
