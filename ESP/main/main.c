// Para configurar UART
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "soc/uart_channel.h"

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


#define UART_PORT_NUM      UART_NUM_2    // Usamos la UART2 para enviar/recibir hacia/desde el LPC
#define BUF_SIZE           1024
// #define BUF_SIZE           32
// #define BUF_SIZE           128
#define RXD_PIN            16            // GPIO para RX
#define TXD_PIN            17            // GPIO para TX 

// Defino nombre de la red Wifi y su psw para poder conectarnos y obtener la hora del servidor externo.
#define WIFI_SSID MY_SSID 
#define WIFI_PASS MY_PASS 

//Defino port y tamaño de buf de las tramas TCP
#define TCP_PORT 10234


/* --- Variables de la aplicacion ---*/ 
static esp_netif_t *sta_netif = NULL;
static const char *TAG = "UART_2";
// uint8_t data[BUF_SIZE];

typedef struct {
    // uint8_t m_ssid[50];
    // uint8_t m_psw[50];
    char * m_ssid;
    char * m_psw;
}m_typeConfigNetwork;


m_typeConfigNetwork m_ConfigNetwork = {
    .m_ssid = WIFI_SSID,
    .m_psw = WIFI_PASS
};
// strcpy( (char*) m_ConfigNetwork.m_ssid   ,WIFI_SSID);
// strcpy( (char*) m_ConfigNetwork.m_psw    ,WIFI_PASS);

int client_sock = -1; // Socket cliente actual ( fd: )

/* ----------------- Conexión WiFi ----------------- */
static void onGotIp(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    const esp_netif_ip_info_t* ip_info = &event->ip_info;
    // ESP_LOGI("NETWORK", "IP: " IPSTR, IP2STR(&ip_info->ip));
    // ESP_LOGI("NETWORK", "Gateway: " IPSTR, IP2STR(&ip_info->gw));
    // ESP_LOGI("NETWORK", "Netmask: " IPSTR, IP2STR(&ip_info->netmask));
}

static void onWifiDisconnect(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    // ESP_LOGW(TAG, "WiFi desconectado. Reintentando...");
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
    // ESP_LOGI(TAG, "WiFi inicializado exitosamente en SSID: %s", WIFI_SSID);
}

/* ------- Configuración UART <> ESP [UART 2] ---------- */
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
/* ------- Configuración QT <> ESP [UART 0 cable USB para reconfigurar wifi] ---------- */
void configUartESPtoQT(void){
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // Instalacion del driver para usar UART de ESP
    uart_driver_install(UART_NUM_0, BUF_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_NUM_0_TXD_DIRECT_GPIO_NUM, UART_NUM_0_RXD_DIRECT_GPIO_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

}

/* ----- Función para mostrar la hora sincronizada [TEST] ----- */
static void timeSyncCallback(struct timeval *tv) {
    // ESP_LOGI("NTP", "Hora sincronizada exitosamente desde el servidor NTP.");
    
    // Mostrar la hora local (con la zona horaria configurada)
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    ESP_LOGI("NTP", "TODAY-IS: %s", asctime(&timeinfo));
}

/* ----- Conexion con servidor NTP para sincronizar fecha y hora actual ----- */
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



/* ------ ESP como Servidor TCP para intercambio de información con app cliente QT ------- */

static void sendUartToClientData(void *arg) {       //funcion que manda desde la uart hacia el servidor
    char buf[BUF_SIZE];                          //define buffer...
    // char buf_sample[3];
    int i;
    uint8_t checksum_rcv = 0;
    while (1) {                                     //entre en el loop...
        if (client_sock < 0) {                      //si el cliente no está en estado readonly espera y vuelve a comenzar la secuencia...
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // espero N segs antes de mandar datos

        // int len = uart_read_bytes(UART_NUM_2, buf, BUF_SIZE, pdMS_TO_TICKS(3500)); // la funcBUF_SIZEión retorna el numero de bytes leídos

        int len = uart_read_bytes(UART_NUM_2, buf, BUF_SIZE, pdMS_TO_TICKS(1000)); // Retorna la cantidad de bytes que leidos del buffer RX
        // necesito que tenga minimanente 6?
        ESP_LOGI("[TCP Server]", "LPC dice -> %s", buf);        //buffer no se limpia, si no llegó info nueva por uart_read_bytes va a loggear lo ultimo que haya almacenado

        if( len >= 6 && (buf[0] == '+' || buf[0] == '_' )) {
            // ESP_LOGI("[TCP Server]", "[Adentro del if] -> %s", buf);
            
            for(i=0; i <= 4; i++ ){
                checksum_rcv += buf[i];
            } 

            if( checksum_rcv == buf[5] ){
                strcpy((char*)buf+5, ",85,");     //Harcodeo la distolica, habria que tener otro sensor para esto
                time_t now;
                struct tm timeinfo;
                char time_buffer[80];

                time(&now);
                localtime_r(&now, &timeinfo);
                strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

                strcpy((char*)buf+9,time_buffer);   //Inyecto fecha y hora a la medición
                strcpy((char*)buf+28,",");          //Fin de trama

                // int sent = send(client_sock, buf, len, 0);
                int sent = send(client_sock, buf, 29, 0); // 29 es cantidad de bytes que se mandan+1
                ESP_LOGI("[TCP Server]", "MANDANDO A QT POR WIFI-> %s", buf);
                checksum_rcv = 0;
                
                if (sent < 0) {
                    ESP_LOGE("[TCP Server]", "Error enviando datos al cliente TCP, cerrando socket");
                    shutdown(client_sock, 0);
                    close(client_sock);
                    client_sock = -1;
                }
            }                
        }
        uart_flush(UART_PORT_NUM);
    }

}

static void sendConfirmClient (void *arg){
    /*  Logica para trama  byte de confirmacion de recepcion de configuracion  */
    int sent = send(client_sock, "$", 1, 0);
    if (sent < 0) {
        ESP_LOGE("[TCP Server]", "Error enviando datos al cliente TCP, cerrando socket");
        shutdown(client_sock, 0);
        close(client_sock);
        client_sock = -1;
    }
    vTaskDelete(NULL);
}

static void sendConfirmNetwork (void *arg){
    /*  Logica para trama  byte de confirmacion de recepcion de configuracion  */
    uart_write_bytes(UART_NUM_0, "?#$&", 5); 
    // vTaskDelete(NULL);
}

static void sendClientToUartData(void *arg) {
    /*
        Recepción de trama de configuración para pasar al LPC. Cualquier otro formato de trama debe ser descartado.
        -Formato de trama: >XXX,YYY,ZZ,HH,CCCCCC
    */

    uint8_t buf[BUF_SIZE];
    while (1) {
        if (client_sock < 0) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        int len = recv(client_sock, buf, BUF_SIZE, 0);
        if (len > 0) {
            /* Si hay datos, evaluar que la trama sea la de configuración de parametros y mandar por UART al LPC*/
            uart_write_bytes(UART_NUM_2, (const char *)buf, len); 
            
            //Avisar al cliente TCP que sus datos de configuración se recepcionaron
            xTaskCreate(sendConfirmClient, "sendConfirmClient", 4096, NULL, 10, NULL);

        } else if (len == 0) {
            // ESP_LOGI(TAG, "Cliente TCP desconectado");
            shutdown(client_sock, 0);
            close(client_sock);
            client_sock = -1;
        } else {
            // ESP_LOGE(TAG, "Error en recv(), cerrando socket");
            shutdown(client_sock, 0);
            close(client_sock);
            client_sock = -1;
        }
    }
}


static void rcvNetworkInfo(void *arg){
    uint8_t buf[100];
    int len = uart_read_bytes(UART_NUM_0, buf, 100, pdMS_TO_TICKS(1000));
    while(1){
        if(len > 0){
            // ESP_LOGI("UART_0", "Recibi datos p/ network: %d", buf);
            xTaskCreate(sendConfirmNetwork, "sendConfirmNetwork", 4096, NULL, 10, NULL);
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
        // ESP_LOGE(TAG, "Error en listen");
        close(listen_sock);
        return;
    }

    // ESP_LOGI(TAG, "Servidor TCP escuchando...");

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

        //Cuando el cliente se conecta, le doy lo que tenga en el buffer de la UART
        xTaskCreate(sendUartToClientData, "sendUartToClientData", 4096, NULL, 10, NULL); 
        
        //El cliente TCP (app QT) puede mandar datos para configurar el LPC, leo lo que me mande por TCP y lo escribo en la UART:
        // xTaskCreate(sendClientToUartData, "sendClientToUartData", 4096, NULL, 10, NULL);
        
        //Si el USB está conectado es posible recibir cambios de SSID y PSW para conectarse a otra red WIFI
        // xTaskCreate(rcvNetworkInfo, "rcvNetworkInfo", 4096, NULL, 10, NULL); //Revisar esto que tiraba error cuand esta en ejecuicion


        while (client_sock >= 0) {
            // vTaskDelay(pdMS_TO_TICKS(500)); //manda datos cada 5ms ?
            vTaskDelay(pdMS_TO_TICKS(5000)); //subo para que tarde un poquito mas
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
    connectEspToWifi();
    

    /* Obtener fecha y hora de servidor externo */
    SetSystemTimeSNTP();

    /* Configurar y habilitar UART */
    configUart();

    // configUartESPtoQT();

    /*  Iniciar servidor y escuchar cliente TCP.
        Dentro de esta función se resuleve el envio de datos, enviando lo que le llega a la UART del ESP hacia el QT via TCP y viceversa */
    initServer();

    while (1) {
        //Test de horario sincronizado.
        vTaskDelay(pdMS_TO_TICKS(5000));
        // ESP_LOGI("ESP", "wait...");        
    }
}