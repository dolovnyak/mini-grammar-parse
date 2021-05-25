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
### There is an mgp structure, it consists of lexemes list and grammar rules list. You need configure it with:
- `mgp = mgp_init();` to initialize it.
- `mgp_lexeme_add(...);` to add lexeme to lexemes list.
- `mgp_grammar_rule_add(...);` to add grammar rules to grammar rules list.
- `mgp_parse(...);` to parse one line, when you completely set up mgp (If you have multiply lines you need to parse it line by line).
###
### There is a lexeme, it consists of:
- `template` - the pattern to which a part of the string will be compared, for example {N}{i}{c}{e}{ }{0-9+}{_+}{a+} - it could be string "Nice 9661247____aaaaaaaaa", there are four types of template:
   - `{a}` "a" - one symbol
   - `{,+}` ",,,,,,,," - many repetitions of a symbol
   - `{0-9}` "7" - any symbol inside the range
   - `{a-z+}` "adsfgasdgdafgqrsghfg" - many repetitions of any symbol inside the range
- `token` - the name of the lexeme, the grammar rule will be specified using these names.
- `parse function` - the function pointer that will parse a string that matches the pattern and fill t_lex_data, and return true if correct and false if there is error (it's your custom function which you need to implement)
```
int     mgp_lexeme_add(          // it return true if success and false if error, also print error in STDERR_FILENO
                t_mgp *mgp,      // each call mgp_lexeme_add configurate and change mgp structure
                const char *template, // {0-9+}{.}{0-9+} - "1345123.123554"
                const char *token,    // POSITIVE_DOUBE
                int (*parse_func)(const char *body, t_lex_data *data));  // pointer on function which parse double from body and fill t_lexe_data (body - the part of the string that matched the pattern),
```
###
### There is a grammar rule, it consists of:
- `string of lexeme names` - lexemes that make up the grammar rule - "POSITIVE_DOUBLE SPACES POSITIVE_INT/NEGATIVE_INT NEW_LINE" (POSITIVE_INT/NEGATIVE_INT means that there can be one of several lexemes)
- `parse function` - function which will take array of t_lex_data if the string matches the grammar rule, and fill your custom (any) void *data, return true if correct and false if error.
```
int     mgp_grammar_rule_add(           // it return true if success and false if error, also print error in STDERR_FILENO
                t_mgp *mgp,             // each call mgp_grammar_rule_add configurate and change mgp structure
                const char *body,       // token list - "SCREEN_RESOLUTION SPACES POSITIVE_INT SPACES POSITIVE_INT NEW_LINE"
	        int (*parse_func)(t_lex_data *lexemes_data, void *data)); // your custom function which will parse lexemes_data, which were filled with your custom functions on the lexemes.
```
###
### There is a parse line function
```
int	mgp_parse(			// it return true if csuccess and false if error, aslo print error in STDERR_FILENO
		const t_mgp *mgp,	// it's mgp structure you set up before
		const char *line,	// line which you parse
		void *data);		// pointer to your custom structure (or pointer to any type) into which data is parsed using your custom functions, which you pass to grammar rules.
```
Example:
--------
```
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mini_grammar_parse.h"

#define EXAMPLE_FILE_NAME "example.txt"
#define SUCCESS 1
#define FAILURE 0

// your custom structure
typedef struct  s_circle
{
    double x;
    double y;
    double z;
    double radius;
    int    color;
}               t_circle;

// your custom error function
int	error(const char **error_strings)
{
    int	i;

    i = 0;
    write(STDERR_FILENO, "ERROR: ", 7);
    while (error_strings[i])
    {
        write(STDERR_FILENO, error_strings[i], strlen(error_strings[i]));
        ++i;
    }
    write(STDERR_FILENO, "\n", 1);
    return (FAILURE);
}

// your custom function which parse string to double
int	parse_double(const char *body, t_lex_data *data)
{
    data->data_type = DoubleNum;
    data->raw_data.num_double = strtod(body, NULL);
    return (SUCCESS);
}

// your custom function which parse string to int
int	parse_int(const char *body, t_lex_data *data)
{
    data->data_type = IntegerNum;
    data->raw_data.num_int = (int)strtol(body, NULL, 10);
    return (SUCCESS);
}

// your custom function which parse circle
int	parse_circle(t_lex_data *lexemes_data, void *v_circle)
{
    // if you want you can add check for correct:
    if (lexemes_data[0].data_type != DoubleNum || lexemes_data[2].data_type != DoubleNum || lexemes_data[4].data_type != DoubleNum  // position xyz
        || lexemes_data[6].data_type != DoubleNum  // radius
        || lexemes_data[8].data_type != IntegerNum)  // color
        return (FAILURE);

    t_circle *circle = (t_circle *)v_circle;
    circle->x = lexemes_data[0].raw_data.num_double;
    circle->y = lexemes_data[2].raw_data.num_double;
    circle->z = lexemes_data[4].raw_data.num_double;
    circle->radius = lexemes_data[6].raw_data.num_double;
    circle->color = lexemes_data[8].raw_data.num_int;
    // lexemes_dara[1/3/5/7] - are spaces
    return (SUCCESS);
}

int main()
{
    t_mgp mgp;

    //SETUP MGP
    mgp = mgp_init();

    // add negative double lexeme, example "-6342.234"
    if (!mgp_lexeme_add(&mgp, "{-}{0-9+}{.}{0-9+}", "N_DOUBLE", parse_double))
        return (error((const char *[]){"N_DOUBLE", NULL}));

    // add positive double lexeme, example "6342.234"
    if (!mgp_lexeme_add(&mgp, "{0-9+}{.}{0-9+}", "DOUBLE", parse_double))
        return (error((const char *[]){"DOUBLE", NULL}));

    // add negative int lexeme, example "-6342"
    if (!mgp_lexeme_add(&mgp, "{-}{0-9+}", "N_INT", parse_int))
        return (error((const char *[]){"N_INT", NULL}));

    // add positive int lexeme, example "6342"
    if (!mgp_lexeme_add(&mgp, "{0-9+}", "INT", parse_int))
        return (error((const char *[]){"INT", NULL}));

    // add multiply spaces lexeme, example "           " (a lot of spaces)
    if (!mgp_lexeme_add(&mgp, "{ +}", "SPACES", NULL))
        return (error((const char *[]){"SPACES", NULL}));

    // add new line lexeme "\n"
    if (!mgp_lexeme_add(&mgp, "{\n}", "NEW_LINE", NULL))
        return (error((const char *[]){"NEW_LINE", NULL}));

    // add grammar rule which parse string only with spaces
    if (!mgp_grammar_rule_add(&mgp, "SPACES", NULL))  // WHEN THE FUNCTION POINTER IS NULL - IT DOES NOTHING
        return (FAILURE);

    // add grammar rule which parse string only with new line
    if (!mgp_grammar_rule_add(&mgp, "NEW_LINE", NULL))
        return (FAILURE);

    // add grammar rule which parse string with spaces and new line
    if (!mgp_grammar_rule_add(&mgp, "SPACES NEW_LINE", NULL))
        return (FAILURE);

    // add grammar rule which parse circle
    if (!mgp_grammar_rule_add(&mgp, "N_DOUBLE/DOUBLE SPACES N_DOUBLE/DOUBLE SPACES N_DOUBLE/DOUBLE SPACES N_DOUBLE/DOUBLE SPACES N_INT/INT NEW_LINE", parse_circle))
        return (FAILURE);

    const char *str = "13.6 -125.65 342.54      4.5       42145\n";
    t_circle circle;

    if (!mgp_parse(&mgp, str, &circle))
            return (error((const char *[]){"Parse line \"", str, "\"", NULL}));

    printf("pos - (%f, %f, %f), radius - %f, color - %d", circle.x, circle.y, circle.z, circle.radius, circle.color);

    return 0;
}
```
