#include "Kikan/ecs/systems/PhysicsSystem.h"
#include "Kikan/ecs/components/Physics.h"

namespace Kikan {
    PhysicsSystem::PhysicsSystem() {
        singleInclude(Physics);
    }

    void PhysicsSystem::update(double dt) {
        for (auto* e : _entities)
        {
            auto* transform = e->getComponent<Transform>();
            auto* physics = e->getComponent<Physics>();

            // TODO: Add stop when collided
            /*if (collider != null)
            {
                // If Collided, stop player in that axis
                if ((collider.HasCollidedBottom && physics.Velocity.Y < 0.0f) ||
                    (collider.HasCollidedTop && physics.Velocity.Y > 0.0f))
                    physics.Velocity = new Vector2(physics.Velocity.X, 0.0f);

                if ((collider.HasCollidedLeft && physics.Velocity.X < 0.0f) ||
                    (collider.HasCollidedRight && physics.Velocity.X > 0.0f))
                    physics.Velocity = new Vector2(0.0f, physics.Velocity.Y);
            }*/

            // Add Gravity to acceleration
            physics->acceleration += glm::vec2(0.0f, gravity);

            // Peak downward acceleration
            if (physics->velocity.y <= 0)
                physics->acceleration += glm::vec2 (0.0f, 2.0f * gravity);

            // Friction
            physics->acceleration += glm::vec2(physics->friction.x * physics->velocity.y * physics->friction.x, 0);

            // Physics calculation
            glm::vec2 oldVelocity = physics->velocity;
            physics->velocity += physics->acceleration * (float)dt;
            transform->position += glm::vec3(0.5f * (oldVelocity + physics->velocity) * (float)dt, transform->position.z);

            physics->acceleration = glm::vec2(0);
        }
    }
}

