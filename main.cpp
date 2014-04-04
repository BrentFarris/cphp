#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <fstream>
#include <map>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;

#ifdef _WIN32
const string FILE_PREFIX = "../";
#else
const string FILE_PREFIX = "";
#endif

// trim from start
static inline string &ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
        return s;
}

// trim from end
static inline string &rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline string &trim(string &s) {
        return ltrim(rtrim(s));
}

const string INJECTION_START = "<%=";
const string INJECTION_END = "%>";

string Exec(string cmd);
string GetContents(string file);
string FindInjection(string content);
string CallFunction(string funcName, string* args);

map<string, string> vars;

int main(int argc, char* argv[])
{
	cout << Exec("php " + FILE_PREFIX + "myprog.php") << endl;
	string content = GetContents(FILE_PREFIX + "index.html");
	
	vars["hello"] = "World";
	
	cout << FindInjection(content) << endl;
	
	cin.get();

	return 0;
}

string Exec(string cmd)
{
	FILE* pipe;
#ifdef _WIN32
	pipe = _popen(cmd.c_str(), "r");
#else
	pipe = popen(cmd.c_str(), "r");
#endif

	if (!pipe)
		return "ERROR";

	char buffer[128];
	string result = "";
	
	while (!feof(pipe))
	{
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}

#ifdef _WIN32
	_pclose(pipe);
#else
	pclose(pipe);
#endif

	return result;
}

string GetContents(string fileName)
{
	ifstream ifs(fileName.c_str());
	string str((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	return str;
}

string FindInjection(string content)
{
	size_t locStart = content.find(INJECTION_START);
	size_t locEnd = content.find(INJECTION_END);
	
	if (locStart != string::npos && locEnd != string::npos)
	{
		string var = content.substr(locStart + INJECTION_START.length(), locEnd - locStart - INJECTION_START.length());
		trim(var);
		cout << var << endl;
		if (vars.find(var) != vars.end())
		{
			cout << "TEST2" << endl;
			content.erase(locStart, locEnd - locStart + INJECTION_END.length());
			content.insert(locStart, vars[var]);
		}
	}
	
	return content;
}

string CallFunction(string funcName, string* args)
{
	string response = "";
	//string response = Exec("php " + funcName + " " + args);
	
	return response;
}
