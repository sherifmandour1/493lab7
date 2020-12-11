#include "mqtt_client.h"
#define NETID_INDEX 0
#define ACTION_INDEX 1
#define MESSAGE_INDEX 2
//this function is used with the user doesn't use the correct format
void printUsage() {
    printf("Usage: mqtt_client [--help] [-v] [-h HOST] [-p PORT] NETID ACTION MESSAGE\n\n");
    printf("Arguments:\n");
    printf("  NETID The NetID of the user.\n");
    printf("  ACTION Must be uppercase, lowercase, title-case,\n");
    printf("  reverse, or shuffle.\n");
    printf("  MESSAGE Message to send to the server\n\n");
    printf("Options:\n");
    printf("  --help");
    printf("  -v, --verbose\n");
    printf("  --host HOSTNAME, -h HOSTNAME\n");
    printf("  --port PORT, -p PORT\n");
}


// Parses the options given to the program. It will return a Config struct with
// the necessary information filled in. argc and argv are provided by main. If
// an error occurs in processing the arguments and options (such as an invalid
// option), this function will print the correct message and then exit.
Config mqtt_client_parse_arguments(int argc, char *argv[]) {
        log_set_quiet(true);
    log_trace("Parsing arguments..");
    Config myConfig;
    myConfig.port = "invalid";
    myConfig.relative_path = "invalid";
    bool stillParsing = true;
    int selectedOption = 0;
    int optionIndex = 0;
    bool portReceived = false;
    bool hostReceived = false;
    struct option long_opts[] = {
        {"help", no_argument, ZERO_VALUE, ZERO_VALUE},
        {"verbose", no_argument, ZERO_VALUE, 'v'},
        {"host", required_argument, ZERO_VALUE, 'h'},
        {"port", required_argument, ZERO_VALUE, 'p'},
        {ZERO_VALUE, ZERO_VALUE, ZERO_VALUE, ZERO_VALUE}};
  while ((selectedOption =
              getopt_long(argc, argv, "vh:p:", long_opts, &optionIndex)) !=
         -1)  {
            switch (selectedOption) {
                break;
            case 'v':
                log_set_quiet(false);
                log_trace("Verbose flag activated\n");
                stillParsing = false;
                break;
            case 'p':
                log_trace("Port option chosen and now parsing parameter passed in");
                stillParsing = false;
                myConfig.port = optarg;
                portReceived = true;
                break;
            case 'h':
                log_info("Received host info and it is: %s\n", optarg);
                myConfig.host = optarg;
                hostReceived = true;
                break;
            case '?':
            printf("Unkown option provided\n");
            printUsage();
                myConfig.port = "invalid";
                myConfig.host = "invalid";
                myConfig.action = "invalid";
                myConfig.message = "invalid";
                myConfig.netid = "invalid";
                return myConfig;
                break;
            default:
                printf("Option not passed in or extra unknown option provided\n");
                printUsage();
                myConfig.port = "invalid";
                myConfig.host = "invalid";
                myConfig.action = "invalid";
                myConfig.message = "invalid";
                myConfig.netid = "invalid";
                return myConfig;
                return myConfig;
            }
    }


    // subtracting the options from the arguments to be left with th arguments
    // passed in
    argc -= optind;
    argv += optind;
    myConfig.netid = argv[NETID_INDEX];
    myConfig.action = argv[ACTION_INDEX];
    myConfig.message = argv[MESSAGE_INDEX];

    if(argc < 2)
    {
                printf("\"netid\", \"aciton\", \"action\"arguments are required\n");
                printUsage();
                myConfig.port = "invalid";
                myConfig.host = "invalid";
                myConfig.action = "invalid";
                myConfig.message = "invalid";
                myConfig.netid = "invalid";
                return myConfig;
    } else if(argc > 3)
    {
                        printf("extra argument provided\n");
                printUsage();
                myConfig.port = "invalid";
                myConfig.host = "invalid";
                myConfig.action = "invalid";
                myConfig.message = "invalid";
                myConfig.netid = "invalid";
                return myConfig;
    }



    // Checking if port received or I need to assign the default ports
    if (!portReceived) {
        myConfig.port = DEFAULT_PORT;
    }
    //checking if the path was provided or assign the default path 
    if (!hostReceived) {
        myConfig.host = DEFAULT_HOST;
    }


    log_trace("Sucessfulyy parsed command line arguments and now returning to main\n");
    return myConfig;
}