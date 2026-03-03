#Matrix calculator

Simple matrix calculator with next actions:
matrix multiplication, matrix multiply on number,transponse matrix.


# Install

In first you should clone project.
After this you need build project(from the root folder of the project):

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
```
You see in builde executable file app.exe

# Run

For  run app enter in build folder and enter next command:

./app

## Test
After build project in build/tests you can run ./tests 
## Usage
Choice operation.
Enter number from menu.
## Example
```text
A*B
Enter size of matrix A <rows> <columns> for example: 2 2
Fill matrix:
You will see position for every digital like this:
```text
[0][0]1
[0][1]2
[1][0]3
[1][1]4
```
Enter size of matrix B <rows> <columns> for example: 2 2 
Fill matrix:
You will see position for every digital like this:
```text
[0][0]5
[0][1]6
[1][0]7
[1][1]8
```
And you will see result:
 Результат(Result): 
 ```text
 19 22
 43 50
```
A*number

Enter size of matrix A <rows> <columns> for example: 2 2
Fill matrix:
You will see position for every digital like this:
```text
[0][0]1
[0][1]2
[1][0]3
[1][1]4
```
Enter number: <number> for example 2

And you will see result:
 Результат(Result): 
 ```text
 2 4
 6 8
```