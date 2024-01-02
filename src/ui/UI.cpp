#include "Kikan/ui/UI.h"
#include "glm/ext/matrix_transform.hpp"
#include "Kikan/Engine.h"

namespace Kikan {
    UI::UI() {

    }

    UI::~UI() {

    }

    void UI::setDimensions(uint32_t width, uint32_t height) {
        _mvp = glm::scale(_mvp, glm::vec3(width, height, 1.0f));
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

        double mouseX =  2 * Engine::Kikan()->getInput()->mouseX() / ((StdRenderer*)Engine::Kikan()->getRenderer())->getWidth()  - 1;
        double mouseY = -2 * Engine::Kikan()->getInput()->mouseY() / ((StdRenderer*)Engine::Kikan()->getRenderer())->getHeight() + 1;

        glm::vec4 mouse = glm::vec4(mouseX, mouseY, 1, 1);
        mouse = glm::inverse(_mvp) * mouse;

        for(auto* interactable : _interactables){
            if(isInside(mouse, interactable->pos, interactable->dim)){
                interactable->changeState(IInteractable::State::HOVERED);
            }
            else{
                interactable->changeState(IInteractable::State::NONE);
            }
        }

        kikanPrint("x: %f  y: %f\n", mouseX, mouseY);
    }

    void UI::render() {
        for (auto* e : _ui_elements) {
            e->render();
        }
    }
}