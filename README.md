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
```
add_executable(${PROJECT_NAME}
        main.c)

# You need to clone/copy mini-grammar-parse into your repository
add_subdirectory(mini-grammar-parse)
target_link_libraries(${PROJECT_NAME} ${MGP_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC ${MGP_INCLUDE_DIRECTORIES})
```
How to work with MGP:
-------
### There is mgp structure, it consists of lexemes list and grammar rules list. You need configure it with:
- `mgp = mgp_init();` to initialize it.
- `mgp_lexeme_add(...);` to add lexeme to lexemes list.
- `mgp_grammar_rule_add(...);` to add grammar rules to grammar rules list.
- `mgp_parse(...);` for parse file line by line, when you completely set up mgp.
###
### There is lexeme, it consists of:
- `template` - the pattern to which a part of the string will be compared, for example {N}{i}{c}{e}{ }{0-9+}{_+}{a+} - it could be string "Nice 1337____aaaaaaaaa", there are four types of template:
   - `{a}` "a" - one character
   - `{,+}` ",,,,,,,," - many repetitions of a symbol
   - `{0-9}` "7" - any symbol inside the range
   - `{a-z+}` "adsfgasdgdafgqrsghfg" - many repetitions of any symbol inside the range
- `token` - the name of the lexeme, the grammar rule will be specified using these names.
- `parse function` - the function pointer that will parse a string that matches the pattern and fill t_lex_data, and return true if correct and false if there is error
```
int     mgp_lexeme_add(          // it return true if success and false if error, also print error in STDERR_FILENO
                t_mgp *mgp,      // each call mgp_lexeme_add configurate and change mgp structure
                const char *template, // {0-9+}{.}{0-9+} - "1345123.123554"
                const char *token,    // POSITIVE_DOUBE
                int (*parse_func)(const char *body, t_lex_data *data));  // pointer on function which parse double from body and fill t_lexe_data (body - the part of the string that matched the pattern),
```
###
### There is grammar rule, it consists of:
- `string of lexeme names` - lexemes that make up the grammar rule - "POSITIVE_DOUBLE SPACES POSITIVE_INT/NEGATIVE_INT NEW_LINE" (POSITIVE_INT/NEGATIVE_INT means that there can be one of several lexemes)
- `parse function` - function which will take array of t_lex_data if the string matches the grammar rule, and fill your custom (any) void *data, return true if correct and false if error.
```
int     mgp_grammar_rule_add(           // it return true if success and false if error, also print error in STDERR_FILENO
                t_mgp *mgp,             // each call mgp_grammar_rule_add configurate and change mgp structure
                const char *body,       // token list - "SCREEN_RESOLUTION SPACES POSITIVE_INT SPACES POSITIVE_INT NEW_LINE"
	        int (*parse_func)(t_lex_data *lexemes_data, void *data)); // your custom function which will parse lexemes_data, which were filled with your custom functions on the lexemes.
```
###
