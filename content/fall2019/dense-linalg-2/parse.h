#include <argp.h>
#include <stdlib.h>

/* This structure is used by main to communicate with parse_opt. */
struct args
{
  size_t array_len;
  unsigned long max_value;
};

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] =
{
  {"array_len",'a',"size_t",0,"the number of elements to sort"},
  {"max_value",'m',"unsigned long",0,"the max allowed entry in the array"},
  {0}
};

/*
   PARSER. Field 2 in ARGP.
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct args *args = state->input;
  switch (key)
    {
    case ARGP_KEY_INIT:
      args->array_len = 15000;
      args->max_value = 1000000;
      break;
    case 'a':
      args->array_len = strtol(arg, NULL, 10);
      break;
    case 'm':
      args->max_value = strtol(arg, NULL, 10);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

void parse_args(struct args *args, int argc, char **argv)
{
  static struct argp argp = {options, parse_opt, NULL, NULL};
  argp_parse(&argp, argc, argv, 0, 0, args);
  return;
}
