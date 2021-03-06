
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2012 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  |          Rack Lin <racklin@gmail.com>                                  |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_phalcon.h"
#include "phalcon.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"

#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"

/**
 * Phalcon\Db\Adapter\Pdo\Sqlite
 *
 * Specific functions for the Sqlite database system
 * <code>
 *
 * $config = array(
 *  "dbname" => "/tmp/test.sqlite"
 * );
 *
 * $connection = new Phalcon\Db\Adapter\Pdo\Sqlite($config);
 *
 * </code>
 */


/**
 * This method is automatically called in Phalcon\Db\Adapter\Pdo constructor.
 * Call it when you need to restore a database connection.
 *
 * @param array $descriptor
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo_Sqlite, connect){

	zval *descriptor = NULL, *dbname = NULL;
	int eval_int;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &descriptor) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!descriptor) {
		PHALCON_ALLOC_ZVAL_MM(descriptor);
		ZVAL_NULL(descriptor);
	} else {
		PHALCON_SEPARATE_PARAM(descriptor);
	}
	
	if (!zend_is_true(descriptor)) {
		PHALCON_INIT_VAR(descriptor);
		phalcon_read_property(&descriptor, this_ptr, SL("_descriptor"), PH_NOISY_CC);
	}
	eval_int = phalcon_array_isset_string(descriptor, SS("dbname"));
	if (!eval_int) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "dbname must be specified");
		return;
	} else {
		PHALCON_INIT_VAR(dbname);
		phalcon_array_fetch_string(&dbname, descriptor, SL("dbname"), PH_NOISY_CC);
		phalcon_array_update_string(&descriptor, SL("dsn"), &dbname, PH_COPY | PH_SEPARATE TSRMLS_CC);
	}
	
	PHALCON_CALL_PARENT_PARAMS_1_NORETURN(this_ptr, "Phalcon\\Db\\Adapter\\Pdo\\Sqlite", "connect", descriptor);
	
	PHALCON_MM_RESTORE();
}

/**
 * Returns an array of Phalcon\Db\Column objects describing a table
 *
 * <code>print_r($connection->describeColumns("posts")); ?></code>
 *
 * @param string $table
 * @param string $schema
 * @return Phalcon\Db\Column[]
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo_Sqlite, describeColumns){


	zval *table = NULL, *schema = NULL, *columns = NULL, *sql = NULL, *fetch_assoc = NULL;
	zval *describe = NULL, *old_column = NULL, *field = NULL, *definition = NULL;
	zval *column_type = NULL;
	zval *status = NULL, *matches = NULL, *pattern = NULL, *attribute = NULL, *column_name = NULL;
	zval *column = NULL, *dialect = NULL;
	zval *r0 = NULL, *r1 = NULL;
	zval *status2 = NULL;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;
	int eval_int;

	PHALCON_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table, &schema) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema) {
		PHALCON_ALLOC_ZVAL_MM(schema);
		ZVAL_NULL(schema);
	}

	PHALCON_INIT_VAR(columns);
	array_init(columns);

	PHALCON_ALLOC_ZVAL_MM(dialect);
	phalcon_read_property(&dialect, this_ptr, SL("_dialect"), PH_NOISY_CC);

	PHALCON_INIT_VAR(sql);
	PHALCON_CALL_METHOD_PARAMS_2(sql, dialect, "describecolumns", table, schema, PH_NO_CHECK);

	PHALCON_INIT_VAR(fetch_assoc);
	phalcon_get_class_constant(fetch_assoc, phalcon_db_ce, SL("FETCH_ASSOC") TSRMLS_CC);

	PHALCON_INIT_VAR(describe);
	PHALCON_CALL_METHOD_PARAMS_2(describe, this_ptr, "fetchall", sql, fetch_assoc, PH_NO_CHECK);

	PHALCON_INIT_VAR(old_column);
	ZVAL_NULL(old_column);

	if (!phalcon_valid_foreach(describe TSRMLS_CC)) {
		return;
	}

	ah0 = Z_ARRVAL_P(describe);
	zend_hash_internal_pointer_reset_ex(ah0, &hp0);

	ph_cycle_start_0:

		if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
			goto ph_cycle_end_0;
		}

		PHALCON_GET_FOREACH_VALUE(field);

		PHALCON_INIT_VAR(definition);
		array_init(definition);
		add_assoc_long_ex(definition, SS("bindType"), 2);

		PHALCON_INIT_VAR(column_type);
		phalcon_array_fetch_string(&column_type, field, SL("type"), PH_NOISY_CC);

		PHALCON_INIT_VAR(status);
		phalcon_fast_strpos_str(status, column_type, SL("int") TSRMLS_CC);
		if (PHALCON_IS_NOT_FALSE(status)) {
			add_assoc_long_ex(definition, SS("type"), 0);
			add_assoc_bool_ex(definition, SS("isNumeric"), 1);
			add_assoc_long_ex(definition, SS("size"), 32); // default 4byte
			add_assoc_long_ex(definition, SS("bindType"), 1);
		} else {
			PHALCON_INIT_VAR(status);
			phalcon_fast_strpos_str(status, column_type, SL("varchar") TSRMLS_CC);
			if (PHALCON_IS_NOT_FALSE(status)) {
				phalcon_array_update_string_long(&definition, SL("type"), 2, PH_SEPARATE TSRMLS_CC);
				phalcon_array_update_string_long(&definition, SL("size"), 255, PH_SEPARATE TSRMLS_CC);
			} else {
				PHALCON_INIT_VAR(status);
				phalcon_fast_strpos_str(status, column_type, SL("date") TSRMLS_CC);
				if (PHALCON_IS_NOT_FALSE(status)) {
					phalcon_array_update_string_long(&definition, SL("type"), 1, PH_SEPARATE TSRMLS_CC);
					phalcon_array_update_string_long(&definition, SL("size"), 0, PH_SEPARATE TSRMLS_CC);
				} else {
					PHALCON_INIT_VAR(status);
					PHALCON_INIT_VAR(status2);
					phalcon_fast_strpos_str(status, column_type, SL("numeric") TSRMLS_CC);
					phalcon_fast_strpos_str(status2, column_type, SL("decimal") TSRMLS_CC);
					if ( (PHALCON_IS_NOT_FALSE(status)) || (PHALCON_IS_NOT_FALSE(status2)) ) {
						add_assoc_long_ex(definition, SS("type"), 3);
						add_assoc_bool_ex(definition, SS("isNumeric"), 1);
						add_assoc_long_ex(definition, SS("size"), 32); // default 4byte
						add_assoc_long_ex(definition, SS("bindType"), 32);
					} else {
						PHALCON_INIT_VAR(status);
						phalcon_fast_strpos_str(status, column_type, SL("char") TSRMLS_CC);
						if (PHALCON_IS_NOT_FALSE(status)) {
							add_assoc_long_ex(definition, SS("type"), 5);
							phalcon_array_update_string_long(&definition, SL("size"), 255, PH_SEPARATE TSRMLS_CC);
						} else {
							PHALCON_INIT_VAR(status);
							phalcon_fast_strpos_str(status, column_type, SL("timestamp") TSRMLS_CC);
							if (PHALCON_IS_NOT_FALSE(status)) {
								add_assoc_long_ex(definition, SS("type"), 4);
								phalcon_array_update_string_long(&definition, SL("size"), 0, PH_SEPARATE TSRMLS_CC);
							} else {
								PHALCON_INIT_VAR(status);
								phalcon_fast_strpos_str(status, column_type, SL("text") TSRMLS_CC);
								if (PHALCON_IS_NOT_FALSE(status)) {
									add_assoc_long_ex(definition, SS("type"), 6);
									phalcon_array_update_string_long(&definition, SL("size"), 2147483647, PH_SEPARATE TSRMLS_CC);
								} else {
									PHALCON_INIT_VAR(status);
									phalcon_fast_strpos_str(status, column_type, SL("float") TSRMLS_CC);
									if (PHALCON_IS_NOT_FALSE(status)) {
										add_assoc_long_ex(definition, SS("type"), 7);
										add_assoc_bool_ex(definition, SS("isNumeric"), 1);
										add_assoc_long_ex(definition, SS("size"), 64); // default 4byte
										add_assoc_long_ex(definition, SS("bindType"), 32);
									} else {
										add_assoc_long_ex(definition, SS("type"), 5);
										PHALCON_INIT_VAR(status);
										phalcon_fast_strpos_str(status, column_type, SL("uuid") TSRMLS_CC);
										if (PHALCON_IS_NOT_FALSE(status)) {
											phalcon_array_update_string_long(&definition, SL("size"), 36, PH_SEPARATE TSRMLS_CC);
										} else {
											phalcon_array_update_string_long(&definition, SL("size"), 255, PH_SEPARATE TSRMLS_CC);
										}
									}
								}
							}
						}
					}
				}
			}
		}

		PHALCON_INIT_VAR(status);
		phalcon_fast_strpos_str(status, column_type, SL("(") TSRMLS_CC);
		if (PHALCON_IS_NOT_FALSE(status)) {
			PHALCON_INIT_VAR(matches);
			array_init(matches);

			PHALCON_INIT_VAR(pattern);
			ZVAL_STRING(pattern, "#\\(([0-9]+)(,[0-9]+)*\\)#", 1);
			Z_SET_ISREF_P(matches);

			PHALCON_INIT_VAR(r0);
			PHALCON_CALL_FUNC_PARAMS_3(r0, "preg_match", pattern, column_type, matches);
			Z_UNSET_ISREF_P(matches);
			if (zend_is_true(r0)) {
				eval_int = phalcon_array_isset_long(matches, 1);
				if (eval_int) {
					PHALCON_INIT_VAR(r1);
					phalcon_array_fetch_long(&r1, matches, 1, PH_NOISY_CC);
					phalcon_array_update_string(&definition, SL("size"), &r1, PH_COPY | PH_SEPARATE TSRMLS_CC);
				}
			}
		}

		PHALCON_INIT_VAR(status);
		phalcon_fast_strpos_str(status, column_type, SL("unsigned") TSRMLS_CC);
		if (PHALCON_IS_NOT_FALSE(status)) {
			phalcon_array_update_string_bool(&definition, SL("unsigned"), 1, PH_SEPARATE TSRMLS_CC);
		}

		if (Z_TYPE_P(old_column) == IS_NULL) {
			phalcon_array_update_string_bool(&definition, SL("first"), 1, PH_SEPARATE TSRMLS_CC);
		} else {
			phalcon_array_update_string(&definition, SL("after"), &old_column, PH_COPY | PH_SEPARATE TSRMLS_CC);
		}

		PHALCON_INIT_VAR(attribute);
		phalcon_array_fetch_string(&attribute, field, SL("pk"), PH_NOISY_CC);
		if (PHALCON_COMPARE_STRING(attribute, "1")) {
			phalcon_array_update_string_bool(&definition, SL("primary"), 1, PH_SEPARATE TSRMLS_CC);
		}else {
			phalcon_array_update_string_bool(&definition, SL("primary"), 0, PH_SEPARATE TSRMLS_CC);
		}

		PHALCON_INIT_VAR(attribute);
		phalcon_array_fetch_string(&attribute, field, SL("notnull"), PH_NOISY_CC);
		if (PHALCON_COMPARE_STRING(attribute, "1")) {
			phalcon_array_update_string_bool(&definition, SL("notNull"), 1, PH_SEPARATE TSRMLS_CC);
		}

		PHALCON_INIT_VAR(r0);
		phalcon_fast_strpos_str(r0, column_type, SL("integer") TSRMLS_CC);
		PHALCON_INIT_VAR(r1);
		phalcon_array_fetch_string(&r1, definition, SL("primary"), PH_NOISY_CC);
		if ((PHALCON_IS_NOT_FALSE(r0)) && (PHALCON_IS_NOT_FALSE(r1))) {
			phalcon_array_update_string_bool(&definition, SL("autoIncrement"), 1, PH_SEPARATE TSRMLS_CC);
		}

		PHALCON_INIT_VAR(column_name);
		phalcon_array_fetch_string(&column_name, field, SL("name"), PH_NOISY_CC);

		PHALCON_INIT_VAR(column);
		object_init_ex(column, phalcon_db_column_ce);
		PHALCON_CALL_METHOD_PARAMS_2_NORETURN(column, "__construct", column_name, definition, PH_CHECK);
		phalcon_array_append(&columns, column, PH_SEPARATE TSRMLS_CC);
		PHALCON_CPY_WRT(old_column, column_name);

		zend_hash_move_forward_ex(ah0, &hp0);
		goto ph_cycle_start_0;

	ph_cycle_end_0:


	RETURN_CTOR(columns);}

/**
 * Lists table indexes
 *
 * @param string $table
 * @param string $schema
 * @return Phalcon\Db\Index[]
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo_Sqlite, describeIndexes){


	zval *table = NULL, *schema = NULL, *dialect = NULL, *fetch_assoc = NULL, *sql = NULL, *sql2 = NULL;
	zval *describe = NULL, *indexes = NULL, *index = NULL, *key_name = NULL, *empty_arr = NULL, *column = NULL, *describe2 = NULL;
	zval *column_name = NULL, *index_objects = NULL, *index_columns = NULL;
	zval *name = NULL;
	HashTable *ah0, *ah1, *ah2;
	HashPosition hp0, hp1, hp2;
	zval **hd, **hd2;
	char *hash_index;
	uint hash_index_len;
	ulong hash_num;
	int hash_type;
	int eval_int;

	PHALCON_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table, &schema) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema) {
		PHALCON_ALLOC_ZVAL_MM(schema);
		ZVAL_NULL(schema);
	}

	PHALCON_INIT_VAR(dialect);
	phalcon_read_property(&dialect, this_ptr, SL("_dialect"), PH_NOISY_CC);

	PHALCON_INIT_VAR(fetch_assoc);
	phalcon_get_class_constant(fetch_assoc, phalcon_db_ce, SL("FETCH_ASSOC") TSRMLS_CC);

	PHALCON_INIT_VAR(sql);
	PHALCON_CALL_METHOD_PARAMS_2(sql, dialect, "describeindexes", table, schema, PH_NO_CHECK);

	PHALCON_INIT_VAR(describe);
	PHALCON_CALL_METHOD_PARAMS_2(describe, this_ptr, "fetchall", sql, fetch_assoc, PH_NO_CHECK);

	PHALCON_INIT_VAR(indexes);
	array_init(indexes);

	if (!phalcon_valid_foreach(describe TSRMLS_CC)) {
		return;
	}

	ah0 = Z_ARRVAL_P(describe);
	zend_hash_internal_pointer_reset_ex(ah0, &hp0);

	ph_cycle_start_0:

		if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
			goto ph_cycle_end_0;
		}

		PHALCON_GET_FOREACH_VALUE(index);

		PHALCON_INIT_VAR(key_name);
		phalcon_array_fetch_string(&key_name, index, SL("name"), PH_NOISY_CC);
		eval_int = phalcon_array_isset(indexes, key_name);
		if (!eval_int) {
			PHALCON_INIT_VAR(empty_arr);
			array_init(empty_arr);
			phalcon_array_update_zval(&indexes, key_name, &empty_arr, PH_COPY | PH_SEPARATE TSRMLS_CC);
		}

		// fetch index columns
		PHALCON_INIT_VAR(sql2);
		PHALCON_CALL_METHOD_PARAMS_1(sql2, dialect, "describeIndex", key_name, PH_NO_CHECK);

		PHALCON_INIT_VAR(describe2);
		PHALCON_CALL_METHOD_PARAMS_2(describe2, this_ptr, "fetchall", sql2, fetch_assoc, PH_NO_CHECK);

		if (phalcon_valid_foreach(describe2 TSRMLS_CC)) {

			ah2 = Z_ARRVAL_P(describe2);
			zend_hash_internal_pointer_reset_ex(ah2, &hp2);

			ph_cycle_start_2:

				if(zend_hash_get_current_data_ex(ah2, (void**) &hd2, &hp2) != SUCCESS){
					goto ph_cycle_end_2;
				}

				PHALCON_INIT_VAR(column);
				ZVAL_ZVAL(column, *hd2, 1, 0);

				PHALCON_INIT_VAR(column_name);
				phalcon_array_fetch_string(&column_name, column, SL("name"), PH_NOISY_CC);
				phalcon_array_update_append_multi_2(&indexes, key_name, column_name, 0 TSRMLS_CC);

				zend_hash_move_forward_ex(ah2, &hp2);
				goto ph_cycle_start_2;

			ph_cycle_end_2:
			if(0){}

		}

		zend_hash_move_forward_ex(ah0, &hp0);
		goto ph_cycle_start_0;

	ph_cycle_end_0:

	PHALCON_INIT_VAR(index_objects);
	array_init(index_objects);

	if (!phalcon_valid_foreach(indexes TSRMLS_CC)) {
		return;
	}

	ah1 = Z_ARRVAL_P(indexes);
	zend_hash_internal_pointer_reset_ex(ah1, &hp1);

	ph_cycle_start_1:

		if(zend_hash_get_current_data_ex(ah1, (void**) &hd, &hp1) != SUCCESS){
			goto ph_cycle_end_1;
		}

		PHALCON_INIT_VAR(name);
		PHALCON_GET_FOREACH_KEY(name, ah1, hp1);
		PHALCON_GET_FOREACH_VALUE(index_columns);

		PHALCON_INIT_VAR(index);
		object_init_ex(index, phalcon_db_index_ce);
		PHALCON_CALL_METHOD_PARAMS_2_NORETURN(index, "__construct", name, index_columns, PH_CHECK);
		phalcon_array_update_zval(&index_objects, name, &index, PH_COPY | PH_SEPARATE TSRMLS_CC);

		zend_hash_move_forward_ex(ah1, &hp1);
		goto ph_cycle_start_1;

	ph_cycle_end_1:


	RETURN_CTOR(index_objects);}

/**
 * Lists table references
 *
 * @param string $table
 * @param string $schema
 * @return Phalcon\Db\Reference[]
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo_Sqlite, describeReferences){


	zval *table = NULL, *schema = NULL, *dialect = NULL, *sql = NULL, *empty_arr = NULL, *references = NULL;
	zval *fetch_assoc = NULL, *describe = NULL, *reference = NULL, *constraint_name = NULL, *constraint_id = NULL;
	zval *referenced_schema = NULL, *referenced_table = NULL;
	zval *reference_array = NULL, *column_name = NULL, *referenced_columns = NULL;
	zval *reference_objects = NULL, *array_reference = NULL;
	zval *name = NULL, *columns = NULL, *definition = NULL;
	zval *t0 = NULL, *t1 = NULL, *t2 = NULL, *t3 = NULL;
	HashTable *ah0, *ah1;
	HashPosition hp0, hp1;
	zval **hd;
	char *hash_index;
	uint hash_index_len;
	ulong hash_num;
	int hash_type;
	int eval_int;

	PHALCON_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table, &schema) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema) {
		PHALCON_ALLOC_ZVAL_MM(schema);
		ZVAL_NULL(schema);
	}

	PHALCON_INIT_VAR(dialect);
	phalcon_read_property(&dialect, this_ptr, SL("_dialect"), PH_NOISY_CC);

	PHALCON_INIT_VAR(sql);
	PHALCON_CALL_METHOD_PARAMS_2(sql, dialect, "describereferences", table, schema, PH_NO_CHECK);

	PHALCON_INIT_VAR(empty_arr);
	array_init(empty_arr);

	PHALCON_INIT_VAR(references);
	array_init(references);

	PHALCON_INIT_VAR(fetch_assoc);
	phalcon_get_class_constant(fetch_assoc, phalcon_db_ce, SL("FETCH_ASSOC") TSRMLS_CC);

	PHALCON_INIT_VAR(describe);
	PHALCON_CALL_METHOD_PARAMS_2(describe, this_ptr, "fetchall", sql, fetch_assoc, PH_NO_CHECK);

	if (!phalcon_valid_foreach(describe TSRMLS_CC)) {
		return;
	}

	ah0 = Z_ARRVAL_P(describe);
	zend_hash_internal_pointer_reset_ex(ah0, &hp0);

	ph_cycle_start_0:

		if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
			goto ph_cycle_end_0;
		}

		PHALCON_GET_FOREACH_VALUE(reference);

		PHALCON_INIT_VAR(constraint_id);
		PHALCON_INIT_VAR(constraint_name);
		phalcon_array_fetch_string(&constraint_id, reference, SL("id"), PH_NOISY_CC);
		PHALCON_CONCAT_SV(constraint_name, "foreign_key_", constraint_id);
		eval_int = phalcon_array_isset(references, constraint_name);
		if (!eval_int) {
			PHALCON_INIT_VAR(referenced_schema);
			ZVAL_STRING(referenced_schema, "main", 1);

			PHALCON_INIT_VAR(referenced_table);
			phalcon_array_fetch_string(&referenced_table, reference, SL("table"), PH_NOISY_CC);

			PHALCON_INIT_VAR(reference_array);
			array_init(reference_array);
			phalcon_array_update_string(&reference_array, SL("referencedSchema"), &referenced_schema, PH_COPY | PH_SEPARATE TSRMLS_CC);
			phalcon_array_update_string(&reference_array, SL("referencedTable"), &referenced_table, PH_COPY | PH_SEPARATE TSRMLS_CC);
			phalcon_array_update_string(&reference_array, SL("columns"), &empty_arr, PH_COPY | PH_SEPARATE TSRMLS_CC);
			phalcon_array_update_string(&reference_array, SL("referencedColumns"), &empty_arr, PH_COPY | PH_SEPARATE TSRMLS_CC);
			phalcon_array_update_zval(&references, constraint_name, &reference_array, PH_COPY | PH_SEPARATE TSRMLS_CC);
		}

		PHALCON_INIT_VAR(column_name);
		phalcon_array_fetch_string(&column_name, reference, SL("from"), PH_NOISY_CC);
		if (Z_TYPE_P(references) == IS_ARRAY) {
			PHALCON_INIT_VAR(t0);
			phalcon_array_fetch(&t0, references, constraint_name, PH_SILENT_CC);
		}
		if (Z_REFCOUNT_P(t0) > 1) {
			phalcon_array_update_zval(&references, constraint_name, &t0, PH_COPY | PH_CTOR TSRMLS_CC);
		}
		if (Z_TYPE_P(t0) != IS_ARRAY) {
			convert_to_array(t0);
			phalcon_array_update_zval(&references, constraint_name, &t0, PH_COPY TSRMLS_CC);
		}
		if (Z_TYPE_P(t0) == IS_ARRAY) {
			PHALCON_INIT_VAR(t1);
			phalcon_array_fetch_string(&t1, t0, SL("columns"), PH_SILENT_CC);
		}
		if (Z_REFCOUNT_P(t1) > 1) {
			phalcon_array_update_string(&t0, SL("columns"), &t1, PH_COPY | PH_CTOR TSRMLS_CC);
		}
		if (Z_TYPE_P(t1) != IS_ARRAY) {
			convert_to_array(t1);
			phalcon_array_update_string(&t0, SL("columns"), &t1, PH_COPY TSRMLS_CC);
		}
		phalcon_array_append(&t1, column_name, 0 TSRMLS_CC);

		PHALCON_INIT_VAR(referenced_columns);
		phalcon_array_fetch_string(&referenced_columns, reference, SL("to"), PH_NOISY_CC);
		if (Z_TYPE_P(references) == IS_ARRAY) {
			PHALCON_INIT_VAR(t2);
			phalcon_array_fetch(&t2, references, constraint_name, PH_SILENT_CC);
		}
		if (Z_REFCOUNT_P(t2) > 1) {
			phalcon_array_update_zval(&references, constraint_name, &t2, PH_COPY | PH_CTOR TSRMLS_CC);
		}
		if (Z_TYPE_P(t2) != IS_ARRAY) {
			convert_to_array(t2);
			phalcon_array_update_zval(&references, constraint_name, &t2, PH_COPY TSRMLS_CC);
		}
		if (Z_TYPE_P(t2) == IS_ARRAY) {
			PHALCON_INIT_VAR(t3);
			phalcon_array_fetch_string(&t3, t2, SL("referencedColumns"), PH_SILENT_CC);
		}
		if (Z_REFCOUNT_P(t3) > 1) {
			phalcon_array_update_string(&t2, SL("referencedColumns"), &t3, PH_COPY | PH_CTOR TSRMLS_CC);
		}
		if (Z_TYPE_P(t3) != IS_ARRAY) {
			convert_to_array(t3);
			phalcon_array_update_string(&t2, SL("referencedColumns"), &t3, PH_COPY TSRMLS_CC);
		}
		phalcon_array_append(&t3, referenced_columns, 0 TSRMLS_CC);

		zend_hash_move_forward_ex(ah0, &hp0);
		goto ph_cycle_start_0;

	ph_cycle_end_0:

	PHALCON_INIT_VAR(reference_objects);
	array_init(reference_objects);

	if (!phalcon_valid_foreach(references TSRMLS_CC)) {
		return;
	}

	ah1 = Z_ARRVAL_P(references);
	zend_hash_internal_pointer_reset_ex(ah1, &hp1);

	ph_cycle_start_1:

		if(zend_hash_get_current_data_ex(ah1, (void**) &hd, &hp1) != SUCCESS){
			goto ph_cycle_end_1;
		}

		PHALCON_INIT_VAR(name);
		PHALCON_GET_FOREACH_KEY(name, ah1, hp1);
		PHALCON_GET_FOREACH_VALUE(array_reference);

		PHALCON_INIT_VAR(referenced_schema);
		phalcon_array_fetch_string(&referenced_schema, array_reference, SL("referencedSchema"), PH_NOISY_CC);

		PHALCON_INIT_VAR(referenced_table);
		phalcon_array_fetch_string(&referenced_table, array_reference, SL("referencedTable"), PH_NOISY_CC);

		PHALCON_INIT_VAR(columns);
		phalcon_array_fetch_string(&columns, array_reference, SL("columns"), PH_NOISY_CC);

		PHALCON_INIT_VAR(referenced_columns);
		phalcon_array_fetch_string(&referenced_columns, array_reference, SL("referencedColumns"), PH_NOISY_CC);

		PHALCON_INIT_VAR(definition);
		array_init(definition);
		phalcon_array_update_string(&definition, SL("referencedSchema"), &referenced_schema, PH_COPY | PH_SEPARATE TSRMLS_CC);
		phalcon_array_update_string(&definition, SL("referencedTable"), &referenced_table, PH_COPY | PH_SEPARATE TSRMLS_CC);
		phalcon_array_update_string(&definition, SL("columns"), &columns, PH_COPY | PH_SEPARATE TSRMLS_CC);
		phalcon_array_update_string(&definition, SL("referencedColumns"), &referenced_columns, PH_COPY | PH_SEPARATE TSRMLS_CC);

		PHALCON_INIT_VAR(reference);
		object_init_ex(reference, phalcon_db_reference_ce);
		PHALCON_CALL_METHOD_PARAMS_2_NORETURN(reference, "__construct", name, definition, PH_CHECK);
		phalcon_array_update_zval(&reference_objects, name, &reference, PH_COPY | PH_SEPARATE TSRMLS_CC);

		zend_hash_move_forward_ex(ah1, &hp1);
		goto ph_cycle_start_1;

	ph_cycle_end_1:


	RETURN_CTOR(reference_objects);}

