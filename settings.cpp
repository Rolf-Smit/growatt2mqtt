#include "settings.h"

#define INVERTER_UNIQUE_ID "9d38"
#define INVERTER_MODEL "MIN 3600TL-XE"

const char* inverterUniqueId = INVERTER_UNIQUE_ID;

const char* ssid = "xxxx";                    // Wifi SSID
const char* password = "xxxx";                // Wifi password
const char* mqtt_server = "192.168.xxx.xxx";  // MQTT server
const char* mqtt_user = "xxxx";               // MQTT userid
const char* mqtt_password = "xxxx";           // MQTT password

const char* clientID = "growatt-" INVERTER_UNIQUE_ID;  // MQTT client ID
const char* topicRoot = "growatt/" INVERTER_UNIQUE_ID; // MQTT root topic for the device


#define STATE_CLASS_MEASUREMENT "measurement"
#define STATE_CLASS_TOTAL_INCREASING "total_increasing"

// Topic postfix for the ESP "reader" entities
#define TOPIC_POSTFIX_READER "status"
// Topic postfix for the inverter entities
#define TOPIC_POSTFIX_INVERTER "data"


void publishHomeAssistantAutoDiscoveryConfiguration(PubSubClient& mqtt);
void publishHomeAssistantConfigurationObject(PubSubClient& mqtt, const bool isReaderEntity, const char* stateClass, const char* entityName, const char* friendlyName, const char* valueTemplate, const char* unit, const char* deviceClass);


void onConnectedToMqttBroker(PubSubClient& mqtt) {
      // (Re)Connected to MQTT broker, to be sure (re)publish auto-discovery configuration
      publishHomeAssistantAutoDiscoveryConfiguration(mqtt);
      mqtt.subscribe("homeassistant/status");
}

void onReceivedMqttMessage(PubSubClient& mqtt, const char* topic, const char* payload) {
  if (strcmp("homeassistant/status", topic) == 0 && strcmp("online", payload) == 0) {
    // Home Assistant MQTT Client came onine, republish auto-discovery configuration
    publishHomeAssistantAutoDiscoveryConfiguration(mqtt);
  }
}

void publishHomeAssistantAutoDiscoveryConfiguration(PubSubClient& mqtt) {

  // ESP reader state
  publishHomeAssistantConfigurationObject(mqtt, true, STATE_CLASS_MEASUREMENT, "reader-uptime", "Reader uptime", "{{value_json['uptime']}}", "min", "duration");

  // Inverter state
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "pv1-voltage", "PV1 Voltage", "{{value_json['pv1voltage']}}", "V", "voltage");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "pv2-voltage", "PV2 Voltage", "{{value_json['pv1voltage']}}", "V", "voltage");

  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "pv1-current", "PV1 Current", "{{value_json['pv1current']}}", "A", "current");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "pv2-current", "PV2 Current", "{{value_json['pv2current']}}", "A", "current");

  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "pv1-current", "PV1 Current", "{{value_json['pv1current']}}", "A", "current");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "pv2-current", "PV2 Current", "{{value_json['pv2current']}}", "A", "current");

  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "pv1-power", "PV1 Power", "{{value_json['pv1power']}}", "W", "power");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "pv2-power", "PV2 Power", "{{value_json['pv2power']}}", "W", "power");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "output-power", "Output Power", "{{value_json['outputpower']}}", "W", "power");

  // TODO add iamge, such as: "mdi:counter"
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_TOTAL_INCREASING, "pv1-energy-today", "PV1 Energy Today", "{{value_json['pv1energytoday']}}", "kWh", "energy");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_TOTAL_INCREASING, "pv2-energy-today", "PV2 Energy Today", "{{value_json['pv2energytoday']}}", "kWh", "energy");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_TOTAL_INCREASING, "energy-today", "Energy Today", "{{value_json['energytoday']}}", "kWh", "energy");

  // TODO add iamge, such as: "mdi:chart-bar-stacked"
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_TOTAL_INCREASING, "pv1-energy-total", "PV1 Energy Total", "{{value_json['pv1energytotal']}}", "kWh", "energy");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_TOTAL_INCREASING, "pv2-energy-total", "PV2 Energy Total", "{{value_json['pv2energytotal']}}", "kWh", "energy");
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_TOTAL_INCREASING, "energy-total", "Energy Total", "{{value_json['energytotal']}}", "kWh", "energy");

  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "grid-voltage", "Grid Voltage", "{{value_json['gridvoltage']}}", "V", "voltage");

  // TODO add iamge, such as: mdi:rotate-3d-variant
  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "grid-frequency", "Grid Frequency", "{{value_json['gridfrequency']}}", "Hz", "frequency");

  publishHomeAssistantConfigurationObject(mqtt, false, STATE_CLASS_MEASUREMENT, "temperature", "Temperature", "{{value_json['tempinverter']}}", "°C", "temperature");

  Serial.println(F("Done publishing Home Assistant auto-discovery topics!"));
}

/**
 *
 * 
 */
void publishHomeAssistantConfigurationObject(PubSubClient& mqtt, const bool isReaderEntity, const char* stateClass, const char* entityName, const char* friendlyName, const char* valueTemplate, const char* unit, const char* deviceClass) {

  // Device name for Home assistant (Should this be configurable?)
  const char* deviceNameFriendly = "Growatt";

  char deviceName[strlen(deviceNameFriendly) + 1];
  strcpy(deviceName, deviceNameFriendly);
  strlwr(deviceName);

  char buffer[400];

  const char* topicPostfix = isReaderEntity ? TOPIC_POSTFIX_READER : TOPIC_POSTFIX_INVERTER;

  // TODO make model configurable? Maybe put this in Flash memory as well?
  const char* jsonTemplate =  "{ \"stat_t\": \"%s/%s\", \"dev\": { \"ids\": [\"%s-%s\"], \"name\": \"%s %s\", \"mdl\": \"%s\", \"mf\": \"Growatt\" }, \"stat_cla\": \"%s\", \"name\": \"%s\", \"val_tpl\": \"%s\", \"unit_of_meas\": \"%s\", \"dev_cla\": \"%s\", \"uniq_id\": \"%s-%s-%s\"}";
  sprintf(buffer, jsonTemplate,
    topicRoot,                               // Entity status topic root
    topicPostfix,                            // Entity status topic root and the postfix together form the home assistant status_topic
    deviceName, inverterUniqueId,            // Device ID
    deviceNameFriendly, inverterUniqueId,    // Device Name
    INVERTER_MODEL,                          // Device Inverter Model
    stateClass,                              // Entity State class
    friendlyName,                            // Entity Name
    valueTemplate,                           // Entity Value template
    unit,                                    // Entity Unit of Measurement
    deviceClass,                             // Entity Device Class
    deviceName, inverterUniqueId, entityName // Entity Unique ID
  );

  char topic[100];
  sprintf(topic, "homeassistant/sensor/%s-%s-%s/config", deviceName, inverterUniqueId, entityName);

  #ifdef DEBUG_SERIAL
    Serial.println(F("Sending auto-discovery message:"));
    Serial.println(topic);
    Serial.println(buffer);
  #endif

  if(!mqtt.publish(topic, buffer)){
    Serial.println(F("Error: Failed to send Home Assistant auto-discovery topic!"));
  }
}