#pragma once
/*
 *    Xcode does not fully support std::optional yet. When compiling
 *    with Xcode use <experimental/optional> and std::experimental as
 *    the namespace.
 */

#ifdef ENGINE_PLATFORM_MACOS
    #include <experimental/optional>
    #define Optional std::experimental::optional
    #define Nullopt  std::experimental::nullopt 
#else
    #include <optional>
    #define Optional std::optional
    #define Nullopt  std::nullopt
#endif