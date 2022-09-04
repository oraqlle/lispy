#ifndef CURLY_UTILITIES_H
#define CURLY_UTILITIES_H

#include <types.h>

#include <mpc.h>

/// \brief Returns the name of a type
///
/// \details Returns a string of of the passed
/// type's name.
///
/// \param t - type int
/// \return char*
char* ltype_name(int t);


/// TODO
// lbuiltin partial(builtinload load, mpc_parser_t* parser_id);

#endif  /// CURLY_UTILITIES_H
