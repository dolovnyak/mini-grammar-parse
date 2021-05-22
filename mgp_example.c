#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "mini_grammar_parse.h"

#define EXAMPLE_FILE_NAME "example.txt"
#define SUCCESS 1
#define FAILURE 0

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

int	parse_double(const char *body, t_lex_data *data)
{
    data->data_type = DoubleNum;
    data->raw_data.num_double = strtod(body, NULL);
    return (SUCCESS);
}

int	parse_int(const char *body, t_lex_data *data)
{
    data->data_type = IntegerNum;
    data->raw_data.num_int = (int)strtol(body, NULL, 10);
    return (SUCCESS);
}

typedef struct  s_vector3
{
    double      x;
    double      y;
    double      z;
}               t_vector3;

typedef struct  s_sphere
{
    t_vector3   position;
    double      radius;
}               t_sphere;

typedef struct 	s_camera
{
    t_vector3   position;
    t_vector3   direction;
}               t_camera;

typedef struct	s_scene_conf
{
    int         width;
    int         height;
    t_sphere    sphere;
    t_camera    camera;
}               t_scene_conf;

/**
 * You can write utilities for more convenient parse or create your own lexer types and function which parse it.
 */
t_vector3   get_vector3(t_lex_data lex_data1, t_lex_data lex_data2, t_lex_data lex_data3)
{
    return ((t_vector3){lex_data1.raw_data.num_double, lex_data2.raw_data.num_double, lex_data3.raw_data.num_double});
}

/**
 * Your custom functions which you need to write for parsing grammar rules
 */
int	parse_resolution(t_lex_data *lexemes_data, void *v_scene_conf)
{
    t_scene_conf	*scene_conf;

    if (lexemes_data[2].data_type != IntegerNum
        || lexemes_data[4].data_type != IntegerNum)
        return (error((const char *[]){"parse_resolution", NULL}));
    scene_conf = (t_scene_conf *)v_scene_conf;
    scene_conf->width = lexemes_data[2].raw_data.num_int;
    scene_conf->height = lexemes_data[4].raw_data.num_int;
    return (SUCCESS);
}

int	parse_camera(t_lex_data *lexemes_data, void *v_scene_conf)
{
    t_scene_conf    *scene_conf;
    t_camera        camera;

    if (lexemes_data[2].data_type != DoubleNum
        || lexemes_data[4].data_type != DoubleNum
        || lexemes_data[6].data_type != DoubleNum
        || lexemes_data[8].data_type != DoubleNum
        || lexemes_data[10].data_type != DoubleNum
        || lexemes_data[12].data_type != DoubleNum)
        return (error((const char *[]){"parse_camera", NULL}));
    scene_conf = (t_scene_conf *)v_scene_conf;
    camera.position = get_vector3(lexemes_data[2], lexemes_data[4], lexemes_data[6]);
    camera.direction = get_vector3(lexemes_data[8], lexemes_data[10], lexemes_data[12]);
    scene_conf->camera = camera;
    return (SUCCESS);
}

int	parse_sphere(t_lex_data *lexemes_data, void *v_scene_conf)
{
    t_scene_conf    *scene_conf;
    t_sphere        sphere;

    if (lexemes_data[2].data_type != DoubleNum
        || lexemes_data[4].data_type != DoubleNum
        || lexemes_data[6].data_type != DoubleNum
        || lexemes_data[8].data_type != DoubleNum)
        return (error((const char *[]){"parse_sphere", NULL}));
    scene_conf = (t_scene_conf *)v_scene_conf;
    sphere.position = get_vector3(lexemes_data[2], lexemes_data[4], lexemes_data[6]);
    sphere.radius = lexemes_data[8].raw_data.num_double;
    scene_conf->sphere = sphere;
    return (SUCCESS);
}

int	add_lexemes(t_mgp *mgp)
{
    // parse_double - your custom function which parse string to double
    // parse_int - your custom function which parse string to int

    // TEMPLATE: NEGATIVE DOUBLE WITH POINT
    if (!mgp_lexeme_add(mgp, "{-}{0-9+}{.}{0-9+}", "NP_DOUBLE", parse_double))
        return (error((const char *[]){"NP_DOUBLE", NULL}));
    // -6342.234

    // TEMPLATE: POSITIVE DOUBLE WITH POINT
    if (!mgp_lexeme_add(mgp, "{0-9+}{.}{0-9+}", "P_DOUBLE", parse_double))
        return (error((const char *[]){"P_DOUBLE", NULL}));
    // 6342.234

    // TEMPLATE: NEGATIVE DOUBLE WITHOUT POINT
    if (!mgp_lexeme_add(mgp, "{-}{0-9+}", "N_DOUBLE", parse_double))
        return (error((const char *[]){"N_DOUBLE", NULL}));
    // -6342

    // TEMPLATE: POSITIVE DOUBLE WITHOUT POINT
    if (!mgp_lexeme_add(mgp, "{0-9+}", "DOUBLE", parse_double))
        return (error((const char *[]){"DOUBLE", NULL}));
    // 6342

    // TEMPLATE: NEGATIVE INT
    if (!mgp_lexeme_add(mgp, "{-}{0-9+}", "N_INT", parse_int))
        return (error((const char *[]){"N_INT", NULL}));
    // -6342

    // TEMPLATE: POSITIVE INT
    if (!mgp_lexeme_add(mgp, "{0-9+}", "P_INT", parse_int))
        return (error((const char *[]){"P_INT", NULL}));
    // 6342

    // TEMPLATE: SKIP SPACES
    if (!mgp_lexeme_add(mgp, "{ +}", "SPACES", NULL))
        return (error((const char *[]){"SPACES", NULL}));
    // A LOT OF SPACES...

    // TEMPLATE: ONE COMMA
    if (!mgp_lexeme_add(mgp, "{,}", "COMMA", NULL))
        return (error((const char *[]){"COMMA", NULL}));
    // ,

    // TEMPLATE: NEW LINE
    if (!mgp_lexeme_add(mgp, "{\n}", "NEW_LINE", NULL))
        return (error((const char *[]){"NEW_LINE", NULL}));
    // \n

    // TEMPLATE: RESOLUTION IDENTIFIER NAME
    if (!mgp_lexeme_add(mgp, "{R}", "RESOLUTION", NULL))
        return (error((const char *[]){"RESOLUTION", NULL}));
    // R

    // TEMPLATE: CAMERA IDENTIFIER NAME
    if (!mgp_lexeme_add(mgp, "{C}", "CAMERA", NULL))
        return (error((const char *[]){"CAMERA", NULL}));
    // C

    // TEMPLATE: SPHERE IDENTIFIER NAME
    if (!mgp_lexeme_add(mgp, "{S}{P}", "SPHERE", NULL))
        return (error((const char *[]){"SPHERE", NULL}));
    // SP

    return (SUCCESS);
}

int	add_grammar_rules(t_mgp *mgp)
{
    if (!mgp_grammar_rule_add(mgp, "SPACES"  // STRING ONLY WITH SPACE
            , NULL))  // WHEN THE FUNCTION REFERENCE IS NULL - IT DOES NOTHING
        return (FAILURE);
    if (!mgp_grammar_rule_add(mgp, "NEW_LINE"  // STRING ONLY WITH NEW LINE
            , NULL))  // WHEN THE FUNCTION REFERENCE IS NULL - IT DOES NOTHING
        return (FAILURE);
    if (!mgp_grammar_rule_add(mgp, "SPACES NEW_LINE"  // STRING ONLY WITH SPACE AND NEW LINE
                              , NULL))  // WHEN THE FUNCTION REFERENCE IS NULL - IT DOES NOTHING
        return (FAILURE);
    if (!mgp_grammar_rule_add(mgp, "RESOLUTION SPACES P_INT SPACES P_INT NEW_LINE"  // WIDTH HEIGHT
                              , parse_resolution))  // FUNCTION THAT PARSES A STRING THAT MATCHES A GRAMMAR RULE
        return (FAILURE);
    if (!mgp_grammar_rule_add(mgp, "CAMERA SPACES " //CAMERA NAME
                                   "P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE COMMA P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE COMMA P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE"  // POSITION
                                   " SPACES "
                                   "P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE COMMA P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE COMMA P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE"  // DIRECTION
                                   " NEW_LINE", parse_camera))  // FUNCTION THAT PARSES A STRING THAT MATCHES A GRAMMAR RULE
        return (FAILURE);
    if (!mgp_grammar_rule_add(mgp, "SPHERE SPACES "  // SPHERE NAME
                                   "P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE COMMA P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE COMMA P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE"  // POSITION
                                   " SPACES "
                                   "P_DOUBLE/NP_DOUBLE/DOUBLE/N_DOUBLE"  // RADIUS
                                   " NEW_LINE", parse_sphere))  // FUNCTION THAT PARSES A STRING THAT MATCHES A GRAMMAR RULE
        return (FAILURE);
    return (SUCCESS);
}

/**
 * This example parsing little scene configuration with screen resolution, camera and sphere
 */
int main()
{
    t_mgp mgp;
    t_scene_conf scene_conf;

    //SETUP MGP
    mgp = mgp_init();
    if (!add_lexemes(&mgp))
        return (error((const char *[]){"add_lexemes", NULL}));
    if (!add_grammar_rules(&mgp))
        return (error((const char *[]){"add_grammar_rules", NULL}));

    //PARSE FILE DEPENDING ON MGP CONFIGURATIONS ABOVE
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    fp = fopen(EXAMPLE_FILE_NAME, "r");
    if (fp == NULL)
        return (error((const char *[]){"Open file \"", EXAMPLE_FILE_NAME, "\"", NULL}));

    while ((getline(&line, &len, fp)) != -1)
    {
        if (!*line)
            continue;
        if (!mgp_parse(&mgp, line, &scene_conf))
            return (error((const char *[]){"Parse line \"", line, "\"", NULL}));
    }
    fclose(fp);

    printf("RESOLUTION: %d/%d\n", scene_conf.width, scene_conf.height);
    printf("CAMERA: position - (%f, %f, %f), direction - (%f, %f, %f)\n",
           scene_conf.camera.position.x, scene_conf.camera.position.y, scene_conf.camera.position.z,
           scene_conf.camera.direction.x, scene_conf.camera.direction.y, scene_conf.camera.direction.z);
    printf("SPHERE: position - (%f, %f, %f), radius - %f\n",
           scene_conf.camera.position.x, scene_conf.camera.position.y, scene_conf.camera.position.z,
           scene_conf.sphere.radius);
    return 0;
}
