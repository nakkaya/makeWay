#include <arpa/inet.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
using namespace std;

vector<string> StringSplit(string str, string delim){
  vector<string> results;
  size_t cutAt;
  while( (cutAt = str.find_first_of(delim)) != str.npos ){

      if(cutAt > 0){
	  results.push_back(str.substr(0,cutAt));	  
	}
      str = str.substr(cutAt+1);

    }
  if(str.length() > 0){
      results.push_back(str);
    }
  return results;
}


int parseInt(const string &num){
  stringstream ss(num);
  int n;
  ss >> n;
  return n;
}

int parseHex(const string &num){
  stringstream ss(num);
  ss << std::hex;
  int n;
  ss >> n;
  
  return n;
}

void printMAC( char * name , uint8_t mac[6] ){
  printf("(*) ");
  printf("%s", name );
  printf(" %x:%x:%x:%x:%x:%x\n",
	 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );
}

void printIP( char * name , uint8_t ip[4] ){
  printf("(*) ");
  printf("%s", name );
  printf(" %i.%i.%i.%i\n",
	 ip[0], ip[1], ip[2], ip[3] );
}

string MACtoString( uint8_t mac[6] ){
  char m[17];
  sprintf( m , "%x:%x:%x:%x:%x:%x",
	   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );
  return string(m);
}

string IPtoString( uint8_t mac[6] ){
  char m[15];
  sprintf( m , "%i.%i.%i.%i", mac[0], mac[1], mac[2], mac[3] );  
  return string(m);
}

void StringtoIP( string s ,uint8_t* ip ){
  vector<string> v = StringSplit( s , "." );
  for( unsigned int j = 0 ; j < v.size() ; j++ ){
    ip[j] = parseInt(v.at(j));
  }
}

void StringtoMAC(string s,uint8_t* mac ){
  vector<string> v = StringSplit( s , ":" );
  for( unsigned int j = 0 ; j < v.size() ; j++ ){
    mac[j] = parseHex(v.at(j));
  }
}
