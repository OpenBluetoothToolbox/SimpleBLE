#pragma once

#include <simpledbus/advanced/Interface.h>
#include <simpledbus/advanced/Property.h>
#include <simpledbus/external/kvn_safe_callback.hpp>

#include <string>

namespace SimpleBluez {

class Battery1 : public SimpleDBus::Interface {
  public:
    Battery1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path);
    virtual ~Battery1();

    // ----- METHODS -----

    // ----- PROPERTIES -----
    SimpleDBus::Property<uint8_t> Percentage = create_property<uint8_t>("Percentage");

    // ----- CALLBACKS -----
    kvn::safe_callback<void()> OnPercentageChanged;

  protected:
    void property_changed(std::string option_name) override;
};

}  // namespace SimpleBluez
