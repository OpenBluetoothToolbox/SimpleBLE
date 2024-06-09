#pragma once
#include <dbus/dbus.h>
#include <dlfcn.h>

namespace SimpleDBus {
struct libdbus {
    libdbus() {}
    ~libdbus() {
        if (impl) dlclose(impl);
    }
    void* impl = dlopen("libdbus-1.so.3", RTLD_LAZY | RTLD_LOCAL | RTLD_NODELETE);

    static const libdbus& instance() {
        static libdbus self;
        return self;
    }
    template <typename T>
    T symbol(const char* const sym) const noexcept {
        if (impl)
            return reinterpret_cast<T>(dlsym(impl, sym));
        else
            return nullptr;
    }

#define SIMPLEBLE_LIBDBUS_SYMBOL(name) \
    decltype(&::dbus##_##name) name = symbol<decltype(&::dbus##_##name)>("dbus_" #name);

    SIMPLEBLE_LIBDBUS_SYMBOL(bus_add_match)
    SIMPLEBLE_LIBDBUS_SYMBOL(bus_get)
    SIMPLEBLE_LIBDBUS_SYMBOL(bus_get_unique_name)
    SIMPLEBLE_LIBDBUS_SYMBOL(bus_remove_match)
    // SIMPLEBLE_LIBDBUS_SYMBOL(bus_type)
    SIMPLEBLE_LIBDBUS_SYMBOL(connection_flush)
    SIMPLEBLE_LIBDBUS_SYMBOL(connection_pop_message)
    SIMPLEBLE_LIBDBUS_SYMBOL(connection_read_write)
    SIMPLEBLE_LIBDBUS_SYMBOL(connection_send)
    SIMPLEBLE_LIBDBUS_SYMBOL(connection_send_with_reply_and_block)
    SIMPLEBLE_LIBDBUS_SYMBOL(connection_unref)
    SIMPLEBLE_LIBDBUS_SYMBOL(error_free)
    SIMPLEBLE_LIBDBUS_SYMBOL(error_is_set)
    SIMPLEBLE_LIBDBUS_SYMBOL(error_init)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_copy)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_get_destination)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_get_interface)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_get_member)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_get_path)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_get_sender)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_get_type)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_get_serial)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_is_signal)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_new_method_call)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_new_method_return)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_new_error)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_append_basic)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_close_container)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_get_arg_type)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_get_basic)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_get_fixed_array)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_get_signature)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_has_next)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_init)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_init_append)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_next)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_open_container)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_iter_recurse)
    SIMPLEBLE_LIBDBUS_SYMBOL(message_unref)
    SIMPLEBLE_LIBDBUS_SYMBOL(threads_init_default)
};
}  // namespace SimpleDBus
