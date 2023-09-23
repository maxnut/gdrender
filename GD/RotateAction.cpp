#include "GameLayer.h"
#include "RotateAction.h"

void RotateAction::stop()
{
    ActionInterval::stop();

    for (auto pair : targetGroup->objects)
    {
        for (auto pair2 : pair.second)
        {
            auto obj = pair2.second;
            sf::Vector2f move = obj->startPosition;

            for (int i : obj->groups)
                move += GameLayer::instance->groups[i]->moveTotal;

            obj->rotateOffsetMovement = obj->getPosition() - move;
            obj->rotateOffset += targetGroup->rotateTotal;
        }
    }

    targetGroup->rotateAround = nullptr;
    targetGroup->rotateTotal = 0;
    targetGroup->rotateTotalMovement = 0;
}

std::shared_ptr<RotateAction> RotateAction::create(float duration, int target, int center, float rotation, bool lock)
{
    std::shared_ptr<RotateAction> ptr(new RotateAction);

    if (ptr->init(duration, target, center, rotation, lock))
        return ptr;

    return nullptr;
}

bool RotateAction::init(float duration, int target, int center, float rotation, bool lock)
{
    ActionInterval::init(duration);

    this->targetGroup = GameLayer::instance->groups[target].get();
    this->centerGroup = GameLayer::instance->groups[center == 0 ? target : center].get();

    if (!centerGroup)
        centerGroup = targetGroup;

    if (!centerGroup)
        return false;

    this->rotateTotal = rotation;
    this->groupID = target;
    this->lockRotation = lock;

    if (target == center)
    {
        targetGroup->rotateAround = nullptr;
        return true;
    }

    int foundAmt = 0;

    for (auto pair : centerGroup->objects)
    {
        for (auto pair2 : pair.second)
        {
            foundAmt++;
            rotateAround = pair2.second;
        }
    }

    targetGroup->rotateAround = foundAmt > 1 ? nullptr : rotateAround;

    return true;
}

void RotateAction::update(float time)
{
    float rotateAmount = rotateTotal * time;

    float deltaRotation = rotateAmount - rotateLast;

    rotateLast = rotateAmount;

    targetGroup->rotateTotalMovement -= deltaRotation;

    if (!lockRotation)
        targetGroup->rotateTotal -= deltaRotation;

    auto dirty = &GameLayer::instance->dirtyGroups;
    if (std::find(dirty->begin(), dirty->end(), groupID) == dirty->end())
        dirty->push_back(groupID);
}