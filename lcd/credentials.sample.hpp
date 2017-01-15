#ifndef CREDENTIALS_HPP
#define CREDENTIALS_HPP

const char* WIFI_SSID = "my-wifi-ssid";
const char* WIFI_PASS = "my-wifi-pass";

#define IBMIOT_ORGID_ "x"
#define IBMIOT_DEVTYPE_ "LCD"
#define IBMIOT_DEVID_ "LCD"

const char* IBMIOT_HOST = IBMIOT_ORGID_ ".messaging.internetofthings.ibmcloud.com";
const char* IBMIOT_DEVID = "d:" IBMIOT_ORGID_ ":" IBMIOT_DEVTYPE_ ":" IBMIOT_DEVID_;
const uint16_t IBMIOT_PORT = 1883;
const char* IBMIOT_TOKEN = "x";

#endif // CREDENTIALS_HPP
