# 🏗️ Daedalus Library - Math, Data Structures, and Game Dev Utils
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![MIDAS Toolchain](https://img.shields.io/badge/MIDAS%20Toolchain-Part%20of-gold.svg)](#)

A high-performance C library providing mathematical tools, data structures, and utilities for game development and systems programming. Part of the MIDAS Toolchain.

## Overview

**Daedalus** is a collection of well-tested data structures and math utilities designed for real-time applications. It includes dynamic/static arrays, hash tables, string builders, vector/matrix math, and a comprehensive logging system.

## Installation

### 🔧 Native Development (System-Wide)

```sh
git clone https://github.com/McCoy1701/Daedalus.git
cd Daedalus
make shared          # Build shared library
sudo make install    # Install to /usr/lib/ and /usr/include/
```

**Then, in your C projects:**

```c
#include <Daedalus.h>  // System-wide header
// Link with: gcc your_code.c -lDaedalus
```

### 🌐 Emscripten Web Projects

```sh
make EM              # Creates bin/libArchimedes.a static library
make EMARCH          # Creates bin/libDaedalus.a static library
```

**Copy these files to your Emscripten project:**

`bin/libArchimedes.a` → your lib/ directory
`include/Daedalus.h` → your include/ directory

**Then compile with:**

```sh
emcc your_code.c lib/libArchimedes.a -I include/ -sALLOW_MEMORY_GROWTH -o output.html
```

### 🧹 Cleanup & Maintenance

```sh
make clean           # Remove all build artifacts
sudo make uninstall  # Remove system installation
```

### 🎮 Sample Transformations

**Before Daedalus (Dangerous C)**


```c
char* name = malloc(256);           // No error checking
strcpy(name, "Player");             // Buffer overflow risk  
strcat(name, " Character");         // More overflow risk
printf("Created: %s\n", name);      // No structured logging
free(name);                         // Manual cleanup required
```

**After Daedalus (Master-Crafted)**

```c
dString_t* name = d_StringInit();
d_StringAppend(name, "Player", 0);
d_StringAppend(name, " Character", 0);  
d_LogInfoF("Created: %s", d_StringPeek(name));
d_StringDestroy(name);              // Automatic, safe cleanup
```

**The Result:** Safer, cleaner code with automatic memory management.

## 📚 Complete Documentation

Comprehensive usage guides are available for all major components:

- **[Dynamic Strings](https://github.com/McCoy1701/Daedalus/wiki/usage-strings)** - Python-like string operations, formatting, slicing, and templates
- **[Dynamic Arrays](https://github.com/McCoy1701/Daedalus/wiki/usage-arrays)** - Resizable arrays with automatic growth and flexible operations
- **[Static Arrays](https://github.com/McCoy1701/Daedalus/wiki/usage-static-arrays)** - Fixed-capacity arrays with predictable memory and file persistence
- **[Dynamic Hash Tables](https://github.com/McCoy1701/Daedalus/wiki/usage-tables)** - Resizable hash tables with automatic rehashing
- **[Static Hash Tables](https://github.com/McCoy1701/Daedalus/wiki/usage-static-tables)** - Fixed-key hash tables optimized for known key sets
- **[Logging System](https://github.com/McCoy1701/Daedalus/wiki/usage-logging)** - Professional-grade logging with filtering, structured output, and rate limiting

Each guide includes quick-start examples, complete API reference, best practices, and real-world usage patterns.

## 🎯 Quick Examples

### Dynamic Strings

```c
dString_t* name = d_StringInit();
d_StringAppend(name, "Player", 0);
d_StringAppend(name, " Character", 0);
printf("%s\n", d_StringPeek(name));  // Output: Player Character
d_StringDestroy(name);
```

### Dynamic Arrays - Automatic Growth

```c
dArray_t* enemies = d_ArrayInit(10, sizeof(Enemy));
Enemy goblin = {.health = 50, .type = GOBLIN};
d_ArrayAppend(enemies, &goblin);
d_ArrayRemove(enemies, 0);  // Remove first enemy
d_ArrayDestroy(enemies);
```

### Hash Tables - Built-in Functions

```c
// String-keyed configuration
dTable_t* config = d_TableInit(sizeof(char*), sizeof(int),
                               d_HashString, d_CompareString, 16);

char* key = "max_health";
int value = 100;
d_TableSet(config, &key, &value);
d_TableDestroy(&config);
```

### Logging

```c
d_LogInfoF("Player %s joined at position (%.2f, %.2f)", name, x, y);
d_LogErrorF("Connection failed: code %d", error_code);
```

For comprehensive examples and complete API documentation, see the [wiki documentation](https://github.com/McCoy1701/Daedalus/wiki).

## Design Goals

- **Performance**: Optimized algorithms and memory layouts for real-time use
- **Type Safety**: Generic data structures with consistent APIs
- **Web Ready**: Full Emscripten/WebAssembly support
- **Easy to Use**: Comprehensive documentation and built-in utilities

## MIDAS Toolchain

Daedalus is part of the MIDAS development toolchain:

- **Metis**: C linter
- **Ixion**: Memory safety tools
- **Daedalus**: Core library (math, data structures, utilities)
- **Archimedes**: UI framework
- **Sisyphus**: Testing framework
