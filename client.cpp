#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "simpleHttpProxy.h"

using namespace std;

void *get_in_addr(struct sockaddr *sa){
  if (sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
    }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char* argv[]) 
{
  int sockfd, nbytes;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];  //used to print server address
  char errorbuffer[256];
  char recvbuffer[1048576];

  if(argc != 4)
  {
    cout<<"wrong usage"<<endl;
    exit(1);
  }

  //set the service information
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }
  if (p == NULL) {
    fprintf(stderr, "\nclient: failed to connect\n");
    return 2;
  }


  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
  printf("client: connecting to %s\n", s);
  freeaddrinfo(servinfo); // all done with this structure




  ////////////
/*
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
  {
    fprintf(stderr, "Client socket creation error\n");
    //char* errorMessage = strerror_r(errno, errorbuffer, 256);
    //printf(errorMessage);
    exit(1);
  }

  struct sockaddr_in serverAddrInfo;
  memset((char*) & serverAddrInfo, '0', sizeof(struct sockaddr_in));
  serverAddrInfo.sin_family= AF_INET;
  serverAddrInfo.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, argv[1], &serverAddrInfo.sin_addr);

  if(connect(sd, (struct sockaddr*)&serverAddrInfo, sizeof(serverAddrInfo)) == -1) 
  {
    fprintf(stderr, "Connection to server error\n");
    //char* errorMessage = strerror_r(errno, errorbuffer, 256);
    //printf(errorMessage);
    exit(1);
  }
  */

  printf("Connected\n");
  string url(argv[3]);
  string message = "GET "+url+" HTTP/1.0\r\nHost: ecen602\r\nUser-Agent: HTMLGET 1.0\r\n\r\n";
  cout<<endl<<"following request will be sent "<<endl<<message;
  //int bytes = send(sd, message.c_str(), message.length(), 0);
  if((write(sockfd,message.c_str(),message.length()))==-1){
    perror("sent request");
  }
  else{
    printf("send request successfully\n");
  }


  nbytes = 0;
  if((nbytes = read(sockfd, recvbuffer, 1048576)) <= 0)
  {
    perror("fail to read the response");    
    exit(1);
  }
  else
  {
    string recvd(recvbuffer, nbytes);
    cout<<"the response received as following"<<endl;
    cout<<recvd<<endl;
  }
   
  close(sockfd);
  return 0;
}