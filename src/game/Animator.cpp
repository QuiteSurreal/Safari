#include "Animator.hpp"

Animator::Animator() {

    accumulator = 0.0f;
    timeStep = 1.0f / 60.0f;
    every6FrameTimer = 0.0f;

    active = true;
    delay = 8;
    timerValue = delay;
    currentAnimation = "default";

    animations["default"] = texturesFromSources(std::vector<std::string>{ "test.png" });
    currentFrame["default"] = 0;
}

void Animator::addAnimation(std::string name, std::vector<std::string> frames) {
    animations[name] = texturesFromSources(frames);
    currentFrame[name] = 0;
}

bool Animator::removeAnimation(std::string name) {
    return animations.erase(name) > 0;
}

bool Animator::switchAnimation(std::string name) {
    if (animations.find(name) != animations.end()) {
        currentAnimation = name;
        return true;
    }
    return false;
}

void Animator::resetFrame() {
    currentFrame[currentAnimation] = 0;
}

void Animator::resetFrameTimer() {
    timerValue = delay;
}

std::vector<Texture> Animator::texturesFromSources(std::vector<std::string> sources) {
    
    std::vector<Texture> textures;
    
    for (const auto& src : sources) {
        const Texture texture = Texture::fromImage(src);
        textures.push_back(texture);
    }

    return textures;

}

void Animator::start(Entity& entity) {
    timerValue = delay;
}

void Animator::update(Entity& entity) {

    accumulator += Input::getDeltaTime();

    while (accumulator >= timeStep) {

        // <------ << Timers >> ------>

        accumulator -= timeStep;
        every6FrameTimer += timeStep;

        // >>--------<<

        if (!active)
            continue;

        timerValue -= 1.0f;

        if (timerValue < 0) {

            timerValue = delay;

            currentFrame[currentAnimation] += 1;

            if (currentFrame[currentAnimation] >= animations[currentAnimation].size()) {
                currentFrame[currentAnimation] = 0;
            }

            entity.getComponent<Sprite>().setTexture(animations[currentAnimation].at(currentFrame[currentAnimation]));

        }

    }

}
