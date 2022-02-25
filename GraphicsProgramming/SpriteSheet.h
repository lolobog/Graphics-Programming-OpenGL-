#pragma once
#include <glew.h>
#include <SDL_opengl.h>
#include<glm.hpp>
#include<gtc/type_ptr.hpp>

class SpriteSheet {
    int spriteWidth, spriteHeight;
    int texWidth, texHeight;

    int tex;

public:
    SpriteSheet(int t, int tW, int tH, int sW, int sH)
        : tex(t), texWidth(tW), texHeight(tH), spriteWidth(sW), spriteHeight(sH)
    {}

    void drawSprite(float posX, float posY, int frameIndex);
};