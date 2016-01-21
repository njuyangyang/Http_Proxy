#include <cstring>
#include <iostream>
#include <sstream>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>
#include "simpleHttpProxy.h"

using namespace std;

//member functions
Header::Header(){

}

EntityHeader::EntityHeader(){

}

void EntityHeader::setContentType(string conType){
    contentType = conType;
}

void EntityHeader::setContentEncoding(string conEncoding){
    contentEncoding = conEncoding;
}

void EntityHeader::setExpires(string ex){
    expires = ex;
}

void EntityHeader::setDate(string d){
    date = d;
}

void EntityHeader::setLastModified(string lm){
    lastModified = lm;
}

void EntityHeader::setLastAccessed(time_t la){
    lastAccessed = la;
}

string EntityHeader::getContentType(){
    return contentType;
}

string EntityHeader::getContentEncoding(){
    return contentEncoding;
}

string EntityHeader::getExpires(){
    return expires;
}

string EntityHeader::getLastModified(){
    return lastModified;
}

time_t EntityHeader::getLastAccessed(){
    return lastAccessed;
}

string EntityHeader::getDate(){
    return date;
}

string EntityHeader::toString(){
    string space(" ");
    string retval = contentType+space+contentEncoding;
    return retval;
}


Entity::Entity(){

}

void Entity::setHeader(EntityHeader h){
    header = h;
}

void Entity::setBody(string b){
    body = b;
}

EntityHeader Entity::getHeader(){
    return header;
}

string Entity::getBody(){
    return body;
}

string Entity::toString(){
  string newline("\n");
  string retval = header.toString()+newline+body;
  return retval;
}


string expires("Expires");
string lastMod("Last-Modified");
string date("Date");
string format("%a, %d %b %Y %H:%M:%S %Z");
//const char *format = formaat.c_str();


//input is in the format of "GET %s HTTP/1.0",take out the %s
string extractRequestUri(char* httpRequestLine){
  //first step get the "GET"
  char* token =strtok(httpRequestLine, " ");
  //second call get the "%s"
  token = strtok(NULL, " ");
  string reqUri(token);
  return reqUri;
}


bool isGetReq(char* t){
  string st(t);
  string getSt("GET");

  if(getSt == st)
    return 1;
  else
    return 0;
}


//this function uses delim to separate a string into several
//substring, and store them into a vector;
vector<string> strsplit(string s, string delim){
  vector<string> tokens;
  int start = 0;
  int len = s.length();
  size_t f;
  int found;
  string token;
  while(start < len)
  {
    f = s.find(delim, start);
    if(f == string::npos)
    {
      //this could be the last token, or the
      //patter does not exist in this string
      tokens.push_back(s.substr(start));
      break;
    }

    found = int(f);
    token = s.substr(start, found - start);
    tokens.push_back(token);
    
    start = found + delim.length();
  }
  return tokens;
}


Entity parseResponse(string response, int responseLen){  
  Entity* entity_ptr = new Entity;
  EntityHeader* entityHeader_ptr = new EntityHeader;

  cout<<"========get a response========"<<endl;
  cout<<response<<endl;

  //first step cut the response into header and body
  string delim("\r\n\r\n");
  string s_response = response;
  vector<string> toplevel = strsplit(response, delim); 

  if(toplevel.size() == 1 || toplevel.size() > 2)
  {
    cout<<"this response can not be parsed in the first step"<<endl;
    exit(1);
  }

  //now take the header and break it up
  string httpHdr = toplevel.at(0);
  delim = "\r\n";
  vector<string> responseHdrFields = strsplit(httpHdr, delim);
  if(responseHdrFields.size() == 0)
  {
    cout<<"the header can not be recognized"<<endl;
    exit(1);
  }

  //now iterate thru the header fields and look for 
  //stuff like Expires, Last-modified, Date etc

  int numFields = responseHdrFields.size();
  vector<string> fieldTokens;
  delim = ": ";
  for(int i=1; i<numFields; i++)
  {
    fieldTokens = strsplit(responseHdrFields.at(i), delim);

    if(fieldTokens.at(0) == expires){
      entityHeader_ptr->setExpires(fieldTokens.at(1));
    }
    else {
      if(fieldTokens.at(0) == lastMod){
        entityHeader_ptr->setLastModified(fieldTokens.at(1));
      }
      else{
        if(fieldTokens.at(0) == date){
          entityHeader_ptr->setDate(fieldTokens.at(1));
        }
      }
    }
  }

  //set the last accessed time as the current time
  time_t curr = getCurrentTime(); 
  entityHeader_ptr->setLastAccessed(curr);
  
  //now set the body
  string httpBody = toplevel.at(1);
  entity_ptr->setBody(httpBody);
  entity_ptr->setHeader(*entityHeader_ptr);
  Entity entity=*entity_ptr;

  return entity;
} 



/*
 * Return the time diff between time2 and time1 in seconds; time2 >= time1
 */
double timeDiff(string time2, string time1)
{
  time_t t1 = toTimeT(time1);
  time_t t2 = toTimeT(time2);

  return difftime(t2, t1);
}


/*
 * converts a string to a time_t object which holds the local time
 */
/*time_t toTimeT(string t)
{
  if(t.empty())
  {
    cout<<"The time string is null"<<endl;
    return -1;
  }
 
  struct tm tm1;
  char * temp;
  strcpy(temp,t.c_str());
  char* ret = strptime(temp, format.c_str(), &tm1);
  if(ret == NULL)
  {
    cout<<"The string "<<t<<" was not recognized as a date format"<<endl;
    return -1;
  }
  cout<<"check3";
  time_t t1 = timegm(&tm1);
  cout<<"check5";
  return t1; 
}*/

time_t toTimeT(string str){
  vector<string> top=strsplit(str,", ");
  struct tm* tms=new struct tm;

  string sub1=top[1];
  vector<string> top1=strsplit(sub1," ");
  tms->tm_mday=stoi(top1[0]);
  if(top1[1]=="Jan") tms->tm_mon=0;
  if(top1[1]=="Feb") tms->tm_mon=1;
  if(top1[1]=="Mar") tms->tm_mon=2;
  if(top1[1]=="Apr") tms->tm_mon=3;
  if(top1[1]=="May") tms->tm_mon=4;
  if(top1[1]=="Jun") tms->tm_mon=5;
  if(top1[1]=="Jul") tms->tm_mon=6;
  if(top1[1]=="Aug") tms->tm_mon=7;
  if(top1[1]=="Sep") tms->tm_mon=8;
  if(top1[1]=="Oct") tms->tm_mon=9;
  if(top1[1]=="Nov") tms->tm_mon=10;
  if(top1[1]=="Dec") tms->tm_mon=11;

  tms->tm_year=stoi(top1[2])-1900;

  string sub2=top1[3];
  vector<string> top2=strsplit(sub2,":");
  tms->tm_hour=stoi(top2[0]);
  tms->tm_min=stoi(top2[1]);
  tms->tm_sec=stoi(top2[2]);
  tms->tm_isdst=1;

  mktime(tms);
  return timegm(tms);


}


/*
 * display a time_t object in UTC
 */
string fromTimeT(time_t t)
{
  struct tm* lt = gmtime(&t);
  char stime[30];

  strftime(stime, 30, format.c_str(), lt);
  string s(stime);
  return s;
}


time_t getCurrentTime()
{
  struct tm* tm1;
  time_t local = time(NULL);
  tm1 = gmtime(&local);
  tm1->tm_isdst=1;
  time_t localutc = timegm(tm1);
  return localutc;
}

time_t addCurrentTime(){
  struct tm* tm1;
  time_t local = time(NULL);
  tm1 = gmtime(&local);
  tm1->tm_sec+=5;
  tm1->tm_isdst=1;
  mktime(tm1);
  time_t localutc = timegm(tm1);
  return localutc;

}


/*
 * stamp the entity with the current timestamp
 */
void stampPage(Entity& en)
{
  EntityHeader entityHeader = en.getHeader();
  entityHeader.setLastAccessed(getCurrentTime());
  en.setHeader(entityHeader);
}