#pragma once

#include <stdexcept> // IWYU pragma: keep

#if defined(PBRT_DISABLE_EXCEPTIONS)
  #define PBRT_THROW_OR_RETURN(exceptionType, returnValue) return returnValue;
#else
  #define PBRT_THROW_OR_RETURN(exceptionType, returnValue) throw exceptionType;
#endif
