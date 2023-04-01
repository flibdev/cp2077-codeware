#pragma once

#define X_RTTI_STR1(x) #x
#define X_RTTI_STR2(x) X_RTTI_STR1(x)
#define X_RTTI_LOCATION __FILE__ ":" X_RTTI_STR2(__LINE__)

#define X_RTTI_TYPENAME(...) ::nameof::nameof_short_type<__VA_ARGS__>()

#define X_RTTI_NAME_SV(...) []() constexpr noexcept { \
  constexpr auto _name = ::nameof::detail::pretty_name(#__VA_ARGS__); \
  return ::nameof::cstring<_name.size()>{_name}; }()
#define X_RTTI_NAME(...) X_RTTI_NAME_SV(__VA_ARGS__).data()

#define X_RTTI_GETTER_NAME_SV(...) []() constexpr noexcept { \
  constexpr auto n1 = Red::Detail::RemoveMemberPrefix(#__VA_ARGS__); \
  constexpr auto n2 = Red::Detail::UpFirstConstStr<n1.size()>(n1.data()); \
  return Red::Detail::ConcatConstStr<3, n2.size() - 1>("Get", n2.data()); }()
#define X_RTTI_GETTER_NAME(...) X_RTTI_GETTER_NAME_SV(__VA_ARGS__).data()

#define X_RTTI_SELECT(_0, _1, _2, _3, ...) _3
#define X_RTTI_RECOMPOSE(_) X_RTTI_SELECT _
#define X_RTTI_EXPAND_NOARG(F) ,,,F ## _0
#define X_RTTI_SELECT_BY_NARG(F, ...) X_RTTI_RECOMPOSE((__VA_ARGS__, F##_3, F##_2, F##_1,))
#define X_RTTI_SELECT_MACRO(F, ...) X_RTTI_SELECT_BY_NARG(F, X_RTTI_EXPAND_NOARG __VA_ARGS__ (F))
#define X_RTTI_OVERLOAD(F, ...) X_RTTI_SELECT_MACRO(F, __VA_ARGS__)(__VA_ARGS__)

#define RTTI_IMPL_TYPEINFO(_class) \
    Red::CClass* GetNativeType() \
    { \
        return Red::GetClass<_class>(); \
    }

#define RTTI_IMPL_ALLOCATOR(...) X_RTTI_OVERLOAD(X_IMPL_ALLOCATOR, __VA_ARGS__)
#define X_IMPL_ALLOCATOR_0() X_IMPL_ALLOCATOR_1(Red::Memory::RTTIAllocator)
#define X_IMPL_ALLOCATOR_1(_allocator) \
public: \
    using AllocatorType = _allocator; \
    static_assert(Red::Detail::IsAllocator<_allocator>, #_allocator " is not a valid allocator type"); \
    Red::Memory::IAllocator* GetAllocator() \
    { \
        return AllocatorType::Get(); \
    }

#define RTTI_DECLARE_FRIENDS(_class) \
    friend class Red::RTTIBuilder<Red::Scope::For<_class>()>; \
    friend class Red::ClassDescriptor<_class>;

#define RTTI_DEFINE_CLASS(...) X_RTTI_OVERLOAD(X_RTTI_DEF_CLASS, __VA_ARGS__)
#define X_RTTI_DEF_CLASS_1(_class) X_RTTI_DEF_CLASS_3(_class, X_RTTI_TYPENAME(_class),)
#define X_RTTI_DEF_CLASS_2(_class, _desc) X_RTTI_DEF_CLASS_3(_class, X_RTTI_TYPENAME(_class), _desc)
#define X_RTTI_DEF_CLASS_3(_class, _name, _desc) \
    template<> \
    struct Red::RTTIBuilder<Red::ClassDefinition<_class>{}> \
    { \
        using Type = _class; \
        using Descriptor = Red::ClassDescriptor<_class>; \
        static constexpr auto Name() \
        { \
            return _name; \
        } \
        static void Describe(Descriptor* type) \
        { \
            _desc; \
        } \
    };

#define RTTI_EXPAND_CLASS(...) X_RTTI_OVERLOAD(X_RTTI_EXT_CLASS, __VA_ARGS__)
#define X_RTTI_EXT_CLASS_2(_class, _desc) X_RTTI_EXT_CLASS_3(_class, _class, _desc)
#define X_RTTI_EXT_CLASS_3(_class, _expansion, _desc) \
    template<> \
    struct Red::RTTIBuilder<Red::ClassExpansion<_class, X_RTTI_LOCATION>{}> \
    { \
        using Type = _expansion; \
        using Descriptor = Red::ClassDescriptor<_class>; \
        static void Describe(Descriptor* type) \
        { \
            _desc; \
        } \
    };

#define RTTI_ABSTRACT() type->MarkAbstract()
#define RTTI_PARENT(_parent) type->SetParent<_parent>()
#define RTTI_ALIAS(_alias) type->SetAlias(_alias)

#define RTTI_METHOD(...) X_RTTI_OVERLOAD(X_RTTI_METHOD, __VA_ARGS__)
#define X_RTTI_METHOD_1(_method) X_RTTI_METHOD_2(_method, #_method)
#define X_RTTI_METHOD_2(_method, _name) type->AddFunction<&Type::_method>(_name)

#define RTTI_METHOD_FQN(...) X_RTTI_OVERLOAD(X_RTTI_METHOD_FQN, __VA_ARGS__)
#define X_RTTI_METHOD_FQN_1(_method) X_RTTI_METHOD_FQN_2(_method, X_RTTI_NAME(_method))
#define X_RTTI_METHOD_FQN_2(_method, _name) type->AddFunction<&_method>(_name)

#define RTTI_CALLBACK(...) X_RTTI_OVERLOAD(X_RTTI_CALLBACK, __VA_ARGS__)
#define X_RTTI_CALLBACK_1(_method) X_RTTI_CALLBACK_2(_method, #_method)
#define X_RTTI_CALLBACK_2(_method, _name) type->AddFunction<&Type::_method>(_name, {.isEvent = true})

#define RTTI_CALLBACK_FQN(...) X_RTTI_OVERLOAD(X_RTTI_CALLBACK_FQN, __VA_ARGS__)
#define X_RTTI_CALLBACK_FQN_1(_method) X_RTTI_CALLBACK_FQN_2(_method, X_RTTI_NAME(_method))
#define X_RTTI_CALLBACK_FQN_2(_method, _name) type->AddFunction<&_method>(_name, {.isEvent = true})

#define RTTI_PROPERTY(...) X_RTTI_OVERLOAD(X_RTTI_PROPERTY, __VA_ARGS__)
#define X_RTTI_PROPERTY_1(_property) X_RTTI_PROPERTY_2(_property, #_property)
#define X_RTTI_PROPERTY_2(_property, _name) type->AddProperty<&Type::_property>(_name)

#define RTTI_PROPERTY_FQN(...) X_RTTI_OVERLOAD(X_RTTI_PROPERTY_FQN, __VA_ARGS__)
#define X_RTTI_PROPERTY_FQN_1(_property) X_RTTI_PROPERTY_FQN_2(_property, X_RTTI_NAME(_property))
#define X_RTTI_PROPERTY_FQN_2(_property, _name) type->AddProperty<&_property>(_name)

#define RTTI_PERSISTENT(...) X_RTTI_OVERLOAD(X_RTTI_PERSISTENT, __VA_ARGS__)
#define X_RTTI_PERSISTENT_1(_property) X_RTTI_PERSISTENT_2(_property, #_property)
#define X_RTTI_PERSISTENT_2(_property, _name) type->AddProperty<&Type::_property>(_name, {.isPersistent = true})

#define RTTI_PERSISTENT_FQN(...) X_RTTI_OVERLOAD(X_RTTI_PERSISTENT_FQN, __VA_ARGS__)
#define X_RTTI_PERSISTENT_FQN_1(_property) X_RTTI_PERSISTENT_FQN_2(_property, X_RTTI_NAME(_property))
#define X_RTTI_PERSISTENT_FQN_2(_property, _name) type->AddProperty<&_property>(_name, {.isPersistent = true})

#define RTTI_GETTER(...) X_RTTI_OVERLOAD(X_RTTI_GETTER, __VA_ARGS__)
#define X_RTTI_GETTER_1(_property) X_RTTI_GETTER_2(_property, X_RTTI_GETTER_NAME(_property))
#define X_RTTI_GETTER_2(_property, _name) type->AddGetter<&Type::_property>(_name)

#define RTTI_DEFINE_ENUM(...) X_RTTI_OVERLOAD(X_RTTI_DEF_ENUM, __VA_ARGS__)
#define X_RTTI_DEF_ENUM_1(_enum) X_RTTI_DEF_ENUM_2(_enum, X_RTTI_TYPENAME(_enum))
#define X_RTTI_DEF_ENUM_2(_enum, _name) \
    template<> \
    struct Red::RTTIBuilder<Red::EnumDefinition<_enum>{}> \
    { \
        using Type = _enum; \
        using Descriptor = Red::EnumDescriptor<_enum>; \
        static constexpr auto Name() \
        { \
            return _name; \
        } \
    };

#define RTTI_DEFINE_FLAGS(...) X_RTTI_OVERLOAD(X_RTTI_DEF_FLAGS, __VA_ARGS__)
#define X_RTTI_DEF_FLAGS_1(_enum) X_RTTI_DEF_FLAGS_2(_enum, X_RTTI_TYPENAME(_enum))
#define X_RTTI_DEF_FLAGS_2(_enum, _name) \
    template<> \
    struct Red::RTTIBuilder<Red::FlagsDefinition<_enum>{}>  \
    { \
        using Type = _enum; \
        using Descriptor = Red::EnumDescriptor<_enum>; \
        static constexpr auto Name() \
        { \
            return _name; \
        } \
    };

#define RTTI_EXPAND_ENUM(...) X_RTTI_OVERLOAD(X_RTTI_EXP_ENUM, __VA_ARGS__)
#define X_RTTI_EXP_ENUM_1(_enum) X_RTTI_EXP_ENUM_2(_enum, _enum)
#define X_RTTI_EXP_ENUM_2(_enum, _expansion) \
    template<> \
    struct Red::RTTIBuilder<Red::EnumExpansion<_enum, X_RTTI_LOCATION>{}> \
    { \
        using Type = _enum; \
        using Descriptor = Red::EnumDescriptor<_enum>; \
        static void Describe(Descriptor* type) \
        { \
            type->template AddOptions<_expansion>(); \
        } \
    };

#define RTTI_EXPAND_FLAGS(...) X_RTTI_OVERLOAD(X_RTTI_EXP_FLAGS, __VA_ARGS__)
#define X_RTTI_EXP_FLAGS_1(_enum) X_RTTI_EXP_FLAGS_2(_enum, _enum)
#define X_RTTI_EXP_FLAGS_2(_enum, _expansion) \
    template<> \
    struct Red::RTTIBuilder<Red::EnumExpansion<_enum, X_RTTI_LOCATION>{}> \
    { \
        using Type = _enum; \
        using Descriptor = Red::EnumDescriptor<_enum>; \
        static void Describe(Descriptor* type) \
        { \
            type->template AddFlags<_expansion>(); \
        } \
    };

#define RTTI_OPTION() static_assert(false, "Not Implemented")

#define RTTI_DEFINE_GLOBALS(...) X_RTTI_OVERLOAD(X_RTTI_DEF_GLOB, __VA_ARGS__)
#define X_RTTI_DEF_GLOB_1(_desc) \
    template<> \
    struct Red::RTTIBuilder<Red::GlobalDefinition<X_RTTI_LOCATION>{}> \
    { \
        using Descriptor = Red::GlobalDescriptor; \
        static void Describe(Descriptor* rtti) \
        { \
            _desc; \
        } \
    };
#define X_RTTI_DEF_GLOB_2(_namespace, _desc) \
    template<> \
    struct Red::RTTIBuilder<<Red::GlobalDefinition<X_RTTI_LOCATION>{}> \
    { \
        using Descriptor = Red::GlobalDescriptor; \
        static void Describe(Descriptor* rtti) \
        { \
            using namespace _namespace; \
            _desc; \
        } \
    };

#define RTTI_FUNCTION(...) X_RTTI_OVERLOAD(X_RTTI_FUNCTION, __VA_ARGS__)
#define X_RTTI_FUNCTION_1(_func) X_RTTI_FUNCTION_2(_func, X_RTTI_NAME(_func))
#define X_RTTI_FUNCTION_2(_func, _name) rtti->AddFunction<&_func>(_name)

#define RTTI_OP() static_assert(false, "Not Implemented")
#define RTTI_CAST() static_assert(false, "Not Implemented")

#define RTTI_REGISTER(_handler) \
    template<> \
    struct Red::RTTIBuilder<<Red::GlobalDefinition<X_RTTI_LOCATION>{}> \
    { \
        using Descriptor = Red::GlobalDescriptor; \
        static void Register(Descriptor* rtti) \
        { \
            _handler; \
        } \
    };

#define RTTI_DESCRIBE(_handler) \
    template<> \
    struct Red::RTTIBuilder<<Red::GlobalDefinition<X_RTTI_LOCATION>{}> \
    { \
        using Descriptor = Red::GlobalDescriptor; \
        static void Describe(Descriptor* rtti) \
        { \
            _handler; \
        } \
    };
