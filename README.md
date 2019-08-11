# JIT Calculator

jit-calc is a compiler of mathematical expressions, in C++.  
It works with ubunu 18.04.2 o a x64 machine, and it isn't portable.  
This project is only for curiosity, in order to understand and realize my own JIT compiler at least once.  

## Building

```
mkdir _build
cd _build
cmake ..
make
```

## Testing

```
python ../testsuite/run.py
```

## Usage

Interactive mode:
```
$ ./calc
```

A prompt will appear, where you can enter math expressions.

Direct evaluation:
```
$ ./calc <expression>
```

The calculator only handles integers, the 5 binary operators (+,-,*,/,%), and parentheses.


## How it works

Like a classic compiler, the expression is parsed into a really simple AST, then converted into a minimalistic IR language.  
Then I compile the IR into an X64 function that compute and returns the result. The binary code is written to an allocated buffer, that is executable thanks to `mprotect (2)`.  
I can then cast the buffer pointer to a function pointer, in order to be able to jump and execute the code.  

It only uses registers (no stack), but this was only to demonstrate the JIT concept, and I didn't used any register allocation or spilling algorithms, I just naively choose registers, and it doesn't work if there isn't enough registers.
