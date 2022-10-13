#ifndef EI_STRING_H
#define EI_STRING_H

#include <string.h>

/**
 * @brief Wrapper around the strcmp string comparisons
 * for a more user-friendly experience.
 *
 * @example STRCMP('toto', ==, 'tata')
 *
 */
#define STRCMP(a, R, b) (strcmp((a), (b)) R 0)

#endif