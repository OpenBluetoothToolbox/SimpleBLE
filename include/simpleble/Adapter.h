#pragma once

#include <memory>
#include <vector>

#include <simpleble/Adapter.h>

namespace SimpleBLE {

class Adapter {
  private:
    friend class std::shared_ptr<Adapter>;

    Adapter(/* args */);
    ~Adapter();

  public:



    static std::vector<std::shared_ptr<Adapter>> get_available_adapters();

  
};

}  // namespace SimpleBLE