#pragma once

namespace Category
{
    enum Type
    {
        None            = 0,
        Scene           = 1 << 0,
        PlayerFlag      = 1 << 1,
        AlliedFlag      = 1 << 2,
        EnemyFlag       = 1 << 3
    };
}