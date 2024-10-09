#pragma once
#include "hook.h"

extern bool start_bad;

namespace hooks 
{
    class sound final : public hook
    {
    public:
        bool install() override;
        void uninstall() override;
    };
}
