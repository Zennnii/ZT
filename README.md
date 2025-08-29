# ZT Compiler

ZT Compiler is a simple compiler written in C that translates programs written in the **ZT language** into C code. It currently supports basic variable handling, input, arithmetic, and control flow constructs.

---

## Features

- **Variable declaration:**  
  Use `let` to declare variables. Example:  
  ```zt
  let int num = 2
  ```

  ---

- **Variable assignment:**  
  Assign values to existing variables. Example:
  ```zt
  num = num + 4
  ```

---

- **User input**  
  Read input from the user into a variable. Example:
  ```zt
  input num1
  ```

---

- **Variable assignment:**  
  Assign values to existing variables. Example:
  * `if`
  * `else if`
  * `else`
  * `while`

  ---

- **Basic types:**  
  Currently supports numeric types (`int`, `float`, etc.)
  * **Note:** Booleans are not yet supported.

  ---
 ## **Getting started:**
 **Requirements**
 * A C compiler (e.g., `gcc`)
 * Basic knowledge of compiling C programs

 **Compiling the compiler:**
  ```Makefile
  make clean
  make all
  ```

  **Using the compiler:**
  ```zt
  ./zt source.zt -o output
  ```
  And then you can run the file normally:
  ```zt
  ./output
  ```
  Example source code is in `programs` folder

  ---

  **Compiler flags:**
  
  `keep-c`  Keeps the intermediate C file

  **Example:**
  ```zt
  ./zt source.zt -o output --keep-c
  ```


<br>

  `--run` Runs the compiled file automatically

  **Example:**
  ```zt
  ./zt source.zt -o output --run
  ```
  ---

  ## Limitations / TODO:
  * No boolean type yet
  * Limited error handling
  * Only basic arithmetic and control flow supported
  * Future improvements could include functions, arrays, and more advanced types

  ---