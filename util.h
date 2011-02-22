// Copyright 2011 Nurullah Akkaya

// Mocha is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.

// Mocha is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with Mocha. If not, see http://www.gnu.org/licenses/.

using namespace std;

int parseHex(const string &num);
int parseInt(const string &num);

vector<string> StringSplit(string str, string deli);

void printMAC( char * name , uint8_t mac[6] );
void printIP( char * name , uint8_t ip[4] );

string MACtoString( uint8_t mac[6] );
string IPtoString( uint8_t mac[6] );
void StringtoIP( string s ,uint8_t* ip );
void StringtoMAC(string s,uint8_t* mac );
