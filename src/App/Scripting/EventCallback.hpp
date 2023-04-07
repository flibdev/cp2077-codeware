#pragma once

namespace App
{
struct EventCallback
{
    EventCallback(const Red::WeakHandle<Red::IScriptable>& aObject, Red::CName aFunction, bool aPermanent)
        : object(aObject)
        , function(aFunction)
        , permanent(aPermanent)
    {
    }

    EventCallback(Red::CName aType, Red::CName aFunction, bool aPermanent)
        : type(aType)
        , function(aFunction)
        , permanent(aPermanent)
    {
    }

    template<typename... Args>
    void operator()(Args&&... aArgs) const
    {
        if (IsStatic())
        {
            Red::CallStatic(type, function, std::forward<Args>(aArgs)...);
        }
        else if (!object.Expired())
        {
            Red::CallVirtual(object.Lock(), function, std::forward<Args>(aArgs)...);
        }
    }

    bool operator==(const EventCallback& aRhs) const noexcept
    {
        return object.instance == aRhs.object.instance && type == aRhs.type && function == aRhs.function;
    }

    [[nodiscard]] bool IsObject() const noexcept
    {
        return object.instance;
    }

    [[nodiscard]] bool IsStatic() const noexcept
    {
        return type;
    }

    [[nodiscard]] bool IsPermanent() const noexcept
    {
        return permanent;
    }

    Red::WeakHandle<Red::IScriptable> object;
    Red::CName type;
    Red::CName function;
    bool permanent;
};
}
