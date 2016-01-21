#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_CACHE_SIZE 10
#define STDIN 0

class Header{
	public:
	Header();
};

class EntityHeader : public Header{
  	string contentType;
  	string contentEncoding;
  	string expires;
  	string lastModified;
  	string date;
  	time_t lastAccessed;

  	public:
  	EntityHeader();
  	void setContentType(string conType);
  	void setContentEncoding(string);
  	void setExpires(string);
  	void setLastModified(string);
  	void setDate(string);
  	void setLastAccessed(time_t);
  	string getContentType();
  	string getContentEncoding();
  	string getExpires();
  	string getLastModified();
  	string getDate();
  	time_t getLastAccessed();
  	string toString();
};

class Entity{
  	EntityHeader header;
  	string body;

  	public:
  	Entity();
  	void setHeader(EntityHeader);
  	void setBody(string);
  	EntityHeader getHeader();
  	string getBody();
  	string toString();
};



//method declaration
string extractRequestUri(char*);
bool isGetReq(char*);
Entity parseResponse(string, int);
vector<string> strsplit(string, string);
double timeDiff(string, string);
time_t toTimeT(string);
string fromTimeT(time_t);
time_t getCurrentTime();
time_t addCurrentTime();
void stampPage(Entity&);


