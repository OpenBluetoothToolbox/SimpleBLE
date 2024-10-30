#pragma once

#include <iostream>
/* Adapted from https://gist.github.com/jcarrano/39f5acbf3d94aeed802bda15aa7e79f9 */

namespace SimpleBLE::internal {

/**
 * Base class for Class Registries.
 *
 * Subclasses must implement a protected static member template:
 *  `template <typename Sub> static void registerCls();`
 *
 * What this method does and how it adds the entries to the container is up to
 * the subclass to implement.
 */
template <typename Container, typename CRTP>
class ClassRegistry {
  public:
    using ContainerT = Container;
    static const ContainerT& classes(void) { return _classes(); }

    /**
     * Register a class in a container contained in a collector class.
     *
     * By declaring a static member using this template, the containing class will
     * be added to a container at startup.
     *
     * @param   Collector   Class that contains the registry (a static member vector
     *                  named `_classes`). It is not necessary for this class
     *                  to be a superclass of SubC.
     * @param   SubC    Class to register (you must use CRTP).
     */
    template <class SubC>
    class ClassRegistrator {
      public:
        ClassRegistrator() { ClassRegistry::AccessRegisterImpl::template registerCls<SubC>(); }
    };

  protected:
    template <class SubC>
    friend class ClassRegistrator;

    /**
     * Helper to allow subclasses to have registerCls as a protected member
     */
    struct AccessRegisterImpl : CRTP {
        template <typename Sub>
        static void registerCls() {
            CRTP::template registerCls<Sub>();
        }
    };

    // By using a static method instead of a member we avoid problems with
    // initialization order (i.e. using the container before it is initialized).
    static ContainerT& _classes(void) {
        static ContainerT c{};
        return c;
    }
};

/**
 * Mixin that registers the class.
 *
 * When this template is instantiated, the derived class "CRTP" will be
 * registered in RegistryClass.
 */
template <typename RegistryClass, typename CRTP>
class WithRegistrator {
  public:
    using RegistratorT = typename RegistryClass::template ClassRegistrator<CRTP>;
#ifdef __clang__
    // The used attribute in the static member variable does not work in clang so
    // we need this.
    static RegistratorT& _clang_workaround(void) __attribute__((used)) { return registrator; }
#endif
  private:
    static inline RegistratorT registrator __attribute__((used)){};
};

}  // namespace SimpleBLE::internal
