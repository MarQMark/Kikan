#ifndef KIKAN_UI_H
#define KIKAN_UI_H

#include <vector>
#include "glm/glm.hpp"

#include "Kikan/ui/elements/IInteractable.h"
#include "Kikan/ui/elements/UINode.h"
#include "Kikan/input/Input.h"

namespace Kikan {
    class UI {
    public:
        UI();
        ~UI();

        float renderLayer = -0.5;

        void setDimensions(float width, float height);
        void setHeight(float height);
        float getHeight() const;

        /*
         * Using this is discouraged if not strictly necessary
         * For scaling the width and height please use setDimensions()
         *
         * If this is used the aspect ratio will no longer be maintained
         */
        void setMVP(glm::mat4x4 mvp);
        glm::mat4x4 getMVP();
        void resetMVP();
        glm::vec2 getUIMousePos();

        void addElement(IUIElement* element, UINode* parent = nullptr);
        void addNode(UINode* node, UINode* parent = nullptr);
        IUIElement* getElement(const std::string& name);
        UINode* getNode(const std::string& name);
        UINode* getNode(IUIElement* element);
        /* void removeElement(IUIElement* element)
         *
         * Removes element from tree
         *
         * Does not delete object
         * Use deleteElement() for this
         *
         * If the same elements is used multiple times,
         * only clears first occurrence in tree.
         * (Not recommended)
         * */
        void removeElement(IUIElement* element);
        /* deleteElement(IUIElement* element);
         *
         * Removes element from tree
         * and deletes object
         *
         * If the same elements is used multiple times,
         * only clears first occurrence in tree.
         * (Not recommended)
         * */
        void deleteElement(IUIElement* element);
        /* void removeNode(UINode* node)
         *
         * Remove node from tree
         * Does not delete object
         * Use deleteNode() for this
         *
         * node cannot contain other nodes or elements
         */
        void removeNode(UINode* node);
        /* void deleteNode(UINode* node)
         *
         * Removes node from tree
         * and deletes object
         *
         * node cannot contain other nodes or elements
         */
        void deleteNode(UINode* node);

        glm::vec2 getPos(IUIElement* element);
        glm::vec2 getPos(UINode* node);

        void update();
    private:
        //std::vector<IInteractable*> _interactables;
        //std::vector<IUIElement*> _ui_elements;
        UINode* _root{};
        std::map<std::string, UINode*> _nodes;
        std::map<std::string, IUIElement*> _elements;

        IInteractable* _focused = nullptr;
        UINode* _focused_node = nullptr;

        float _width, _height;
        glm::mat4x4 _mvp{};
        glm::mat4x4 _i_mvp{};
        bool _custom_mvp = false;
        glm::vec2 _ui_mouse;

        bool _prev_left_click = false;

        bool _enter_pressed = false;
        bool _next_pressed = false;
        void focus_update(Input* input);
        bool focus_first(UINode* node);
        void focus_next(UINode* node);
        void focus_set(IInteractable* interactable, UINode* node);

        bool is_node_enabled(UINode* node);

        UINode* get_node(IUIElement* element, UINode* node);

        void rec_update(UINode* node, bool leftClick);
        void get_interactable(UINode* node, std::vector<IInteractable*>& interactables);
        void update_interactable(bool leftClick);
    };
}



#endif //KIKAN_UI_H
