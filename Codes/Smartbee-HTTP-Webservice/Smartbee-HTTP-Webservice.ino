#define TINY_GSM_MODEM_SIM800
#define SENSOR_TENSAO A4
#define HX711_DT A1
#define HX711_SCK A0
#define SENSOR_DHT 4
#define DHTTYPE DHT22
#define RX_do_Arduino 11
#define TX_do_Arduino 10
#define Timeout 20000

#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <HX711.h>

//////////////////////////////////////////////////////////////////
//Definição de variaveis iniciais

const char apn[]  = "claro.com.br";
const char user[] = "claro";
const char pass[] = "claro";

const char* IP = "35.199.66.232";
const char* PORT = "8080";
const char* URL = "/webservice/coleta/";
const String webservice = String(IP) + ":" + String(PORT) + String(URL);

struct payload
{
  int colmeia = 4;
  float peso;
  float tensao_lida;
  float tensaoGSM_lida;
  float temp_lida;
  float umidade_lida;
};

SoftwareSerial SIM800L(RX_do_Arduino, TX_do_Arduino); // RX do arduino liga com TX do Sim800L, contrario com o TX

TinyGsm modem(SIM800L);
HX711 scale(HX711_DT, HX711_SCK); // se for usar o pino digital faca->  scale(7,6);
DHT dht(SENSOR_DHT, DHT22);

int connection_retry = 0; //Quantidade de tentativas de reconexão

float calibration_factor = 21097.164; // Esse fator de calibração depende sempre da celula de carga usada.
float offset_definido;
payload pacote;


//////////////////////////////////////////////////////////////////

void connection() //Função que tenta realizar todas as etapas da conexão com o broker MQTT
//Se em qualquer etapa a conexão falhar > 3 vezes, código para de tentar realizar tentativas de conexão
//Para que force, mais na frente no código, o reset do Sim800L
{
  Serial.println("Inicializando GSM...");
  modem.restart();
  Serial.println("Ok");

  Serial.println("Aguardando rede...");
  while (!modem.waitForNetwork())
  {
    connection_retry++;
    Serial.println("Sem acesso a rede...");
    Serial.print("Tentando novamente em ");
    Serial.print("Número de tentativas - ");
    Serial.println(connection_retry);
    if (connection_retry >= 3)
    {
      break;
    }
    delay(Timeout);
  }
  resetRetries();
  Serial.println("Sucesso.");

  Serial.print("Conectando a ");
  Serial.print(apn);
  Serial.println("...");
  while (!modem.gprsConnect(apn, user, pass))
  {
    connection_retry++;
    Serial.println("Tentando reconectar a APN...");
    Serial.print("Número de tentativas - ");
    Serial.println(connection_retry);
    if (connection_retry >= 3)
    {
      break;
    }
    delay(Timeout);
  }
  resetRetries();
  Serial.println("Sucesso.");
}


void resposta() {
  while (SIM800L.available())
  {
    if (SIM800L.available() > 0)   //se recebe dados
    {
      Serial.write(SIM800L.read()); //impreme a resposta

    }
  }
  Serial.flush();
  Serial.flush();
}


void lerTensao()
{
  float valor_lido_tensao = analogRead(SENSOR_TENSAO);
  pacote.tensao_lida = ((valor_lido_tensao * 0.00489) * 5);
}


void lerDHT()
{
    pacote.temp_lida = dht.readTemperature();
    pacote.umidade_lida = dht.readHumidity();
    
    pacote.temp_lida = ((pacote.temp_lida < 0)||(isnan(pacote.temp_lida))) ? 0 : pacote.temp_lida;
    pacote.umidade_lida = ((pacote.umidade_lida < 0)||(isnan(pacote.umidade_lida))) ? 0 : pacote.umidade_lida;
}


void lerPeso()
{
  pacote.peso = scale.get_units(), 4;
  pacote.peso = (pacote.peso < 0) ? (pacote.peso * -1) : 0;
}


void lerTensaoGSM()
{
  pacote.tensaoGSM_lida = modem.getBattVoltage() / 1000.0;
}


void envioHTTP()
{

  SIM800L.println("AT+HTTPINIT");
  delay (2000);
  resposta();

  SIM800L.println("AT+HTTPPARA=\"CID\",1");
  delay (2000);
  resposta();

  SIM800L.println("AT+HTTPPARA=\"URL\",\"" + webservice + String(pacote.colmeia) + "/" + String(pacote.temp_lida) + "/" + String(pacote.umidade_lida) + "/" + String(pacote.tensao_lida) + "/" + String(0) + "/" + String(0) + "/" + String(0) + "/" + String(pacote.tensaoGSM_lida) + "\"");
  delay (1000);
  resposta();

  SIM800L.println("AT+HTTPACTION=0");
  delay (3000);
  resposta();

  SIM800L.println("AT+HTTPREAD");
  delay (3000);
  resposta();
  //SIM800L.println("AT+CIPACK");
  //delay (3000);
  //resposta();

  SIM800L.println("AT+HTTPTERM");
  delay (1000);
  resposta();

  SIM800L.println("AT+SAPBR= 0,1");
  delay (1000);
  resposta();

  cleanBuffer();
  delay(1000);

  delay(Timeout);

}


void publicar()
{
  connection();
  hasIP();
  envioHTTP();
}

void imprimirDados()
{
  Serial.print("Offset : ");
  Serial.println(offset_definido);
  Serial.print("Peso : ");
  Serial.print(pacote.peso, 3);
  Serial.print("Kg\t");

  Serial.print("Fator de calibração: ");
  Serial.println(calibration_factor);
  Serial.print("Tensao : ");
  Serial.print(pacote.tensao_lida, 3);
  Serial.print("\t");

  Serial.print("Tensao GSM : ");
  Serial.println(pacote.tensaoGSM_lida, 3);
  Serial.print("Temperatura : ");
  Serial.print(pacote.temp_lida, 3);
  Serial.print("\t");

  Serial.print("Umidade : ");
  Serial.println(pacote.umidade_lida, 3);
}

void resetRetries() {
  connection_retry = 0;
}


bool hasIP()
{
  cleanBuffer();
  SIM800L.println("AT+SAPBR=2,1");
  if ( waitFor("0.0.0.0", "OK") != 2 ) return false;

  return true;
}

void cleanBuffer()
{
  delay( 250 );
  while ( SIM800L.available() > 0)
  {
    SIM800L.read();    // Clean the input buffer
    delay(50);
  }
}

int8_t waitFor(const char* expected_answer1, const char* expected_answer2)
{
  uint8_t x = 0, answer = 0;
  char response[100];
  unsigned long previous;

  memset(response, (char)0, 100);    // Initialize the string

  delay( 250 );

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do {
    // if there are data in the UART input buffer, reads it and checks for the asnwer
    if (SIM800L.available() > 0) {
      response[x] = SIM800L.read();
      x++;
      // check if the desired answer 1  is in the response of the module
      if (strstr(response, expected_answer1) != NULL)
      {
        answer = 1;
      }
      // check if the desired answer 2 is in the response of the module
      else if (strstr(response, expected_answer2) != NULL)
      {
        answer = 2;
      }
    }
    delay(10);
  }
  // Waits for the asnwer with time out
  while ((answer == 0) && ((millis() - previous) < 10000 ));

  return answer;
}


void setup() {
  Serial.begin(9600);
  delay(10);

  SIM800L.begin(9600);
  delay(3000);

  dht.begin();

  SIM800L.println("AT"); //enviando Comando AT
  delay (9000); //Tempo de espera
  resposta();
  SIM800L.println("AT+CMEE=2");
  delay(2000);
  resposta();

  connection();
  scale.set_offset(3327136.00);//Tara setada com base na tara encontrada ao pesar a tabua superior da balança.
  //scale.set_scale();
  //scale.tare();
  scale.set_scale(calibration_factor);
  offset_definido = scale.get_offset();
}

void loop()
{
    resetRetries();

    lerPeso();
    lerTensao();
    lerDHT();
    lerTensaoGSM();

    imprimirDados();
    publicar();
}


