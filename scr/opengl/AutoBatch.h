#ifndef KIKAN_AUTO_BATCH_H
#define KIKAN_AUTO_BATCH_H

#include "Batch.h"

class AutoBatch : Batch{
public:
     explicit AutoBatch(BatchType type);
    ~AutoBatch();

    template<class T>
    int addVertices(std::vector<IVertex> vertices);

    void bind() override;
    void render() override;

private:
    int _max_texture_units = 4;
    GLuint* _texture_slots;
    unsigned int _last_slot = 0;
    int find_texture(GLuint texture);

    std::vector<IVertex*> _vertices;
};


#endif //KIKAN_AUTO_BATCH_H
