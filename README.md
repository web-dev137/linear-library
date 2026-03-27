## Matrix calculator

Matrix Calculator is built on top of an extensible matrix operations library that provides template‑based support for multiple numeric types, intuitive arithmetic through overloaded operators, a robust Rule‑of‑Five matrix implementation, and a flexible architecture using the Strategy and Factory patterns to enable adding new operations without switch statements.


## Install

In first you should clone project.
After this you need build project(from the root folder of the project):

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
```
You see in builde executable file app.exe

## Run

For  run app enter in build folder and enter next command:

./app

## Test
After build project in build/tests you can run ./tests 
## Usage
Choice operation.
Enter number from menu.
## Examples

A*B
Enter size of matrix A <rows> <columns> for example: 2 2
Fill matrix:
![Alt text](docs/multiply.png)

A*number
![Alt text](docs/scalar.png)

Transponse matrix
![Alt text](docs/transponse.png)