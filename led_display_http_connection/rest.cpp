#include "mbed.h"
#include "WiFiInterface.h"

extern volatile bool lock_state;
extern volatile bool state_changed;

#define SERVER_IP "IP_ADDRESS_OF_SERVER" // replace with  server ip
#define SERVER_PORT 5000
#define WIFISSID "WIFI_SSID" // replace with  Wifi ssid
#define WIFIPWD "WIFI_PASSWORD" // replace with  wiFi password


void setup_wifi_and_wait() {
    WiFiInterface *wifi = WiFiInterface::get_default_instance();
    if (!wifi) return;

    wifi->set_credentials(WIFISSID, WIFIPWD, NSAPI_SECURITY_WPA2);
    if (wifi->connect() != 0) return;

    printf("connected to wifi\n");

    while (true) {
        if (state_changed) {
            state_changed = false;

            const char* state_str = lock_state ? "LOCK" : "UNLOCK";
            char json_body[128];
            snprintf(json_body, sizeof(json_body),
                     "{\"device\": \"stm32l475\", \"lock\": \"%s\"}", state_str);

            char http_request[512];
            snprintf(http_request, sizeof(http_request),
                     "POST /api/report HTTP/1.1\r\n"
                     "Host: 192.168.2.30\r\n"
                     "Content-Type: application/json\r\n"
                     "Content-Length: %d\r\n"
                     "Connection: close\r\n"
                     "\r\n"
                     "%s", strlen(json_body), json_body);

            TCPSocket socket;
            socket.open(wifi);
            SocketAddress server;
            wifi->gethostbyname(SERVER_IP, &server);
            server.set_port(SERVER_PORT);
            if (socket.connect(server) == 0) {
                socket.send(http_request, strlen(http_request));

                char buffer[256];
                int len;
                do {
                    len = socket.recv(buffer, sizeof(buffer) - 1);
                    if (len > 0) {
                        buffer[len] = '\0';
                        printf("%s", buffer);
                    }
                } while (len > 0);

                socket.close();
            } else {
                printf("socket failed\n");
            }
        }

        ThisThread::sleep_for(100ms);
    }
}

