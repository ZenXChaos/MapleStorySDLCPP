#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <fstream>
#include <map>
#include <string>

using namespace std;


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 15900
#define DEFAULT_PORT "80"

#define KEENIO_HTTP_GET KEENIO_HTTP::METHOD_GET()
#define KEENIO_HTTP_POST KEENIO_HTTP::METHOD_POST()
#define KEENIO_HTTP_PUT KEENIO_HTTP::METHOD_PUT()

#define KEENIO_MASTER_KEY masterKey()
#define KEENIO_WRITE_KEY writeKey()
#define KEENIO_READ_KEY readKey()

#define KQL_EQUAL "eq" //fully supported
#define KQL_NOT_EQUAL "ne" //fully supported
#define KQL_GREATER_THAN "gt" //
#define KQL_GREATER_THAN_EQUAL "gte" //
#define KQL_LESS_THAN "lt" //
#define KQL_LESS_THAN_EQUAL "lte" //
#define KQL_CONTAINS "contains"
#define KQL_NOT_CONTAINS "not_contains"

/*
[
{"property_name":"path","operator":"ne","property_value":"/"},
{"property_name":"action","operator":"eq","property_value":"/"},
{"property_name":"action","operator":"gt","property_value":"/"},
{"property_name":"action","operator":"gte","property_value":"/"},
{"property_name":"action","operator":"lt","property_value":"/"},
{"property_name":"action","operator":"lte","property_value":"/"},
{"property_name":"action","operator":"contains","property_value":"/"},
{"property_name":"action","operator":"not_contains","property_value":"/"}
]
*/
//Handles HTTP RELATED FUNCTIONS AND DATA
class KEENIO_HTTP {
	//INSERT YOUR WRITE/READ/MASTER KEYS
	string _writeKey = "<key>";
	string _readKey = "<key>";
	string _masterKey = "<key>";



public:

	static inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
		static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while (in_len--) {
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_chars[char_array_4[j]];

			while ((i++ < 3))
				ret += "%3D";

		}

		return ret;

	}

	std::string base64_decode(std::string const& encoded_string) {
		static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		int in_len = encoded_string.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
			char_array_4[i++] = encoded_string[in_]; in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++)
					char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret += char_array_3[i];
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
		}

		return ret;
	}

	string bodyRequest;
	static string METHOD_GET() {
		return "GET";
	}

	static string METHOD_POST() {
		return "POST";
	}

	static string METHOD_PUT() {
		return "PUT";
	}

	//API REQUEST URL
	string reqURL;

	//MAP OF HTTP HEADERS
	map<string, string> _headers;

	//MAP OR HTTP PARAMETERS
	map<string, string> _params;


	//DEFAULT HEADERS REQUIRED FOR AN HTTP RESPONSE
	void addDefHeaders(void) {
		this->addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		this->addHeader("Accept-Encoding", "gzip, deflate, sdch");
		this->addHeader("Accept-Language", "en-US,en;q=0.8");
		this->addHeader("Connection", "keep-alive");
		this->addHeader("Host", "api.keen.io");
		this->addHeader("Upgrade-Insecure-Requests", "1");
		this->addHeader("User-Agent", "Carrots/KeenIO HTTP-1.0");
	}

	string masterKey(string key = "") {
		if (key != "") {
			this->_masterKey = key;
		}
		return this->_masterKey;
	}

	string readKey(string key = "") {
		if (key != "") {
			this->_readKey = key;
		}
		return this->_readKey;
	}

	string writeKey(string key = "") {
		if (key != "") {
			this->_writeKey = key;
		}
		return this->_writeKey;
	}



	//ADD A HEADER TO MAP
	void addHeader(string head, string content) {
		this->_headers[head] = content;
	}

	string data_param(string prop, string val) {

	}

	//ADD A PARAMETER TO MAP
	void addParam(string head, string content) {

		this->_params[head] = content;

	}

	//ADD A PARAMETER TO MAP
	void addDataParam(string head, string content[2]) {
		int len_s = strlen(("{ \"" + content[0] + "\" : \"" + content[1] + "\" }").c_str());
		this->_params[head] = this->base64_encode((unsigned const char*)("{ \"" + content[0] + "\" : \"" + content[1] + "\" }").c_str(), len_s);

	}

	//GET ALL HEADERS IN APPROPRIATE HTTP REQUEST FORMAT
	string headers() {
		string plain;

		//LOOP THROUGH HEADERS MAP -> CONVERT TO HTTP HEADER REQUEST
		for each (std::pair<string, string> header in this->_headers)
		{
			string header_t = header.first + ":" + header.second + "\r\n";
			plain += header.first + ":" + header.second + "\r\n";
		}

		return plain;
	}

	//GET ALL PARAMETERS
	string params() {
		string plain = this->reqURL;
		int index = 0;

		//LOOP THROUGH PARAMETERS MAP -> CONVERT TO HTTP HEADER REQUEST
		for each (std::pair<string, string> header in this->_params)
		{
			//FIRST PARAMETER SHOULD START WITH ?
			if (index < 1) {
				plain += "?" + header.first + "=" + header.second;
			}
			else { //OTHERWISE &
				plain += "&" + header.first + "=" + header.second;
			}
			index++;
		}

		return plain;
	}
};

//KEENIO CLIENT ::: MAIN SDK CLIENT
class KEENIO_CLIENT {

	string _method = "GET";
public:

	//BODY RESPONSE OF KEENIO_HTTP
	string body;

	//HTTP HANDLER
	KEENIO_HTTP kHTTP;

	//FPR ERROR DETECTION
	bool err = false;

	string method(string _get_post_method = "") {
		if (_get_post_method != "") {
			this->_method = _get_post_method;
		}
		return this->_method;
	}

	//HELP ENSURE WE GET EVERY BYTE OR CHARACTER FROM RESPONSE.
	bool recvraw(SOCKET socket, char *buf, int buflen)
	{
		//AS LONG AS RESPONSE IS NOT EMPTY
		while (buflen > 0)
		{
			//RECEIVE THE BYTE
			int received = recv(socket, buf, buflen, 0);

			//IF BYTE NOT RECEIVED, QUIT
			if (received < 1) return false;


			buflen -= received;
		}
		return true;
	}

	//INITIATES THE KEENIO_HTTP REQUEST
	int request(KEENIO_HTTP kHTTP)
	{
		WSADATA wsaData;
		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		char *sendbuf = "this is a test";
		char recvbuf[DEFAULT_BUFLEN] = "";
		int iResult;
		int recvbuflen = DEFAULT_BUFLEN;


#ifdef DEBUG

		printf("WinSock initialized\n");

#else

#endif // DEBUG

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
#ifdef DEBUG

		printf("Resolving KeenIO IP");

#else

#endif // DEBUG

		// Resolve the server address and port
		iResult = getaddrinfo("108.168.254.50", DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}
			else {
#ifdef DEBUG

				printf("Established socket @ https://api.keen.io/");

#else

#endif // DEBUG
			}

			// Connect to server.
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			else {
#ifdef DEBUG

				printf("Connected to KeenIO's servers");

#else

#endif // DEBUG
			}
			break;
		}

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}

		// Send an initial buffer

		string headers = this->method() + " " + kHTTP.params();
		headers += "\r\n";
		headers += kHTTP.headers();

		printf("%s\n",headers.c_str());

		iResult = send(ConnectSocket, headers.c_str(), (int)strlen(headers.c_str()), 0);

		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		//printf("Bytes Sent: %ld\n", iResult);

#ifdef DEBUG

		printf("Sent HTTP request\n");

#else

#endif // DEBUG


		// shutdown the connection since no more data will be sent
		//iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}


#ifdef DEBUG

		printf("Waiting for KeenIO's response...");

#else

#endif // DEBUG
		// Receive until the peer closes the connection

		string rec;
		u_long iMode = 1;

		Sleep(1000);
		while (recvraw(ConnectSocket, recvbuf, 1)) {
			rec += recvbuf;
			strcpy(recvbuf, "");
		}



#ifdef DEBUG

		printf("Cleaning up");

#else

#endif // DEBUG
		closesocket(ConnectSocket);
		WSACleanup();

		this->body = rec;

		int keenErr = this->body.find("\"error_code\":");
		if (keenErr >= 0) {
			this->err = true;
		}
		return 0;
	}
};

//KEENIO_QUERY LANGUAGE
namespace KEENIO_QUERYLANGUAGE {
	class KEENIO_FILTER {
	public:
		string op;
		string attr;
		string val;
	};


	class KEENIO_QUERY {
	private:

		map<int, string> statements;
		map<int, string> results;
		void query_exec() {

		}
		//API Key
		string api_key;

	public:
		class FILE_HANDLER {
		public:
			string fileContents;
			string fileName;
			string filePath;

			string getFileContents() {
				string fileData;
				string line;
				ifstream in(this->filePath + "" + this->fileName);
				while (getline(in, fileData))
				{
					line += fileData + '\n';
				}
				fileData += '\0';
				in.close();

				if (line.length() > 0) {
					this->fileContents = line;
				}
				return line;
			}
		};

		class STRING_UTILS {
		public:
			static map<int, string> splitString(string str, string delim) {

				map<int, string> strings;
				char *token = strtok((char*)str.c_str(), (char*)delim.c_str());
				while (token) {
					strings[strings.size()] = token;
					token = strtok(NULL, " ");
				}

				return strings;
			}
		};

		map<string, string> VARS;

		//The API URL Generated from SQL
		string queryURL;

		//.EXPORT directive used?
		int downloadRep = -1;
		string downloadRepData = "";

		//Execute the query
		string QueryExec(string queryText) {

			//CONVERT queryText TO char*
			char *str = (char*)queryText.c_str();
			char * pch;

			//SPLIT QUERY BY WORDS
			pch = strtok(str, " ");

			//FILTER MODE :: IF STATEMENT USED?
			bool filterMode = false;

			//CONCATE MODE :: 'USED TO DETECT MULTI-WORD STRINGS'
			bool concatMode = false;


			bool initSetterDone = false;

			string sel;
			string collection;
			string projectid;
			string qParams;

			string word;
			map<string, KEENIO_FILTER> keenQueryFilters;
			string keenQueryFilterIndex;
			string fText = "[";

			//LOOP WORDS
			while (pch != NULL)
			{
				word = pch;


				/*
				LOAD GLOBAL VARIABLES
				REPLACE WITH STRING X
				*/
				map<string, string>::iterator VARS_IT;
				for (VARS_IT = this->VARS.begin(); VARS_IT != this->VARS.end(); VARS_IT++) {

					int pos = word.find(VARS_IT->first);
					if (pos < 0) {
						break;
					}
					word = word.replace(pos, VARS_IT->first.length(), VARS_IT->second);
				}

				//IF NOT IN FILTER MODE
				if (!filterMode) {

					//LIST OF SUPPORTED QUERY TYPES
					string supported_types = "count count_unique sum average extraction";

					//IS THE WORD FOUND A SUPPORTED TYPE
					int supportedType = supported_types.find(word);

					//IS SETTER FOUND? :: USED TO SET (PROJECT_ID)
					int setter = word.find("(");

					//PARAMETER BUILDER :: option=value . ?option=value
					int psetter = word.find("=");

					//IF STATEMENT :: FILTER MODE?
					bool fmode = (word == "if");

					//.EXPORT DIRECTIVE :: EXPORT TO FILE?
					int dRep = word.find(".export=");

					//IF EXPORT TO FILE, SET downloadRep
					if (dRep >= 0) {
						downloadRep = dRep;

					}

					//IF `if` STATEMENT, SET FILTER_MODE TRUE
					if (fmode) {
						filterMode = true;
						goto restart_ql_loop;
					}

					//IF SUPPORTED TYPE FOUND WITHOUT psetter && setter
					if (supportedType >= 0 && setter < 0 && psetter < 0) {
						sel = word;
					}

					//IF DOWNLOAD REP, SET REP FILE NAME
					else if (dRep >= 0) {
						//PARSE FILENAME, EXTRACT FILE_NAME OF `.export=file.json`
						string repName = word.substr(downloadRep + 8, word.length() - (downloadRep + 8));

						//SET THE DOWNLOAD_REPORT_NAME
						downloadRepData = repName;
					}


					else if (setter >= 0) {
						//IS SELECTOR

						//PARSE (PROJECT_ID)
						int paren1 = word.find("(");
						collection = word.substr(0, paren1);

						int paren2 = word.find(")") - 1 - paren1;
						projectid = word.substr(paren1 + 1, paren2);
						//this->_query = collection;

					}
					else if (psetter >= 0) {
						//IS SELECTOR
						qParams += "&";
						qParams += word;
					}
					int x = 0;

				}

				else { //IF NOT IN FILTER MODE
					if (concatMode == false) {
						int initSetter = word.find("(");

						{
							//EQUALS OPERATOR FOUND
							int eq = word.find("=");

							//NOT EQUALS OPERATOR FOUND
							int ne = word.find("<>");

							//GREATER THAN OPERATOR FOUND
							int gt = word.find(">");

							//LESS THAN OPERATOR FOUND
							int lt = word.find("<");

							//GREATER THAN EQUAL TO OPERATOR FOUND
							int gte = word.find(">=");

							//LESS THAN EQUAL TO OPERATOR FOUND
							int lte = word.find("<=");

							//CONTAINS OPERATOR FOUND
							int contains = word.find("^=");

							//NOT CONTAINS OPERATOR FOUND
							int not_contains = word.find("!^=");
							string word1;
							string wordval;

							//FOR FILTER SUPPORT
							KEENIO_FILTER keenFilter;


							if (eq >= 0 && gte<0 && lte < 0 && contains < 0 && not_contains < 0) { //IF OPERATOR IS `EQUAL_TO`

								word1 = word.substr(0, eq);
								wordval = word.substr(eq, word.length() - eq);
								wordval = word.substr(eq + 1, word.length() - eq - 1);

								if (wordval[0] == '\'') {
									concatMode = true;
									wordval = word.substr(eq + 2, word.length() - eq);

									int closeQuote = wordval.find("'");
									if (closeQuote >= 0) {
										concatMode = false;
										wordval = word.substr(eq + 2, word.length() - eq - 3);
									}
								}
								keenQueryFilterIndex = word1;


								keenFilter.op = "eq";
								keenFilter.attr = word1;
								keenFilter.val = wordval;
								keenQueryFilters[word1] = keenFilter;
							}
							else if (ne >= 0 && gte<0 && lte < 0 && contains < 0 && not_contains < 0) { //IF OPERATOR IS `NOT_EQUALS`

								word1 = word.substr(0, ne);
								wordval = word.substr(ne, word.length() - ne);
								wordval = word.substr(ne + 2, word.length() - ne - 3);

								if (wordval[0] == '\'') { //IF START OF WORD IS APOSTROPHE? CONCAT_MODE=True
									concatMode = true;
									wordval = word.substr(ne + 2, word.length() - ne);

									int closeQuote = wordval.find("'");
									if (closeQuote >= 0) {
										concatMode = false;
										wordval = word.substr(ne + 3, word.length() - ne - 4);
									}
								}
								keenQueryFilterIndex = word1;


								keenFilter.op = "ne";
								keenFilter.attr = word1;
								keenFilter.val = wordval;
								keenQueryFilters[word1] = keenFilter;
							}
							else if (gt >= 0 && gte < 0) { // IF OPERATOR IS `GREATER_THAN_EQUAL_TO`

								word1 = word.substr(0, gt);
								wordval = word.substr(gt, word.length() - gt);
								wordval = word.substr(gt + 1, word.length() - gt - 1);

								if (wordval[0] == '\'') { //IF START OF WORD IS APOSTROPHE? CONCAT_MODE=True
									concatMode = true;
									wordval = word.substr(gt + 2, word.length() - gt);

									//IF CLOSED QUOTE, CONCAT_MODE=False
									int closeQuote = wordval.find("'");
									if (closeQuote >= 0) {
										concatMode = false;
										wordval = word.substr(gt + 2, word.length() - gt - 3);
									}
								}
								keenQueryFilterIndex = word1;


								keenFilter.op = "gt";
								keenFilter.attr = word1;
								keenFilter.val = wordval;
								keenQueryFilters[word1] = keenFilter;
							}
							else if (lt >= 0 && lte < 0) {

								word1 = word.substr(0, lt);
								wordval = word.substr(lt, word.length() - lt);
								wordval = word.substr(lt + 1, word.length() - lt - 1);

								if (wordval[0] == '\'') { //IF START OF WORD IS APOSTROPHE? CONCAT_MODE=True
									concatMode = true;
									wordval = word.substr(lt + 2, word.length() - lt);

									int closeQuote = wordval.find("'");
									if (closeQuote >= 0) {
										concatMode = false;
										wordval = word.substr(lt + 2, word.length() - lt - 3);
									}
								}
								keenQueryFilterIndex = word1;


								keenFilter.op = "lt";
								keenFilter.attr = word1;
								keenFilter.val = wordval;
								keenQueryFilters[word1] = keenFilter;
							}
							else if (gte >= 0) {

								word1 = word.substr(0, gte);
								wordval = word.substr(gte, word.length() - gte);
								wordval = word.substr(gte + 2, word.length() - gte - 1);

								if (wordval[0] == '\'') { //IF START OF WORD IS APOSTROPHE? CONCAT_MODE=True
									concatMode = true;
									wordval = word.substr(gte + 2, word.length() - gte);

									int closeQuote = wordval.find("'");
									if (closeQuote >= 0) {
										concatMode = false;
										wordval = word.substr(gte + 3, word.length() - gte - 4);
									}
								}
								keenQueryFilterIndex = word1;


								keenFilter.op = "gte";
								keenFilter.attr = word1;
								keenFilter.val = wordval;
								keenQueryFilters[word1] = keenFilter;
							}
							else if (lte >= 0) {

								word1 = word.substr(0, lte);
								wordval = word.substr(lte, word.length() - lte);
								wordval = word.substr(lte + 2, word.length() - lte - 1);

								if (wordval[0] == '\'') { //IF START OF WORD IS APOSTROPHE? CONCAT_MODE=True
									concatMode = true;
									wordval = word.substr(lte + 2, word.length() - lte);

									int closeQuote = wordval.find("'");
									if (closeQuote >= 0) {
										concatMode = false;
										wordval = word.substr(lte + 3, word.length() - lte - 4);
									}
								}
								keenQueryFilterIndex = word1;


								keenFilter.op = "lte";
								keenFilter.attr = word1;
								keenFilter.val = wordval;
								keenQueryFilters[word1] = keenFilter;
							}
							else if (contains >= 0 && not_contains < 0) {

								word1 = word.substr(0, contains);
								wordval = word.substr(contains, word.length() - contains);
								wordval = word.substr(contains + 1, word.length() - contains - 1);

								if (wordval[0] == '\'') { //IF START OF WORD IS APOSTROPHE? CONCAT_MODE=True
									concatMode = true;
									wordval = word.substr(contains + 2, word.length() - contains);

									int closeQuote = wordval.find("'");
									if (closeQuote >= 0) {
										concatMode = false;
										wordval = word.substr(contains + 3, word.length() - contains - 4);
									}
								}
								keenQueryFilterIndex = word1;


								keenFilter.op = "contains";
								keenFilter.attr = word1;
								keenFilter.val = wordval;
								keenQueryFilters[word1] = keenFilter;
							}
							else if (not_contains >= 0) {

								word1 = word.substr(0, not_contains);
								wordval = word.substr(not_contains, word.length() - not_contains);
								wordval = word.substr(not_contains + 3, word.length() - not_contains - 1);

								if (wordval[0] == '\'') { //IF START OF WORD IS APOSTROPHE? CONCAT_MODE=True
									concatMode = true;
									wordval = word.substr(not_contains + 3, word.length() - not_contains);

									int closeQuote = wordval.find("'");
									if (closeQuote >= 0) {
										concatMode = false;
										wordval = word.substr(not_contains + 4, word.length() - not_contains - 5);
									}
								}
								keenQueryFilterIndex = word1;


								keenFilter.op = "not_contains";
								keenFilter.attr = word1;
								keenFilter.val = wordval;
								keenQueryFilters[word1] = keenFilter;
							}

						}
					}
					else { // IF CONCAT MODE = TRUE
						   // CONCAT VALUE
						int closeQuote = word.find("'");
						if (closeQuote >= 0) {
							string tmpWord = word.substr(0, closeQuote);
							keenQueryFilters[keenQueryFilterIndex].val += " " + tmpWord;
							concatMode = false;

						}
						else {

							keenQueryFilters[keenQueryFilterIndex].val += " " + word;

						}
					}
				}

			restart_ql_loop:
				//GET NEXT WORD
				pch = strtok(NULL, " ");
			}

			//SET QUERY STRING, EXECUTE THE QUERY
			this->queryURL = queryText;
			this->query_exec();
			string qURL = "https://api.keen.io/3.0/projects/" + projectid;
			qURL += "/queries/" + sel;
			qURL += "?api_key=" + this->api_key;
			qURL += qParams;

			this->queryURL = qURL;

			int kFilIndex = 0;

			//CREATE FILTER JSON STRING
			std::map<string, KEENIO_FILTER>::iterator kFil;
			kFil = keenQueryFilters.begin();
			for (kFil = keenQueryFilters.begin(); kFil != keenQueryFilters.end(); kFil++) {
				if (kFilIndex == 0) {
					fText += "{\"property_name\":\"";
					fText += kFil->second.attr;
					fText += "\", \"operator\":\"";
					fText += kFil->second.op;
					fText += "\", \"property_value\":\"";
					fText += kFil->second.val;
					fText += "\"}";
				}
				else {
					fText += ", {\"property_name\":\"";
					fText += kFil->second.attr;
					fText += "\", \"operator\":\"";
					fText += kFil->second.op;
					fText += "\", \"property_value\":\"";
					fText += kFil->second.val;
					fText += "\"}";
				}

				kFilIndex++;
			}

			fText += "]";

			qURL += "&filters=" + fText;

			return qURL;
		}

		void KEY(string key) {
			this->api_key = key;
		}

		string ProcessQuery() {
			/* SIMPLIFIES THE WORK */
			KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
			kCLIENT->kHTTP.reqURL = (char*)this->queryURL.c_str();
			kCLIENT->kHTTP.addDefHeaders();

			kCLIENT->request(kCLIENT->kHTTP);

			//IF DOWNLOAD_REP, WRITE TO FILE
			if (downloadRep >= 0) {
				{
					//WRITE/EXPORT TO FILE
					ofstream out(downloadRepData, ios::out);
					if (out)
					{
						out << (char*)kCLIENT->body.c_str();
						out.close();
					}

				}
			}

			return kCLIENT->body;
		}

		KEENIO_QUERY() {

			/*
			SET FILE TO LOAD FOR INITIAL VARIABLE LOADING
			LOAD GLOBAL VARIABLES
			*/

			FILE_HANDLER fHandle;
			fHandle.filePath = "";
			fHandle.fileName = "vars.keenio";

			//READ FILES CONTENTS
			string fileContents = fHandle.getFileContents();

			//SPLIT STRING INTO PARTS SEPARATED BY `=`
			map<int, string> Lines = STRING_UTILS::splitString(fileContents, "\n");

			//LOOP THROUGH `Lines` VARIABLE
			for (unsigned int i = 0; i < Lines.size(); i++) {
				string line = Lines[i];
				map<int, string> var_line = STRING_UTILS::splitString(line, "=");

				//IF SPLIT BY `=` SUCCESSFUL
				if (var_line.size() > 1) {

					//ADD GLOBAL VARIABLE
					this->VARS[var_line[0]] = var_line[1];
				}
			}

			/* END LOAD GLOBAL VARIABLES! */
		}
	};
}
