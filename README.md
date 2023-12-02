# Lispy

The original implementation of Lispy in C. Lispy is a small but robust programming language that inherits many features from Lisp. Lispy comes with a REPL as well as an interpreter. Like other Lisps, Lispy uses _macros-like_ objects called Q-expressions to build out unevaluated code whose structure and content can be manipulated and then later evaluated.

## Example

```lisp
; Fibonacci (this is a comment)

; `()` denotes a S-expression which will evaluate immediately
; `{}` denotes a Q-expression which will remain unevaluated
; Define a function `fib` taking an parameter called `n`.
; `fun` takes two Q-expressions the first being the name and parameters,
; the second is the body of the function
(fun {fib n} {
    ; `select` expressions evaluate the first element (here a S-expression),
    ; returning the evaluated the second element if the first evaluates to true
    ; continues until a selection is made, `otherwise` is reached or no 
    ; selection is made which result in an `error`.
    select
        { (== n 0) 0 }
        { (== n 1) 1 }
        { otherwise (+ (fib (- n 1)) (fib (- n 2))) }
})

; Create list of numbers and apply `fib` to each element
; Yielding a new list
(print (map fib {0 1 2 3 4 5 6 7 8 9}))
```

[Examples](examples/)

## Prerequisites

```sh
# Building
C11 compliant compiler (GCC, Clang, MSVC)
CMake >= 3.15.0
vcpkg

# Testing, coverage
C++11 compliant compiler
lcov

# Linting, formatting and spelling
clang-format
clang-tidy
cppcheck >= 2.7.0
codespell >= 2.2.4

# Docs
doxygen
jinja2
Pygments
```

## Building

```sh
cmake --preset=dev
cmake --build --preset=dev
```

This project contains a few prebuilt presets for cross-platform configuration in the [`CMakePresets.json`](CMakePresets.json). It is common then to create your own presets in a `CMakeUserPresets.json` file to configure the project to your setup and for debugging. A sample one has been provided in [`CMakeUserPresets-Sample.json`](CMakeUserPresets-Sample.json) which contains the presets for different platforms in the form of `dev-<platform>`. There is also a plain `dev` preset which is an alias for `dev-linux`, which can be modified for your default platform. There are also `dev` presets for building and testing which simply require the `dev` preset.

## Running

### REPL

```sh
$ ./build/dev/lispy_interpreter
Lispy v0.3.1
Press Ctrl+D to exit.

Lispy>
```

### Interpreter

```sh
./build/dev/lispy_interpreter examples/hello.lpy
"hello world!"
```

## Tests

```sh
ctest --preset=dev

#  ... or
cmake --build --preset=dev -t test
```

> [!NOTE]
> I haven't really written any tests yet.

## Credit

This implementation of Lispy is based on Daniel Holden's book [Build Your Own Lisp](https://www.buildyourownlisp.com/) Book/Blog. If you are interested in creating your own programming language or even just learning C, I highly recommend giving it a read.
