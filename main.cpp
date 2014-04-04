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

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

using namespace std;

const string INJECTION_START = "<%=";
const string INJECTION_END = "%>";

string Exec(string cmd);
string GetContents(string file);
string FindInjection(string content);
string CallFunction(string funcName, string* args);

map<string, string> vars;

int main(int argc, char* argv[])
{
	cout << Exec("php myprog.php") << endl;
	string content = GetContents("index.html");
	
	vars["hello"] = "World";
	
	cout << FindInjection(content) << endl;
	
	return 0;
}

string Exec(string cmd)
{
	FILE* pipe;
#if _WINDOWS
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

#if _WINDOWS
	_pclose(pipe);
#else
	pclose(pipe);
#endif

	return result;
}

string GetContents(string file)
{
	ifstream ifs(file.c_str());
	string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
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
