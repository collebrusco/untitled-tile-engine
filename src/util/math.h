/** 
 * math.h
 * created 08/23/24 by frank collebrusco 
 */
#ifndef MATH_H
#define MATH_H
#include <flgl/glm.h>

glm::vec2 rotate_around_origin(glm::vec2 pt, float deg);

glm::vec2 rotate_around_pt(glm::vec2 pt, glm::vec2 c, float deg);

/**
 * step float or vec 'cur' 'wt'/1 of the way to 'tar'
 */
template <typename R>
R inline step_proportion(R cur, R tar, float wt) {
    return cur + ((tar - cur) * wt);
}

/**
 * lowpass filter (push vals, get rolling avg of last 2^l2size)
 */
template <uint16_t l2size>
struct lowpass {
    inline void push(float v) {buf[(top++)&((1<<l2size)-1)] = v; if (top&(~((1<<l2size)-1))) fll=1;}
    inline float get() const {float r=0; uint32_t n = fll ? (1<<l2size) : top; for (uint16_t i=0; i<n; i++) {r+=buf[i];} return r/((float)(1<<l2size));}
private:
    float buf[1<<l2size];
    uint16_t fll{0};
    uint16_t top{0};
};

#endif /* MATH_H */
