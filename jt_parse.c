#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "jt_types.h"
#include "jt_colour.h"
#include "jt_material.h"
#include "jt_vector.h"
#include "jt_ray.h"
#include "jt_primitive.h"
#include "jt_scene.h"
#include "jt_parse.h"

jt_float_t jt_parse_float (FILE *file)
{
    jt_float_t result;
    double buffer;
    fscanf (file, "%lf", &buffer);
    result = buffer;
    return result;
}

uint32_t jt_parse_int (FILE *file)
{
    unsigned long result;
    fscanf (file, "%lu", &result);
    return result;
}

jt_vector_t jt_parse_vector (FILE *file)
{
    jt_vector_t result;
    double buffer;
    fscanf (file, "%lf", &buffer);
    result.x = buffer;
    fscanf (file, "%lf", &buffer);
    result.y = buffer;
    fscanf (file, "%lf", &buffer);
    result.z = buffer;
    return result;
}

jt_colour_t jt_parse_colour (FILE *file)
{
    jt_colour_t result;
    double buffer;
    fscanf (file, "%lf", &buffer);
    result.r = buffer;
    fscanf (file, "%lf", &buffer);
    result.g = buffer;
    fscanf (file, "%lf", &buffer);
    result.b = buffer;
    return result;
}

jt_scene_t *jt_parse_scene (char *filename)
{
    jt_bool_t parse_okay = JT_TRUE;
    jt_scene_t *scene = NULL;
    char  buffer[80]; /* TODO: Long words -> crash */
    FILE *file = fopen (filename, "r");
    int material_index = 0;
    int primitive_index = 0;

    if (file == NULL)
    {
        fprintf (stderr, "Error: Unable to open file: %s.\n", filename);
        return NULL;
    }

    scene = malloc (sizeof (jt_scene_t));
    if (scene == NULL)
    {
        fprintf (stderr, "Error: Unable to malloc scene.\n");
        return NULL;
    }
    scene->material = NULL;
    scene->material_count = 0;
    scene->primitive = NULL;
    scene->primitive_count = 0;

    while (fscanf (file, "%s", buffer) != EOF)
    {
        /* Comment */
        if (!strcmp ("#", buffer))
        {
            int c;
            do
            {
                c = fgetc (file);
            } while (c != '\n' && c != EOF);
        }

        else if (!strcmp ("Eye", buffer))
        {
            scene->eye = jt_parse_vector (file);
            fscanf (file, "%s", buffer);
            if (strcmp (";", buffer))
            {
                fprintf (stderr, "Syntax error in %s: Expected ';' after 'Eye'.\n",
                         filename);
                parse_okay = JT_FALSE;
                break;
            }
        }

        else if (!strcmp ("Up", buffer))
        {
            scene->up = jt_parse_vector (file);
            fscanf (file, "%s", buffer);
            if (strcmp (";", buffer))
            {
                fprintf (stderr, "Syntax error in %s: Expected ';' after 'Up'.\n",
                         filename);
                parse_okay = JT_FALSE;
                break;
            }
        }

        else if (!strcmp ("Lookat", buffer))
        {
            scene->lookat = jt_parse_vector (file);
            fscanf (file, "%s", buffer);
            if (strcmp (";", buffer))
            {
                fprintf (stderr, "Syntax error in %s: Expected ';' after 'Lookat'.\n",
                         filename);
                parse_okay = JT_FALSE;
                break;
            }
        }

        else if (!strcmp ("Fov", buffer))
        {
            scene->fov = jt_parse_float (file) / 0.0174532925;
;
            fscanf (file, "%s", buffer);
            if (strcmp (";", buffer))
            {
                fprintf (stderr, "Syntax error in %s: Expected ';' after 'Fov'.\n",
                         filename);
                parse_okay = JT_FALSE;
                break;
            }
        }

        else if (!strcmp ("Background", buffer))
        {
            scene->background = jt_parse_colour (file);
            fscanf (file, "%s", buffer);
            if (strcmp (";", buffer))
            {
                fprintf (stderr, "Syntax error in %s: Expected ';' after 'Background'.\n",
                         filename);
                parse_okay = JT_FALSE;
                break;
            }
        }

        else if (!strcmp ("Material_count", buffer))
        /* For now: Trust that the user has not used this line twice... */
        {
            scene->material_count = jt_parse_int (file);
            fscanf (file, "%s", buffer);
            if (strcmp (";", buffer))
            {
                fprintf (stderr, "Syntax error in %s: Expected ';' after 'Material_count'.\n",
                         filename);
                parse_okay = JT_FALSE;
                break;
            }
            scene->material = malloc (scene->material_count * sizeof (jt_material_t));
            if (scene->material == NULL)
            {
                fprintf (stderr, "Error: Unable to allocate memory for %d materials.\n",
                          scene->material_count);
                parse_okay = JT_FALSE;
                break;
            }
        }

        else if (!strcmp ("Primitive_count", buffer))
        {
            scene->primitive_count = jt_parse_int (file);
            fscanf (file, "%s", buffer);
            if (strcmp (";", buffer))
            {
                fprintf (stderr, "Syntax error in %s: Expected ';' after 'Primitive_count'.\n",
                         filename);
                parse_okay = JT_FALSE;
                break;
            }
            scene->primitive = malloc (scene->primitive_count * sizeof (jt_primitive_t));
            if (scene->primitive == NULL)
            {
                fprintf (stderr, "Error: Unable to allocate memory for %d primitives.\n",
                          scene->primitive_count);
                parse_okay = JT_FALSE;
                break;
            }
        }

        else if (!strcmp ("Material", buffer))
        {
            if (material_index < scene->material_count)
            {
                scene->material[material_index].colour = jt_parse_colour (file);
                scene->material[material_index].shine  = jt_parse_float  (file);
                material_index++;
                fscanf (file, "%s", buffer);
                if (strcmp (";", buffer))
                {
                    fprintf (stderr, "Syntax error in %s: Expected ';' after 'Material'.\n",
                             filename);
                    parse_okay = JT_FALSE;
                    break;
                }
            }
            else
            {
                    fprintf (stderr, "Error in %s: More that %d materials.\n",
                             filename, scene->material_count);
                    parse_okay = JT_FALSE;
                    break;
            }
        }

        else if (!strcmp ("Sphere", buffer))
        {
            if (primitive_index < scene->primitive_count)
            {
                scene->primitive[primitive_index].intersect = jt_sphere_intersect;
                scene->primitive[primitive_index].sphere.centre = jt_parse_vector (file);
                scene->primitive[primitive_index].sphere.radius = jt_parse_float  (file);
                scene->primitive[primitive_index].material  = &scene->material[jt_parse_int (file)];
                primitive_index++;
                fscanf (file, "%s", buffer);
                if (strcmp (";", buffer))
                {
                    fprintf (stderr, "Syntax error in %s: Expected ';' after 'Sphere'.\n",
                             filename);
                    parse_okay = JT_FALSE;
                    break;
                }
            }
            else
            {
                    fprintf (stderr, "Error in %s: Sphere: More that %d primitives.\n",
                             filename, scene->primitive_count);
                    parse_okay = JT_FALSE;
                    break;
            }
        }
    }

    fclose (file);

    if (material_index != scene->material_count)
    {
        fprintf (stderr, "Error in %s: Fewer that %d materials.\n",
                 filename, scene->material_count);
        parse_okay = JT_FALSE;
    }

    if (primitive_index != scene->primitive_count)
    {
        fprintf (stderr, "Error in %s: Fewer that %d primitives.\n",
                 filename, scene->primitive_count);
        parse_okay = JT_FALSE;
    }

    if (parse_okay)
        return scene;
    /* TODO: Free on error case */
    return NULL;
}
