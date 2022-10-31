# C-Lix

The original implementation of Lix in C. Lix is a small but robust programming language that inherits many features from Lisp. Lix comes with an REPL as well as an interpreter. Like other Lisps, Lix uses macros to build out expressive functionality however, unlike other Lisps, Lix uses Q-expressions to create unevaluated list of data and commands that can be manipulated as a Q-expression or evaluated to obtain a result. 

## Notes

- This implementation of Lix is toy-like and has no real production testing. It has fundamental, yet limited feature set.
- A more complete version of lix is being developed on the [Lix](https://github.com/oraqlle/lix) repository as well as well as a Rust implementation on the [Rust-Lix](https://github.com/oraqlle/rust-lix) repository.
- Lix is a personal project aimed for me to explore programming language design and implementation.

## Credit

Lix is derived and inspired from the language created by Daniel Holden in his [Build Your Own Lisp](https://www.buildyourownlisp.com/) Book/Blog. Most of the design of this implementation reflects that of the source code in his implementation. Daniel Holden is also the creator of Q-expressions. If you are interesting in creating your own programming language or even just learning C, I highly recommend giving it a read.
