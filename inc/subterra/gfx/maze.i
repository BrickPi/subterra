#include "cglm/cglm.h"

vec3 Xwalls[41] = {
    -3,0.075,1.75,

    -2,0.075,1.75,
    -2,0.075,-0.75,

    -1,0.075,1.75,
    -1,0.075,-0.75,

    0,0.075,1.75,
    0,0.075,-0.75,

    1,0.075,1.75,
    1,0.075,-0.75,

    2,0.075,1.75,
    2,0.075,-0.75,

    3,0.075,-8.25,

    2,0.075,-8.25,
    2,0.075,-5.75,

    1,0.075,-8.25,

    1,0.075,-3.25,

    0,0.075,-3.25,
    0,0.075,-5.75,

    -1,0.075,-3.25,

    -2,0.075,-3.25,
    -2,0.075,-5.75,

    -3,0.075,-5.75,

    -1,0.075,-10.75,

    0,0.075,-8.25,
    0,0.075,-10.75,

    -4,0.075,-5.75,
    -4,0.075,-3.25,

    -5,0.075,-3.25,

    -6,0.075,-5.75,

    -6,0.075,-0.75,

    -5,0.075,-15.75,

    -4,0.075,-15.75,
    -4,0.075,-13.25,

    -3,0.075,-15.75,
    -3,0.075,-13.25,

    -2,0.075,-15.75,
    -2,0.075,-13.25,

    -1,0.075,-15.75,
    -1,0.075,-13.25,

    0,0.075,-13.25,

    0,0.075,-18.25
};

vec3 Zwalls[31] = {
    0,0.075,9.25,
    0,0.075,-8.25,

    -1,0.075,-5.75,
    -1,0.075,-8.25,

    -1,0.075,6.75,
    -1,0.075,9.25,

    1,0.075,-5.75,
    1,0.075,-8.25,

    2,0.075,-5.75,
    2,0.075,-8.25,

    -2,0.075,-5.75,
    -2,0.075,-8.25,

    -3,0.075,-8.25,

    -3,0.075,-0.75,

    -2,0.075,-3.25,
    
    -3,0.075,1.75,
    -3,0.075,4.25,

    -4,0.075,4.25,

    -4,0.075,-0.75,
    
    -2,0.075,16.75,

    -1,0.075,16.75,
    -1,0.075,14.25,

    -3,0.075,14.25,
    -3,0.075,11.75,

    -4,0.075,14.25,
    -4,0.075,11.75,

    -5,0.075,14.25,
    -5,0.075,11.75,

    -6,0.075,14.25,

    -7,0.075,1.75,
    -7,0.075,-0.75
};

vec3 floors[37] = {
    -7,0,2.5,
    -6,0,2.5,
    -6,-5,2.5,
    -6,-4,2.5,
    -6,-3,2.5,
    -6,-2,2.5,
    -6,-1,2.5,
    -5,-5,2.5,
    -4,-5,2.5,
    -4,-1,2.5,
    -4,0,2.5,
    -3,-1,2.5,
    -3,3,2.5,
    -3,2,2.5,
    -3,1,2.5,
    -3,-5,2.5,
    -2,3,2.5,
    -2,1,2.5,
    -2,0,2.5,
    -2,-1,2.5,
    -2,-2,2.5,
    -2,-3,2.5,
    -2,-4,2.5,
    -2,-5,2.5,
    -2,-6,2.5,
    -1,-3,2.5,
    -1,3,2.5,
    -1,-6,2.5,
    0,-3,2.5,
    0,-2,2.5,
    0,-1,2.5,
    0,0,2.5,
    0,1,2.5,
    0,2,2.5,
    0,3,2.5,
    1,3,2.5,
    2,3,2.5,
};