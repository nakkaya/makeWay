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
