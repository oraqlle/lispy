# Sample Elements for Curly Standard Library

Some examples of what could go in the standard Curly Library.

## Contents

- [Sample Elements for Curly Standard Library](#sample-elements-for-curly-standard-library)
  - [Contents](#contents)
  - [Function](#function)
    - [Simple Function Signatures](#simple-function-signatures)
    - [Function Declaration](#function-declaration)
    - [Example](#example)
  - [Packing and Unpacking](#packing-and-unpacking)
    - [Packing](#packing)
    - [Unpacking](#unpacking)
  - [Currying](#currying)
    - [Examples of Currying Technique](#examples-of-currying-technique)

---

## Function

### Simple Function Signatures

```haskell
\ {args body} {def (head args) (\ (tail args) body)}
```

### Function Declaration

Syntatic sugar for function declaration

```haskell
def {fun} (\ {args body} {def (head args) (\ (tail args) body)})
```

### Example

```haskell
fun {add-together x y} {+ x y}
```

## Packing and Unpacking

### Packing

Packs variadic arguments into a list and evaluates `f` over the list.

```haskell
fun {pack f & xs} {f xs}
```

### Unpacking

Unpacks a list into variadic arguments and evalutes `f` over it.

```haskell
fun {unpack f xs} {eval (join (list f) xs)}
```

## Currying

Using the above functions we can define currying.

```haskell
def {uncurry} pack

def {curry} unpack
```

### Examples of Currying Technique

```haskell
curly> def {add-uncurried} +
()
curly> def {add-curried} (curry +)

curly> add-uncurried 5 6 7
18

curly> add-curried {5, 6, 7}
18
```
