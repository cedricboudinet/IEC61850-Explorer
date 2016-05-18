/*
 * test de serveur iec61850
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <libiec61850/iec61850_server.h>
#include <libiec61850/hal_thread.h>
#include <time.h>
#include <pthread.h>

static int running = 0;
static IedServer iedServer = NULL;
DataAttribute *varSAV_setMagF, *varSAV_t ;
char* password;

void print_help()
{
	printf("Usage :\n");
	printf("  --help    show this help message and exit\n");
	printf("  -P ARG    set port number\n");
	printf("  -W ARG    set password\n");
}

void parseCmdLine(int argc, char **argv, int * iecPort, int * useAuth)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-P")==0)
		{
			*iecPort = atoi(argv[i+1]);
			if((*iecPort<=0)||(*iecPort>=0xFFFF))
			{
				fprintf(stderr, "Please specify a valid port number\n");
				exit(-1);
			}
			i++;
		}
		else if(strcmp(argv[i], "-W")==0)
		{
			password=(char*)malloc(strlen(argv[i+1])*sizeof(char));
			strcpy(password, argv[i+1]);
			*useAuth=1;
			i++;
		}
		else if(strcmp(argv[i], "--help")==0)
		{
			print_help(); exit(1);
		}
	}
}

static bool myClientAuthenticator(void* parameter, AcseAuthenticationParameter authParameter, void** securityToken)
{
	//printf("Authenticating password is %s\n", password);
	if (authParameter->mechanism == ACSE_AUTH_PASSWORD) {
		if (authParameter->value.password.passwordLength == (int) strlen(password)) {
			if (memcmp(authParameter->value.password.octetString, password,
						authParameter->value.password.passwordLength) == 0)
			{
				*securityToken = (void*) password;
				return true;
			}
		}
	}
	return false;
}

void sigint_handler(int signalId)
{
	running = 0;
}

int launchIedServer(int port_61850, bool useAuth)
{
	time_t currTime;
	currTime=time(NULL);
	printf("Starting IED server %s", ctime(&currTime));
	IedModel* myModel = IedModel_create("test");
	LogicalDevice* lDevice1 = LogicalDevice_create("Device", myModel);
	LogicalNode* lln0 = LogicalNode_create("LLN0", lDevice1);
	CDC_ASG_create("varASG", (ModelNode*) lln0, 0, false);
	DataObject * varSAV = CDC_SAV_create("varSAV", (ModelNode*) lln0, 0, false);
	varSAV_setMagF = (DataAttribute*) ModelNode_getChild((ModelNode*) varSAV, "instMag.f");
	varSAV_t = (DataAttribute*) ModelNode_getChild((ModelNode*) varSAV, "t");
	CDC_ENG_create("varENG", (ModelNode*) lln0, 0);
	CDC_MV_create("varMV", (ModelNode*) lln0, 0, false);
	iedServer = IedServer_create(myModel);
	if(useAuth)
		IedServer_setAuthenticator(iedServer, myClientAuthenticator, NULL);

	IedServer_start(iedServer, port_61850);
	if (!IedServer_isRunning(iedServer)) {
		fprintf(stderr, "Starting IED server failed! Exit.\n");
		IedServer_destroy(iedServer);
		exit(-1);
	}
	printf("Iec61850 test server is ready\n");
	return 1;
}

int main(int argc, char **argv)
{
	int port_61850 = 102;
	int useAuth=0;
	parseCmdLine(argc, argv, &port_61850, &useAuth);
	running = 1;
	signal(SIGINT, sigint_handler);
	launchIedServer(port_61850, useAuth);
	float myfloat=0;
	while (running) 
	{
		sleep(1);
		uint64_t timeval = Hal_getTimeInMs();
		IedServer_lockDataModel(iedServer);
		IedServer_updateUTCTimeAttributeValue(iedServer, varSAV_t, timeval);
		IedServer_updateFloatAttributeValue(iedServer, varSAV_setMagF, myfloat);
		IedServer_unlockDataModel(iedServer);
		myfloat+=1e-3;
	}

	printf("Stoping iec server\n");
	IedServer_stop(iedServer);
	IedServer_destroy(iedServer);

	return (0);
}

