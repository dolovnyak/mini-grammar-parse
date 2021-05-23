# mini-grammar-parse

Simple line by line parsing with lexemes and grammar rules
-------

Build and run example on `MacOS`:
-------
```
cmake -S . -B build
make -C build

#run example program:
./mini-grammar-parse-example
```
Build with your program through `gcc` on `MacOS` (after actions above):
-------
```
gcc main.c ${PATH_TO_LIB_FOLDER}/build/liblibmgp.a -I ${PATH_TO_LIB_FOLDER}/include
```
Build with your program through `cmake` on `MacOS`:
-------
