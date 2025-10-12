// Para configurar UART
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"

// Para configurar la conexión Wifi
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include <nvs.h>

// Para poder obtener fecha y hora actual
#include "time.h"
#include "esp_sntp.h"
#include "esp_http_client.h"

// Para poder levantar servidor TCP
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


// Archivito con los datos para WIFI_SSID y WIFI_PASS
#include "netdata.h"


#define UART_PORT_NUM      UART_NUM_2    // Usamos la UART2
#define BUF_SIZE           1024
#define RXD_PIN            16            // GPIO para RX
#define TXD_PIN            17            // GPIO para TX 

// Defino nombre de la red Wifi y su psw para poder conectarnos y obtener la hora del servidor externo.
#define WIFI_SSID MY_SSID 
#define WIFI_PASS MY_PASS 

//Defino port y tamaño de buf de las tramas TCP
#define TCP_PORT 10234
// #define BUF_SIZE 256 //1 byte = 8 bits, 


/* --- Variables de la aplicacion ---*/ 
static esp_netif_t *sta_netif = NULL;
static const char *TAG = "UART_2";
uint8_t data[BUF_SIZE];



int client_sock = -1; // Socket cliente actual ( fd: )

/* ----------------- Conexión WiFi ----------------- */
static void onGotIp(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    const esp_netif_ip_info_t* ip_info = &event->ip_info;
    ESP_LOGI("NETWORK", "IP: " IPSTR, IP2STR(&ip_info->ip));
    ESP_LOGI("NETWORK", "Gateway: " IPSTR, IP2STR(&ip_info->gw));
    ESP_LOGI("NETWORK", "Netmask: " IPSTR, IP2STR(&ip_info->netmask));
}

static void onWifiDisconnect(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    ESP_LOGW(TAG, "WiFi desconectado. Reintentando...");
    esp_wifi_connect();
}

void connectEspToWifi(void)
{
    esp_netif_init();
    esp_event_loop_create_default();
    sta_netif = esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &onGotIp, NULL);
    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &onWifiDisconnect, NULL);

    // esp_wifi_set_mode(WIFI_MODE_STA);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            // .threshold.authmode = WIFI_AUTH_WPA2_PSK
        }
    };

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);

    
    esp_wifi_start();
    esp_wifi_connect();
    // ESP_LOGI(TAG, "WiFi inicializado con SSID: %s", WIFI_SSID);
}

/* ------- Configuración UART ESP* ---------- */
void configUart(void){
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // Instalacion del driver para usar UART de ESP
    uart_driver_install(UART_PORT_NUM, BUF_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_PORT_NUM, &uart_config);
    uart_set_pin(UART_PORT_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

}

/* ----- Función para mostrar la hora sincronizada [TEST] ----- */
static void timeSyncCallback(struct timeval *tv) {
    ESP_LOGI("NTP", "Hora sincronizada exitosamente desde el servidor NTP.");
    
    // Mostrar la hora local (con la zona horaria configurada)
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    ESP_LOGI("NTP", "TODAY-IS: %s", asctime(&timeinfo));
}

/* ----- Conexion con servidor NTP para sincronizar fecha y hora actual [ NO ANDA =( ]----- */
void configTimeNTP(void){
   
    // Configuramos el modo de sincronización y el servidor
    sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");

    // Registramos el callback que se llama automáticamente al sincronizar
    sntp_set_time_sync_notification_cb(timeSyncCallback);

    // Configuramos la zona horaria (Argentina UTC-3)
    setenv("TZ", "ART3", 1);
    tzset();

    // Iniciamos SNTP
    sntp_init();

    ESP_LOGI(TAG, "Iniciando sincronización con servidor NTP...");
}

static void obtainTime(void){
    configTimeNTP();
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while (esp_sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI("TIME-SYNC", "Aguardando res NTP... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}

void SetSystemTimeSNTP()  {

	time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    if (timeinfo.tm_year < (2025 - 1900)) {
        ESP_LOGI("TIME_SYNC", "Fecha sin sincronizar");
        obtainTime();
        time(&now);
    }
}


/* ------ ESP como cliente HTTP para obtner fecha y hora por API pública [solucion parchesisima] -----*/
// esp_err_t handlerGetTimeSync(esp_http_client_event_handle_t evt)
// {
//     switch (evt->event_id)
//     {
//     case HTTP_EVENT_ON_DATA:
//         printf("HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
//         break;

//     default:
//         break;
//     }
//     return ESP_OK;
// }
// static void getTimeSync(void)
// {
//     esp_http_client_config_t config = {
//         .url = "http://worldclockapi.com/api/json/utc/now",
//         .method = HTTP_METHOD_GET,
//         .cert_pem = NULL,
//         .event_handler = handlerGetTimeSync
//     };
//     ESP_LOGI("TIME-SYNC-LOW-COST", "Sync hora de API pública...");

//     esp_http_client_handle_t client = esp_http_client_init(&config);
//     esp_err_t err = esp_http_client_perform(client);
    

//     if (err == ESP_OK) {
//         ESP_LOGI("TIME-SYNC-LOW-COST", "HTTPS Status = %d, content_length = %"PRId64,
//                 esp_http_client_get_status_code(client),
//                 esp_http_client_get_content_length(client));
//     } else {
//         ESP_LOGE("TIME-SYNC-LOW-COST", "Error perform http request %s", esp_err_to_name(err));
//     }

//     esp_http_client_cleanup(client);
// }




/* ------ ESP como Servidor TCP para intercambio de información con app cliente QT ------- */

static void sendUartToClientData(void *arg) {           //funcion que manda desde la uart hacia el servidor
    uint8_t buf[BUF_SIZE];                          //define buffer
    while (1) {                                     //entro en el loop
        if (client_sock < 0) {                      // si el cliente no está en estado readonly
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }
        int len = uart_read_bytes(UART_NUM_2, buf, BUF_SIZE, pdMS_TO_TICKS(100));
        if (len > 0) {
            int sent = send(client_sock, buf, len, 0);
            if (sent < 0) {
                ESP_LOGE(TAG, "Error enviando datos al cliente TCP, cerrando socket");
                shutdown(client_sock, 0);
                close(client_sock);
                client_sock = -1;
            }
        }
    }
}

static void sendClientToUartData(void *arg) {
    uint8_t buf[BUF_SIZE];
    while (1) {
        if (client_sock < 0) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }
        int len = recv(client_sock, buf, BUF_SIZE, 0);
        if (len > 0) {
            uart_write_bytes(UART_NUM_2, (const char *)buf, len);
        } else if (len == 0) {
            ESP_LOGI(TAG, "Cliente TCP desconectado");
            shutdown(client_sock, 0);
            close(client_sock);
            client_sock = -1;
        } else {
            ESP_LOGE(TAG, "Error en recv(), cerrando socket");
            shutdown(client_sock, 0);
            close(client_sock);
            client_sock = -1;
        }
    }
}

void initServer(void) {
    ESP_LOGI(TAG, "Iniciando servidor TCP en puerto %d", TCP_PORT);

    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);         //Abro un socket de tipo stream de datos
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Error creando socket");
        return;
    }

    struct sockaddr_in server_addr = {                                  //Genero el struct de configuración
        .sin_family = AF_INET,                                          //Conexión IPv4
        .sin_addr.s_addr = INADDR_ANY,                                  //Escucha requerimientos de cualquiera de las interfaces de red
        .sin_port = htons(TCP_PORT),                                    //Puerto en el que abrimos el servidor    
    };

    if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) { // Bind enlaza el socket abierto con la info de red de server_addr
        ESP_LOGE(TAG, "Error en bind");                                                
        close(listen_sock);
        return;
    }

    if (listen(listen_sock, 1) != 0) {          // Listen pone al socket a escuchar las peticiones de los clientes
        ESP_LOGE(TAG, "Error en listen");
        close(listen_sock);
        return;
    }

    ESP_LOGI(TAG, "Servidor TCP escuchando...");

    while (1) {
        struct sockaddr_in client_addr;             //Estructura para la información del cliente
        socklen_t addr_len = sizeof(client_addr);   
        client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &addr_len);  //Servidor aceptando solicitudes del cliente 
        if (client_sock < 0) {
            ESP_LOGE(TAG, "Error en accept");
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        ESP_LOGI(TAG, "Cliente TCP conectado");

        //Cuando el cliente se conecta, le doy lo que tenga en el buffer de la UART y escribo en la UART lo que me mande el cliente
        xTaskCreate(sendUartToClientData, "sendUartToClientData", 4096, NULL, 10, NULL); 
        xTaskCreate(sendClientToUartData, "sendClientToUartData", 4096, NULL, 10, NULL);

        while (client_sock >= 0) {
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        ESP_LOGI(TAG, "Cliente desconectado, esperando nuevo cliente");
    }
}




/* ----------------- MAIN APP ----------------- */
void app_main(void){

    /* --- Chequeo de errores --- */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    /* Cnofigurar y conectar WIFI*/
    ESP_LOGI("WIFI","Conectando WiFi en SSID %s ...", WIFI_SSID);
    connectEspToWifi();
    

    /* Obtener fecha y hora de servidor externo */
    ESP_LOGI("TIME-SYNC", "Chequeando fecha y hora del sistema...");
    SetSystemTimeSNTP();
    // getTimeSync();

    /* Configurar y habilitar UART */
    // ESP_LOGI(TAG, "Init UART");
    // configUart();

    /* Iniciar servidor y escuchar cliente TCP. Dentro de esta función se resuleve el envio de datos desde la UART hacia el cliente y viceversa */
    // initServer();

    while (1) {
        //Test de horario sincronizado.
        vTaskDelay(pdMS_TO_TICKS(20000));
        time_t now;
        struct tm timeinfo;
        char time_buffer[80];

        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

        ESP_LOGI("TIME", "Hora actual: %s", asctime(&timeinfo));
        ESP_LOGI("TIME", "Hora actual: %s", time_buffer);
    


        /* 1.Reviso si hay datos para levantar por la UART */
        // int len = uart_read_bytes(UART_PORT_NUM, data, BUF_SIZE - 1, pdMS_TO_TICKS(5000));
        
        // if (len > 0) {
        //     data[len] = '\0'; // agrega terminador de string
        //     ESP_LOGI(TAG, "Recibieno de LPC: %s", (char *)data);
        // }

        /* 2. Envío los datos de la UART al cliente*/
        
    }
}