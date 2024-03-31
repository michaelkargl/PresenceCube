# Coding Guidelines

## Naming

When naming identifiers follow these rules:

- use [snake casing] for identifiers
- mind [reserved keywords and identifiers]


## File names

1. files use `snake_casing`
1. interface type headers are suffixed `logger.iface.h`
   - Interface type headers intended to have multiple
     implementations (for instance a `native` library and an `embedded` library).
   - Adding an `iface` suffix tells library users that 
     a specific header does not have a default default
     implementation yet and needs a concrete  
     implementation before it can be used
## `public` members

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

## `private` members

> As a rule: anything that doesn't have a module name prefix is intended to be `private` / `static`. Inversely, anything with a module name is intended to be used by other modules.

- `static` declarations are not prefixed by default
  - we used to use `_` to have a visual separation of `private`s and `public`s but these identifiers turned out
    to be reserved (see section on **Naming**)

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

### Reason

Variables beginning with `_` [are reserved][reserved keywords and identifiers] by the C standard and result in undefined behavior. Prefixing them with a letter like `p_` results in longer and confusing names than necessary and might result in confusion since this notation is sometimes used to mark variables as pointers. Since every `public` function or variable is prefixed with their containing module name, anything without a prefix can assumed to be private `private`.

## Libraries

Platform IO code is managed in libraries placed in the [lib folder](/lib). The structure of libraries is inherently flat.

### [Library Dependency Finder Mode][dependency-finder-mode]

#### Do

Keep the `lib_ldf_mode` to `off`

#### Reason

The library dependency finder automatically analyzes project
and library dependencies by name and #includes. This being an  amazing feature that reduces a lot of tedious dependency management, it also introduces a handful of negative aspects

1. it can not handle multiplatform setups as it takes the first applicable library it can find
1. we loose a lot of transparency since a single unnecessary
   `#include` can mean the introduction of an unnecessary ilbrary reference. Additionally, when something doesn't work, the dependencies are clear. This is especially helpful for developers inexperienced / out of practice with the C language.
1. manually having to add library dependencies forces the developer to think about the project architecture first.

### Library composition

#### Don't

```c
#ifdef PLATFORM='arduino'
  #include "arduino_logger.h"
#else if PLATFORM='esp32'
  #include "esp32_logger.h"
#else
  #include "native_logger.h"
#endif
```

#### Do

Handle these cases through platform io libraries and dependency inversion. Create an "interface" that is implemented in 3 concrete ways `native`, `esp32`, `arduino`. The platformio dependency finder is responsible for resolving the right implementation. 

#### Reason

Using the `#ifdef` approach

1. introduces coupling between code of different platforms
1. introduces orthogonality (changes in native means retesting the other platforms)
1. different platforms have different needs, the amount of "special cases" only increases causig more strange `#ifdef` constructs in the strangest of places.
1. as fans of garfield, we choose lasagna over spaghetti any time

![picture showing a clip of garfield finding a piece of lasagna lying on the floor saying: "There you are! I love lasagna! But alas, it was only a fling" He had devoured it in seconds.](https://i0.wp.com/fratelliaifornelli.it/wp-content/uploads/2016/03/lasagna-garfiled-love.png)

## References

- [snake casing]
- [reserved keywords and identifiers]

[dependency-finder-mode]: https://docs.platformio.org/en/latest/librarymanager/ldf.html#dependency-finder-mode
[snake casing]: https://www.freecodecamp.org/news/snake-case-vs-camel-case-vs-pascal-case-vs-kebab-case-whats-the-difference/#snake-case
[reserved keywords and identifiers]: https://www.gnu.org/software/libc/manual/html_node/Reserved-Names.html
