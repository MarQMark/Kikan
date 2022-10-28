#ifndef KIKAN_VERTEX_REGISTRY_H
#define KIKAN_VERTEX_REGISTRY_H

#include "map"
#include "string"
#include "typeinfo"

class VertexRegistry {
public:

    template<class T>
    static unsigned int getSignature(){
        std::string sig = typeid(T).name();

        if(_vertex_signatures.count(sig))
            return _vertex_signatures[sig];

        _vertex_signatures.insert(std::make_pair(sig, _last_signature++));
        return _last_signature - 1;
    }

private:
    static std::map<std::string, unsigned int> _vertex_signatures;
    static int _last_signature;
};


#endif //KIKAN_VERTEX_REGISTRY_H
