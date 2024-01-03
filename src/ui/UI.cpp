#include "Kikan/ui/UI.h"
#include "glm/ext/matrix_transform.hpp"
#include "Kikan/Engine.h"

namespace Kikan {
    UI::UI() {
        resetMVP();
    }

    UI::~UI() {

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

    void UI::addElement(IUIElement *element) {
        _ui_elements.push_back(element);

        auto* interactable = dynamic_cast<IInteractable*>(element);
        if(interactable)
            _interactables.push_back(interactable);
    }

    bool isInside(glm::vec2 p, glm::vec2 pos, glm::vec2 dim){
        return (p.x > pos.x && p.x < pos.x + dim.x && p.y < pos.y && p.y > pos.y - dim.y);
    }

    void UI::update() {
        auto renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        auto* input = Engine::Kikan()->getInput();

        if(!_custom_mvp)
            setDimensions((float)renderer->getWidth() / (float)renderer->getHeight() * _height, _height);

        render();

        double mouseX =   2 * input->mouseX() / renderer->getWidth()  - 1;
        double mouseY =  -2 * input->mouseY() / renderer->getHeight() + 1;

        glm::vec4 mouse = glm::vec4(mouseX, mouseY, 1, 1);
        mouse = _i_mvp * mouse;

        bool leftClick = input->mousePressed(Mouse::BUTTON_LEFT);


        for(auto* interactable : _interactables){
            if(isInside(mouse, interactable->pos, interactable->dim)){

                if(leftClick && _prev_left_click) {
                    interactable->changeState(IInteractable::State::HELD);
                }
                else if(leftClick) {
                    interactable->changeState(IInteractable::State::PRESSED);
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

        _prev_left_click = leftClick;
    }

    void UI::render() {
        for (auto* e : _ui_elements) {
            e->render();
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
}
