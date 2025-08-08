#include "mbed.h"
#include "WiFiInterface.h"
#include <cstdio>
#include <cstring>
#include "encrypt.hpp"
#include "toggle.hpp"

extern volatile bool lock_state;
extern volatile bool state_changed;

#define SERVER_IP "10.66.169.168" 
#define SERVER_PORT 5000
#define WIFISSD "Pixel_2073"
#define WIFIPWD "pass1122"


void setup_wifi_and_wait() {
    WiFiInterface *wifi = WiFiInterface::get_default_instance();
    if (!wifi) return;

    wifi->set_credentials(WIFISSD, WIFIPWD, NSAPI_SECURITY_WPA2);
    printf("Connecting to WiFi...\n");
    fflush(stdout);

    if (wifi->connect() != 0) return;
    printf("Connected to WiFi\n");
    fflush(stdout);

    char register_body[256];
    SocketAddress my_addr;
    nsapi_error_t result = wifi->get_ip_address(&my_addr);
    if (result != NSAPI_ERROR_OK) {
        printf("Failed to get IP address\n");
        return;
    }
    const char* ip = my_addr.get_ip_address();
    snprintf(register_body, sizeof(register_body),
            "{\"role\": \"commander\", \"ip\": \"%s\", \"port\": 0}", ip);

    char register_request[512];
    snprintf(register_request, sizeof(register_request),
            "POST /api/register HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s", SERVER_IP, strlen(register_body), register_body);


    TCPSocket register_socket;
    register_socket.open(wifi);
    SocketAddress server;
    wifi->gethostbyname(SERVER_IP, &server);
    server.set_port(SERVER_PORT);
    
    if (register_socket.connect(server) == 0) {
        register_socket.send(register_request, strlen(register_request));

        char buffer[256];
        int len;
        do {
            len = register_socket.recv(buffer, sizeof(buffer) - 1);
            if (len > 0) {
                buffer[len] = '\0';
                printf("%s", buffer);
            }
        } while (len > 0);

        register_socket.close();
    } else {
        printf("Failed to register with server\n");
        fflush(stdout);
        return;  // Skip further actions if registration failed
    }

    // ===== STEP 2: Continue with sending system state =====
    while (true) {
        if (state_changed) {
            state_changed = false;

            const char* state_str = lock_state ? "LOCK" : "UNLOCK";
            unsigned char b64_output[512];
            size_t b64_len;

            encrypt_data(key, (const unsigned char*)state_str, strlen(state_str), b64_output, sizeof(b64_output), &b64_len);

            
            char json_body[128];
            snprintf(json_body, sizeof(json_body),
                     "{\"device\": \"stm32l475\", \"lock\": \"%s\"}", b64_output);

            char http_request[512];
            snprintf(http_request, sizeof(http_request),
                     "POST /api/report HTTP/1.1\r\n"
                     "Host: %s\r\n"
                     "Content-Type: application/json\r\n"
                     "Content-Length: %d\r\n"
                     "Connection: close\r\n"
                     "\r\n"
                     "%s", SERVER_IP, strlen(json_body), json_body);

            TCPSocket socket;
            socket.open(wifi);
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
                printf("Socket connection failed\n");
                fflush(stdout);
            }
        }

        ThisThread::sleep_for(100ms);
    }
}