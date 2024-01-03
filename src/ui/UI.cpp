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
        _mvp = glm::scale(_mvp, glm::vec3(1.f/width, 1.f/height, 1.0f));
    }

    void UI::setMVP(glm::mat4x4 mvp) {
        _mvp = mvp;
    }

    void UI::addElement(IUIElement *element) {
        _ui_elements.push_back(element);

        auto* interactable = dynamic_cast<IInteractable*>(element);
        if(interactable)
            _interactables.push_back(interactable);
    }

    bool isInside(glm::vec2 p, glm::vec2 pos, glm::vec2 dim){
        return (p.x > pos.x && p.x < pos.x + dim.x && p.y > pos.y && p.y < pos.y + dim.y);
    }

    void UI::update() {
        render();

        auto* input = Engine::Kikan()->getInput();
        double mouseX =  2 * input->mouseX() / ((StdRenderer*)Engine::Kikan()->getRenderer())->getWidth()  - 1;
        double mouseY =  2 * input->mouseY() / ((StdRenderer*)Engine::Kikan()->getRenderer())->getHeight() + 1;

        glm::vec4 mouse = glm::vec4(mouseX, mouseY, 1, 1);
        mouse = glm::inverse(_mvp) * mouse;

        // TODO: Fix this mess
        for(auto* interactable : _interactables){
            if(isInside(mouse, interactable->pos, interactable->dim)){
                if(input->mousePressed(Mouse::BUTTON_LEFT)){
                    if(interactable->getState() == IInteractable::State::PRESSED)
                        interactable->changeState(IInteractable::State::HELD);
                    else
                        interactable->changeState(IInteractable::State::PRESSED);
                }
                else
                    interactable->changeState(IInteractable::State::HOVERED);
            }
            else{
                interactable->changeState(IInteractable::State::NONE);
            }

            if(interactable->getState() == IInteractable::State::PRESSED)
                kikanPrint("%d \n", interactable->getState());
        }

        //kikanPrint("x: %f  y: %f\n", mouse.x, mouse.y);
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
        //TODO: Fix matrix
        _mvp = glm::mat4x4(1.0f);
        _mvp = glm::translate(_mvp, glm::vec3(-1, 1, 0));
        _mvp = glm::scale(_mvp, glm::vec3(2.f, 2.f, 1.0f));
    }
}
