# **1️⃣ Optimization Flags**

| Flag     | Description                                              |
| -------- | -------------------------------------------------------- |
| `-O0`    | No optimization (debug builds)                           |
| `-O1`    | Basic optimization                                       |
| `-O2`    | Default optimization, balance size/performance           |
| `-O3`    | Maximum optimization (may increase size)                 |
| `-Os`    | Optimize for size (common in embedded)                   |
| `-Ofast` | Aggressive optimizations, may break standards            |
| `-Og`    | Optimize for debugging (good debug+optimization balance) |

---

# **2️⃣ Debugging Flags**

| Flag            | Description                         |
| --------------- | ----------------------------------- |
| `-g`            | Include debug information           |
| `-ggdb`         | Debug info tailored for GDB         |
| `-gdwarf-4`     | Specify DWARF version for debugging |
| `-DDEBUG`       | Define debug macro                  |
| `-fstack-usage` | Generate stack usage info           |

---

# **3️⃣ Warning / Code Quality**

| Flag                | Description                     |
| ------------------- | ------------------------------- |
| `-Wall`             | Most common warnings            |
| `-Wextra`           | Extra warnings                  |
| `-Werror`           | Treat warnings as errors        |
| `-pedantic`         | Strict standard compliance      |
| `-Wunused-variable` | Warn on unused variables        |
| `-Wunused-function` | Warn on unused functions        |
| `-Wshadow`          | Warn on variable shadowing      |
| `-Wformat`          | Warn format string mismatches   |
| `-Wconversion`      | Warn implicit conversions       |
| `-Wfloat-equal`     | Warn float equality comparisons |

---

# **4️⃣ Language Standard Flags**

| Flag              | Description                   |
| ----------------- | ----------------------------- |
| `-std=c11`        | C11 standard                  |
| `-std=c99`        | C99 standard                  |
| `-std=c++11`      | C++11                         |
| `-std=c++14`      | C++14                         |
| `-std=c++17`      | C++17                         |
| `-std=c++20`      | C++20                         |
| `-fno-exceptions` | Disable exceptions (C++ only) |
| `-fno-rtti`       | Disable RTTI (C++ only)       |

---

# **5️⃣ Linker / Code Size Flags**

| Flag                  | Description                                                |
| --------------------- | ---------------------------------------------------------- |
| `-ffunction-sections` | Place each function in its own section                     |
| `-fdata-sections`     | Place each data item in its own section                    |
| `-Wl,--gc-sections`   | Linker removes unused code/data                            |
| `-flto`               | Link-Time Optimization (optimize across translation units) |
| `-fuse-linker-plugin` | Required for full LTO                                      |
| `-specs=nosys.specs`  | For bare-metal applications without OS                     |

---

# **6️⃣ MCU / Architecture Flags (STM32)**

| Flag                           | Description                      |
| ------------------------------ | -------------------------------- |
| `-mcpu=cortex-m0/m3/m4/m7`     | Target MCU                       |
| `-mthumb`                      | Thumb instruction set            |
| `-mfpu=fpv4-sp-d16`            | FPU type (if MCU has FPU)        |
| `-mfloat-abi=hard/soft/softfp` | Hardware/software floating point |
| `-mno-unaligned-access`        | Disable unaligned memory access  |

---

# **7️⃣ Preprocessor / Macro Flags**

| Flag            | Description                             |
| --------------- | --------------------------------------- |
| `-DNAME`        | Define macro NAME                       |
| `-DNAME=value`  | Define macro with value                 |
| `-Ipath`        | Include path for headers                |
| `-include file` | Force include a file before compilation |
| `-Uname`        | Undefine macro                          |

---

# **8️⃣ Other Useful Flags**

| Flag                      | Description                                           |
| ------------------------- | ----------------------------------------------------- |
| `-fno-builtin`            | Don’t assume standard functions exist                 |
| `-fno-common`             | Detect multiple definitions of global vars            |
| `-fPIC`                   | Position-independent code (usually not used on STM32) |
| `-fshort-enums`           | Use smallest size for enums (embedded memory saving)  |
| `-fno-threadsafe-statics` | Disable static initialization overhead in C++         |
| `-ffast-math`             | Faster floating-point, may break strict compliance    |

---

### **Tips for STM32**

1. **Debug builds:** `-O0 -g -Wall -Wextra -std=c++17`
2. **Release builds:** `-Os -ffunction-sections -fdata-sections -flto -Wl,--gc-sections -std=c++17`
3. Always specify **MCU flags**: `-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard`

