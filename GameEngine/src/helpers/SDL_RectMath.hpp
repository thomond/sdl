#pragma once
#include <SDL2/SDL.h>
#include <algorithm> // for std::max, std::min
// Author: ChatGPT

// --- Arithmetic Operators ---
inline SDL_Rect operator+(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_Rect{ a.x + b.x, a.y + b.y, a.w, a.h };
}

inline SDL_Rect operator-(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_Rect{ a.x - b.x, a.y - b.y, a.w, a.h };
}

inline SDL_Rect& operator+=(SDL_Rect& a, const SDL_Rect& b) {
    a.x += b.x; a.y += b.y;
    return a;
}

inline SDL_Rect& operator-=(SDL_Rect& a, const SDL_Rect& b) {
    a.x -= b.x; a.y -= b.y;
    return a;
}

inline SDL_Rect operator+(const SDL_Rect& a, const SDL_Point& b) {
    return SDL_Rect{ a.x + b.x, a.y + b.y, a.w, a.h };
}

inline SDL_Rect operator-(const SDL_Rect& a, const SDL_Point& b) {
    return SDL_Rect{ a.x - b.x, a.y - b.y, a.w, a.h };
}

inline SDL_Rect& operator+=(SDL_Rect& a, const SDL_Point& b) {
    a.x += b.x; a.y += b.y;
    return a;
}

inline SDL_Rect& operator-=(SDL_Rect& a, const SDL_Point& b) {
    a.x -= b.x; a.y -= b.y;
    return a;
}

inline bool operator==(SDL_Rect& a, const SDL_Rect& b) {
   return ((a.h == b.h)&&(a.w == b.w)&&(a.x == b.x)&&(a.y == b.y));
}



// --- Scalar Transformations ---
inline SDL_Rect operator*(const SDL_Rect& a, int scalar) {
    return SDL_Rect{ a.x * scalar, a.y * scalar, a.w * scalar, a.h * scalar };
}

inline SDL_Rect operator/(const SDL_Rect& a, int scalar) {
    return SDL_Rect{ a.x / scalar, a.y / scalar, a.w / scalar, a.h / scalar };
}

// --- Containment ---
inline bool contains(const SDL_Rect& outer, const SDL_Rect& inner) {
    return inner.x >= outer.x &&
           inner.y >= outer.y &&
           (inner.x + inner.w) <= (outer.x + outer.w) &&
           (inner.y + inner.h) <= (outer.y + outer.h);
}

// --- Intersection Check ---
inline bool intersects(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b) == SDL_TRUE;
}

// --- Intersection Result ---
inline SDL_Rect getIntersection(const SDL_Rect& a, const SDL_Rect& b) {
    SDL_Rect result;
    if (SDL_IntersectRect(&a, &b, &result))
        return result;
    return SDL_Rect{0, 0, 0, 0}; // No intersection
}

// --- Centering ---
inline SDL_Point getCenter(const SDL_Rect& r) {
    return SDL_Point{ r.x + r.w / 2, r.y + r.h / 2 };
}
