#pragma once

#include <optional>
#include <string>
#include <vector>

#include <simpleble/Backend.h>

namespace SimpleBLE {

namespace Safe {

class Adapter;

/**
 * Safe (noexcept) wrapper over SimpleBLE::Backend.
 *
 * This is merely a wrapper around a SimpleBLE::Backend pointer. For this
 * reason, it is handled as a value and not as a pointer.
 */
class Backend {
  public:
    Backend() = default;
    virtual ~Backend() = default;

    /**
     * Get a list of all available adapters from this backend.
     */
    std::optional<std::vector<Adapter>> get_adapters() noexcept;

    /**
     * Check if Bluetooth is enabled for this backend.
     */
    std::optional<bool> bluetooth_enabled() noexcept;

    /**
     * Check if the Bluetooth is enabled for any of the backends.
     *
     * @return True if any backend has Bluetooth enabled, false if
     *    no backends have Bluetooth enabled, std::nullopt or an error occurred.
     */
    static std::optional<bool> any_bluetooth_enabled() noexcept;

    /**
     * Get the first backend that has Bluetooth enabled.
     *
     * @return The first backend that has Bluetooth enabled, or std::nullopt if
     *    no backends have Bluetooth enabled or an error occurred.
     */
    static std::optional<Backend> first_bluetooth_enabled() noexcept;

    /**
     * Get the name of the backend.
     */
    std::string backend_name() const noexcept;

    /**
     * Get all registered backends.
     */
    static std::optional<std::vector<Backend>> get_backends() noexcept;

    /**
     * Get the underlying (non-safe) backend object.
     */
    SimpleBLE::Backend underlying() const noexcept;

    /**
     * Cast to the underlying (non-safe) backend object.
     */
    operator SimpleBLE::Backend() const noexcept;

  protected:
    SimpleBLE::Backend internal_;
};

}  // namespace Safe

}  // namespace SimpleBLE
