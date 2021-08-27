#pragma once

// Dont include this in header files unless u want super long 
// compile time ;)
// prefer to use forward class declarations
// I made this header cos im lazy
#include "Logging/Logging.h"
#include "Assets/GameAssetSoftPointer.h"
#include "Audio/GameAudio.h"
#include "ECS/GameEntity.h"
#include "ECS/GameComponent.h"
#include "Rendering/GameRendererComponent.h"
#include "WindowManager.h"
#include "GameManager.h"
#include "GunGame.h"
#include "Rendering/BasicRendererComponent.h"
#include "Utility/Utility.h"
#include "Physics/PhysicsWorld.h"
#include "Rendering/Camera.h"
#include "Input/InputManager.h"
#include <SDL2/SDL.h>
#include "Rendering/BasicRendererComponent.h"
#include "Rendering/SquareRendererComponent.h"
#include "Physics/RigidbodyComponent.h"
#include "Textures/TextureAsset.h"
#include "Rendering/SpriteRendererComponent.h"
#include "Rendering/Particles/Particle.h"
#include "PlayerCharacter/PlayerTrackerEntity.h"
#include "PlayerCharacter/PlayerCharacterEntity.h"
#undef PlaySound
