#ifndef SQL_RESET_AUTOINCREMENT_INCLUDED
#define SQL_RESET_AUTOINCREMENT_INCLUDED

#include "lex_string.h"

class THD;
struct TABLE_LIST;
struct TABLE_SHARE;

bool mysql_reset_autoincrement(THD *thd, TABLE_LIST *table);

#endif /* SQL_RESET_AUTOINCREMENT_INCLUDED */
