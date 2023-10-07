#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

string CSV_PATH;
const string NAME_PROJECT = "Ñalculating Ñoncentration of VMR";
const string CSV_NAME_INDEX = "_index.csv";

const int RESPONSE_ERROR_CSV_PATH_NOT_FOUND = -1;
const int RESPONSE_ERROR_AMOUNT = -2;
const int RESPONSE_ERROR_CONVERT_FLOAT = -3;
const int RESPONSE_ERROR_INCORRECT_DENSITY_WATER = -4;
const int RESPONSE_ERROR_INCORRECT_DENSITY_WMR = -5;
const int RESPONSE_ERROR_INCORRECT_TEMPERATURE = -6;
const int RESPONSE_ERROR_ZERO_OR_MINUS_RESULT = -7;

vector<vector<string>> CSVRead(string, bool);
vector<string> EquationFind(vector<vector<string>>, float);
string CSVFindName(vector<vector<string>>, float);

float ResolveEquation(float, float, float, float, float);
float ResultChecked(float);
float ConvertToFloat(string);

void CheckTrueAmount(int);
void CheckNotZeroOrMinus(float, int);
void CSVSetPath(string);
void ErrorOut(int);

#ifdef _DEBUG
void CSVOut(vector<vector<string>>);
void VectorOut(vector<string>);
void ArgsOut(float, float, float);
#endif

int main(int argc, char* argv[]) {

    CheckTrueAmount(argc);
    float densityWater = ConvertToFloat(argv[1]);
    float densityWMR = ConvertToFloat(argv[2]);
    float temperature = ConvertToFloat(argv[3]);
    CSVSetPath(argv[0]);
    CheckNotZeroOrMinus(densityWater, RESPONSE_ERROR_INCORRECT_DENSITY_WATER);
    CheckNotZeroOrMinus(densityWMR, RESPONSE_ERROR_INCORRECT_DENSITY_WMR);

    //Check Args in Console
    #ifdef _DEBUG
    //ArgsOut(densityWater, densityWMR, temperature);
    #endif

    string CSVFName = CSVFindName(CSVRead(CSV_NAME_INDEX, false), densityWater);

    //Check Table in Console
    #ifdef _DEBUG
    //CSVOut(table);
    #endif

    vector<string> equation = EquationFind(CSVRead(CSVFName, false), temperature);

    //Check Equation in Console
    #ifdef _DEBUG
    //cout << endl << equation[8] << endl;
    #endif

    float resultUnChecked = ResolveEquation(ConvertToFloat(equation[1]), ConvertToFloat(equation[2]), ConvertToFloat(equation[3]), ConvertToFloat(equation[4]), densityWMR);

    //Check Result in Console
    #ifdef _DEBUG
    //cout << resultUnChecked;
    #endif

    cout << ResultChecked(resultUnChecked);

    return 0;
}

float ResolveEquation(float a, float b, float c, float d, float x) {
    return a * x * x * x + b * x * x + c * x + d;
}

vector<vector<string>> CSVRead(string fname, bool isStartWithfirst = true) {
    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(CSV_PATH + fname, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (!isStartWithfirst) {
                isStartWithfirst = true;
                continue;
            }
            row.clear();

            stringstream str(line);

            while (getline(str, word, ';'))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        ErrorOut(RESPONSE_ERROR_CSV_PATH_NOT_FOUND);

    return content;
}

string CSVFindName(vector<vector<string>> csv, float densityWater) {

    for (int i = 0; i < csv.size(); i++)
    {
        if (ConvertToFloat(csv[i][0]) < densityWater && ConvertToFloat(csv[i][1]) >= densityWater) {
            return csv[i][2];
        }
    }
    
    ErrorOut(RESPONSE_ERROR_INCORRECT_DENSITY_WATER);
}

vector<string> EquationFind(vector<vector<string>> csv, float temperature) {

    for (int i = 0; i < csv.size(); i++)
    {
        if (ConvertToFloat(csv[i][5]) < temperature && ConvertToFloat(csv[i][6]) >= temperature) {
            return csv[i];
        }
    }

    ErrorOut(RESPONSE_ERROR_INCORRECT_TEMPERATURE);
}

float ConvertToFloat(string arg) {
    float result;

    replace(arg.begin(), arg.end(), ',', '.');

    try {
        result =stof(arg);
    }
    catch (...) {
        ErrorOut(RESPONSE_ERROR_CONVERT_FLOAT);
    }

    return result;
}

float ResultChecked(float resultUnChecked) {

    if (resultUnChecked <= 0) {
        ErrorOut(RESPONSE_ERROR_ZERO_OR_MINUS_RESULT);
    }

    return resultUnChecked;
}

void CheckTrueAmount(int amount) {
    if (amount != 4) {

        ErrorOut(RESPONSE_ERROR_AMOUNT);
    }
}

void CheckNotZeroOrMinus(float number, int RESPONSE_ERROR) {
    if (number <= 0)
        ErrorOut(RESPONSE_ERROR);
}

void CSVSetPath(string fullPath) {
    CSV_PATH = fullPath.substr(0, fullPath.find(NAME_PROJECT)) + NAME_PROJECT + "\\BD\\";
}

void ErrorOut(int RESPONSE_ERROR) {
    cout << RESPONSE_ERROR;
    exit(0);
}

#ifdef _DEBUG
void CSVOut(vector<vector<string>> csv) {
    cout << endl;

    for (int i = 0;i < csv.size();i++)
    {
        for (int j = 0;j < csv[i].size();j++)
        {
            cout << csv[i][j] << " || ";
        }
        cout << endl;
    }
}

void VectorOut(vector<string> vector) {
    cout << endl;

    for (int i = 0;i < vector.size();i++)
    {
        cout << vector[i]<< " || ";
    }
}

void ArgsOut(float densityWater, float densityWMR, float temperature) {
    cout << densityWater << endl;
    cout << densityWMR << endl;
    cout << temperature << endl;
}
#endif

