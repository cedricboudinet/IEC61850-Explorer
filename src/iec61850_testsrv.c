/*
 * test de serveur iec61850
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <libiec61850/iec61850_server.h>
#include <libiec61850/hal_thread.h>
#include <time.h>

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

static bool myClientAuthenticator(void* parameter, AcseAuthenticationParameter authParameter, void** securityToken, IsoApplicationReference* appRef)
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
	/*DataObject* lln0_mod = */CDC_ENS_create("Mod", (ModelNode*) lln0, 0);
	/*DataObject* lln0_health = */CDC_ENS_create("Health", (ModelNode*) lln0, 0);
	CDC_ASG_create("varASG", (ModelNode*) lln0, 0, false);
	DataObject * varSAV = CDC_SAV_create("varSAV", (ModelNode*) lln0, 0, false);
	varSAV_setMagF = (DataAttribute*) ModelNode_getChild((ModelNode*) varSAV, "instMag.f");
	varSAV_t = (DataAttribute*) ModelNode_getChild((ModelNode*) varSAV, "t");
	CDC_ENG_create("varENG", (ModelNode*) lln0, 0);
	CDC_ENS_create("varENS", (ModelNode*) lln0, 0);
	CDC_DPS_create("varDPS", (ModelNode*) lln0, 0);
	CDC_MV_create("varMV", (ModelNode*) lln0, CDC_OPTION_RANGE, false);
	CDC_SPC_create("varSPC", (ModelNode*) lln0, 0, 0);
	CDC_SPS_create("varSPS", (ModelNode*) lln0, 0);
	DataObject * varLPL = CDC_LPL_create("varLPL", (ModelNode*) lln0, 0);

	// Adding DO in rw mode
	DataObject* newDA = DataObject_create("editValues", (ModelNode*)lln0 , 0);
	DataAttribute * DAStr = DataAttribute_create("string", (ModelNode*) newDA, IEC61850_VISIBLE_STRING_255, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("boolean", (ModelNode*) newDA, IEC61850_BOOLEAN, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("int32", (ModelNode*) newDA, IEC61850_INT32, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("int16", (ModelNode*) newDA, IEC61850_INT16, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("uint32", (ModelNode*) newDA, IEC61850_INT32U, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("int32[10]", (ModelNode*) newDA, IEC61850_INT32, IEC61850_FC_SP, 0, 10, 0);
	
	iedServer = IedServer_create(myModel);
	if(useAuth)
		IedServer_setAuthenticator(iedServer, myClientAuthenticator, NULL);

	DataAttribute *tempDA = (DataAttribute*) ModelNode_getChild((ModelNode*) varLPL, "vendor");
	IedServer_updateVisibleStringAttributeValue(iedServer, tempDA, "IEC61850-Explorer");
	IedServer_updateVisibleStringAttributeValue(iedServer, DAStr, "Change me");
	tempDA = (DataAttribute*) ModelNode_getChild((ModelNode*) varLPL, "swRev");
	IedServer_updateVisibleStringAttributeValue(iedServer, tempDA, "1.0.0");

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
		Thread_sleep(1);
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

