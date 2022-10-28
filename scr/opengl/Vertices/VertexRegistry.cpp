#include "VertexRegistry.h"

int VertexRegistry::_last_signature = 0;
std::map<std::string, unsigned int> VertexRegistry::_vertex_signatures;