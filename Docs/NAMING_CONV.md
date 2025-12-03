# Naming Conventions


In C++, **naming conventions** help make code more readable, maintainable, and consistent. While there isn't a strict, one-size-fits-all rule for naming conventions, there are some general best practices that are widely adopted in the C++ community. These conventions can vary slightly depending on the project or organization, but I’ll cover the most common ones:

## **1. General Naming Conventions**

### **a. Variable and Function Names**

* **Lower Camel Case**: Start with a lowercase letter and capitalize subsequent words (e.g., `totalAmount`, `calculateSum`).

  * **Example**:

    ```cpp
    int totalAmount;
    double calculateSum(int a, int b) { return a + b; }
    ```

### **b. Class and Struct Names**

* **Upper Camel Case (Pascal Case)**: Start each word with a capital letter (e.g., `CarModel`, `SensorData`).

  * **Example**:

    ```cpp
    class CarModel {
    public:
        void startEngine();
    };
    ```

### **c. Constants and Macros**

* **All Uppercase with Underscores**: Use all uppercase letters with underscores to separate words (e.g., `MAX_BUFFER_SIZE`, `PI_CONSTANT`).

  * **Example**:

    ```cpp
    #define MAX_BUFFER_SIZE 1024
    const double PI_CONSTANT = 3.14159;
    ```

### **d. Member Variables**

* **Prefix with `m_` or `s_` for Static Variables**: Member variables in classes often start with `m_` to distinguish them from local variables or parameters. Static member variables may start with `s_`.

  * **Example**:

    ```cpp
    class Car {
    private:
        int m_speed;   // member variable
        static int s_count;  // static member variable
    public:
        void setSpeed(int speed) { m_speed = speed; }
    };
    ```

### **e. Function Parameters**

* **Lowercase with Underscores**: Function parameters generally use lowercase letters, with underscores for multiple words.

  * **Example**:

    ```cpp
    void setSpeed(int speed_limit);
    ```

## **2. Specific Naming Practices**

### **a. Namespaces**

* **Lowercase**: Typically, namespaces are written in lowercase, though some projects may use `CamelCase` (depends on project conventions).

  * **Example**:

    ```cpp
    namespace utility {
        void printMessage(const std::string& msg);
    }
    ```

### **b. Templates**

* **Upper Camel Case**: Template parameters often follow **PascalCase**. For example, `T` or `ValueType`.

  * **Example**:

    ```cpp
    template <typename T>
    void swap(T& a, T& b);
    ```

### **c. Classes vs. Structs**

* **Class names** are usually **PascalCase** (first letter uppercase).
* **Struct names** are generally the same, but some projects might use **lowercase** or different conventions to distinguish them from classes.

  * **Example**:

    ```cpp
    class Employee { ... };
    struct Point { ... };
    ```

## **3. Special Case: `getters` and `setters`**

* **Getters**: The name should be `get<T>()` or something more descriptive, like `getName()`.
* **Setters**: The name should be `set<T>()`, or `setX()`.

  * **Example**:

    ```cpp
    class Person {
    private:
        std::string m_name;
    public:
        std::string getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
    };
    ```

## **4. File and Folder Naming Conventions**

* **Source files** (`.cpp`): Use lowercase and underscores (e.g., `car_model.cpp`, `sensor_data.cpp`).
* **Header files** (`.h` or `.hpp`): Also typically lowercase with underscores (e.g., `car_model.h`, `sensor_data.hpp`).
* **Classes in Header Files**: Match the header file name to the class name (e.g., `CarModel.h` for `CarModel` class).

  * **Example**:

    * `car_model.cpp` - the source file for the `CarModel` class.
    * `car_model.h` - the header file for the `CarModel` class.

### **Folder Names**

* Use lowercase or lowercase with hyphens (`-`) for folder names in projects.

  * **Example**:

    ```
    src/
    include/
    lib/
    ```

## **5. Naming Conventions for C++ Features**

### **a. Exception Classes**

* Exception class names typically use **PascalCase** and end with `Exception` or `Error`.

  * **Example**:

    ```cpp
    class FileNotFoundException : public std::exception { ... };
    ```

### **b. Enumerations**

* Enum types use **PascalCase**, and enum constants are usually **ALL_CAPS**.

  * **Example**:

    ```cpp
    enum class Color {
        RED,
        GREEN,
        BLUE
    };
    ```

### **c. Smart Pointers**

* **std::unique_ptr**, **std::shared_ptr**, **std::weak_ptr** should be used without custom naming, but **`ptr`** can be used as a common suffix for variables holding pointers.

  * **Example**:

    ```cpp
    std::unique_ptr<Car> carPtr;
    ```

## **6. Miscellaneous**

* **Avoid single-character variable names** unless used for counters or iterators (e.g., `i`, `j`).
* **Avoid unnecessary abbreviations** in variable or class names (e.g., `calculateSpeed()` is better than `calcSpeed()`).

### **Example of a C++ Class with Proper Naming Conventions**:

```cpp
// CarModel.h
#pragma once

class CarModel {
private:
    int m_speed;
    std::string m_modelName;

public:
    CarModel(const std::string& modelName, int speed);
    
    int getSpeed() const;
    void setSpeed(int speed);
    
    void accelerate(int increment);
    void brake(int decrement);
};

// CarModel.cpp
#include "CarModel.h"

CarModel::CarModel(const std::string& modelName, int speed)
    : m_modelName(modelName), m_speed(speed) {}

int CarModel::getSpeed() const {
    return m_speed;
}

void CarModel::setSpeed(int speed) {
    m_speed = speed;
}

void CarModel::accelerate(int increment) {
    m_speed += increment;
}

void CarModel::brake(int decrement) {
    m_speed -= decrement;
}
```

## Summary:

* **Use CamelCase** for variables and function names.
* **Use PascalCase** for class names and struct names.
* **Use ALL_CAPS** for constants and macros.
* **Member variables** can use the `m_` prefix (or `_` depending on your style guide).
* Keep **naming consistent** throughout your codebase.

If you're working in a team or contributing to an existing project, make sure to **check the project's style guide** and adhere to its conventions. Having a consistent naming scheme helps everyone understand the code better and reduces errors in large projects.

Let me know if you need more specific examples or have questions about particular C++ conventions!
