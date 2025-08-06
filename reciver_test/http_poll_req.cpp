#include "mbed.h"
#include "WiFiInterface.h"
#include "TCPSocket.h"
#include "rtos/ThisThread.h"
#include <cstdio>
#include <cstring>

extern volatile bool lock_state;
extern volatile bool state_updated;

#define SERVER_IP "10.66.169.168" 
#define SERVER_PORT 5000
#define RECEIVER_PORT 8080  // Port for receiver to listen on
#define WIFISSD "Pixel_2073"
#define WIFIPWD "pass1122"

// Function to parse the server response and update lock state
void parse_state_response(const char* response) {
    const char* json_start = strstr(response, "\r\n\r\n");
    if (!json_start) {
        printf("Failed to find JSON body in response\n");
        fflush(stdout);
        return;
    }
    json_start += 4; // Skip teh "\r\n\r\n"
    
    // expected: {"lock": LOCK/UNLOCK}
    const char* lock_field = strstr(json_start, "\"lock\"");
    if (lock_field) {
        const char* value_start = strstr(lock_field, ":");
        if (value_start) {
            value_start++; // Skip the ':'
            
            // Skip whitespace
            while (*value_start == ' ' || *value_start == '\t') {
                value_start++;
            }
            
            // Skip opening quote if present
            if (*value_start == '"') {
                value_start++;
            }
            
            bool new_lock_state;
            if (strncmp(value_start, "LOCK", 4) == 0) {
                new_lock_state = true;
            } else if (strncmp(value_start, "UNLOCK", 6) == 0) {
                new_lock_state = false;
            } else {
                printf("Invalid lock_state value in response: '%.10s'\n", value_start);
                fflush(stdout);
                return;
            }
            
            // Update the global state if it changed
            if (new_lock_state != lock_state) {
                lock_state = new_lock_state;
                state_updated = true;
                printf("Lock state updated to: %s\n", lock_state ? "LOCKED" : "UNLOCKED");
                fflush(stdout);
            }
        }
    } else {
        printf("No lock_state field found in response\n");
        fflush(stdout);
    }
}

void setup_wifi_and_poll() {
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
            "{\"role\": \"receiver\", \"ip\": \"%s\", \"port\": %d}", ip, RECEIVER_PORT);

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
        printf("Successfully registered with server\n");
        fflush(stdout);
    } else {
        printf("Failed to register with server\n");
        fflush(stdout);
        return;  
    }

    printf("Starting lock state polling loop...\n");
    fflush(stdout);
    
    while (true) {
        rtos::ThisThread::sleep_for(2000ms);
        
        printf("Polling server for lock state...\n");
        fflush(stdout);
        
        char poll_request[256];
        snprintf(poll_request, sizeof(poll_request),
                "GET /api/lock_state HTTP/1.1\r\n"
                "Host: %s\r\n"
                "Connection: close\r\n"
                "\r\n", SERVER_IP);

        TCPSocket poll_socket;
        poll_socket.open(wifi);
        
        SocketAddress poll_server;
        wifi->gethostbyname(SERVER_IP, &poll_server);
        poll_server.set_port(SERVER_PORT);
        
        if (poll_socket.connect(poll_server) == 0) {
            poll_socket.send(poll_request, strlen(poll_request));
            
            char response_buffer[512];
            int total_received = 0;
            int len;
            
            do {
                len = poll_socket.recv(response_buffer + total_received, 
                                     sizeof(response_buffer) - total_received - 1);
                if (len > 0) {
                    total_received += len;
                }
            } while (len > 0 && total_received < sizeof(response_buffer) - 1);
            
            if (total_received > 0) {
                response_buffer[total_received] = '\0';
                printf("Server response received (%d bytes)\n", total_received);
                fflush(stdout);
                
                parse_state_response(response_buffer);
            }
        } else {
            printf("Failed to connect to server\n");
            fflush(stdout);
        }
        
        poll_socket.close();
    }
}