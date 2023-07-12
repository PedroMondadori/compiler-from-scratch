# Compiler From Scratch in C

## General Description of the Language

A program in the `CFS` language consists of a list of global declarations, which can be variables or functions, in any order, even interleaved. 

Each function is described by a header followed by its body, where the body of the function is a command. The commands can be assignments, flow control statements, or the print and return commands.

A block is also syntactically considered as a command, and it can appear in place of any command. 

Additionally, the language supports an empty command.

# Requirements

To build CFS you need to have installed:
- A C compiler (The project is configured to use gcc)
- Make
- Bison (yacc)
- Flex (lex)

```
sudo apt install build-essentials
sudo apt install bison
sudo apt install flex
```

# Usage

Compile the `CFS` compiler:
```
make
```
Create a .cfs file to put your code into and run the following command:
```
./bin/cfs {PATH_TO_YOUR_CODE} {NAME_OF_DECOMPILED_OUTPUT}.cfs
```

`CFS` does 2 things: 

- First, it compiles your code into an out.s file with the generated assembly code

You can disable the compiler's output by commenting out the DEBUG_INFO_FLAGS in the Makefile.

- Second, it decompiles the generated assembly code back into a .cfs file.

*This was originally done for testing, and comparing the input and output .cfs files.*

After running `CFS`, compile your out.s file into an executable
```
gcc out.s -o name_of_your_program
```

Finally, run the executable:
```
./name_of_your_program
```

## Variable Declaration

### Declaring Variables

In the `CFS` language, variables are declared using the following syntax:

```
TYPE IDENTIFIER : INITIAL_VALUE;
```
- The initialization value is mandatory for all variables but arrays.
- Variables of type `int` or `char` can be integers or characters literals.  
- Variables of type `float` are defined by a fraction of two integer literals separated by the '/' operator.
- All variable declarations are terminated by a semicolon (';').

### Declaring Arrays

The `CFS` language also supports the declaration of arrays.

Arrays can be of types `char`, `int`, or `float`.

```
\\ array declaration without initial values
TYPE IDENTIFIER[SIZE];

\\ array declaration with initial values
TYPE IDENTIFIER[SIZE] : VAL_1 VAL_2 VAL_3 ... VAL_SIZE;
```

It is not possible to initialize vectors of type `float` with initial values.

### Identifiers

Identifiers in `CFS` are used to designate variables, arrays and function names. They consist of a sequence of one or more alphabetic lowercase characters, that is, only in the interval `[a-z]`, and also the characters `'_'` and `'-'`, i.e the subtraction symbol, in which case it cannot be an identifier by itself.

Identifiers are global, which means you cannot have a variable named "a" and also have a function that takes in a parameter called "a".

### Examples

Here are examples of variable declarations in `CFS`:

```
int x: 10;
int a: 'A';
char y: 20;
char b: 'B';
float f: 3/2;
int matrix[100];
int numbers[5]: 1 2 3 4 5;
char vowels[5]: 'a' 'e' 'i' 'o' 'u';
float grades[3];
```

## Literals

In the `CFS` language, literals are used to represent constants in the source code. Here are the different types of literals and their representations:

- Integer literals: Integer literals are formed by a sequence of one or more decimal digits. For example, `42`, ``0``, `98765` are integer literals.

- Character literals: Character literals are represented by a single character enclosed in single quotes (apostrophes). For example, `'a'`, `'X'`, `'-'` are character literals. The character should be within the ASCII range, and it is represented by its decimal value (e.g., ASCII decimal 39 for the apostrophe).

- Float literals: There are no specific literals for float type in `CFS`.

- String literals: String literals are represented by any sequence of characters enclosed in double quotes. They are only used for printing messages using the `print` command. Consecutive strings cannot be treated as a single string, meaning that the double-quote character cannot be part of a string directly. To include double-quote and newline characters in a string, escape sequences should be used, like `"\""` and `"\n"`.


## Comments

`CFS` allows for single line and multi-line comments

```
\\ This is a single line comment

\*
This
is
a
multi-line
comment
*\
```

## Simple Commands

In the `CFS` language, commands can be classified into the following categories: assignment, flow control constructs, `print`, `return`, and the empty command.

- Empty command ( `{}` ): The empty command follows the same rules as other commands and, if it appears within a block, it should be terminated by a semicolon (';').

- Print command: This command is identified by the reserved keyword `print`, followed by a comma-separated list of elements to be printed. Each element can be a string literal or an expression to be printed.

- Return command: This command is identified by the reserved keyword `return`, followed by an expression that represents the return value of a function.

- Read command: This command is identified by the reserved keyword `read` and reads input from the user and returns it as an expression.

- Assignment: Assignments are performed using one of the following forms:

    ```
    variable = expression;
    array[expression] = expression;
    ```

The correct types for assignment and array indices are checked during semantic analysis.


## Operators

In the `CFS` language, the valid operators can be categorized as follows:

- Arithmetic operators: ```+, -, *, /```.

- Relational operators: ```<, >, <=, >=, ==, !=```.

Although the current version of the CFS language does not have explicit boolean expressions, boolean values can be the result of relational operators applied to arithmetic expressions.

## Function Declaration

In the `CFS` language, functions are declared using the following syntax:

```
TYPE IDENTIFIER(TYPE ARG_1, TYPE ARG_2, ... TYPE ARG_3) {


    // Block of code


    return VALUE;
}
```
- Function Header: The function header includes the return type, function name, and a list of input parameters (if any) enclosed in parentheses. Parameters are defined by their type and name, separated by commas.

- Function Body: The function body consists of a block command.

- Block Command: A block command is a sequence of commands enclosed in curly braces ('{' and '}'). It allows for the grouping of multiple commands, such as variable declarations and other statements, within the function body.

### Example of a program using a function in CFS

```
int sum: 0;
int result: 0;

int add(int a, int b) {
    sum = a + b;
    return sum;
}

int main() {
    result = add(2, 3);
    print(result);
}
```

## Flow Control

`CFS` has three constructs for controlling the flow of execution of a program.

### if then else
```
if BOOLEAN_EXPRESSION then {
    // do something
}
else {
    // do something else
}
```

### while
```
while BOOLEAN_EXPRESSION {
    // do something
}
```

### goto and labels
```
{
    label_a:

    ...

    goto label_a;
};
```

- For the `if then else` construct, the `then` clause can be omitted.
- For `if`, `else` and `while` constructs, curly braces ("{" and "}") can be omitted if the following expression is a single command.

## Todo

- General refactoring
- Finish adding compiler error messages
- Add some more optimizations to the generated assembly code
- Create more .cfs examples
- (Maybe) add more language features
- Fix bugs (😢)
