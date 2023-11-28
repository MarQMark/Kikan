#ifndef KIKAN_TYPE_REGISTRY_H
#define KIKAN_TYPE_REGISTRY_H

#include "map"
#include "string"
#include "typeinfo"

namespace Kikan {

/*
 * You should not use these macros outside the appropriate functions.
 * Otherwise, make sure to delete the resulting vector or else it will cause a memory leak.
 * Since this is pretty obscured, it is no recommended
*/
#define sig(x) Kikan::TypeRegistry::getSignature<x>()

// used to query for Entities
#define getSig(x) new std::vector<unsigned int>({ sig(x) })
#define getSigs(args...) new std::vector<unsigned int>({ args })

// used to define Signatures in System
#define include(x) includeSignatures(new std::vector<unsigned int>({ x }))
#define includeAnd(args...) includeSignatures(new std::vector<unsigned int>({ args }))
#define includeSingle(x) include(sig(x))

    class TypeRegistry{
    public:

        template<class T>
        static unsigned int getSignature(){
            std::string sig = typeid(T).name();

            if(_component_signatures.count(sig))
                return _component_signatures[sig];

            _component_signatures.insert(std::make_pair(sig, _last_signature++));
            return _last_signature - 1;
        }

    private:
        static std::map<std::string, unsigned int> _component_signatures;
        static int _last_signature;
    };
}

#endif //KIKAN_TYPE_REGISTRY_H
