/** 
 * math.h
 * created 08/23/24 by frank collebrusco 
 */
#ifndef MATH_H
#define MATH_H
#include <flgl/glm.h>

glm::vec2 rotate_around_origin(glm::vec2 pt, float deg);
glm::vec2 rotate_around_pt(glm::vec2 pt, glm::vec2 c, float deg);
glm::vec3 rotate_around_origin(glm::vec3 pt, float deg);
glm::vec3 rotate_around_pt(glm::vec3 pt, glm::vec2 c, float deg);

/** closest vec from Vi -> Vt considering crossing 0 <-> max */
float circle_diff(float Vt, float Vi, float max = 360);
float posimod(float x, float min=0.f, float max=359.999999f);

template <typename T>
T amin(T a, T b) {
    return glm::abs(a) < glm::abs(b) ? a : b;
}

/**
 * step float or vec 'cur' 'wt'/1 of the way to 'tar'
 */
template <typename R>
R inline step_proportion(R cur, R tar, float wt) {
    return cur + ((tar - cur) * wt);
}

/**
 * R must be float or float vec
 * lowpass filter (push vals, get rolling avg of last 2^l2size)
 */
template <typename R, uint16_t l2size>
struct lowpass {
    inline void push(R v) {buf[(top++)&((1<<l2size)-1)] = v; if (top&(~((1<<l2size)-1))) fll=1;}
    inline R get() const {R r{0}; uint32_t n = fll ? (1<<l2size) : top; for (uint16_t i=0; i<n; i++) {r+=buf[i];} return r/((float)(1<<l2size));}
private:
    R buf[1<<l2size];
    uint16_t fll{0};
    uint16_t top{0};
};

/** PI loop style approach to target */
template <typename R, uint16_t f = 60>
struct pi_value {
    void step(float Kp, float Ki, float dt) {
        const float fs = dt * (float)f;
        R e = tar - val;
        R p = (fs * Kp) * e;
        i = i + ((fs * Ki) * e);
        val = p + i;
    }
    R tar, val;
private:
    R i;
};

template <typename R, uint16_t f = 60>
struct piv_value {
    void step(float Kp, float Ki, float dt) {
        const float fs = dt * (float)f;
        R e = tar - val;
        R p = (fs * Kp) * e;
        i = i + ((fs * Ki) * e);
        val = val + (p + i);
    }
    R tar, val;
private:
    R i;
};

template <uint16_t f = 60>
struct apiv_value {
    void step(float Kp, float Ki, float dt) {
        const float fs = dt * (float)f;
        float e = circle_diff(tar, val);
        float p = (fs * Kp) * e;
        i = i + ((fs * Ki) * e);
        val = val + (p + i);
    }
    float tar, val;
private:
    float i;
};

template <uint16_t f = 60>
struct alpiv_value {
    void step(float Kp, float Ki, float Kl, float dt) {
        const float fs = dt * (float)f;
        float e = circle_diff(tar, val);
        float p = (fs * Kp) * e;
        i = i + ((fs * Ki) * e);
        lp.push(val + (p + i));
        val = ((1.f-Kl)*val) + (Kl*lp.get());
    }
    float tar, val;
private:
    lowpass<float, 3> lp;
    float i;
};

/** PI loop (on 1st deriv) style approach to target */
template <typename R, uint16_t f = 60>
struct lpiv_value {
    void step(float Kp, float Ki, float Kl, float dt) {
        const float fs = dt * (float)f;
        R e = tar - val;
        R p = (fs * Kp) * e;
        i = i + ((fs * Ki) * e);
        lp.push(val + (p + i));
        val = ((1.f-Kl)*val) + (Kl*lp.get());
    }
    R tar, val;
private:
    lowpass<R, 3> lp;
    R i;
};

/** value accelerates (y'' = Ka * err) toward targ */
template <typename R, uint16_t f = 60>
struct acc_value {
    void step(float Ka, float dt) {
        const float fs = dt * (float)f;
        R e = tar - val;
        v = v + ((fs * Ka) * e);
        val = val + v;
    }
    R tar, val;
private:
    R v;
};

/** value accelerates (y'' = Ka * err) toward targ */
template <typename R, uint16_t f = 60>
struct av_value {
    void step(float Ka, float dt) {
        const float fs = dt * (float)f;
        R e = tar - val;
        v = v + ((fs * Ka) * e);
        val = val + v;
    }
    R tar, val;
private:
    R v;
};

#endif /* MATH_H */
