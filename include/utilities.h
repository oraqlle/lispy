#ifndef LISPY_UTILITIES_H
#define LISPY_UTILITIES_H

#include <types.h>

/// @brief Returns the name of a type
///
/// @details Returns a string of of the passed
/// type's name.
///
/// @param t - type int
/// @return char*
const char* ltype_name(int type_id);

#endif  /// LISPY_UTILITIES_H
