===
FAQ
===

Right now, the best way to have your questions answered is on our `Discord`_ server.
More information will be made available here soon.

**Why do I get UUIDs for the peripheral address on MacOS?**

MacOS has a policy of not allowing applications to access the Bluetooth address of
peripherals. Instead, it provides a UUID that is unique to the peripheral. This
UUID is not persistent across reboots, so you should not use it to identify a
peripheral. Instead, you should use the name of the peripheral, which is
persistent across reboots.

.. _Discord: https://discord.gg/N9HqNEcvP3
