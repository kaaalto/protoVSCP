

#include <jsoncpp/json.h>
#include <string>
#include <iostream>
#include "JsonFile.hpp"


/*
			JSON example:

{

{"nums":[
    {
      "origNum" : "0123456789",
      "transferredNum" : "1Dxxy0123456789",
    },
    {
      "origNum" : "9876543210",
      "transferredNum" : 1Dxxy9876543210,

    },
  ]
}
*/

JsonFile::JsonFile()
{
string file = "numsi.json";
Json::Reader reader;
Json::Value root;

bool parsedSuccess = reader.parse(file,
                                   root,
                                   false);

 if(not parsedSuccess)
 {
   // Report failures and their locations
   // in the document.
   cout<<"Failed to parse JSON"<<endl
       <<reader.getFormatedErrorMessages()
       <<endl;
   return 1;
 }


}

unsigned char JsonFile::find(unsigned char& origNum)
{
	const Json::Value nums = root["nums"];

	std::string sOrigNum(reinterpret_cast<char*>(origNum));			//unsigned char to string


	for(Json::ValueConstIterator it = nums.begin(); it != nums.end() ; ++it )
	{
		const Json::Value& num = *it;

		if (num["origNum"].asString == sOrigNum)
		{
			String tmp = num["transferredNum"];
			unsigned char *val = new unsigned char[tmp.length()+1];
			strcpy((char *)val,tmp.c_str());						// string to unsigned char

			return val;
		}

	}
	return -1;
}



//void JsonFile::jsonAdd() { }

//void JsonFile::jsonDel() { }
