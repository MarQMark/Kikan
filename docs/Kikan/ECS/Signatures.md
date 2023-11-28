
Every Component gets a unique signature. This is used to add Entities into the correct Systems.
It can also be used to query for Entities

**The following macros should not be used elsewhere as they will create a vector, which when not dealt with correctly will lead to a memory leak**
## Define System Signatures

System Signatures are usually defined in the Constructor of the System.
The simplest way to define a Signature is to used the `includeSingle()` macro.
This will add a single Signature to a System. 

For example:
```c++
PhysicsSystem::PhysicsSystem() {  
    includeSingle(Kikan::Physics);  
}
```
Every Entity containing the `Kikan::Physics` Component will be added to this system

If you want to only include Entities with `Kikan::Physics` **AND** `Kikan::LineQuadSprite` you can use the macro `includeAND()`. This would look like the following:
```C++
PhysicsSystem::PhysicsSystem() {  
    includeAnd(sig(Kikan::Physics), sig(Kikan::LineQuadSprite));  
}
```
Notice that the Class has to be surrounded by the `sig()` macro when using `includeAnd()`.
Also you can add more than two components with `includeAnd()`.

Finally if you want to include all Entities with `Kikan::Physics` **AND** `Kikan::LineQuadSprite` **OR** `Kikan::PolygonSprite`, you can simply multiple macros:
```c++
PhysicsSystem::PhysicsSystem() {  
    includeAnd(sig(Kikan::Physics), sig(Kikan::LineQuadSprite));  
    includeSingle(Kikan::PolygonSprite);
}
```


## Query Entities

There are two ways to query for Entities:
- `getEntity(SIGNATURES)` which returns the first Entity with the matching Component Signatures
- `getEntities(SIGNATURES, std::vector<Kikan::Entity*>*)` which adds all Entities with matching Components to the provided vector

Every Scene as well as the whole ECS can be queried.
**When querying withing a Scene it only checks Entities within that Scene**

To define the Signatures the macros `getSig()` and `getSigs()` can be used.
With `getSig()` you can only specify one Component, similar to `includeSingle()`.
And analogous to `includeAnd()`, `getSigs()` provides the capabilities for a logical AND.

Currently there is **no** option available for a logical OR.

Example:
The following code queries for all Entities with `Kikan::Physics` and `Kikan::LineQuadSprite` from the currently active Scene and puts them into the vector called entities
```c++
std::vector<Kikan::Entity*> entities;
engine->getECS()->getScene()->getEntities(
	getSigs(
		sig(Kikan::Physics), 
		sig(Kikan::LineQuadSprite)
	),
	&entities
);
```
