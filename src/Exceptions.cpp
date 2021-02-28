#include <simpleble/Exceptions.h>

using namespace SimpleBLE::Exception;

InvalidReference::InvalidReference() : std::runtime_error("Underlying reference to object has been invalidated") {}