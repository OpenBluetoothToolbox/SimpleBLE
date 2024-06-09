#include <simpledbus/base/Connection.h>
#include <simpledbus/base/Exceptions.h>
#include <chrono>
#include <thread>

#include "Logging.h"

using namespace SimpleDBus;

Connection::Connection(DBusBusType dbus_bus_type) : _dbus_bus_type(dbus_bus_type) {}

Connection::~Connection() {
    if (_initialized) {
        uninit();
    }
}

void Connection::init() {
    if (_initialized) {
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    ::DBusError err;
    dbus.error_init(&err);

    dbus.threads_init_default();
    _conn = dbus.bus_get(_dbus_bus_type, &err);
    if (dbus.error_is_set(&err)) {
        std::string err_name = err.name;
        std::string err_message = err.message;
        dbus.error_free(&err);
        throw Exception::DBusException(err_name, err_message);
    }
    _initialized = true;
}

void Connection::uninit() {
    if (!_initialized) {
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    // In order to prevent a crash on any third party environment
    // we need to flush the connection queue.
    SimpleDBus::Message message;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        read_write();
        message = pop_message();
    } while (message.is_valid());

    dbus.connection_unref(_conn);
    _initialized = false;
}

bool Connection::is_initialized() { return _initialized; }

void Connection::add_match(std::string rule) {
    if (!_initialized) {
        throw Exception::NotInitialized();
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    ::DBusError err;
    dbus.error_init(&err);

    dbus.bus_add_match(_conn, rule.c_str(), &err);
    dbus.connection_flush(_conn);
    if (dbus.error_is_set(&err)) {
        std::string err_name = err.name;
        std::string err_message = err.message;
        dbus.error_free(&err);
        throw Exception::DBusException(err_name, err_message);
    }
}

void Connection::remove_match(std::string rule) {
    if (!_initialized) {
        throw Exception::NotInitialized();
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    ::DBusError err;
    dbus.error_init(&err);

    dbus.bus_remove_match(_conn, rule.c_str(), &err);
    dbus.connection_flush(_conn);
    if (dbus.error_is_set(&err)) {
        std::string err_name = err.name;
        std::string err_message = err.message;
        dbus.error_free(&err);
        throw Exception::DBusException(err_name, err_message);
    }
}

void Connection::read_write() {
    if (!_initialized) {
        throw Exception::NotInitialized();
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    // Non blocking read of the next available message
    dbus.connection_read_write(_conn, 0);
}

Message Connection::pop_message() {
    if (!_initialized) {
        throw Exception::NotInitialized();
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    DBusMessage* msg = dbus.connection_pop_message(_conn);
    if (msg == nullptr) {
        return Message();
    } else {
        return Message(msg);
    }
}

void Connection::send(Message& msg) {
    if (!_initialized) {
        throw Exception::NotInitialized();
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    uint32_t msg_serial = 0;
    dbus.connection_send(_conn, msg._msg, &msg_serial);
    dbus.connection_flush(_conn);
}

Message Connection::send_with_reply_and_block(Message& msg) {
    if (!_initialized) {
        throw Exception::NotInitialized();
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    ::DBusError err;
    dbus.error_init(&err);
    DBusMessage* msg_tmp = dbus.connection_send_with_reply_and_block(_conn, msg._msg, -1, &err);

    if (dbus.error_is_set(&err)) {
        std::string err_name = err.name;
        std::string err_message = err.message;
        dbus.error_free(&err);
        throw Exception::SendFailed(err_name, err_message, msg.to_string());
    }

    return Message(msg_tmp);
}

std::string Connection::unique_name() {
    if (!_initialized) {
        throw Exception::NotInitialized();
    }

    std::lock_guard<std::recursive_mutex> lock(_mutex);

    return std::string(dbus.bus_get_unique_name(_conn));
}
