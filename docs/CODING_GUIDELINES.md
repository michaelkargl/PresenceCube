# Coding Guidelines

## Naming

When naming identifiers follow these rules:

- use [snake casing] for identifiers
- mind [reserved keywords and identifiers]

## public members

Publicly accessible declarations

1. are always prefixed with the module name separated with double-underscore `__`
   - decreases name collisions with external libraries
   - adds context to the function or variable

### Do

```c
int math_util__add(int left, int right);
extern bool math_util__initialized;
```

### Don't

```c
int add(int left, int right);
extern bool initialized;
```

## private members

As a rule: anything that doesn't have a module name prefix is intended for private use

- `static` declarations are not prefixed by default
  - we used to use `_` to have a visual separation of privates and publics but these
    identifiers turned out to be reserved (see section on **Naming**)

### Do

```c
static bool initialized = true;
static bool get_initialized();
```

### Don't

```c
// see seciton on 'Naming'
static int __counter = 0;
static bool _initialized = true;
static bool p_get_initialized();
```

## References

- [snake casing]
- [reserved keywords and identifiers]

[snake casing]: https://www.freecodecamp.org/news/snake-case-vs-camel-case-vs-pascal-case-vs-kebab-case-whats-the-difference/#snake-case
[reserved keywords and identifiers]: https://www.gnu.org/software/libc/manual/html_node/Reserved-Names.html
