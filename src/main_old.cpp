// erro de permissão na porta ao fazer upload ESP32: sudo chmod -R 777 /dev/ttyUSB0

// Lib. do PlatformIO que implementa as funções do Arduino (pinMode, digitalRead, digitalWrite, ...)
#include <Arduino.h>

// Lib. para estabelecer conexão com a rede WiFi
#include <WiFi.h>

// Lib. para gerar uma conexão segura com a rede
#include <WiFiClientSecure.h>

// Lib. para estabelecer conexão com o servidor do bot no Telegram
#include <UniversalTelegramBot.h>

// Lib. para acessar a memória EEPROM
#include <EEPROM.h>

// Arquivo com as informações pessoais necessárias (não deve ser removido do .gitignore) - Instruções para criar em: https://github.com/dimeno157/GrowBot
#include "personal_info.h"

//-------------------------------------------------------------------------------------------------------------

// Numero de milissegundos em uma hora
#define ONE_HOUR 3600000

// Estados Ligado e Desligado de algumas funções
#define OFF 0
#define ON 1

// Ciclos de luz
#define GER 11
#define VEG 22
#define FLOR 33

// Variáveis --------------------------------------------------------------------------------------------------

// PINOS ----------------------

// Pino das luzes Full Spectrum
int lightFSPin = 25;

// Pino das luzes de LED
int lightLEDPin = 27;

// Pino das bombas de irrigação
int irrigationPin = 26;

// Pino dos coolers de saída de ar
int coolerOutPin = 32;

// Pino dos coolers de entrada de ar
int coolerInPin = 33;

// ----------------------------

/* CRIAR MENU COM OS COMANDOS NO CANTO SUPERIOR ESQUERDO DO TECLADO NA CONVERSA COM O BOT.
* Enviar para o @BotFather o comando /setcommands,
* Escolher o bot, caso haja mais de um, e enviar a seguinte mensagem:

status - Status gerais do GrowBot.
luz - Status da luz.
ligaluz - Liga a luz.
desligaluz - Desliga a luz.
ciclo - Ciclo de luz atual.
ger - Muda para germinação(16/8).
veg - Muda para vegetativo(18/6).
flor - Muda para floração(12/12).
ligaeconomia - Liga o modo de economia de energia.
desligaeconomia - Desliga o modo de economia de energia.
irrigacao - Status da irrigação.
irrigar - Realiza uma irrigação.
irrigado - Registra o momento da irrigação.
ligaautoirrigacao - Liga a irrigação automática.
desligaautoirrigacao - Desiga a irrigação automática.
intervaloirrigacao - Muda o intervalo entre irrigações.
tempoirrigacao - Muda o tempo de uma irrigação
ventilacao - Status da ventilação.
ligaventilacao - Liga a ventilação.
desligaventilacao - Desliga a ventilação.

Modifique de acordo com os seus comandos.
Os comandos não podem conter letras maiúsculas e devem estar na forma:
comando - explicação
*/

// Strings dos comandos
struct Commands
{
  String status = "/status";
  String light = "/luz";
  String lightOn = "/ligaluz";
  String lightOff = "/desligaluz";
  String photoperiod = "/ciclo";
  String ger = "/ger";
  String veg = "/veg";
  String flor = "/flor";
  String powerSavingOn = "/ligaeconomia";
  String powerSavingOff = "/desligaeconomia";
  String irrigation = "/irrigacao";
  String irrigate = "/irrigar";
  String irrigated = "/irrigado";
  String autoIrrigationOn = "/ligaautoirrigacao";
  String autoIrrigationOff = "/desligaautoirrigacao";
  String irrigationInterval = "/intervaloirrigacao";
  String irrigationTime = "/tempoirrigacao";
  String ventilation = "/ventilacao";
  String ventilationOn = "/ligaventilacao";
  String ventilationOff = "/desligaventilacao";

} commands;

// Cliente para conexões WiFi
WiFiClientSecure client;

// Conexão com o bot no Telegram
UniversalTelegramBot GrowBot(TOKEN, client);

// Status do modo de economia de energia.
// O modo de economia de energia divide o período que a luz fica ligada em três e as luzes que consomem mais energia ficam ligadas apenas no período do meio.
//    0 -> 1/3 do tempo de luz ligada. Apenas as luzes de LED ligadas.
//    1 -> 1/3 do tempo de luz ligada. Luzes de LED e luzes Full Spectrum ligadas.
//    2 -> 1/3 do tempo de luz ligada. Apenas as luzes de LED ligadas.
//    3 -> Tempo de luz desligada. Todas as luzes desligadas.
int powerSavingStatus;

// Fotoperíodo de luz - luz ligada / luz desligada:
//    VEG -> Vegetativo (16h/8h)
//    FLOR -> Floração (12h/12h)
//    GER -> Germinação (18h/6h)
int photoperiod;

// 0 -> 1/3 do tempo de luz ligada. Apenas as luzes de LED ligadas.
// 1 -> 1/3 do tempo de luz ligada. Luzes de LED e luzes Full Spectrum ligadas.
// 2 -> 1/3 do tempo de luz ligada. Apenas as luzes de LED ligadas.
// 3 -> Tempo de luz desligada. Todas as luzes desligadas.
int currentLightStep;

// Endereço EEPROM para o intervalo entre irrigações
int irrigationIntervalAddress = 0;

// Endereço EEPROM para a flag de validação do intervalo entre irrigações
int irrigationIntervalFlagAddress = 1;

// Endereço EEPROM para a flag do estado da auto-irrigação
int autoIrrigationAddress = 2;

// Endereço EEPROM para o tempo de irrigação
int irrigationTimeAddress = 3;

// Endereço EEPROM para a flag de validação do tempo de irrigação
int irrigationTimeFlagAddress = 4;

// Períodos em horas de cada etapa da iluminação no modo e economia de energia
int lightPeriodsInHours[4];

// Intervalo entre irrigações (em dias)
int irrigationIntervalInDays;

// Tempo em segundos que a bomba fica ligada em uma irrigação
int irrigationTimeInSeconds;

// Tempo em milissegundos da ultima medição do tempo para a luz
unsigned long lightTimeLast;

// Tempo em milissegundos da ultima medição do tempo para a irrigação
unsigned long irrigationTimeLast;

// Tempo em milissegundos medido no momento
unsigned long timeNow;

// Tempo em horas dês de a ultima mudança na luz
unsigned int hoursSinceLastLightChange;

// Tempo total dês de a ultima mudança na luz (só muda quando ela liga ou desliga)
unsigned int totalHoursSinceLastLightChange;

// Tempo em horas dês de a ultima irrigação
unsigned int hoursSinceLastIrrigation;

// Status das luzes de LED
int lightFSStatus;

// Status das luzes Full Spectrum
int lightLEDStatus;

// Indica se a mensagem de inicialização ja foi envidada
bool sentFirstMessage;

// Indica que a mensagem lembrando de irrigar ja foi enviada
bool sentIrrigationMessage;

// Status da auto-irrigação
int autoIrrigationStatus;

// Status da ventilação
int ventilationStatus;

// Tempo total de luz ligada
int lightTotalTimeOn;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

// Lê as novas mensagens e executa o comando correspondente.
void handleNewMessages(int numNewMessages);

// Seta as variáveis dos períodos de tempo (luz, irrigação, etc) de acordo com o ciclo atual.
void setLightIntervals();

// Realiza a irrigação (auto-irrigação ativada) ou envia uma mensagem lembrando da irrigação (auto-irrigação desativada).
void checkAndIrrigate();

// Checa e altera (caso seja necessário) o estado da luz.
void checkAndChangeLightState();

// Checa se ja passou uma hora e acresce as variáveis de medição de tempo.
void checkAndRaiseHours();

// Conecta na rede WiFi.
void connectToNetwork();

// Envia o menu da luz.
void sendLightStatusMessage(String chatId);

// Envia o menu da irrigação.
void sendIrrigationStatusMessage(String chatId, bool lastIrrigationInfo = true, bool nextIrrigationInfo = true);

// Muda o estado da luz.
void changeLightState(int state);

// Muda o ciclo de luz.
void changePhotoperiod(String chatId, int cycle);

// Realiza uma irrigação.
void irrigate(String chatId);

// Liga ou desliga a irrigação automática
void changeAutoIrrigationState(String chatId, int status);

// Registra a irrigação.
void registerIrrigation(String chatId);

// Lê o valor do intervalo entre irrigações na EEPROM
int getIrrigationInterval();

// Grava o valor do intervalo entre irrigações na EEPROM
void setIrrigationInterval(int interval);

// Carrega os valores da irrigação
void initIrrigationData();

// Escreve na EEPROM
void writeEEPROM(int address, uint8_t val);

// Atualiza o intervalo entre irrigações a partir de uma mensagem
void updateIrrigationInterval(String message, String chatId);

// Atualiza o tempo de irrigação a partir de uma mensagem
void updateIrrigationTime(String message, String chatId);

// Lê o tempo de irrigação da EEPROM
int getIrrigationTime();

// Atualiza o tempo de irrigação e grava o valor na EEPROM
void setIrrigationTime(int time);

// Lê um valor inteiro em uma mensagem da forma:
//
// mensagem = "/comando N"
//
// onde N é o valor inteiro retornado
int getValueFromMessage(String command, String message);

// Envia a mensagem com os status do GrowBot
void sendStatusInfo(String chatId);

// Retorna uma string com o nome completo do ciclo de luz
String getLightCycleName(int cycle, bool withTimes = true);

// Atualiza a etapa de iluminação
void setLightStep(int step, bool sendMessage = true);

// Muda o status da ventilação
void changeVentilationStatus(int status);

// Envia o status da ventilação
void sendVentilationStatusMessage(String chatId);

// Muda o status da economia de energia
void changePowerSavingStatus(int status);

// Envia a mensagem com o status da economia de energia
void sendPowerSavingStatus(String chatId);

// Configura os valores iniciais dos pinos de saída
void setOutPinsInitialValues();

// Retorna o status da iluminação
int getLightStatus();

// Muda o estado do pino de saída da luz
void changeLightPinState(int pin, int state);

// Define os valores iniciais das variáveis
void setVariablesInitialValues();

// MAIN LOOPS ----------------------------------------------------------------------------------------------

void setup1()
{
  setVariablesInitialValues();

  setLightIntervals();

  initIrrigationData();

  setOutPinsInitialValues();

  connectToNetwork();
}

//-----------------------

void loop1()
{
  // caso não a placa não esteja conectada a rede WiFi
  if (WiFi.status() != WL_CONNECTED)
  {
    connectToNetwork();
  }
  // caso a placa esteja conectada a rede WIfi
  else
  {
    // pega o numero de novas mensagens dês de a ultima checagem
    int numNewMessages = GrowBot.getUpdates(GrowBot.last_message_received + 1);
    handleNewMessages(numNewMessages);
  }

  setLightIntervals();

  checkAndRaiseHours();

  checkAndChangeLightState();

  checkAndIrrigate();
}

// FUNCTIONS -----------------------------------------------------------------------------------------------

void setVariablesInitialValues()
{
  // Valores iniciais das variáveis gerais
  sentFirstMessage = false;
  timeNow = 0;
  client.setInsecure();
  EEPROM.begin(512);

  // Valores inicias das variáveis de iluminação
  currentLightStep = 0;
  hoursSinceLastLightChange = 0;
  totalHoursSinceLastLightChange = 0;
  photoperiod = VEG;
  lightTimeLast = 0;
  lightLEDStatus = ON;
  lightFSStatus = OFF;
  powerSavingStatus = ON;

  // Valores iniciais das variáveis de irrigação
  irrigationTimeLast = 0;
  hoursSinceLastIrrigation = 0;
  irrigationTimeInSeconds = 15;
  irrigationIntervalInDays = 5;
  sentIrrigationMessage = false;
  autoIrrigationStatus = ON;

  // Valores iniciais das variáveis de ventilação
  ventilationStatus = ON;

  return;
}

//-----------------------

void setOutPinsInitialValues()
{
  // Seta o pino da luz LED como saída e liga
  pinMode(lightLEDPin, OUTPUT);
  changeLightPinState(lightLEDPin, lightLEDStatus);

  // Seta o pino da luz FS como saída e liga
  pinMode(lightFSPin, OUTPUT);
  changeLightPinState(lightFSPin, lightFSStatus);

  // Seta o pino da irrigação como saída e desliga
  pinMode(irrigationPin, OUTPUT);
  digitalWrite(irrigationPin, LOW);

  // Seta o pino da exaustão como saída e liga
  pinMode(coolerOutPin, OUTPUT);
  digitalWrite(coolerOutPin, HIGH);

  // Seta o pino da ventilação como saída e liga
  pinMode(coolerInPin, OUTPUT);
  digitalWrite(coolerInPin, HIGH);
}

//-----------------------

void checkAndRaiseHours()
{
  timeNow = millis();

  // Faz a checagem do tempo para aluz
  if (lightTimeLast > timeNow)
  {
    lightTimeLast = timeNow;
  }
  if (timeNow - lightTimeLast >= ONE_HOUR)
  {
    hoursSinceLastLightChange += 1;
    totalHoursSinceLastLightChange += 1;
    lightTimeLast = timeNow;
  }

  // Faz a checagem do tempo para a irrigação
  if (irrigationTimeLast > timeNow)
  {
    irrigationTimeLast = timeNow;
  }
  if (timeNow - irrigationTimeLast >= ONE_HOUR)
  {
    hoursSinceLastIrrigation += 1;
    irrigationTimeLast = timeNow;
  }
  return;
}

//-----------------------

void setLightIntervals()
{
  switch (photoperiod)
  {
  case GER:
    lightTotalTimeOn = 16;
    break;
  case FLOR:
    lightTotalTimeOn = 12;
    break;
  default:
    lightTotalTimeOn = 18;
    break;
  }

  if (powerSavingStatus == ON)
  {
    lightPeriodsInHours[0] = int(lightTotalTimeOn / 3);                       // LED ON - FS OFF
    lightPeriodsInHours[1] = lightTotalTimeOn - (2 * lightPeriodsInHours[0]); // LED ON - FS ON
    lightPeriodsInHours[2] = lightPeriodsInHours[0];                          // LED ON - FS OFF
    lightPeriodsInHours[3] = 24 - lightTotalTimeOn;                           // LED OFF - FS OFF
  }
  else
  {
    lightPeriodsInHours[0] = 0;
    lightPeriodsInHours[1] = lightTotalTimeOn; // LED ON - FS ON
    lightPeriodsInHours[2] = 0;
    lightPeriodsInHours[3] = 24 - lightTotalTimeOn; // LED OFF - FS OFF
  }

  return;
}

//-----------------------

void checkAndIrrigate()
{
  if (hoursSinceLastIrrigation >= irrigationIntervalInDays * 24)
  {
    if (autoIrrigationStatus == ON)
    {
      irrigate(MY_ID);
    }
    else if (!sentIrrigationMessage)
    {
      sendIrrigationStatusMessage(MY_ID, true, false);
      sentIrrigationMessage = true;
    }
  }
  return;
}

//-----------------------

void checkAndChangeLightState()
{
  if (hoursSinceLastLightChange >= lightPeriodsInHours[currentLightStep])
  {
    // vai para o próximo período da luz - 0 -> 1 -> 2 -> 3 -> 0 -> ...
    setLightStep((currentLightStep + 1) % 4);
    hoursSinceLastLightChange = 0;
  }
  return;
}

//-----------------------

void handleNewMessages(int numNewMessages)
{
  if (numNewMessages > 0)
  {
    for (int i = 0; i < numNewMessages; i++)
    {
      String comando = GrowBot.messages[i].text;
      String chatId = GrowBot.messages[i].chat_id;

      if (chatId == MY_ID)
      {
        if (comando.equalsIgnoreCase(commands.status)) // Status do GrowBot
        {
          sendStatusInfo(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.photoperiod)) // Informa o fotoperíodo
        {
          GrowBot.sendMessage(chatId, getLightCycleName(photoperiod));
        }
        else if (comando.equalsIgnoreCase(commands.irrigation)) // Informa o status da irrigação
        {
          sendIrrigationStatusMessage(chatId, true, autoIrrigationStatus == ON);
        }
        else if (comando.equalsIgnoreCase(commands.irrigate)) // Realiza uma irrigação
        {
          irrigate(chatId);
          sendIrrigationStatusMessage(chatId, false);
        }
        else if (comando.equalsIgnoreCase(commands.irrigated)) // Registra uma irrigação manual
        {
          registerIrrigation(chatId);
          sendIrrigationStatusMessage(chatId, false);
        }
        else if (comando.indexOf(commands.irrigationInterval) >= 0) // Muda o intervalo entre irrigações
        {
          updateIrrigationInterval(comando, chatId);
        }
        else if (comando.indexOf(commands.irrigationTime) >= 0) // Muda o tempo de uma irrigação
        {
          updateIrrigationTime(comando, chatId);
        }
        else if (comando.equalsIgnoreCase(commands.autoIrrigationOn) && autoIrrigationStatus != ON) // Liga a auto-irrigação
        {
          changeAutoIrrigationState(chatId, ON);
          sendIrrigationStatusMessage(chatId, true, autoIrrigationStatus == ON);
        }
        else if (comando.equalsIgnoreCase(commands.autoIrrigationOff) && autoIrrigationStatus != OFF) // Desliga a auto-irrigação
        {
          changeAutoIrrigationState(chatId, OFF);
          sendIrrigationStatusMessage(chatId, true, autoIrrigationStatus == ON);
        }
        else if (comando.equalsIgnoreCase(commands.veg) && photoperiod != VEG) // Muda para o fotoperíodo de vegetativo
        {
          changePhotoperiod(chatId, VEG);
          sendLightStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.flor) && photoperiod != FLOR) // Muda para o fotoperíodo de floração
        {
          changePhotoperiod(chatId, FLOR);
          sendLightStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.ger) && photoperiod != GER) // Muda para o fotoperíodo de germinação
        {
          changePhotoperiod(chatId, GER);
          sendLightStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.light)) // Envia a mensagem com o status da iluminação
        {
          sendLightStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.lightOn) && currentLightStep > 2) // Liga a luz
        {
          changeLightState(ON);
          sendLightStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.lightOff) && currentLightStep < 3) // Desliga a luz
        {
          changeLightState(OFF);
          sendLightStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.ventilation)) // Envia a mensagem com o status da ventilação
        {
          sendVentilationStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.ventilationOn) && ventilationStatus != ON) // Liga a ventilação
        {
          changeVentilationStatus(ON);
          sendVentilationStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.ventilationOff) && ventilationStatus != OFF) // Desliga a a ventilação
        {
          changeVentilationStatus(OFF);
          sendVentilationStatusMessage(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.powerSavingOn) && powerSavingStatus != ON) // Liga o modo de economia de energia
        {
          changePowerSavingStatus(ON);
          sendPowerSavingStatus(chatId);
        }
        else if (comando.equalsIgnoreCase(commands.powerSavingOff) && powerSavingStatus != OFF) // Desliga o modo de economia de energia
        {
          changePowerSavingStatus(OFF);
          sendPowerSavingStatus(chatId);
        }
      }
    }
  }
  return;
}

//-----------------------

void connectToNetwork()
{
  // Inicia em modo station (mais um dispositivo na rede, o outro modo é o Access Point em que aplaca fornece uma rede)
  WiFi.mode(WIFI_STA);
  // Conecta na rede com o ssid e senha
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Espera 10 segundos para garantir que a conexão seja estabelecida
  delay(10000);
  // Se ja tiver conectado
  if (WiFi.status() == WL_CONNECTED)
  {
    // Se ja tiver enviado a primeira mensagem significa que a conexão caiu
    if (sentFirstMessage)
    {
      GrowBot.sendMessage(MY_ID, "--- Conexão reestabelecida ---");
    }
    // Se não tiver enviado a primeira mensagem significa que a placa acabou de ligar
    else
    {
      sentFirstMessage = GrowBot.sendMessage(MY_ID, "--- GrowBot ativa ---");
      sendStatusInfo(MY_ID);
    }
  }
  return;
}

//-----------------------

int getLightStatus()
{
  return lightLEDStatus == ON || lightFSStatus == ON;
}

//-----------------------

void sendLightStatusMessage(String chatId)
{
  if (getLightStatus() == ON)
  {
    GrowBot.sendMessage(chatId, "Luz ligada ha " + String(hoursSinceLastLightChange) + " horas\nRestam " + String(lightTotalTimeOn - hoursSinceLastLightChange) + " para desligar");
  }
  else
  {
    GrowBot.sendMessage(chatId, "Luz desligada ha " + String(hoursSinceLastLightChange) + " horas\nRestam " + String((24 - lightTotalTimeOn) - hoursSinceLastLightChange) + " para ligar");
  }
  return;
}

//-----------------------

void sendIrrigationStatusMessage(String chatId, bool lastIrrigationInfo, bool nextIrrigationInfo)
{
  if (lastIrrigationInfo)
  {
    GrowBot.sendMessage(chatId, "Ultima irrigação realizada ha " + String(int(hoursSinceLastIrrigation / 24)) + " dias e " + String(int(hoursSinceLastIrrigation % 24)) + " horas.");
  }
  if (nextIrrigationInfo)
  {
    GrowBot.sendMessage(chatId, String(int(((irrigationIntervalInDays * 24) - hoursSinceLastIrrigation) / 24)) + " dias e " + String(int(((irrigationIntervalInDays * 24) - hoursSinceLastIrrigation) % 24)) + " horas restantes até a próxima irrigação.");
  }
  return;
}

//-----------------------

void changeLightState(int state)
{
  switch (state)
  {
  case ON:
    setLightStep(0);
    break;
  case OFF:
    setLightStep(3);
    break;
  default:
    break;
  }
  hoursSinceLastLightChange = 0;
  return;
}

//-----------------------

void setLightStep(int step, bool sendMessage)
{
  currentLightStep = step;
  switch (currentLightStep)
  {
  case 0:
    lightLEDStatus = ON;
    lightFSStatus = OFF;
    if (sendMessage)
    {
      GrowBot.sendMessage(MY_ID, String("Luz ligada após ") + String(totalHoursSinceLastLightChange) + String(" horas"));
    }
    totalHoursSinceLastLightChange = 0;
    break;
  case 1:
    lightLEDStatus = ON;
    lightFSStatus = ON;
    break;
  case 2:
    lightLEDStatus = ON;
    lightFSStatus = OFF;
    break;
  case 3:
    lightLEDStatus = OFF;
    lightFSStatus = OFF;
    if (sendMessage)
    {
      GrowBot.sendMessage(MY_ID, String("Luz desligada após ") + String((totalHoursSinceLastLightChange)) + String(" horas"));
    }
    totalHoursSinceLastLightChange = 0;
    break;
  default:
    break;
  }
  changeLightPinState(lightLEDPin, lightLEDStatus);
  changeLightPinState(lightFSPin, lightFSStatus);
  return;
}

//-----------------------

void changeAutoIrrigationState(String chatId, int status)
{
  switch (status)
  {
  case ON:
    autoIrrigationStatus = ON;
    writeEEPROM(autoIrrigationAddress, 1);
    GrowBot.sendMessage(chatId, "Irrigação automática ligada.");
    break;
  case OFF:
    autoIrrigationStatus = OFF;
    writeEEPROM(autoIrrigationAddress, 0);
    GrowBot.sendMessage(chatId, "Irrigação automática desligada.");
    break;
  default:
    break;
  }
  return;
}

//-----------------------

void changePhotoperiod(String chatId, int cycle)
{
  photoperiod = cycle;
  GrowBot.sendMessage(chatId, "Ciclo atual alterado para " + getLightCycleName(photoperiod));
  setLightIntervals();
  return;
}

//-----------------------

void irrigate(String chatId)
{
  digitalWrite(irrigationPin, HIGH);
  delay(irrigationTimeInSeconds * 1000);
  digitalWrite(irrigationPin, LOW);
  hoursSinceLastIrrigation = 0;
  sentIrrigationMessage = false;
  GrowBot.sendMessage(chatId, "Irrigação realizada.");
  return;
}

//-----------------------

void registerIrrigation(String chatId)
{
  hoursSinceLastIrrigation = 0;
  sentIrrigationMessage = false;
  GrowBot.sendMessage(chatId, "Irrigação registrada.");
  return;
}

// -----------------------

int getIrrigationInterval()
{
  if (EEPROM.read(irrigationIntervalFlagAddress) == 1 && EEPROM.read(irrigationIntervalAddress) > 0)
  {
    return EEPROM.read(irrigationIntervalAddress);
  }
  return irrigationIntervalInDays;
}

// -----------------------

int getIrrigationTime()
{
  if (EEPROM.read(irrigationTimeFlagAddress) == 1 && EEPROM.read(irrigationTimeAddress) > 0)
  {
    return EEPROM.read(irrigationTimeAddress);
  }
  return irrigationTimeInSeconds;
}

//-----------------------

void setIrrigationInterval(int interval)
{
  irrigationIntervalInDays = interval;
  writeEEPROM(irrigationIntervalAddress, interval);
  writeEEPROM(irrigationIntervalFlagAddress, 1);
}

// -----------------------

void setIrrigationTime(int time)
{
  irrigationTimeInSeconds = time;
  writeEEPROM(irrigationTimeAddress, time);
  writeEEPROM(irrigationTimeFlagAddress, 1);
}

//-----------------------

void initIrrigationData()
{
  // init irrigation interval
  if (EEPROM.read(irrigationIntervalFlagAddress) != 1)
  {
    setIrrigationInterval(irrigationIntervalInDays);
  }
  irrigationIntervalInDays = getIrrigationInterval();

  // init irrigation time
  if (EEPROM.read(irrigationTimeFlagAddress) != 1)
  {
    setIrrigationTime(irrigationTimeInSeconds);
  }
  irrigationTimeInSeconds = getIrrigationTime();

  // init auto irrigation
  if (EEPROM.read(autoIrrigationAddress) == 1)
  {
    autoIrrigationStatus = ON;
  }
  else
  {
    autoIrrigationStatus = OFF;
  }

  return;
}

//-----------------------

void writeEEPROM(int address, uint8_t val)
{
  EEPROM.write(address, val);
  EEPROM.commit();
}

//-----------------------

void updateIrrigationInterval(String message, String chatId)
{
  int interval = getValueFromMessage(commands.irrigationInterval, message);
  if (interval == 0)
  {
    GrowBot.sendMessage(chatId, "Para modificar o intervalo de irrigação mande a mensagem da forma:\n\n" + String(commands.irrigationInterval) + " N\n\nN é o intervalo de irrigação em dias e deve ser maior que zero.");
    return;
  }
  int oldInterval = irrigationIntervalInDays;
  setIrrigationInterval(interval);
  GrowBot.sendMessage(chatId, "Intervalo de irrigação alterado de " + String(oldInterval) + " dias para " + String(interval) + " dias.");
  return;
}

// -----------------------

void updateIrrigationTime(String message, String chatId)
{
  int time = getValueFromMessage(commands.irrigationTime, message);
  if (time == 0)
  {
    GrowBot.sendMessage(chatId, "Para modificar o tempo de irrigação mande a mensagem da forma:\n\n" + String(commands.irrigationTime) + " N\n\nOnde N é o tempo de irrigação em segundos e deve ser maior que zero.");
    return;
  }
  int oldTime = irrigationTimeInSeconds;
  setIrrigationTime(time);
  GrowBot.sendMessage(chatId, "Tempo de irrigação alterado de " + String(oldTime) + " segundos para " + String(time) + " segundos.");
  return;
}

//-----------------------

int getValueFromMessage(String command, String message)
{
  String formattedCommand = String(command) + " ";
  if (message.length() <= formattedCommand.length())
  {
    return 0;
  }

  int index = message.indexOf(formattedCommand);
  if (index < 0)
  {
    return 0;
  }

  int interval = message.substring(formattedCommand.length()).toInt();
  if (interval <= 0)
  {
    return 0;
  }

  return interval;
}

//-----------------------

void sendStatusInfo(String chatId)
{
  String message = "Status:\n\n";

  // light status
  message += "LUZ \xF0\x9F\x92\xA1 \n";
  message += "- Ciclo de luz: " + getLightCycleName(photoperiod) + ".\n";
  message += "- Status da luz LED: " + String(lightLEDStatus == ON ? "ligada" : "desligada") + ".\n";
  message += "- Status da luz Full Spectrum: " + String(lightFSStatus == ON ? "ligada" : "desligada") + ".\n";
  message += "- Etapa de iluminação: " + String(currentLightStep) + ".\n";
  message += "- Tempo dês de a ultima mudança na luz: " + String(totalHoursSinceLastLightChange) + " horas.\n";
  message += "- Status da economia de energia: " + String(powerSavingStatus == ON ? "ligada" : "desligada") + ".\n";
  // add new light status here:

  message += "\n";

  // irrigation status
  message += "IRRIGAÇÃO \xF0\x9F\x9A\xBF \n";
  message += "- Intervalo entre irrigações: " + String(irrigationIntervalInDays) + " dias.\n";
  message += "- Tempo de irrigação: " + String(irrigationTimeInSeconds) + " segundos.\n";
  message += "- Status da auto-irrigação: " + String(autoIrrigationStatus == ON ? "ligada" : "desligada") + ".\n";
  message += "- Tempo dês de a ultima irrigação: " + String(int(hoursSinceLastIrrigation / 24)) + " dias e " + String(int(hoursSinceLastIrrigation % 24)) + " horas.\n";
  // add new irrigation status bellow:

  message += "\n";

  message += "VENTILAÇÃO \xF0\x9F\x86\x92 \n";
  message += "- Status da ventilação: " + String(ventilationStatus == ON ? "ligada" : "desligada") + ".\n";

  GrowBot.sendMessage(chatId, message);
}

//-----------------------

String getLightCycleName(int cycle, bool withTimes)
{
  switch (cycle)
  {
  case VEG:
    return "Vegetativo" + String(withTimes ? " (18/6)" : "");
  case GER:
    return "Germinação" + String(withTimes ? " (16/8)" : "");
  case FLOR:
    return "Floração" + String(withTimes ? " (12/12)" : "");
  default:
    return "Unknown";
  }
}

//-----------------------

void changePowerSavingStatus(int status)
{
  powerSavingStatus = status;
  // se a luz estiver desligada basta apenas mudar o status da economia de energia
  if (getLightStatus() == OFF)
  {
    setLightIntervals();
    return;
  }

  // se a luz estiver ligada e a economia de energia estiver sendo desligada
  if (status == OFF)
  {
    hoursSinceLastLightChange = totalHoursSinceLastLightChange;
    setLightIntervals();
    setLightStep(1, false);
    return;
  }

  // se a luz estiver ligada e a economia de energia estiver sendo ligada
  int currentLightStepAux = 0;
  int hoursSinceLastLightChangeAux = hoursSinceLastLightChange;
  setLightIntervals();
  while (hoursSinceLastLightChangeAux - lightPeriodsInHours[currentLightStepAux] >= 0)
  {
    hoursSinceLastLightChangeAux -= lightPeriodsInHours[currentLightStepAux];
    currentLightStepAux += 1;
  }
  hoursSinceLastLightChange = hoursSinceLastLightChangeAux;
  setLightStep(currentLightStepAux, false);
  return;
}

//-----------------------

void sendPowerSavingStatus(String chatId)
{
  String message = "Economia de energia " + String(powerSavingStatus == ON ? "ligada" : "desligada") + ".";
  GrowBot.sendMessage(chatId, message);
  return;
}

//-----------------------

void changeVentilationStatus(int status)
{
  switch (status)
  {
  case ON:
    digitalWrite(coolerInPin, HIGH);
    digitalWrite(coolerOutPin, HIGH);
    ventilationStatus = ON;
    break;
  case OFF:
    digitalWrite(coolerInPin, LOW);
    digitalWrite(coolerOutPin, LOW);
    ventilationStatus = OFF;
    break;
  default:
    break;
  }
  return;
}

//-----------------------

void changeLightPinState(int pin, int state)
{
  // O relé da luz liga em LOW e desliga em HIGH
  if (state == ON)
  {
    digitalWrite(pin, LOW);
  }
  else
  {
    digitalWrite(pin, HIGH);
  }
}

//-----------------------

void sendVentilationStatusMessage(String chatId)
{
  String message = "Ventilação " + String(ventilationStatus == ON ? "ligada" : "desligada") + ".";
  GrowBot.sendMessage(chatId, message);
}