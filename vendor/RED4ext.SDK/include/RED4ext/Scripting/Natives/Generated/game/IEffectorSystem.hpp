#pragma once

// clang-format off

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/IGameSystem.hpp>

namespace RED4ext
{
namespace game { 
struct IEffectorSystem : game::IGameSystem
{
    static constexpr const char* NAME = "gameIEffectorSystem";
    static constexpr const char* ALIAS = "IEffectorSystem";

};
RED4EXT_ASSERT_SIZE(IEffectorSystem, 0x48);
} // namespace game
using IEffectorSystem = game::IEffectorSystem;
} // namespace RED4ext

// clang-format on
