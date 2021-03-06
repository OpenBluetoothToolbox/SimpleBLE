#pragma once

#include "AdapterBase.h"

#import <Foundation/Foundation.h>

namespace SimpleBLE {

class AdapterBaseMacOS : public AdapterBase {
  public:
    AdapterBaseMacOS();
    ~AdapterBaseMacOS();

    std::string identifier() override;
    BluetoothAddress address() override;

  private:
};

}  // namespace SimpleBLE