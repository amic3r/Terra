
#ifndef __included_terra_arg_parser_h
#define __included_terra_arg_parser_h

void TArgParserInit(void);
void TArgParserDestroy(void);

/**
 * Allow a new argument to the parser
 *
 * @param name                The name of the argument. Only used for switches.
 * @param type                The type of the argument. Check printf.
 * @param amount              The amount of arguments allowed. '?' or NULL for 0 or 1, '0-9' for fixed or '+' for any.
 * @param def                 The default argument if none is provided (unused)
 * @param help                A description of the argument. used when printing usage.
 *
 */
void TArgParserAddArgument(const char *name, char type, char amount, char def, const char *help);

/**
 * Output the usage to stdout
 *
 */
void TArgParserHelp(void);

/**
 * Give the parser the data to parse
 *
 * @param argc                The amount of arguments.
 * @param argv                The arguments.
 *
 */
void TArgParserFeed(int argc, const char **argv);

/**
 * Parse arguments.
 *
 * @return                    The current argument.
 *
 */
const char *TArgParserNext(void);

/**
 * Parse argument parameters.
 *
 * @return                    The current parameter for the argument.
 *
 */
const void *TArgParserNextParameter(void);

#endif
