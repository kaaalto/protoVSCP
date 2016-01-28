

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
