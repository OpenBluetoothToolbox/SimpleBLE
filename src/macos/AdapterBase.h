#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

/**
  This class definition acts as an abstraction layer between C++ and ObjC.
  If ObjC headers are included here, everything blows up.
 */
class AdapterBase {
  public:
    AdapterBase();
    virtual ~AdapterBase();

    virtual std::string identifier() = 0;
    virtual BluetoothAddress address() = 0;

    static std::vector<std::shared_ptr<AdapterBase> > get_adapters();

  private:
};

}  // namespace SimpleBLE