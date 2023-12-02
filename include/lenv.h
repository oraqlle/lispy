#ifndef LISPY_LENV_H
#define LISPY_LENV_H

#include <builtin.h>
#include <lval.h>
#include <types.h>

#include <stdlib.h>

///////////////////////////
/// `lenv` Constructors ///
///////////////////////////

/// @brief Constructs a new lenv.
///
/// @details Constructs a new lenv.
///
/// @return lenv*
lenv* lenv_new(void);

//////////////////////////
/// `lenv` Destructors ///
//////////////////////////

/// @brief Destroys the lenv `env`.
///
/// @details Destroys the lenv `env` by
/// freeing all of its entries.
///
/// @param e - type: lenv*
void lenv_del(lenv* env);

//////////////////////
/// `lenv` Methods ///
//////////////////////

/// @brief Gets an lval from an lenv.
///
/// @details Returns a copy of the lval
/// `k` from the lenv `env` if it exists
/// otherwise returns an error.
///
/// @param e - type: lenv*
/// @param k - type: lval*
/// @return lval*
lval* lenv_get(lenv* env, const lval* key);

/// TODO
lenv* lenv_copy(const lenv* env);

/// @brief Sets an lval in an lenv.
///
/// @details Sets an lval in an lenv.
/// Replaces any existing entry with
/// the same key.
///
/// @param e - type: lenv*
/// @param k - type: lval*
/// @param v - type: lval*
void lenv_put(lenv* env, const lval* key, lval* value);

/// TODO
void lenv_def(lenv* env, const lval* key, lval* value);

/// @brief Adds a builtin function to the environment.
///
/// @details Adds a builtin function to the environment
/// with the name `name` and the function signature `func`.
///
/// @param e - type: lenv*
/// @param name - type: char*
/// @param f - type: lbuiltin
void lenv_add_builtin(lenv* env, const char* name, lbuiltin func);

/// @brief Adds all the builtin functions to the environment.
///
/// @details Adds all the builtin functions to the environment.
///
/// @param e - type: lenv*
void lenv_add_builtins(lenv* env);

#endif /// LISPY_LENV_H
