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

const char* broker = "35.199.66.232";
const char* testeChannel = "/webservice/coleta/1/1/1/1/1/1/1/1";
const char* PesoChannel = "channels/373671/publish/fields/field2/EETRUP5URNSFV1N7";
const char* TensaoChannel = "channels/373671/publish/fields/field1/EETRUP5URNSFV1N7";
const char* TempChannel = "channels/373671/publish/fields/field4/EETRUP5URNSFV1N7";
const char* umidadeChannel = "channels/373671/publish/fields/field3/EETRUP5URNSFV1N7";
const char* tensaoGSMChannel = "channels/373671/publish/fields/field6/EETRUP5URNSFV1N7";

SoftwareSerial SerialAT(RX_do_Arduino, TX_do_Arduino); // RX do arduino liga com TX do Sim800L, contrario com o TX

TinyGsm modem(SerialAT);
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

  mqtt.setServer(broker, 8080);
  Serial.println("Conectando ao broker...");
  while (!mqtt.connect("Smartbee2"))
  {
    connection_retry++;
    Serial.println("Tentando reconectar ao broker...");
    Serial.print("Número de tentativas - ");
    Serial.println(connection_retry);
    if (connection_retry >= 3)
    {
      break;
    }
    delay(Timeout);
  }
  resetRetries();
  Serial.print("Sucesso.");
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
   tensaoGSM_lida = modem.getBattVoltage()/1000.0;
}


void publicar(float dado, const char* channel)
{
  char dado_char[10] = "";
  dtostrf(dado, 8, 3, dado_char);
  mqtt.publish(channel, dado_char);
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
  Serial.println(umidade_lida,3);
}

void resetRetries() {
  connection_retry = 0;
}

void setup() {
  Serial.begin(9600);
  delay(10);

  SerialAT.begin(9600);
  delay(3000);

  dht.begin();

  connection();
  scale.set_offset(3242793.00);//Tara setada com base na tara encontrada ao pesar a tabua superior da balança.
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
    if (!mqtt.connected()) {
      connection_retry++;
      if (connection_retry < 50) {
        Serial.println("GSM desconectado.");
        Serial.println("Tentando reconexão em 10 segundos.");
        delay(Timeout);
        if (!modem.restart())
        {
          Serial.println("Módulo esta desligado.");
        }
        else
        {
          connection();
        }
      }
      else {
        Serial.println("Não foi possivel reconectar o módulo. Superou numero de tentativas estipuladas");
        while (true);
      }
    }
    else
    {
      resetRetries();
      
      lerPeso();
      lerTensao();
      lerTemp();
      lerTensaoGSM();

      imprimirDados();

//      publicar(units, PesoChannel);
//      publicar(tensao_lida, TensaoChannel);
//      publicar(temp_lida, TempChannel);
//      publicar(umidade_lida, umidadeChannel);
//      publicar(tensaoGSM_lida, tensaoGSMChannel);
      publicar(0, testeChannel);      
      resetCooldown++;
    }
  }
}

