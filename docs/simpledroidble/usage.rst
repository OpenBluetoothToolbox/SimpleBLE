=====
Usage
=====

SimpleDroidBLE is an Android-specific package that provides an API similar to
SimpleBLE, using modern Kotlin idiomatic code.

This code is currently under active development and some features are not yet
implemented or their API might change, but should be enough to help you get
started.

Obtain AAR dependencies
=======================

Using prebuilt AAR
------------------

1. Download the latest SimpleDroidBLE packages from the `releases page <https://github.com/OpenBluetoothToolbox/SimpleBLE/releases/>`_.

2. Follow `Add SimpleDroidBLE to your project`_.

Building from source
--------------------

1. Clone `the repo <https://github.com/OpenBluetoothToolbox/SimpleBLE>`_.

2. Build the libraries using gradle (``./simpledroidble/gradlew assembleRelease``) or Android Studio. The following libraries will be built:
    - ``simpledroidble/simpledroidble/build/outputs/aar/simpledroidble-release.aar`` 
    - ``simpleble/src/backends/android/simpleble-bridge/build/outputs/aar/simpleble-bridge-release.aar``

3. Follow `Add SimpleDroidBLE to your project`_.

Add SimpleDroidBLE to your project
==================================

From AAR file
-------------

1. Place the package on your app's directory, and take note of its path. We'll use ``example-app/app/libs/{simpledroidble,simpleble-bridge}.aar``.

2. Declare the dependency on your app's ``build.gradle{.kts}`` file (note: relative paths begin from your app's module folder)::

    dependencies {
      /* ... */
      implementation(files('libs/simpledroidble.aar', 'libs/simpleble-bridge.aar'))
    }

3. You're ready to start using SimpleBLE on Android! Go to `Using SimpleDroidBLE`_ to continue with the onboarding.

Consuming Locally
-----------------

If you want to use SimpleDroidBLE as part of your project from a local copy,
you can do so by adding the following to your ``settings.gradle`` or ``settings.gradle.kts`` file.
Make sure this include is before your ``include(":app")`` statement::

    includeBuild("path/to/simpledroidble") {
        dependencySubstitution {
            substitute module("org.simpleble.android:simpledroidble") with project(":simpledroidble")
        }
    }

Or, in Kotlin::

    includeBuild("path/to/simpledroidble") {
        dependencySubstitution {
            substitute(module("org.simpleble.android:simpledroidble")).using(project(":simpledroidble"))
        }
    }

Then, inside your ``build.gradle`` or ``build.gradle.kts`` file, you can add the
following dependency::

    dependencies {
        implementation "org.simpleble.android:simpledroidble"
    }

Kotlin::

    dependencies {
        implementation("org.simpleble.android:simpledroidble")
    }

Using SimpleDroidBLE
====================

First you'll need to provide ``SimpleDroidBLE.contextReference`` with a WeakReference to the app's context. In this step, the native code will be loaded in memory and permissions will be requested to the user.

The Adapter class
-----------------

The main entry point to SimpleDroidBLE is the Adapter object. You can get an Adapter instance via the static ``Adapter#getAdapters`` method, where the device's main bluetooth adapter will be returned.

When using SimpleDroidBLE make sure to only reference a single instance of the Adapter object, as every instance creates a new internal referece in the native code.
Having multiple instances (by unbounded calls to ``Adapter#getAdapters``, for example) may cause a memory leak.
This can be accomplished by acquiring said instance either on your app's Application class, or, in a single activity app, on your Activity's viewmodel; and treating the Adapter as a global, singleton object.
For an specific example you can check out `the sample app <https://github.com/OpenBluetoothToolbox/SimpleBLE/tree/main/examples/simpleble-android>`_.

Connecting to devices
---------------------

SimpleDroidBLE is a `flow <https://kotlinlang.org/api/kotlinx.coroutines/kotlinx-coroutines-core/kotlinx.coroutines.flow/-flow/>`_-driven interface.

That means that when looking to connect to a device, you should ``collect`` from ``Adapter#onScanFound`` and then start scanning for devices with ``Adapter#scanStart``. You'll get all found ``Peripheral`` s on said flow, where you can ``Peripheral#connect`` to whichever you need.


Using the SimpleBLE C++ interface
=================================

This requires you to install ``simpleble-bridge`` to your Android app, as this is needed for the C++ Android backend.
Then, you may follow `Usage alongside native code in Android <../simpleble/usage.html#usage-alongside-native-code-in-android>`_ and the `API Reference for C++ <../simpleble/api.html>`_ for more information.