#include "Batch.h"

void Batch::render() {
    glDrawElements(GL_TRIANGLES, _last_vertex, GL_UNSIGNED_SHORT, 0);
}
