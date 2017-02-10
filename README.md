# C++ generic dynamic variables

With the **[DynVars](dynvars.h)** class you can simulate the creation of well-defined-and-named variables at runtime just like dynamically typed languages do, including type-safety and garbage collection

#### Methods

- `T * Get(key)` - gets pointer to variable, returns _nullptr_ if variable not set

- `T * GetOrCreate(key)` - gets pointer to variable. ensures a valid pointer: if variable doesn't exist, it's created. to be used only for fundamental types

- `T GetValue(key)` - gets direct value (might imply object copy if value is object), returns `T()` if variable not set

- `DynVars * Set(key, value)` - set value, overwrites if already existing

- `DynVars * Set<T>(key, value)` - set value with explicit type, overwrites if already existing

- `anyspace::any SetAndReturnValue(key, value)` - set value and return the same value

- `T SetAndReturnValue<T>(key, value)` - set value with explicit type and return the same value

- `DynVars * Reset<T>(key)` - set value to `T()`

- `bool IsSet(key)` - check if variable set

- `DynVars * Remove(key, regexCaseSensitive = true)` - remove variable, can contain wildcards *

- `DynVars * Clear()` - removes all variables

- `DynVars * ToggleBool(key)` - sets variable to `true` if it doesn't exist or removes it if it exists

- `T * GetAuto(key, createIfInexistent = true)` - gets/creates a non-fundamental type (vectors, maps, custom classes, etc); the destructor is called automatically and the memory is freed automatically on variable remove

#### Examples
- [Simple example showing most DynVars methods](examples/dynvars.cpp)

Output:
```
anInt = 14

aChar = x

aFloat = 0.000000
aFloat = 3.140000

Is 'unsetVariable' set = 0, unsetVariable value = 0

anotherInt = 15

int2 = 22, &int3 = 0x1b80ff8, int3 = 33, &int4 = (nil), int4 = 0

current variables: int3, int2, anotherInt, aFloat, aChar, anInt,
removing *int* named vars, remaining: aFloat, aChar,

vector has size = 2, v[0] = 123, v[1] = 456

someClass should be created next:
 - someClass created
someClass should be destroyed next:
 - someClass destroyed, value = 16, secondVal = 34

another someClass should be created next and automatically destroyed at scope end:
 - someClass created
{scope ended}
 - someClass destroyed, value = 0, secondVal = 333
```
- [Practical example with a Config class](examples/dynvars2.cpp)

Output:
```
Welcome to HappyAnmials zoo !
 - We allow Cats
 - We don't allow Dogs
 - We can store at most 5 animals in here
```

- [Simple example showing user input defined variables and type-safety](examples/dynvars3.cpp)

While this example may seem to overcomplicate the usage of DynVars, the idea behind it is to show how dynamic variables are created and how type safety check behaves (throwing `any_bad_cast` on incorrect `Get<T>`)
