#ifndef KIKAN_IINTERACTABLE_H
#define KIKAN_IINTERACTABLE_H

#include "IUIElement.h"
#include "Kikan/core/Logging.h"
#include <vector>

namespace Kikan {
    class IInteractable : public IUIElement {
    public:
        IInteractable() : IUIElement() {
        }

        ~IInteractable(){
            for(auto* callback : _callbacks)
                delete callback;
        }

        enum State{
            NONE,
            HOVERED,
            PRESSED,
            HELD,
            RELEASED,

            ALL, // Only for callbacks
        };

        void changeState(State state){
            if(_prev_state == state)
                return;

            _prev_state = _state;
            _state = state;
            stateChange();
        }

        State getState(){
            return _state;
        }

        State getPrevState(){
            return _prev_state;
        }

        /*
         *  Register a new callback
         *
         *  callback:   Function pointer to callback
         *  state:      If other than ALL callback will only be called if state switches to the specified state
         *  data:       Option to pass data to the function, for example object which should be influenced by change (will not get freed by automatically)
         */
        uint32_t registerCallback(void(*callback)(IInteractable* interactable, void* data), State state = State::ALL, void* data = nullptr){
            auto* cb = new Callback();
            cb->callback = callback;
            cb->state = state;
            cb->data = data;
            _callbacks.push_back(cb);
            return _callbacks.size();
        }

        void unregisterCallback(uint32_t id){
            if(id > _callbacks.size()) {
                kikanPrintE("[ERROR] no callback with id: %d\n", id);
                return;
            }

            // TODO: Fix
            //std::remove(_callbacks.begin(), _callbacks.end(), _callbacks[id]);
        }

        void stateChange(){
            if(!_enabled)
                return;

            for(auto* callback : _callbacks){
                if(callback->state == State::ALL || callback->state == _state)
                    callback->callback(this, callback->data);
            }
        }

        void disable(){
            _enabled = false;
        }
        void enable(){
            _enabled = true;
        }

    private:
        State _state = State::NONE;
        State _prev_state = State::NONE;

        bool _enabled = true;

        struct Callback{
            void(*callback)(IInteractable* interactable, void* data);
            void* data;
            State state = State::ALL; // only call if state switches to this
        };

        std::vector<Callback*> _callbacks;
    };
}

#endif //KIKAN_IINTERACTABLE_H
