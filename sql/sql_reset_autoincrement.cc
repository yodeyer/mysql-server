#include "sql/sql_reset_autoincrement.h"

#include "sql/dd/cache/dictionary_client.h"
#include "sql/thd_raii.h"
#include "sql/table_cache.h"

/**
  Reset autoincrement to 1

  @param[in] thd   thread handler
  @param[in] table_list table to reset

  @retval false OK
  @retval true Error
*/
bool mysql_reset_autoincrement(THD *thd, TABLE_LIST *table_list) {
  DBUG_TRACE;

  dd::cache::Dictionary_client::Auto_releaser releaser(thd->dd_client());

  const dd::Abstract_table *at = nullptr;
  if (thd->dd_client()->acquire(table_list->db, table_list->table_name, &at)) return true;

  if (at == nullptr) {
    String tbl_name(table_list->db, system_charset_info);
    tbl_name.append('.');
    tbl_name.append(String(table_list->table_name, system_charset_info));

    my_error(ER_BAD_TABLE_ERROR, MYF(0), tbl_name.c_ptr());
    return true;
  } else if (at->type() != dd::enum_table_type::BASE_TABLE) {
    my_error(ER_WRONG_OBJECT, MYF(0), table_list->db, table_list->table_name, "TABLE");
    return true;
  }
  Alter_table_prelocking_strategy prelocking_strategy;
  if (open_and_lock_tables(thd, table_list, 0)) return true;
  if (table_list->table->file->reset_auto_increment()) return true;

  my_ok(thd);
  return false;
}
