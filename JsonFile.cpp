

#include <jsoncpp/json.h>
#include <string>
#include <iostream>
#include "JsonFile.hpp"


/*
			JSON example:

{

{"nums":[
    {
      "origNum" : "123456789",
      "transferredNum" : "1Dxxy561234987",
    },
    {
      "origNum" : "987654321",
      "transferredNum" : 1Dxxy123456789,
    },
  ]
}
*/

JsonFile::JsonFile()
{
std::string file = "numsi.json";
Json::Reader reader;
Json::Value root;

bool parsedSuccess = reader.parse(file,
                                   root,
                                   false);

 if(not parsedSuccess)
 {
   // Report failures and their locations
   // in the document.
   cout<<"Failed to parse JSON"<< std::endl
       <<reader.getFormatedErrorMessages()
       <<endl;
   return ;
 }


}

unsigned char JsonFile::find(unsigned char& origNum)
{
	Json::Value root;

	const Json::Value nums = root["nums"];

	std::string sOrigNum(reinterpret_cast<char*>(origNum));			//unsigned char to string

	for(Json::ValueConstIterator it = nums.begin(); it != nums.end() ; ++it )
	{
		const Json::Value& num = *it;

		if (num["origNum"].asString == sOrigNum)
		{
			std::string tmp = num["transferredNum"];
			unsigned char *val = new unsigned char[tmp.length()+1];
			strcpy((char *)val,tmp.c_str());						// string to unsigned char

			return val;
		}

	}
	return ;
}



//void JsonFile::jsonAdd() { }

//void JsonFile::jsonDel() { }
