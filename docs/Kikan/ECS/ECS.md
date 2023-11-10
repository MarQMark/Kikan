Kikan allows the use of ECS

Overall Architecture:

ECS.cpp
- systems
	- entities
- add/remote entities
- scenes
	- add/remove entities
	- systems
		- entities

Adding an Entity to the main ECS will not add it to any scene.
Removing entities from the main ECS does not remove them from the scene systems.
Deleting however is recursive.



