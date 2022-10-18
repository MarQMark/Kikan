#ifndef KIKAN_ISYSTEM_H
#define KIKAN_ISYSTEM_H

#define includeANDStart includeSignatures(new std::vector<std::string>({
#define includeANDEnd }))

#include "typeinfo"
#include "string"
#include "vector"
#include "../Entity.h"

class ISystem{
public:
    ~ISystem(){
        for (auto signatures : _signatures) {
            delete signatures;
        }
    }

    virtual void update(double dt) = 0;

    void addEntity(Entity* entity){
        _entities.push_back(entity);
    }

    void removeEntity(Entity* entity){
        for (int i = 0; i < _entities.size(); ++i) {
            if(_entities.at(i) == entity){
                _entities.erase(_entities.begin() + i);
                return;
            }
        }
    }

protected:
    template<class T>
    std::string signature(){
        return typeid(T).name();
    }

    void includeSignatures(std::vector<std::string>* signatures){
        _signatures.push_back(signatures);
    }

private:
    std::vector<Entity*> _entities;
    std::vector<std::vector<std::string>*> _signatures;
};

#endif //KIKAN_ISYSTEM_H
