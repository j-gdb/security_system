// mqtt_receiver.cpp

#include "mbed.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include "WiFiInterface.h"

// global
extern volatile bool lock_state;

// config
static const char* WIFI_SSID     = "Bell738";
static const char* WIFI_PASSWORD = "4165097451";
static const char* BROKER_IP     = "192.168.2.30";
static const int   BROKER_PORT   = 1883;
static const char* TOPIC         = "home/lock";

// callback
void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &msg = md.message;
    char buf[16] = {0};
    size_t len = msg.payloadlen < sizeof(buf) - 1 ? msg.payloadlen : sizeof(buf) - 1;
    memcpy(buf, msg.payload, len);

    if (strcmp(buf, "LOCK") == 0) {
        lock_state = true;
    } else if (strcmp(buf, "UNLOCK") == 0) {
        lock_state = false;
    }
}

// wifi and mqtt
void setup_mqtt_and_wait()
{
    WiFiInterface *wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        printf("ERROR: No WiFiInterface found.\n");
        return;
    }

    printf("Connecting to %s...\n", WIFI_SSID);
    int ret = wifi->connect(WIFI_SSID, WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("ERROR: WiFi connect failed (%d)\n", ret);
        return;
    }

    SocketAddress ip;
    wifi->get_ip_address(&ip);
    printf("Connected to WiFi, IP = %s\n", ip.get_ip_address());

    MQTTNetwork mqttNet(wifi);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNet);

    if (mqttNet.connect(BROKER_IP, BROKER_PORT) != 0) {
        printf("ERROR: TCP connect failed\n");
        return;
    }

    MQTTPacket_connectData connData = MQTTPacket_connectData_initializer;
    connData.MQTTVersion = 3;
    connData.clientID.cstring = (char*)"stm32_lock_client";

    if (client.connect(connData) != 0) {
        printf("ERROR: MQTT connect failed\n");
        return;
    }

    if (client.subscribe(TOPIC, MQTT::QOS0, messageArrived) != 0) {
        printf("ERROR: MQTT subscribe failed\n");
        return;
    }

    printf("MQTT connected and subscribed to %s\n", TOPIC);

    // main loop
    while (true) {
        client.yield(100);
        ThisThread::sleep_for(100ms);
    }
}
