#include <iostream>
#include <string>
#include <fstream>

#define DEBUG_OUTPUT                0   // 1 for console debug, else 0

#define ACCEPT_0_AS_INPUT           0   // 0 or 1, for switch between first and second solution
/*
First solution:

Will accept 0 values as part of interger input.
i.e.: If user input is 123456789012345
This solution will save
1234567890
1234500000
to file!

Execution is finished when input is 0.

Second solution:

Won't accept 0 values as part of integer input.
i.e.: If user input is 123456789012345
This solution will save
1234567890
to file!

Note: The last 0 in 1234567890 (value saved to file) is not the 0 from input, it is a 0 that complete the 10 characters line!

Another
i.e.: If user input is 12345678987654321098765
This solution will save
1234567898
7654321000
to file!

Execution is finished after 1 valid input (only digits).

*/

#define MENSAJE_BIENVENIDA          "***  Bienvenido al ejercicio 1  ***"
#define MENSAJE_INGRESO_ENTERO      "Ingrese un numero entero:"
#define RANGO_CARACTERES            "Ingrese solo numeros (0-9)"
#define CARACTERES_LINEA            10

using namespace std;

// Global variables
string userInput;
string dataToFile;
string exitInput = "0";
bool atLeastOneInput = false;

bool checkInput(string input)
{
    for(int i = 0; i < input.size(); i++){
        if(!isdigit(input[i])){
            cout << RANGO_CARACTERES << endl;
            return false;
        }
    }
    return true;
}

string parseDataForFile(string input)
{
    int inputSize;
    inputSize = input.size();
    if(inputSize <= CARACTERES_LINEA){
        for(int i = inputSize; i < CARACTERES_LINEA; i++){
            input = input + "0";
        }
        input = input + "\r\n";
        return input;
    }
    else{
        int linesQty;
        string line;
        linesQty = inputSize / CARACTERES_LINEA;
        for(int i = 0; i < linesQty; i++){
            line = line + input.substr((i*CARACTERES_LINEA), CARACTERES_LINEA);
            line = line + "\r\n";
        }
        if((inputSize % CARACTERES_LINEA) != 0){
            line = line + input.substr((linesQty*CARACTERES_LINEA), (inputSize - (linesQty*CARACTERES_LINEA)));
            for(int k=(inputSize - (linesQty*CARACTERES_LINEA)); k < CARACTERES_LINEA; k++){
                line = line + "0";
            }
            line = line + "\r\n";
        }
        return line;
    }
}

bool checkFileName(string _fileName)
{
    /*
    < (less than)
    > (greater than)
    : (colon - sometimes works, but is actually NTFS Alternate Data Streams)
    " (double quote)
    / (forward slash)
    \ (backslash)
    | (vertical bar or pipe)
    ? (question mark)
    * (asterisk)
    */
    if( (_fileName.find("<") != string::npos)   ||
        (_fileName.find(">") != string::npos)   ||
        (_fileName.find(":") != string::npos)   ||
        (_fileName.find("\"") != string::npos)  ||
        (_fileName.find("/") != string::npos)   ||
        (_fileName.find("|") != string::npos)   ||
        (_fileName.find("\\") != string::npos)  ||
        (_fileName.find("?") != string::npos)   ||
        (_fileName.find("*") != string::npos)
        )
    {
            cout << "El nombre del archivo tiene caracteres invalidos. Evite usar < > : \\ \" / | ? *" << endl;
            return false;
    }
    else{
        return true;
    }
}

void saveToFile(string fileName, string data)
{
    // choose txt as format
    fileName = fileName + ".txt";
    fstream archivo;
    archivo.open(fileName, ios::out);
    if(archivo.is_open()){
        archivo << data;
        archivo.close();
        cout << "Se pudo guardar el archivo." << endl;
    }
    else{
        cout << "No se pudo guardar el archivo." << endl;
    }
    return;
}

#if !ACCEPT_0_AS_INPUT
void firstSolution()
{
    cout << MENSAJE_INGRESO_ENTERO << endl;
    cin >> userInput;
    while(userInput != exitInput){
        if(checkInput(userInput)){
            atLeastOneInput = true;
            dataToFile = dataToFile + parseDataForFile(userInput);
        }
        cout << MENSAJE_INGRESO_ENTERO << endl;
        cin >> userInput;
    }
}
#endif

#if ACCEPT_0_AS_INPUT
void secondSolution()
{
    // New development
    char cInput = '1';  // just different from 0 to enter in while loop
    userInput = "";
    while(!checkInput(userInput) || userInput == ""){
        userInput = ""; // Initialization
        cout << MENSAJE_INGRESO_ENTERO << endl;
        while((cInput != '0') && (cInput != '\n')){
            cInput = getchar();
#if DEBUG_OUTPUT
            cout << "cInput: " << cInput << endl;
#endif
            if((cInput != '0') && (cInput != '\n')){
                //atLeastOneInput = true;
                userInput = userInput + cInput;
            }
        }
        if((cInput == '0') && (userInput == "")){
            return;
        }
        cInput = '1';   // Initialization
#if DEBUG_OUTPUT
        cout << "userInput: " << userInput << endl;
#endif
    }
    atLeastOneInput = true;
    dataToFile = dataToFile + parseDataForFile(userInput);
    return;
    // New development
}
#endif

int main()
{
    //string userInput;
    //string exitInput = "0";
    //string dataToFile;
    string fileName;
    bool /*atLeastOneInput = false,*/ fileNameChecked = false;
    //exitInput="0";
    cout << MENSAJE_BIENVENIDA << endl;
    
#if !ACCEPT_0_AS_INPUT
    firstSolution();
#else
    secondSolution();
#endif
    

    if(atLeastOneInput){
        cout << "Ingrese el nombre de archivo: " << endl;
        cin >> fileName;
        fileNameChecked = checkFileName(fileName);
        while(!fileNameChecked){
            fileName = "";
            cout << "Reingrese el nombre de archivo: " << endl;
            cin >> fileName;
            fileNameChecked = checkFileName(fileName);
        }
        saveToFile(fileName, dataToFile);
    }
}