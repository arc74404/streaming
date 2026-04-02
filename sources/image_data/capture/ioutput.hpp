#pragma once

#include <string>

namespace stream::image
{
struct ScreenInfo final
{
    std::string name;
};

template<typename T>
concept OutputImplementationConcept = requires(const T& t) {
    std::string(t.getNameImpl());
};

template<template Derived>
class IOutput
{
public:
    ScreenInfo getInfo()
    {
        return ScreenInfo{.name = static_cast<Derived*>(this)->getNameImpl()};
    }

private:
};
} // namespace stream::image
