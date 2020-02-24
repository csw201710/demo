#pragma once


#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>

class TSQLite3
{
private:
	const char* m_testDbName = "testinfo.db";

	const char* m_dbName = "dinfo.db";
	sqlite3* m_db = 0;
public:
	TSQLite3(){
		
	}
	int openDB(){
		int rc = sqlite3_open(m_dbName, &m_db);
		if (rc != SQLITE_OK) {
			std::cout << "Open " << m_dbName << " failed! Error msg: " << sqlite3_errmsg(m_db) << std::endl;
			return -1;
		}
		return 0;
	}

	int executeSQL(const char* sql){
		int rc;
		char* error_msg = NULL;
		if (m_db == 0){
			std::cout << "database not be open" << std::endl;
			return -1;
		}
		// 如果失败会动态分配一个error_msg,注意要手动free。
		rc = sqlite3_exec(m_db, sql, NULL, NULL, &error_msg);
		if (rc != SQLITE_OK) {
			std::cout << "executeSQL failed! Error msg: " << error_msg << std::endl;
			sqlite3_free(error_msg);
			return -1;
		}
		return 0;
	}
	int initTable(){
		int rc;
		const char* kCreateTableSql = "CREATE TABLE IF NOT EXISTS FILELIST( \
						ID INTEGER PRIMARY KEY AUTOINCREMENT, \
						ORI_NAME TEXT  NOT NULL, \
						NEW_NAME TEXT UNIQUE NOT NULL);";

		rc = executeSQL(kCreateTableSql);
		if (rc != 0) {
			return -1;
		}
		return 0;

	}
	int deleteData(const char* new_name){
		int rc;
		const char* sql = "DELETE FROM FILELIST WHERE NEW_NAME = ?;";
		char* error_msg = NULL;

		sqlite3_stmt* stmt = NULL;
		// sqlite3_prepare2 接口把一条SQL语句编译成字节码留给后面的执行函数。
		rc = sqlite3_prepare_v2(m_db, sql, strlen(sql), &stmt, NULL);
		if (rc != SQLITE_OK) {
			std::cout << "Failed to prepare sql!  Error msg : " << sqlite3_errmsg(m_db) << std::endl;
			sqlite3_finalize(stmt);
			return -1;
		}

		sqlite3_bind_text(stmt, 1, new_name, -1, 0);

		sqlite3_step(stmt);
		rc = sqlite3_finalize(stmt);
		if (rc != SQLITE_OK) {
			std::cout << "Failed to prepare sql!  Error msg : " << sqlite3_errmsg(m_db) << std::endl;
			return -1;
		}
		return 0;
	}
	int selectData(const char* new_name,std::string & retValue){
		const char* kSelectSql = "SELECT ORI_NAME FROM FILELIST WHERE NEW_NAME = ? ;";
		int rc;
		sqlite3_stmt* stmt = NULL;
		// sqlite3_prepare2 接口把一条SQL语句编译成字节码留给后面的执行函数。
		rc = sqlite3_prepare_v2(m_db, kSelectSql, strlen(kSelectSql), &stmt, NULL);
		if (rc != SQLITE_OK) {
			std::cout << "Failed to prepare sql!" << std::endl;
			sqlite3_finalize(stmt);
			return -1;
		}

		// sqlite3_bind 所包含的全部接口，它们是用来给SQL声明中的通配符赋值的。
		sqlite3_bind_text(stmt, 1, new_name, -1, 0);

		do {
			rc = sqlite3_step(stmt);
			if (rc == SQLITE_ERROR) {
				std::cout << "Failed to step execute!" << std::endl;
				sqlite3_finalize(stmt);
				return -1;
			}

			if (rc == SQLITE_DONE) {
				std::cout << "Finished to step execute!" << std::endl;
				break;
			}

			int column_count = sqlite3_column_count(stmt);

			const char * column_name = sqlite3_column_name(stmt, 0);
			if (stricmp(column_name, "ORI_NAME") == 0){
				const unsigned char* column_value = sqlite3_column_text(stmt, 0);
				retValue = (const char* )column_value;
				sqlite3_finalize(stmt);
				return 0;
			}
		} while (rc == SQLITE_ROW);
		sqlite3_finalize(stmt);
		return -2; // NOT FOUND
	}


	int isHasData(){
		const char* kSelectSql = "SELECT count(*) FROM FILELIST ;";
		int rc;
		sqlite3_stmt* stmt = NULL;
		// sqlite3_prepare2 接口把一条SQL语句编译成字节码留给后面的执行函数。
		rc = sqlite3_prepare_v2(m_db, kSelectSql, strlen(kSelectSql), &stmt, NULL);
		if (rc != SQLITE_OK) {
			std::cout << "Failed to prepare sql!" << std::endl;
			sqlite3_finalize(stmt);
			return -1;
		}

		do {
			rc = sqlite3_step(stmt);
			if (rc == SQLITE_ERROR) {
				std::cout << "Failed to step execute!" << std::endl;
				sqlite3_finalize(stmt);
				return -1;
			}

			if (rc == SQLITE_DONE) {
				std::cout << "Finished to step execute!" << std::endl;
				break;
			}
			const char * column_name = sqlite3_column_name(stmt, 0);
			int column_count = sqlite3_column_int(stmt, 0);
			sqlite3_finalize(stmt);
			return column_count;
			
		} while (rc == SQLITE_ROW);
		sqlite3_finalize(stmt);
		return 0; // NOT FOUND
	}

	int truncateTable(){
		int rc;
		const char* sql = "delete from FILELIST ";

		rc = executeSQL(sql);
		if (rc != 0) {
			return -1;
		}
		return 0;
	}


	int insertData(const char* ori_data, const char* new_data){
		int rc;
		const char* sql = "INSERT INTO FILELIST( ORI_NAME, NEW_NAME) VALUES(?,?);";
		char* error_msg = NULL;
		
		sqlite3_stmt* stmt = NULL;
		// sqlite3_prepare2 接口把一条SQL语句编译成字节码留给后面的执行函数。
		rc = sqlite3_prepare_v2(m_db, sql, strlen(sql), &stmt, NULL);
		if (rc != SQLITE_OK) {
			std::cout << "Failed to prepare sql!  Error msg : " << sqlite3_errmsg(m_db) << std::endl;
			sqlite3_finalize(stmt);
			return -1;
		}
		sqlite3_bind_text(stmt, 1, ori_data, -1, 0);
		sqlite3_bind_text(stmt, 2, new_data, -1, 0);
		
		sqlite3_step(stmt);
		rc = sqlite3_finalize(stmt);
		if (rc != SQLITE_OK) {
			std::cout << "Failed to prepare sql!  Error msg : " << sqlite3_errmsg(m_db) << std::endl;
			return -1;
		}
		return 0;

	}
	void closeDB(){
		if (m_db){
			sqlite3_close(m_db);
			m_db = 0;
		}

	}
	virtual ~TSQLite3(){
		closeDB();
	}





public:
	int test_OpenDB();
	int test_CreateTable();
	int test_InsertData();
	int test_SelectData();
	int test_UpdateData();
	int test_DeleteData();
	int test_StepExecute();
};

