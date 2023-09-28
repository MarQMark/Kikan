#ifndef KIKAN_MATHS_H
#define KIKAN_MATHS_H

#include <math.h>

namespace Kikan { namespace Core {
    union vec2{
         struct {
             float x;
             float y;
         };

         float len(){
             return sqrt(x * x + y * y);
         }

         vec2 operator+(vec2 const& vec){
             vec2 ret;
             ret.x = x + vec.x;
             ret.y = y + vec.y;
             return ret;
         }
        vec2 operator-(vec2 const& vec){
            vec2 ret;
            ret.x = x - vec.x;
            ret.y = y - vec.y;
            return ret;
        }
    };

    union vec3{
        struct {
            float x;
            float y;
            float z;
        };
        struct {
            float r;
            float g;
            float b;
        };

        float len(){
            return sqrt(x * x + y * y);
        }
    };

    union vec4{
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        struct {
            float r;
            float g;
            float b;
            float a;
        };

        float len(){
            return sqrt(x * x + y * y);
        }
    };

}}

#endif //KIKAN_MATHS_H
