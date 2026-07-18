# Lambda Calculus Language

A REPL for the untyped lambda calculus: type an expression, watch it beta-reduce step by step.

## Grammar

```
expr := name | (\name.expr) | (expr expr)
```

Examples:

```
(\x.x)              identity
((\x.x) y)           applying identity to y
(\f.((\x.(f (x x))) (\x.(f (x x)))))    the Y combinator
```

## What it does

A hand-written lexer and recursive-descent parser build an expression tree, then `eval1` beta-reduces it in normal order (leftmost-outermost, reducing under lambdas too), printing the expression again after each step. It stops after 10 steps and prints `...` if the expression hasn't reached a normal form by then, or stops early if it reaches a fixed point.

Bound variables get renamed to unique global IDs on parse to avoid capture during substitution. This isn't classic De Bruijn indexing: the IDs are just unique counters, not depth-based positions.

## Building

```bash
cc -o lamb lamb.c
```

The `nob.c` and `nob.h` files in this directory are a stray copy left over from `venation_leaf/`; they reference a `main.c` and a vendored raylib directory that don't exist here, so don't try to build with them.

## Running

```bash
./lamb
```

Type an expression and press Enter to see it reduce. Exit with Ctrl+D.
