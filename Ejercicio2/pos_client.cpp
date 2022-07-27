#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>

#define WRITE_DAT_FILES     1   // 1 para crear archivos ranges.dat y cards.dat con valores hardcodeados en writeRangeFile y writeCardFile, sino 0
#define DEBUG_OUTPUT        0   // 1 for console debug, else 0

#define MENSAJE_BIENVENIDA  "***  Bienvenido al ejercicio 2 - Client side  ***"
#define SOLICITUD_MONTO     "Ingrese el monto de la transaccion (##########,##)"
#define SOLICITUD_TARJETA   "Ingrese el numero de tarjeta (minimo 14 digitos)."
#define SOLICITUD_CODIGO    "Ingrese el codigo de seguridad."
#define SOLO_DIGITOS        "Por favor, ingrese solo digitos."
#define ERROR_COMUNICACION  "ERROR DE COMUNICACION"
#define CANT_MAX_MONTO      13  // ##########,##
#define CANT_MIN_TARJETA    14
#define CANT_CARAC_CODIGO   3
#define TIPO_MENSAJE_RQST   "0200"
#define PORT                8080

using namespace std;

typedef struct {
    char rangeLow [8 + 1];
    char rangeHigh [8 + 1];
    unsigned char len;
    int id;
} ranges_t;

typedef struct {
    char label [12 + 1];
    int id;
} card_t;

bool checkMonto(string _monto)
{
    if(_monto.size() > CANT_MAX_MONTO){
        cout << "Monto demasiado grande." << endl;
        return false;
    }
    if(_monto.find(",") == string::npos){
        return false;
    }
    else{
        int commapos = _monto.find(",");
        if(commapos != (_monto.size() - 3)){
            cout << "Ingrese solo 2 digitos despues de la coma." << endl;
            return false;
        }
        // checkear que el resto son solo digitos
        for(int i = 0; i < _monto.size(); i++){
            if(i == commapos){
                continue;
            }
            if(!isdigit(_monto[i])){
                cout << SOLO_DIGITOS << endl;
                return false;
            }
        }
        return true;
    }
}

bool checkTarjetaInput (string _tarjeta)
{
    if(_tarjeta.size() < CANT_MIN_TARJETA){
        cout << "Cantidad minima de digitos de tarjeta 14." << endl;
        return false;
    }
    if(_tarjeta.size() > 99){
        cout << "Cantidad maxima de digitos de tarjeta 99." << endl;
        return false;
    }
    for(int i=0; i < _tarjeta.size(); i++){
        if(!isdigit(_tarjeta[i])){
            cout << SOLO_DIGITOS << endl;
            return false;
        }
    }
    return true;
}

void tipoDeTarjeta(int _id)
{
    // abrir cards.dat para verificar registro por registro
    fstream archivoTarjetas;
    archivoTarjetas.open("cards.dat", ios::in);
    if(!archivoTarjetas.is_open()){
        cout << "No se pudo abrir el archivo cards.dat" << endl;
        return;
    }
    while(!archivoTarjetas.eof()){
        card_t tarjeta{};
        archivoTarjetas >> tarjeta.label;
        archivoTarjetas >> tarjeta.id;
        if(tarjeta.id == _id){
            cout << tarjeta.label << endl;
            return;
        }
    }
    cout << "No se encontro ninguna tarjeta con id: " << _id << endl;
}

bool checkTarjetaValida (string _tarjeta)
{
    // reconocimiento de tarjeta
    string sRegistro;
    // abrir ranges.dat para verificar registro por registro
    fstream archivoRangos;
    archivoRangos.open("ranges.dat", ios::in);
    if(!archivoRangos.is_open()){
        cout << "No se pudo abrir el archivo ranges.dat" << endl;
        return false;
    }
    while(!archivoRangos.eof()){
        ranges_t registro{};
        // check 8 digitos de _tarjeta dentro de rango
        archivoRangos >> registro.rangeLow;
        archivoRangos >> registro.rangeHigh;
        archivoRangos >> registro.len;
        archivoRangos >> registro.id;
#if DEBUG_OUTPUT
        cout << "range low: " << registro.rangeLow << "range high: " << registro.rangeHigh << "len: " << registro.len << "id: " << registro.id << endl;
#endif
        string digitosTarjeta8;
        digitosTarjeta8 = _tarjeta.substr(0, 8);
        int i_digitosTarjeta8 = stoi(digitosTarjeta8);
        int i_rangeLow = stoi(registro.rangeLow);
        int i_rangeHigh = stoi(registro.rangeHigh);
        int i_len = (int) registro.len;
#if DEBUG_OUTPUT
        cout    << "i_len: " << i_len
                << " registro.len: " << registro.len 
                << " i_rangeLow: " << i_rangeLow
                << " i_rangeHigh: " << i_rangeHigh
                << endl;
#endif
        if((i_digitosTarjeta8 <= i_rangeHigh) && (i_digitosTarjeta8 >= i_rangeLow) && (i_len == _tarjeta.size())){
            // tarjeta valida!
            // identificar el tipo de tarjeta en cards.dat
            tipoDeTarjeta(registro.id);
            return true;
        }
    }
    // out of range
    return false;
}

bool checkCodigoSeguridad (string _codigo)
{
    if(_codigo.size() != CANT_CARAC_CODIGO){
        cout << "El codigo debe contener 3 digitos." << endl;
        return false;
    }
    else{
        for(int i = 0; i < _codigo.size(); i++){
            if(!isdigit(_codigo[i])){
                cout << "El codigo debe contener solo digitos 0-9." << endl;
                return false;
            }
        }
        return true;
    }
}

string parsearMonto(string _monto)
{
    int commapos = _monto.find(",");
    string montoParseado = "";

    // checkear si es menor
    if(_monto.size() < CANT_MAX_MONTO){
        // necesito agregar 0 adelante
        int cantCeros = 12 - (_monto.size() - 1);
        for(int j = 0; j < cantCeros; j++){
            montoParseado = montoParseado + "0";
        }
    }
    for(int i = 0; i < _monto.size(); i++){
        if(i == commapos){
            continue;
        }
        montoParseado = montoParseado + _monto[i];
    }

    return montoParseado;

}

string requestMessage(string _monto, string _tarjeta, string _codigo)
{
    string message = "";
    string montoParseado;
    string longitudTarjeta;

    // tipo mensaje
    message = message + TIPO_MENSAJE_RQST;

    // numero tarjeta
    // parseo longitud
    longitudTarjeta = to_string(_tarjeta.size());
    message = message + longitudTarjeta + _tarjeta;

    // monto
    montoParseado = parsearMonto(_monto);
    message = message + montoParseado;

    // codigo
    message = message + _codigo;

    return message;
}

#if WRITE_DAT_FILES
void writeRangeFile()
{
    fstream datFile;
    datFile.open("ranges.dat", ios::out);
    if(!datFile.is_open()){
        cout << "No se pudo abrir ranges.dat" << endl;
        return;
    }
    ranges_t registro{};
    char low[8+1];
    char high[8+1];
    int _id;

    strcpy(registro.rangeLow, "12345678");
    strcpy(registro.rangeHigh, "87654321");
    registro.len = 14;
#if DEBUG_OUTPUT
    cout << "registro.len: " << registro.len << endl;
#endif
    registro.id = 1;
    datFile << registro.rangeLow << " ";
    datFile << registro.rangeHigh << " ";
    datFile << registro.len << " ";
    datFile << registro.id;

    datFile.close();

}

void writeCardFile()
{
    fstream datFile;
    datFile.open("cards.dat", ios::out);
    if(!datFile.is_open()){
        cout << "No se pudo abrir cards.dat" << endl;
        return;
    }
    card_t card{};
    strcpy(card.label, "TARJETAVISA");
    card.id = 1;
    datFile << card.label << " ";
    datFile << card.id;
    datFile.close();
}
#endif

int socketClient(string _rqstMessage)
{
    // creacion client socket
    int hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hSocket < 0) {
        cout << ERROR_COMUNICACION << endl;
        return -1;
    }

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << ERROR_COMUNICACION << endl;
        return -1;
    }

    int client;

    if ((client = connect(hSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        cout << ERROR_COMUNICACION << endl;
        return -1;
    }

    // enviar el rqstMessage, disparar un timer de 5 segundos para abortar en caso de que no tenga conexion con el host (server)
    char message[_rqstMessage.size() + 1];
    strcpy(message, _rqstMessage.c_str());

    send(hSocket, message, strlen(message), 0);
    char buffer[3] = { };
    clock_t now = clock();
    int valread = -1;
    int timeout = 0;
    while((timeout < 5000) || (valread == -1)){
        valread = read(hSocket, buffer, 3);
        timeout = clock() - now;
    }
    if(valread == -1){
        cout << ERROR_COMUNICACION << endl;
    }
#if DEBUG_OUTPUT
    cout << "Host response: " << buffer << endl;
#endif

    if(strcmp (buffer, "00") == 0){
        cout << "APROBADA" << endl;
        close(client);
        return 0;
    }
    else{
        cout << "RECHAZADA" << endl;
        close(client);
        return -1;
    }

}

int main()
{
    string monto;
    string tarjeta;
    string codigoSeguridad;
    string rqstMessage;
    bool montoValido = false;
    bool tarjetaInput = false;
    bool tarjetaValida;
    bool codSeguridad = false;

    cout << MENSAJE_BIENVENIDA << endl;

    while(!montoValido){
        cout << SOLICITUD_MONTO << endl;
        cin >> monto;
        montoValido = checkMonto(monto);
    }

    while(!tarjetaInput){
        cout << SOLICITUD_TARJETA << endl;
        cin >> tarjeta;
        tarjetaInput = checkTarjetaInput(tarjeta);
    }

    // just for development write .dat files
#if WRITE_DAT_FILES
    writeRangeFile();
    writeCardFile();
#endif

    tarjetaValida = checkTarjetaValida(tarjeta);
    if(!tarjetaValida){
        cout << "TARJETA NO SOPORTADA" << endl;
        return -1;
    }

    while(!codSeguridad){
        cout << SOLICITUD_CODIGO << endl;
        cin >> codigoSeguridad;
        codSeguridad = checkCodigoSeguridad(codigoSeguridad);
    }

    rqstMessage = requestMessage(monto, tarjeta, codigoSeguridad);
#if DEBUG_OUTPUT
    cout << "rqstMessage: " << rqstMessage << endl;
#endif

    // creacion client socket
    socketClient(rqstMessage);    
    
    return 0;
}