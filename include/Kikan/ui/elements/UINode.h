#ifndef KIKAN_UINODE_H
#define KIKAN_UINODE_H

#include "glm/glm.hpp"
#include "Kikan/ui/elements/IUIElement.h"

namespace Kikan{
    class UINode {
    public:
        UINode(std::string name) : _name(std::move(name)) {}
        ~UINode() = default;

        UINode* parent = {};
        std::vector<IUIElement*> elements;
        std::vector<UINode*> nodes;

        bool enabled = true;

        glm::vec2 pos = glm::vec2(0,0);

        std::string getName(){
            return _name;
        }

        void render(glm::vec2 parentPos){
            for (auto e : elements) {
                if(e->enabled){
                    e->update();
                    e->render(pos + parentPos);
                }
            }

            for(auto n : nodes){
                if(n->enabled)
                    n->render(pos + parentPos);
            }
        }

    private:
        std::string _name;
    };
}

#endif //KIKAN_UINODE_H
