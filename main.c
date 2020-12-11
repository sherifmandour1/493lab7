#include "mqtt_client.h"

#define UNUSED(x) (void)(x)

#define QOS 1
#define TIMEOUT 10000L
#define DEFAULT_PORT "1883"
#define DEFAULT_HOST "localhost"

#include "log.h"
#include <MQTTClient.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define UNUSED(x) (void)(x)

static volatile bool waitForResp = true;

volatile MQTTClient_deliveryToken deliveryToken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    UNUSED(context);

    deliveryToken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    UNUSED(context);
    UNUSED(topicLen);

    int i;
    char *payloadptr;

    payloadptr = message->payload;
    for (i = 0; i < message->payloadlen; i++) {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    waitForResp = false;

    return EXIT_FAILURE;
}
void connlost(void *context, char *cause) {
    UNUSED(context);
    UNUSED(cause);
    log_error("Connection lost because: %s", cause);

    log_trace("No connection now exiting");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

    // parse the command line arguemnts to fill the information struct provided in the header file
    Config parsed = mqtt_client_parse_arguments(argc, argv);

    if (parsed.host == NULL || parsed.port == NULL || parsed.netid == NULL ||
        parsed.action == NULL || parsed.message == NULL) {
        return EXIT_FAILURE;
    } else if (strcmp(parsed.host, "invalid") == STRINGS_MATCH &&
               strcmp(parsed.port, "invalid") == STRINGS_MATCH &&
               strcmp(parsed.netid, "invalid") == STRINGS_MATCH &&
               strcmp(parsed.action, "invalid") == STRINGS_MATCH &&
               strcmp(parsed.message, "invalid") == STRINGS_MATCH) {
        return EXIT_SUCCESS;
    }

    // getting the length of the MQTT broker address
    size_t addressLen = snprintf(NULL, ZERO_RESET_INIT_VALUE, "tcp://%s:%s", parsed.host, parsed.port);
    //allocate the address
    char *address = malloc(sizeof(char) * (addressLen + ONE_VALUE));
    //populate the memory allocated
    snprintf(address, addressLen + ONE_VALUE, "tcp://%s:%s", parsed.host, parsed.port);

    //creating the topic for subscription
    size_t subTopicLen = snprintf(NULL, ZERO_RESET_INIT_VALUE, "%s/response", parsed.netid);
    //allocate memory to save the topic in
    char *subTopic = malloc(sizeof(char) * (subTopicLen + ONE_VALUE));
    //populate topic menu
    snprintf(subTopic, subTopicLen + ONE_VALUE, "%s/response", parsed.netid);


    // creating MQTT client
    MQTTClient client;
    // options init
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    // create the connection return code variabls
    int rc;
    errno = ZERO_RESET_INIT_VALUE;



    log_trace("About to create client..");
    if((rc = MQTTClient_create(&client, address, parsed.netid, MQTTCLIENT_PERSISTENCE_NONE,
                                   NULL) != MQTTCLIENT_SUCCESS)) {
        log_info("Error connecting to client, error code: %d", rc);
        log_trace("Cannot continue without a client, now exiting");
        exit(EXIT_FAILURE);
    }

    log_info("Client created successfully");

    // edit the default connection options
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = ONE_VALUE;


    log_trace("Setting callback functions");

    // setting up callbacks for updating
    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)) !=
        MQTTCLIENT_SUCCESS) {
        log_error("Resetting callbacks failed. Error: %s...", strerror(errno));
        fprintf(stderr, "Callback error: %s...\n\n", strerror(errno));
        log_trace("Can't reset callbacks, now exiting");
        exit(EXIT_FAILURE);
    }
    log_trace("Callback functions set correctly");


    log_trace("Connecting to client");

    log_trace("About to connect to the broker");
    // connect to the broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        log_error("Could not connect to broker");

    }
    log_trace("Connected to the broker successfully");


    log_trace("Client Subscribing");

    log_trace("Subscribing to the topic..");
    if ((rc = MQTTClient_subscribe(client, subTopic, QOS)) != MQTTCLIENT_SUCCESS) {
        log_error("An error occured while subscribing to topic", strerror(errno));
        fprintf(stderr, "Subscription error: %s...", strerror(errno));

        exit(EXIT_FAILURE);
    }
    log_info("Subscribtion to broker has been comopleted successfully");


    log_trace("Client Publishing");



    // creating two objects one for publication and one for subscription
    // this is the topic for the publishing
    size_t pubTopicLen = snprintf(NULL, ZERO_RESET_INIT_VALUE, "%s/%s", parsed.netid, parsed.action);
    char *pubTopic = malloc(sizeof(char) * (pubTopicLen + ONE_VALUE));
    snprintf(pubTopic, pubTopicLen + 1, "%s/%s", parsed.netid, parsed.action);

    // creating publication object 
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    // changing the publication object
    pubmsg.payload = parsed.message;
    pubmsg.payloadlen = strlen(parsed.message);
    pubmsg.qos = QOS;
    pubmsg.retained = ZERO_RESET_INIT_VALUE;

    // this is the token that indicates the deliver
    MQTTClient_deliveryToken myToken;
    // reset the token
    deliveryToken = ZERO_RESET_INIT_VALUE;

    log_trace("About to publish MQTT topic..");
    if ((rc = MQTTClient_publishMessage(client, pubTopic, &pubmsg, &myToken)) != MQTTCLIENT_SUCCESS) {
        log_error("Publication failed");
        fprintf(stderr, "Publication error: %s...", strerror(errno));

        exit(EXIT_FAILURE);
    }

    log_info("%s Topic has been published successfully from NETID %s\n", pubTopic, parsed.netid);


    //delivring the payload
    log_trace("About to deliver payload...");
    while (deliveryToken != myToken) {
    }
    log_trace("Payload has been succesfully delivered");


    //wait until the server response is received
    log_trace("Currently waiting for server response...");
    while (waitForResp) {
    }
    log_info("Succesfully received server response(Read line above)");

    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);


    log_trace("MQTT protocal done");
    return EXIT_SUCCESS;
}