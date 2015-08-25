#include <stdio.h>
#include <stdlib.h>

#include "Database.hpp"

Database::Database()
{
	open();
}

Database::~Database()
{
}

void Database::open()
{
   int rc;
   const char *dbName = "numsi.db";

   rc = sqlite3_open(dbName, &db);

   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
}

std::string Database::find(std::string fNum)
{
	sqlite3_stmt *stmt;
	std::string sql = "SELECT * FROM nums WHERE orignum = '" + fNum + "';" ;
//	const unsigned char * transNum;
	std::string transNum;

	LOG("SQL QUERY: " << sql);


	int retval = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

	if(retval == SQLITE_OK)
		{
		int res = sqlite3_step(stmt);

		if((res == SQLITE_ROW))
			{
				transNum = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
			}

		}else{
			LOG("SQL QUERY ERROR " << retval );
			LOG(sqlite3_errmsg(db));
		}


	sqlite3_finalize(stmt);

	return transNum;
}

void Database::close()
{
	sqlite3_close(db);
}


