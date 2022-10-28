#ifndef KIKAN_VERTEX_BUFFER_LAYOUT_H
#define KIKAN_VERTEX_BUFFER_LAYOUT_H

#include <vector>
#include "GL/glew.h"

struct VertexBufferElement{
    unsigned int type;
    unsigned int count;
    bool normalized;
};

template<typename T>
struct always_false : std::false_type
{ };

class VertexBufferLayout{
public:
    VertexBufferLayout() = default;

    template<typename T>
    void add(unsigned int count){
        static_assert(always_false<T>::value);
    }

    template<>
    void add<float>(unsigned int count){
        _elements.push_back({GL_FLOAT, count, false});
        _stride += sizeof(GLfloat);
    }

    template<>
    void add<unsigned int>(unsigned int count){
        _elements.push_back({GL_UNSIGNED_INT, count, false});
        _stride += sizeof(GLuint);
    }

    template<>
    void add<unsigned char>(unsigned int count){
        _elements.push_back({GL_UNSIGNED_BYTE, count, true});
        _stride += sizeof(GLbyte);
    }

    inline unsigned int getStride() const {return _stride;}
    inline std::vector<VertexBufferElement> getElements() const& {return _elements;}

private:
    std::vector<VertexBufferElement> _elements;
    unsigned int _stride;
};

#endif //KIKAN_VERTEX_BUFFER_LAYOUT_H
