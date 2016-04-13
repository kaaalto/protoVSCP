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

#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <string>
#include <sqlite3.h>
#include "Common.h"

class Database
{

	/*TODO addNum(), deleteNum(), list()...  CLI:n kautta toimiva käyttöliittymä
	*/

private:
	sqlite3 *db;



public:
	Database();
	~Database();
	void open();
	void close();
	std::string find(std::string);



};
#endif /* DATABASE_HPP_ */
