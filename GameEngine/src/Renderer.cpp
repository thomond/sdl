#include "Renderer.hpp"

// Static definitions:
Renderer* Renderer::instance = nullptr;
std::list<IRenderable*> Renderer::renderList;