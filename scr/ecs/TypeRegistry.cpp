#include "TypeRegistry.h"

int TypeRegistry::_last_signature = 0;
std::map<std::string, unsigned int> TypeRegistry::_component_signatures;