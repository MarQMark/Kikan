#include "Batch.h"

void Batch::render() {
    glDrawElements(GL_TRIANGLES, _last_vertex, GL_UNSIGNED_SHORT, 0);
}

void Batch::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
