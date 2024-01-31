# Overview
Kikan::UI provides the components for a basic user interface. 

The following components are implemented:
- [[Label|Kikan::Label]]
- [[Button|Kikan::Button]]
- [[Textbox|Kikan::Textbox]]

A basic means to create layouts exists, but is not implemented.

The UI has its own MVP matrix  for rendering.
# Dependencies
The UI is completely separated from the [[ECS]]. However, it is heavily dependent on the Kikan::StdRenderer, especially the [[Textbox|Kikan::Textbox]], since it uses the auto-batching directly.

# Structure
The UI is build in a tree-like structure. Kikan::UI has a root [[UINode|Kikan::UINode]]. Each [[UINode|Kikan::UINode]] contains a std::vector with further  [[UINode|Kikan::UINodes]] and a vector with [[IUIElement|Kikan::IUIElements]].

A [[UINode|Kikan::UINode]] as well as a [[IUIElement|Kikan::IUIElement]] contains a flag *enabled*. If false the element will not be rendered or updated. If a [[UINode|Kikan::UINode]] is disabled all child nodes will also not be rendered and updated anymore, even if they are enabled. 

# Access
Each [[UINode|Kikan::UINode]] and [[IUIElement|Kikan::IUIElement]] must have a unique name. Through this name the element can be retrieved from a main std::map. For this to function properly, all [[UINode|Kikan::UINodes]] and [[IUIElement|Kikan::IUIElements]] have to be added to the UI via 
```c++
Kikan::UI::addElement(Kikan::IUIElement* element, Kikan::UINode* parent) 
```
or 
```c++
Kikan::UI::addNode(Kikan::UINode* node, Kikan::UINode* parent) 
```
if the parent is not set it will default to the UI  root node.

