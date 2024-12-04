/**
 * @brief functions return flags to indicate success/failure etc;
 *
*/

#ifndef BBFLAG_H
#define BBFLAG_H

//some modules use positive values for indices and negative values for flags
#define f_Success     0
#define f_None        (-1)

// the vast majority of functions return bbFlag, and other values are
// returned by reference. names had better not overwrite other C keywords
typedef enum {
    Success,
    None,
    Full,
    Break,
    Repeat,
    Continue,
    Delete,
    Head,
    Tail,
} bbFlag;

#endif // BBFLAG_H