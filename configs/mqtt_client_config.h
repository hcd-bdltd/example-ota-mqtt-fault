/******************************************************************************
* File Name:   mqtt_client_config.h
*
* Description: This file contains all the configuration macros used by the
*              MQTT client in this example.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2020-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#ifndef MQTT_CLIENT_CONFIG_H_
#define MQTT_CLIENT_CONFIG_H_

#include "cy_mqtt_api.h"

/*******************************************************************************
* Macros
********************************************************************************/

/***************** MQTT CLIENT CONNECTION CONFIGURATION MACROS *****************/
/* MQTT Broker/Server address and port used for the MQTT connection. */
#define MQTT_BROKER_ADDRESS               "192.168.1.56"
#define MQTT_PORT                         ( 8884 )

/* Set this macro to 1 if a secure (TLS) connection to the MQTT Broker is
 * required to be established, else 0.
 */
#define MQTT_SECURE_CONNECTION            ( 1 )

/* Configure the user credentials to be sent as part of MQTT CONNECT packet */
#define MQTT_USERNAME                     ""
#define MQTT_PASSWORD                     ""


/********************* MQTT MESSAGE CONFIGURATION MACROS **********************/
/* The MQTT topics to be used by the publisher and subscriber. */
#define MQTT_PUB_TOPIC                    "ledstatus"
#define MQTT_SUB_TOPIC                    "ledstatus"

/* Set the QoS that is associated with the MQTT publish, and subscribe messages.
 * Valid choices are 0, 1, and 2. Other values should not be used in this macro.
 */
#define MQTT_MESSAGES_QOS                 ( 1 )

/* Configuration for the 'Last Will and Testament (LWT)'. It is an MQTT message
 * that will be published by the MQTT broker if the MQTT connection is
 * unexpectedly closed. This configuration is sent to the MQTT broker during
 * MQTT connect operation and the MQTT broker will publish the Will message on
 * the Will topic when it recognizes an unexpected disconnection from the client.
 *
 * If you want to use the last will message, set this macro to 1 and configure
 * the topic and will message, else 0.
 */
#define ENABLE_LWT_MESSAGE                ( 0 )
#if ENABLE_LWT_MESSAGE
    #define MQTT_WILL_TOPIC_NAME          MQTT_PUB_TOPIC "/will"
    #define MQTT_WILL_MESSAGE             ("MQTT client unexpectedly disconnected!")
#endif

/* MQTT messages which are published on the MQTT_PUB_TOPIC that controls the
 * device (user LED in this example) state in this code example.
 */
#define MQTT_DEVICE_ON_MESSAGE            "TURN ON"
#define MQTT_DEVICE_OFF_MESSAGE           "TURN OFF"


/******************* OTHER MQTT CLIENT CONFIGURATION MACROS *******************/
/* A unique client identifier to be used for every MQTT connection. */
#define MQTT_CLIENT_IDENTIFIER            "psoc6-mqtt-client"

/* The timeout in milliseconds for MQTT operations in this example. */
#define MQTT_TIMEOUT_MS                   ( 5000 )

/* The keep-alive interval in seconds used for MQTT ping request. */
#define MQTT_KEEP_ALIVE_SECONDS           ( 60 )

/* Every active MQTT connection must have a unique client identifier. If you
 * are using the above 'MQTT_CLIENT_IDENTIFIER' as client ID for multiple MQTT
 * connections simultaneously, set this macro to 1. The device will then
 * generate a unique client identifier by appending a timestamp to the
 * 'MQTT_CLIENT_IDENTIFIER' string. Example: 'psoc6-mqtt-client5927'
 */
#define GENERATE_UNIQUE_CLIENT_ID         ( 1 )

/* The longest client identifier that an MQTT server must accept (as defined
 * by the MQTT 3.1.1 spec) is 23 characters. However some MQTT brokers support
 * longer client IDs. Configure this macro as per the MQTT broker specification.
 */
#define MQTT_CLIENT_IDENTIFIER_MAX_LEN    ( 23 )

/* As per Internet Assigned Numbers Authority (IANA) the port numbers assigned
 * for MQTT protocol are 1883 for non-secure connections and 8883 for secure
 * connections. In some cases there is a need to use other ports for MQTT like
 * port 443 (which is reserved for HTTPS). Application Layer Protocol
 * Negotiation (ALPN) is an extension to TLS that allows many protocols to be
 * used over a secure connection. The ALPN ProtocolNameList specifies the
 * protocols that the client would like to use to communicate over TLS.
 *
 * This macro specifies the ALPN Protocol Name to be used that is supported
 * by the MQTT broker in use.
 * Note: For AWS IoT, currently "x-amzn-mqtt-ca" is the only supported ALPN
 *       ProtocolName and it is only supported on port 443.
 *
 * Uncomment the below line and specify the ALPN Protocol Name to use this
 * feature.
 */
// #define MQTT_ALPN_PROTOCOL_NAME           "x-amzn-mqtt-ca"

/* Server Name Indication (SNI) is extension to the Transport Layer Security
 * (TLS) protocol. As required by some MQTT Brokers, SNI typically includes the
 * hostname in the Client Hello message sent during TLS handshake.
 *
 * Uncomment the below line and specify the SNI Host Name to use this extension
 * as specified by the MQTT Broker.
 */
// #define MQTT_SNI_HOSTNAME                 "SNI_HOST_NAME"

/* A Network buffer is allocated for sending and receiving MQTT packets over
 * the network. Specify the size of this buffer using this macro.
 *
 * Note: The minimum buffer size is defined by 'CY_MQTT_MIN_NETWORK_BUFFER_SIZE'
 * macro in the MQTT library. Please ensure this macro value is larger than
 * 'CY_MQTT_MIN_NETWORK_BUFFER_SIZE'.
 */
#define MQTT_NETWORK_BUFFER_SIZE          ( 2 * CY_MQTT_MIN_NETWORK_BUFFER_SIZE )

/* Maximum MQTT connection re-connection limit. */
#define MAX_MQTT_CONN_RETRIES            (150u)

/* MQTT re-connection time interval in milliseconds. */
#define MQTT_CONN_RETRY_INTERVAL_MS      (2000)


/**************** MQTT CLIENT CERTIFICATE CONFIGURATION MACROS ****************/

/**********************************************
 * Certificates and Keys - TLS Mode only
 *********************************************/
/* Root CA Certificate -
   Must include the PEM header and footer:

        "-----BEGIN CERTIFICATE-----\n" \
        ".........base64 data.......\n" \
        "-----END CERTIFICATE-------\n"
*/
#define ROOT_CA_CERTIFICATE "-----BEGIN CERTIFICATE-----\n"\
"MIIDrzCCApegAwIBAgIUcVg9stKZvLrC+BLIpXccoC6doxwwDQYJKoZIhvcNAQEL\n"\
"BQAwZzELMAkGA1UEBhMCSU4xEjAQBgNVBAgMCUthcm5hdGFrYTESMBAGA1UEBwwJ\n"\
"QmVuZ2FsdXJ1MQswCQYDVQQKDAJDWTEUMBIGA1UECwwLRW5naW5lZXJpbmcxDTAL\n"\
"BgNVBAMMBG15Q0EwHhcNMjQwOTMwMDMyMjQyWhcNMzQwOTI4MDMyMjQyWjBnMQsw\n"\
"CQYDVQQGEwJJTjESMBAGA1UECAwJS2FybmF0YWthMRIwEAYDVQQHDAlCZW5nYWx1\n"\
"cnUxCzAJBgNVBAoMAkNZMRQwEgYDVQQLDAtFbmdpbmVlcmluZzENMAsGA1UEAwwE\n"\
"bXlDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANBwG+CDMA7VapPv\n"\
"7miLbJ2E2GcCn402e3kPRc82MiFQtd6jtCWDYDx2rPgK6gJ0FVcLQ+PwFoIJUGUA\n"\
"8PTvvVxj4oCcbX17ukXbCvwSwa04zY52pb3uvqBzIoGVt7R/0Vq0kr7pz0DdDDDt\n"\
"9bhMNKpi6hiqzTa7Vp0fXB6yUYpivNcjyEPi8mrkxc/GLEX1ZnSGTw+/CiQBIVQD\n"\
"3mMX6Ze4o8Otxa0jFdpyruU41tWDB9sMhHkhi0xS+D5Cpb5Dok13TeOEXLYjRxwt\n"\
"JYvRtrUTtEbYpsc07SAHcXa2hPycIhovbNRzLvi6mLruVjrAObNaj6/HknF0y+DF\n"\
"QemxEQsCAwEAAaNTMFEwHQYDVR0OBBYEFDjxkivosG+i7uQgtivmkbxwjhoNMB8G\n"\
"A1UdIwQYMBaAFDjxkivosG+i7uQgtivmkbxwjhoNMA8GA1UdEwEB/wQFMAMBAf8w\n"\
"DQYJKoZIhvcNAQELBQADggEBAF2DuIKn+sE7gdUkf/P+Yf4OImOFpkcGHMzjqxK8\n"\
"t0qIabwwSs2JZk6PyVlew1iazFACH/YP1Sz+xgMregFignCbl7driGUXV9QQb3Lq\n"\
"IkvrGU9A+GeI+NrJlUOES1jQVP6H5DQyIjwN7jv8lDfYQvgpqFvsJB2cr2XEaUFv\n"\
"KjM0vZhU/youOAMc8v+/X5m0rSTaZZFskCsXvNHmpvrkc2CtJfGkjJdVTxRcIoku\n"\
"B1rcx71Fdg8GwhTNCDkyZtNsL7iBuP2Cp9B5q8qgQkvSZjh8q3yqofWzgcfGMExv\n"\
"mF2Jol14k4VoOceNxwayU/wY0ccu4s4nskbgYu31aTaDgos=\n"\
"-----END CERTIFICATE-----\n"

/* Client Certificate
   Must include the PEM header and footer:

        "-----BEGIN CERTIFICATE-----\n" \
        ".........base64 data.......\n" \
        "-----END CERTIFICATE-------\n"
*/
#define CLIENT_CERTIFICATE "-----BEGIN CERTIFICATE-----\n"\
"MIIDqjCCApKgAwIBAgIUC975JNGflZQvirR/AcWYp4lGm2EwDQYJKoZIhvcNAQEL\n"\
"BQAwZzELMAkGA1UEBhMCSU4xEjAQBgNVBAgMCUthcm5hdGFrYTESMBAGA1UEBwwJ\n"\
"QmVuZ2FsdXJ1MQswCQYDVQQKDAJDWTEUMBIGA1UECwwLRW5naW5lZXJpbmcxDTAL\n"\
"BgNVBAMMBG15Q0EwHhcNMjQwOTMwMDMyMjQyWhcNMzQwOTI4MDMyMjQyWjBzMQsw\n"\
"CQYDVQQGEwJJTjESMBAGA1UECAwJS2FybmF0YWthMRIwEAYDVQQHDAlCZW5nYWx1\n"\
"cnUxCzAJBgNVBAoMAkNZMRQwEgYDVQQLDAtFbmdpbmVlcmluZzEZMBcGA1UEAwwQ\n"\
"bW9zcXVpdHRvX2NsaWVudDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n"\
"ANdS0bcvxfczbaD5S9zefAAMtD/OaWaEqHPkxegvA4eYblvFHlmV+mrjRoS1Dl4J\n"\
"fsmf5h+JkRucg1dmdJonfXuSE73p5eVLKd9yB0XF5LGM+5Z4asud1x4q79id6LyQ\n"\
"TtMb7LjYI21OJ7mtwxsbdeOOJJ6enE3vr85Wm6rn+qgWd/M7N7wX/RJYEnpY8xnQ\n"\
"2B3ZJKnBlPHrjrQ8mDLdVzlIf+bi1XjVuux8DPh1pc3dRCpwDY1n8WpdRCABvAip\n"\
"VzuGNxzTxXwArDyiWvq2KZ7h1NylunRQVJ3WtQ7bP5cBUYaQv92prFdSGPAtobBW\n"\
"dwqezTxm/6106NWEH1UI3HMCAwEAAaNCMEAwHQYDVR0OBBYEFMbQQmDVGTHGm5ys\n"\
"iLqUa3qWt0YjMB8GA1UdIwQYMBaAFDjxkivosG+i7uQgtivmkbxwjhoNMA0GCSqG\n"\
"SIb3DQEBCwUAA4IBAQCkQYDTlg4dx6ooMAnUjYrLlJfYsM4YX8aUtc9KHZcR/h1G\n"\
"5KXdaQvQmfDA/5cvQ4smIvsivfz6FPfnagZoMNvV/M2Aj/FIRpFxoBm0jY19Qd8W\n"\
"nn/nQiOyfeZ0bLeSDDIAN8S4R8U4OgAHrzbGUUBohVLWyagGTp4ZGpFJT1J0TBex\n"\
"1z37fhCqsHS3qjLD7c7NAD443xfrkO2ne4VinXAjkkWcTa/uMiq1TxI9LwGZlgyC\n"\
"xs3niyGl2aXGNqHwsQzg0CwFcB2APLBi3pVOz6mABexTnZscAx8SpAn7gTTCibPm\n"\
"5iRXgklRUjb+RAK2+/Cnwi5siLge1ezPx+/xBfYw\n"\
"-----END CERTIFICATE-----\n"

/* Private Key
   Must include the PEM header and footer:

        "-----BEGIN RSA PRIVATE KEY-----\n" \
        "...........base64 data.........\n" \
        "-----END RSA PRIVATE KEY-------\n"
*/
#define CLIENT_PRIVATE_KEY "-----BEGIN PRIVATE KEY-----\n"\
"MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDXUtG3L8X3M22g\n"\
"+Uvc3nwADLQ/zmlmhKhz5MXoLwOHmG5bxR5Zlfpq40aEtQ5eCX7Jn+YfiZEbnINX\n"\
"ZnSaJ317khO96eXlSynfcgdFxeSxjPuWeGrLndceKu/Ynei8kE7TG+y42CNtTie5\n"\
"rcMbG3XjjiSenpxN76/OVpuq5/qoFnfzOze8F/0SWBJ6WPMZ0Ngd2SSpwZTx6460\n"\
"PJgy3Vc5SH/m4tV41brsfAz4daXN3UQqcA2NZ/FqXUQgAbwIqVc7hjcc08V8AKw8\n"\
"olr6time4dTcpbp0UFSd1rUO2z+XAVGGkL/dqaxXUhjwLaGwVncKns08Zv+tdOjV\n"\
"hB9VCNxzAgMBAAECggEAQO9k902c8fO8d/M2gaFG2+z43tUszBHtiyXO2Tp0YgwC\n"\
"HALSKEomft2Tgqnnb0yFw+BI0fH2pNEKeu0W9PY7zWw8L+W9wuxr8Eq/DrejRDg0\n"\
"FyixbD5phVuFgVn9uf/7BYdCWGw2ItyS9lgEosQhRUK0oHFMyeQirKNJxry2xsGR\n"\
"wyygiEzHIPndKF9tBQC/wBaJ3nfoQfhUi8044b3BjS2PkKbK8IOA+2bh4cGs4BMN\n"\
"ubuEhoy8RWfIgLxXhtYZPio5v9+9G1DdiIFKKi2Yw+rApIOT4sr8KbWZNBjy1w0k\n"\
"m/+gGKyAa7tO7kH2gpaS/15086cIIIHmDB4SlUTIYQKBgQD/rsGEkdKhB+snsxVO\n"\
"4KB2UqCOaeQAComsJwEHp8BQEb6xup4R9qJU0m20lgMppuIMh4htjTYtvnBuLrGF\n"\
"4hk+vpDbQ7gS3S5XZjnJQYNtMX9c5jee0PHvL9ByNWzdBlkL8Qs6v+LWhkN+eGdA\n"\
"Cd/NrsTnEegFMxrRYMtnbFKLpwKBgQDXlz0wH0UbndDiqrVmt73D9y3jb3UK9SgV\n"\
"K3ETsQFRafjT1/Di6k9bi+MrdLDHq6vRqVAz8n/2hcqQTSn4qUMNFhFjzMH/4iae\n"\
"FMC3tUN8HBlNvu/xLIcvbu8q1rWJKh9xPwubSac3UpbH8B8FULOffU1vLgT3KwCh\n"\
"0WkZ5JNSVQKBgDCdXMRFKcReihLRVTLbOQdmbIrHjYeIeWbXP7Xh6ooO5bBLaHTH\n"\
"7VOtcxiMEWvhBEgXmRQjbnLCLAR3Ml/ffqdUepyGQsCrd308va4Lm96uM6ZJS7gR\n"\
"yH6PG9YrXFimNVY07cqs44egncK1SASAHCKyoVPCLtiWP/ohqmsuyH8vAoGAYgYL\n"\
"9iNhoqrg3Q7NKAUHLWDn5m85AmBxdx6nbVKfzMqoLnoStrnelztFcO6/qimuF8gq\n"\
"BVSPRiGS9oV/PeaH7B1Kjtv8oUHMWTgKX/z00CteffEA2+FpHfIMoKe7JsCWR0Fp\n"\
"H5QfiZHtqL7O+/WzliK84H5GF7Pft7OlW/aH3XkCgYB+rV1nHtoIFFdqwhEwbzew\n"\
"2K/NBzqGsoX4b3yO8dfDoyTRFi33CVD/zP+E8HaOWcQcmn4SibRVPDR04KnXwWTv\n"\
"7KbGsotfP+8GN0J85Lh8IWNO6Wh3fgdklzKFrgEYTSJb7F8/Pswr4xK7jEtjuizP\n"\
"iEGaynUn21L8KpftAuqKfw==\n"\
"-----END PRIVATE KEY-----\n"

/******************************************************************************
* Global Variables
*******************************************************************************/
extern cy_mqtt_broker_info_t broker_info;
extern cy_awsport_ssl_credentials_t  *security_info;
extern cy_mqtt_connect_info_t connection_info;


#endif /* MQTT_CLIENT_CONFIG_H_ */
