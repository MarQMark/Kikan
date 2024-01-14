#include "Kikan/ui/UI.h"
#include "glm/ext/matrix_transform.hpp"
#include "Kikan/Engine.h"

namespace Kikan {
    UI::UI() {
        _root = new UINode("root");
        _nodes[_root->getName()] = _root;
        _width = 720;
        _height = 1280;
        resetMVP();
    }

    UI::~UI() {
        for(const auto& n : _nodes)
            delete n.second;
        for(const auto& e : _elements)
            e.second->destroy();
    }

    void UI::setDimensions(float width, float height) {
        if(_width == width && _height == height)
            return;

        resetMVP();
        _mvp = glm::scale(_mvp, glm::vec3(1.f/width, 1.f/height, 1.0f));
        _i_mvp = glm::inverse(_mvp);
        _width = width;
        _height = height;
    }

    void UI::setMVP(glm::mat4x4 mvp) {
        _custom_mvp = true;
        _mvp = mvp;
        _i_mvp = glm::inverse(_mvp);
    }

    void UI::update() {
        auto renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        auto* input = Engine::Kikan()->getInput();

        focus_update(input);

        if(!_custom_mvp)
            setDimensions((float)renderer->getWidth() / (float)renderer->getHeight() * _height, _height);

        if(_root->enabled)
            _root->render(glm::vec2(0));

        double mouseX =   2 * input->mouseX() / renderer->getWidth()  - 1;
        double mouseY =  -2 * input->mouseY() / renderer->getHeight() + 1;
        glm::vec4 mouse = glm::vec4(mouseX, mouseY, 1, 1);
        mouse = _i_mvp * mouse;
        _ui_mouse = mouse;

        bool leftClick = input->mousePressed(Mouse::BUTTON_LEFT);

        if(_root->enabled)
            rec_update(_root, leftClick);

        _prev_left_click = leftClick;
    }

    bool isInside(glm::vec2 p, glm::vec2 pos, glm::vec2 dim){
        return (p.x > pos.x && p.x < pos.x + dim.x && p.y < pos.y && p.y > pos.y - dim.y);
    }

    void UI::rec_update(UINode *node, bool leftClick) {
        for(auto* element : node->elements){
            if(!element->enabled)
                continue;

            auto* interactable = dynamic_cast<IInteractable*>(element);
            if(!interactable)
                continue;

            if(!interactable->interactable)
                continue;

            if(_focused == interactable && _enter_pressed)
                continue;

            if(isInside(_ui_mouse, interactable->pos, interactable->dim)){

                if(leftClick && _prev_left_click) {
                    interactable->changeState(IInteractable::State::HELD);
                }
                else if(leftClick) {
                    interactable->changeState(IInteractable::State::PRESSED);
                    focus_set(interactable, node);
                    kikanPrint("test\n");
                }
                else if(_prev_left_click){
                    interactable->changeState(IInteractable::State::RELEASED);
                }
                else{
                    interactable->changeState(IInteractable::State::HOVERED);
                }

            }
            else{
                interactable->changeState(IInteractable::State::NONE);
            }
        }

        for(auto* n : node->nodes){
            if(n->enabled)
                rec_update(n, leftClick);
        }
    }

    glm::mat4x4 UI::getMVP() {
        return _mvp;
    }

    void UI::resetMVP() {
        _mvp = glm::mat4x4(1.0f);
        _mvp = glm::translate(_mvp, glm::vec3(-1, -1, 0));
        _mvp = glm::scale(_mvp, glm::vec3(2.f, 2.f, 1.0f));
        _i_mvp = glm::inverse(_mvp);
    }

    void UI::setHeight(float height) {
        _height = height;
    }
    float UI::getHeight() const {
        return _height;
    }

    void UI::addElement(IUIElement *element, UINode *parent) {
        if(_elements.count(element->getName())){
            kikanPrintE("[ERROR] UI element with name %s already exists\n", element->getName().c_str());
            return;
        }

        if(!parent)
            parent = _root;
        parent->elements.push_back(element);
        _elements[element->getName()] = element;
    }

    void UI::addNode(UINode *node, UINode *parent) {
        if(_nodes.count(node->getName())){
            kikanPrintE("[ERROR] UI node with name %s already exists\n", node->getName().c_str());
            return;
        }

        if(!parent)
            parent = _root;
        parent->nodes.push_back(node);
        node->parent = parent;
        _nodes[node->getName()] = node;
    }

    IUIElement *UI::getElement(const std::string& name) {
        if(!_elements.count(name)){
            kikanPrintE("[ERROR] No UI element with name %s\n", name.c_str());
            return nullptr;
        }

        return _elements[name];
    }

    UINode *UI::getNode(const std::string& name) {
        if(!_nodes.count(name)){
            kikanPrintE("[ERROR] No UI node with name %s\n", name.c_str());
            return nullptr;
        }

        return _nodes[name];
    }

    UINode *UI::getNode(IUIElement *element) {
        return get_node(element, _root);
    }

    UINode *UI::get_node(IUIElement *element, UINode *node) {
        if(!node)
            return nullptr;

        for (auto* e : node->elements) {
            if(e == element)
                return node;
        }

        for (auto* n : node->nodes){
            auto ret = get_node(element, n);
            if(ret)
                return ret;
        }

        return nullptr;
    }

    void UI::removeElement(IUIElement *element) {
        if(!element)
            return;

        if(element == _focused)
            focus_set(nullptr, nullptr);

        auto* node = getNode(element);
        if(node)
            node->elements.erase(std::remove(node->elements.begin(), node->elements.end(), element), node->elements.end());
        _elements[element->getName()] = nullptr;
    }

    void UI::deleteElement(IUIElement *element) {
        removeElement(element);
        delete element;
    }

    void UI::removeNode(UINode *node) {
        if(!node)
            return;

        if(!node->elements.empty()){
            kikanPrintE("[ERROR] Cannot delete node %s. It still contains elements\n", node->getName().c_str());
            return;
        }
        if(!node->nodes.empty()){
            kikanPrintE("[ERROR] Cannot delete node %s. It still child nodes elements\n", node->getName().c_str());
            return;
        }

        if(node->parent){
            node->parent->nodes.erase(std::remove(node->parent->nodes.begin(), node->parent->nodes.end(), node), node->parent->nodes.end());
        }

        _nodes[node->getName()] = nullptr;
    }

    void UI::deleteNode(UINode *node) {
        removeNode(node);
        delete node;
    }

    void UI::focus_update(Input *input) {
        if(_focused){
            if(!_focused_node){
                kikanPrintE("[ERROR] UI interactable selected but no node. This should not happen\n");
            }
            else if(!is_node_enabled(_focused_node)){
                focus_first(_root);
            }
            else if(_focused->interactable){
                if(input->keyPressed(Key::ENTER) && _enter_pressed){
                    _focused->changeState(IInteractable::State::HELD);
                }
                else if(input->keyPressed(Key::ENTER)){
                    _focused->changeState(IInteractable::State::PRESSED);
                }
                else if(_enter_pressed){
                    _focused->changeState(IInteractable::State::RELEASED);
                }
            }
        }

        if(_next_pressed && !input->keyPressed(Key::TAB)){
            if(!_focused_node)
                _focused_node = _root;
            if(!_focused)
                focus_first(_root);
            else
                focus_next(_focused_node);
        }

        _enter_pressed = input->keyPressed(Key::ENTER);
        _next_pressed = input->keyPressed(Key::TAB);
    }

    bool UI::focus_first(UINode* node) {
        if(!node || !node->enabled)
            return false;

        for (auto* e : node->elements) {
            auto* interactable = dynamic_cast<IInteractable*>(e);
            if(interactable && interactable->enabled){
                focus_set(interactable, node);
                return true;
            }
        }

        for(auto* n : node->nodes){
            if(focus_first(n))
                return true;
        }

        return false;
    }

    void UI::focus_next(UINode *node) {
        if(node == _focused_node){
            if(!is_node_enabled(node)){
                focus_first(_root);
                return;
            }

            int index = -1;
            for (uint32_t i = 0; i < node->elements.size(); ++i) {
                if(_focused == node->elements[i]){
                    index = i;
                }
            }

            if(index == -1){
                focus_set(nullptr, nullptr);
                return;
            }

            for (uint32_t i = index + 1; i < node->elements.size(); ++i) {
                auto* interactable = dynamic_cast<IInteractable*>(node->elements[i]);
                if(interactable && interactable->enabled){
                    focus_set(interactable, node);
                    return;
                }
            }
        }

        for(auto* n : node->nodes){
            if(focus_first(n))
                return;
        }

        focus_first(_root);
    }

    void UI::focus_set(IInteractable *interactable, UINode* node) {
        if(_focused)
            _focused->focused = false;
        if(interactable && interactable->interactable)
            interactable->focused = true;
        _focused = interactable;
        _focused_node = node;
    }

    bool UI::is_node_enabled(UINode *node) {
        if(!node->enabled)
            return false;
        if(!node->parent)
            return node->enabled;

        return is_node_enabled(node->parent);
    }

    glm::vec2 UI::getUIMousePos() {
        return _ui_mouse;
    }

    // TODO: Add position manipulation to nodes (layouts)
    // TODO: Check if return of vec2 is possible
    glm::vec2 UI::getPos(IUIElement *element) {
        auto* node = getNode(element);
        glm::vec2 pos = element->pos;
        pos += getPos(node);
        return pos;
    }

    glm::vec2 UI::getPos(UINode *node) {
        glm::vec2 pos = node->pos;
        if(node->parent)
            pos += getPos(node->parent);
        return pos;
    }
}
