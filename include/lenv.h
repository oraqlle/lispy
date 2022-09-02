#ifndef CURLY_LENV_H
#define CURLY_LENV_H

#include <builtins.h>
#include <lval.h>
#include <types.h>

#include <stdlib.h>


///////////////////////////
/// `lenv` Constructors ///
///////////////////////////

/// \brief Constructs a new lenv.
///
/// \details Constructs a new lenv.
///
/// \return lenv*
lenv* lenv_new(void);


//////////////////////////
/// `lenv` Destructors ///
//////////////////////////

/// \brief Destroys the lenv `e`.
///
/// \details Destroys the lenv `e` by
/// freeing all of its entries.
///
/// \param e - type: lenv*
void lenv_del(lenv* e);


//////////////////////
/// `lenv` Methods ///
//////////////////////

/// \brief Gets an lval from an lenv.
/// 
/// \details Returns a copy of the lval 
/// `k` from the lenv `e` if it exists 
/// otherwise returns an error.
///
/// \param e - type: lenv*
/// \param k - type: lval*
/// \return lval*
lval* lenv_get(lenv* e, lval* k);


/// TODO
lenv* lenv_copy(lenv* e);


/// \brief Sets an lval in an lenv.
///
/// \details Sets an lval in an lenv.
/// Replaces any existing entry with 
/// the same key.
///
/// \param e - type: lenv*
/// \param k - type: lval*
/// \param v - type: lval*
void lenv_put(lenv* e, lval* k, lval* v);


/// TODO
void lenv_def(lenv* e, lval* k, lval* v);


/// \brief Adds a builtin function to the environment.
///
/// \details Adds a builtin function to the environment
/// with the name `name` and the function signature `func`.
///
/// \param e - type: lenv*
/// \param name - type: char*
/// \param f - type: lbuiltin
void lenv_add_builtin(lenv* e, char* name, lbuiltin func);


/// \brief Adds all the builtin functions to the environment.
///
/// \details Adds all the builtin functions to the environment.
///
/// \param e - type: lenv*
void lenv_add_builtins(lenv* e);


#endif  /// CURLY_LENV_H
