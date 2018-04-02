#define TINY_GSM_MODEM_SIM800
#define SENSOR_TENSAO A4
#define Porta_HX711_A A1 //Era porta A1
#define Porta_HX711_B A0 //Era porta A0
#define SENSOR_DHT 4    //VERIFICAR SE PORTA ESTA VAGA
#define DHTTYPE DHT22
#define RX_do_Arduino 11
#define TX_do_Arduino 10
#define Timeout 10000
#define Timeout_envios 20000

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include "HX711.h"

//////////////////////////////////////////////////////////////////
//Definição de variaveis iniciais

const char apn[]  = "claro.com.br";
const char user[] = "claro";
const char pass[] = "claro";

const char* PesoChannel = "api.thingspeak.com/update?api_key=EETRUP5URNSFV1N7&field2=";
const char* TensaoChannel = "api.thingspeak.com/update?api_key=EETRUP5URNSFV1N7&field1=";
const char* TempChannel = "api.thingspeak.com/update?api_key=EETRUP5URNSFV1N7&field4=";
const char* umidadeChannel = "api.thingspeak.com/update?api_key=EETRUP5URNSFV1N7&field3=";
const char* tensaoGSMChannel = "api.thingspeak.com/update?api_key=EETRUP5URNSFV1N7&field6=";

SoftwareSerial SIM800L(RX_do_Arduino, TX_do_Arduino); // RX do arduino liga com TX do Sim800L, contrario com o TX

TinyGsm modem(SIM800L);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

HX711 scale(Porta_HX711_A, Porta_HX711_B); // se for usar o pino digital faca->  scale(7,6);
DHT dht(SENSOR_DHT, DHT22);

int connection_retry = 0; //Quantidade de tentativas de reconexão
int resetCooldown = 1; //Cooldown de tentativas de conexão, que, caso ultrapassado um certo valor, força o reset do Sim800L


float calibration_factor = 21097.164; // Esse fator de calibração depende sempre da celula de carga usada.
float offset_definido;
float units;
float tensao_lida;
float tensaoGSM_lida;
float temp_lida;
float umidade_lida;

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
  tensao_lida = ((valor_lido_tensao * 0.00489) * 5);
}


void lerTemp()
{
  int DHT_Retries = 0;
  do
  {
    delay(500);
    DHT_Retries += 1;
    Serial.print("Lendo valores de temperatura e umidade - ");
    Serial.println(DHT_Retries);
    temp_lida = dht.readTemperature();
    umidade_lida = dht.readHumidity();
  } while ((isnan(temp_lida) || isnan(umidade_lida)) && (DHT_Retries <= 5));
  if (DHT_Retries > 20)
  {
    Serial.println("Limite de tentativas de coletas ultrapassado, prosseguindo...");
  }

}

void lerPeso()
{
  scale.set_scale(calibration_factor);
  offset_definido = scale.get_offset();
  units = scale.get_units(), 4;
  if (units < 0)
  {
    units = units * -1;
  }
  else
  {
    units = 0.00;
  }
}

void lerTensaoGSM()
{
  tensaoGSM_lida = modem.getBattVoltage() / 1000.0;
}


void publicar(float dado, const char* channel)
{

  SIM800L.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""); //enviando Comando AT
  delay (1000); //Tempo de espera
  resposta();

  SIM800L.println("AT+SAPBR=3,1,\"APN\",\"claro.com.br\""); //enviando Comando AT
  delay (3000); //Tempo de espera
  resposta();

  SIM800L.println("AT+SAPBR=3,1,\"USER\",\"claro\""); //enviando Comando AT
  delay (3000); //Tempo de espera
  resposta();

  SIM800L.println("AT+SAPBR=3,1,\"PWD\",\"claro\""); //enviando Comando AT
  delay (3000); //Tempo de espera
  resposta();

  SIM800L.println("AT+SAPBR=1,1");
  delay (2000);
  resposta();

  SIM800L.println("AT+SAPBR=2,1");
  delay (2000);
  resposta();

  SIM800L.println("AT+HTTPINIT");
  delay (2000);
  resposta();

  SIM800L.println("AT+HTTPPARA=\"CID\",1");
  delay (2000);
  resposta();

  SIM800L.println("AT+HTTPPARA=\"URL\",\"" + String(channel) + String(dado) + "\"");
  delay (3000);
  resposta();

  SIM800L.println("AT+HTTPACTION=0");
  delay (3000);
  resposta();

  SIM800L.println("AT+HTTPREAD");
  delay (3000);
  resposta();

  SIM800L.println("AT+HTTPTERM");
  delay (1000);
  resposta();

  SIM800L.println("AT+SAPBR= 0,1");
  delay (1000);
  resposta();

  delay(Timeout_envios);
}


void publicarTudo(float dadoW, float dadoV, float dadoT, float dadoH, float dadoVGSM)
{



  Serial.println("Inicializando GSM...");
  modem.restart();
  Serial.println("Ok");


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

  hasIP();
  //setupGPRS();

  SIM800L.println("AT+HTTPINIT");
  delay (2000);
  resposta();

  SIM800L.println("AT+HTTPPARA=\"CID\",1");
  delay (2000);
  resposta();

  SIM800L.println("AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update?api_key=EETRUP5URNSFV1N7&field1=" + String(dadoV) + "&field2=" + String(dadoW) + "&field3=" + String(dadoH) + "&field4=" + String(dadoT) + "&field6=" + String(dadoVGSM) + "\"");
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

  SIM800L.println("AT+SAPBR= 0,1"); //EHCERRA
  delay (1000);
  resposta();

  cleanBuffer();
  delay(1000);

  delay(Timeout_envios);
}

void imprimirDados()
{
  Serial.print("Offset : ");
  Serial.println(offset_definido);
  Serial.print("Peso : ");
  Serial.print(units, 3);
  Serial.print("Kg\t");

  Serial.print("Fator de calibração: ");
  Serial.println(calibration_factor);
  Serial.print("Tensao : ");
  Serial.print(tensao_lida, 3);
  Serial.print("\t");

  Serial.print("Tensao GSM : ");
  Serial.println(tensaoGSM_lida, 3);
  Serial.print("Temperatura : ");
  Serial.print(temp_lida, 3);
  Serial.print("\t");

  Serial.print("Umidade : ");
  Serial.println(umidade_lida, 3);
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


}

void loop()
{
  if (resetCooldown % 53 == 0) { //Se após 53(número primo definido empiricamente) tentativas de conexão
    //não for possível realizar conexão, resetar o Sim800L
    Serial.println("Cooldown alcançado, reiniciando GSM...");
    modem.restart();
    connection();
    resetCooldown = 1;
  }
  else {
    //    if (!mqtt.connected()) {
    //      connection_retry++;
    //      if (connection_retry < 50) {
    //        Serial.println("GSM desconectado.");
    //        Serial.println("Tentando reconexão em 10 segundos.");
    //        delay(Timeout);
    //        if (!modem.restart())
    //        {
    //          Serial.println("Módulo esta desligado.");
    //        }
    //        else
    //        {
    //          connection();
    //        }
    //      }
    //      else {
    //        Serial.println("Não foi possivel reconectar o módulo. Superou numero de tentativas estipuladas");
    //        while (true);
    //      }
    //    }
    //    else
    //    {
    resetRetries();

    lerPeso();
    lerTensao();
    lerTemp();
    lerTensaoGSM();

    imprimirDados();



    //    publicar(units, PesoChannel);
    //    publicar(tensao_lida, TensaoChannel);
    //    publicar(temp_lida, TempChannel);
    //    publicar(umidade_lida, umidadeChannel);
    //    publicar(tensaoGSM_lida, tensaoGSMChannel);
    publicarTudo(units, tensao_lida, temp_lida, umidade_lida, tensaoGSM_lida);

    
    resetCooldown++;
  }
}


