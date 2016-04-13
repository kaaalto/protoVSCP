/**
 *   Prototype VSCP
 *   Copyright (C) 2012, 2013  Kalle Aalto <kalle.aalto@vmail.me>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <stdio.h>
#include <stdlib.h>

#include "Database.hpp"

Database::Database()
{
}

Database::~Database()
{
}

void Database::open()
{
   int rc;
   const char *dbName = "numsi.db";		// TODO  path

   rc = sqlite3_open(dbName, &db);

   if( rc == SQLITE_OK ){
	   LOG("Database opened successfully\n");
     return;
   }else{
      LOG("Can't open database: " << sqlite3_errmsg(db));
      return;
   }
}

std::string Database::find(std::string fNum)
{
	sqlite3_stmt *stmt;
	std::string sql = "SELECT * FROM NUMS WHERE ORIGNUM = '" + fNum + "';" ;
	std::string transNum;
	std::string noNum = "3BD"; 		// number not found

	LOG("SQL QUERY: " << sql);


	int retval = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

	if(retval == SQLITE_OK)
		{
		int res = sqlite3_step(stmt);

		if((res == SQLITE_ROW))
			{
				transNum = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
			}
		else if(res == SQLITE_DONE)
		{
			transNum = noNum;							// number not in database
			LOG("NUMBER NO " << retval );
		}

		}else{
			LOG("SQL QUERY ERROR " << retval );		// TODO tälle jotain
			transNum = noNum;
			LOG(sqlite3_errmsg(db));
		}


	sqlite3_finalize(stmt);

	return transNum;
}

void Database::close()
{
	LOG("Database closed");
	sqlite3_close(db);
}


