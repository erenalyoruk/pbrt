#pragma once

namespace pbrt::debug
{
/**
 * @brief Triggers a breakpoint interrupt for debugging purposes.
 *
 * When called, this function causes the program to halt execution and
 * transfer control to a debugger if one is attached. It is typically used
 * to intentionally pause execution at a specific point for inspection.
 *
 * Usage of this function is platform-dependent and may invoke
 * architecture-specific instructions (e.g., `__debugbreak()` on MSVC,
 * `__builtin_trap()` or `asm("int3")` on GCC/Clang).
 */
void debug_break();

} // namespace pbrt::debug
