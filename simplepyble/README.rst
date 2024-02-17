SimplePyBLE
===========

The ultimate fully-fledged cross-platform bindings for Bluetooth Low Energy (BLE).

|Latest Documentation Status| |PyPI Licence|

Overview
--------

SimplePyBLE provides Python bindings for `SimpleBLE`_, a fully cross-platform BLE
library written in C++. It is designed for simplicity and ease of use with a
licencing scheme chosen to be friendly towards commercial use.

If you want to use SimpleBLE and need help. **Please do not hesitate to reach out!**

* Visit our `ReadTheDocs`_ page.
* Join our `Discord`_ server.
* Contact me: ``kevin at dewald dot me``

Are you using SimplePyBLE on your own project and would like to see it featured here?
Reach out and I'll add a link to it below!

Supported platforms
-------------------
=========== ============= =================================== =====
Windows     Linux         MacOS                               iOS
=========== ============= =================================== =====
Windows 10+ Ubuntu 20.04+ 10.15+ (except 12.0, 12.1 and 12.2) 15.0+
=========== ============= =================================== =====

**NOTE:** WSL does not support Bluetooth.

Installation
------------

You can install SimplePyBLE from PyPI using pip: ::

   pip install simplepyble

Please review our `code examples`_ on GitHub for more information on how to use
SimplePyBLE.

Contributing
------------
Pull requests are welcome. For major changes, please open an issue first to discuss
what you would like to change.

License
=======

Since February 15th 2024, SimpleBLE is now available under a dual-licensing scheme.

**For non-commercial or open-source projects**, SimpleBLE is available under the BSD 3-Clause License,
allowing free use and modification. However, if derivatives of these projects are used commercially,
a commercial license for SimpleBLE is required by the entity selling the software containing SimpleBLE.

**For commercial usage**, the library is licensed under the GNU General Public License version 3 (GPLv3),
with the option for a commercial license without the GPLv3 restrictions available for a fee. To obtain
commercial license, please contact me at ``contact at simpleble dot org``.


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
of the project. The BSD 3-Clause license option ensures continued accessibility for non-commercial and
open-source projects, and I pledge to actively contribute to and collaborate with the community whenever
possible. Ultimately, the success of SimpleBLE has been fueled by its open nature, and I believe this
dual-licensing model strengthens that success by enabling both community-driven growth and targeted
enhancements that benefit everyone.


I'm already using SimpleBLE. What happens to my project?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Please reach out at ``contact at simpleble dot org`` and we can discuss the specifics of your
situation. It is my intention to make this transition as smooth as possible for existing users,
and I'm open to finding a solution that works for everyone.


What is offered as part of the commercial license?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* **Prioritized Support:** Your feature requests and bug fixes receive top priority.
* **Company Spotlight:**  Showcase your company logo on our documentation and website.
* **Dedicated Assistance:** Get personalized guidance with a few hours of one-on-one support for in-depth implementation or debugging challenges.


What do you consider "commercial use"?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Commercial use in the context of SimpleBLE refers to any usage of the library or its derivatives
in a commercial setting where it contributes, either directly or indirectly, to financial gain.
This includes, but is not limited to, incorporating SimpleBLE into commercial software products,
using it in the development of services offered for a fee, or deploying it within internal tools
or software in a for-profit organization where it adds business value. The key factor is the use
of the library as a component in a revenue-generating or business-advancing context, irrespective
of whether the library itself is sold or offered as a standalone product.


What do you consider "derivative work"?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A derivative work, in relation to SimpleBLE, refers to any software or application that includes,
builds upon, or modifies any part of the SimpleBLE library. This encompasses not only direct
modifications of the library's source code but also software that incorporates the library as a
significant functional element. For instance, an application that uses SimpleBLE as a foundational
component for Bluetooth connectivity would be considered a derivative work. The commercial use
clause applies when such a derivative work is used for commercial purposes, as previously defined.
It's important to note that mere usage of SimpleBLE as an incidental component does not necessarily
constitute a derivative work, especially if it does not form a significant part of the application's
overall functionality or value proposition.


What does the GPLv3 license imply for my commercial project?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The GPLv3 license ensures that end users have the freedom to run, study, share, and modify the software.
It requires that any modified versions of SimpleBLE, or any software incorporating it, also be
distributed under the GPLv3. Essentially, if your project incorporates SimpleBLE and is distributed,
the entire codebase must be open-source under the GPLv3.

You can find the full text of the GPLv3 license at https://www.gnu.org/licenses/gpl-3.0.html.


What does the BSD 3-Clause license imply for my project?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The BSD 3-Clause license is a permissive open-source license that allows you to use, modify, and
redistribute SimpleBLE in your project, whether it's open-source or proprietary. It does not require
that your project be open-source, and it does not impose any restrictions on how you use SimpleBLE
in your project. The only requirement is that you include the original copyright notice and license
text in any distribution of SimpleBLE or its derivatives.

You can find the full text of the BSD 3-Clause license at https://opensource.org/licenses/BSD-3-Clause.

.. Links

.. _SimplePyBLE: https://pypi.org/project/simplepyble/

.. _SimpleBLE: https://github.com/OpenBluetoothToolbox/SimpleBLE/

.. _code examples: https://github.com/OpenBluetoothToolbox/SimpleBLE/tree/main/examples/simplepyble

.. _Discord: https://discord.gg/N9HqNEcvP3

.. _ReadTheDocs: https://simpleble.readthedocs.io/en/latest/

.. |Latest Documentation Status| image:: https://readthedocs.org/projects/simpleble/badge?version=latest
   :target: http://simpleble.readthedocs.org/en/latest

.. |PyPI Licence| image:: https://img.shields.io/pypi/l/simplepyble
