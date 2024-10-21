#pragma once

#include <dbus/dbus.h>

#include <atomic>
#include <stack>
#include <string>
#include <vector>

#include "Holder.h"

namespace SimpleDBus {

class Message {
  public:
    typedef enum {
        INVALID = DBUS_MESSAGE_TYPE_INVALID,
        METHOD_CALL = DBUS_MESSAGE_TYPE_METHOD_CALL,
        METHOD_RETURN = DBUS_MESSAGE_TYPE_METHOD_RETURN,
        ERROR = DBUS_MESSAGE_TYPE_ERROR,
        SIGNAL = DBUS_MESSAGE_TYPE_SIGNAL,
    } Type;

    const int INVALID_UNIQUE_ID = -1;

    Message();
    Message(Message&& other);                  // Custom move constructor
    Message(const Message& other);             // Custom copy constructor
    Message& operator=(Message&& other);       // Custom move assignment
    Message& operator=(const Message& other);  // Custom copy assignment
    ~Message();

    operator DBusMessage*() const;

    bool is_valid() const;
    void append_argument(Holder argument, std::string signature);

    Holder extract();
    void extract_reset();
    bool extract_has_next();
    void extract_next();
    std::string to_string(bool append_arguments = false) const;

    uint32_t get_ref_count();
    int32_t get_unique_id();
    uint32_t get_serial();
    std::string get_signature();
    std::string get_interface();
    std::string get_path();
    std::string get_member();
    Type get_type() const;

    bool is_signal(std::string interface, std::string signal_name);
    bool is_method_call(const std::string& interface, const std::string& method);

    static Message from_acquired(DBusMessage*);

    static Message create_method_call(std::string bus_name, std::string path, std::string interface,
                                      std::string method);

    static Message create_method_return(const Message& msg);

    static Message create_error(const Message& msg, std::string error_name, std::string error_message);

    static Message create_signal(std::string path, std::string interface, std::string signal);

  private:
    static std::atomic_int32_t creation_counter;
    int indent = 0;

    int _unique_id = INVALID_UNIQUE_ID;
    DBusMessageIter _iter;
    bool _iter_initialized = false;
    bool _is_extracted = false;
    Holder _extracted;
    DBusMessage* _msg = nullptr;

    Holder _extract_bytearray(DBusMessageIter* iter);
    Holder _extract_array(DBusMessageIter* iter);
    Holder _extract_dict(DBusMessageIter* iter);
    Holder _extract_generic(DBusMessageIter* iter);

    /**
     * @brief Append argument to the DBus message iterator.
     * @param iter      DBus message iterator.
     * @param argument  Argument to append.
     * @param signature Signature of the argument.
     */
    void _append_argument(DBusMessageIter* iter, Holder& argument, std::string signature);

    void _invalidate();
    void _safe_delete();

    std::vector<Holder> _arguments;
};

}  // namespace SimpleDBus
