/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <libiec61850/iec61850_server.h>
#include <libiec61850/iec61850_client.h>
#include <libiec61850/hal_thread.h>
#include <time.h>
#include <assert.h>
#include "iec61850Exp_fun.h"
#include "MmsValueWrapper.h"

static IedServer iedServer = NULL;

int launchIedTestServer(int port_61850)
{
	time_t currTime;
	currTime=time(NULL);
	printf("Starting IED server %s", ctime(&currTime));
	IedModel* myModel = IedModel_create("test");
	LogicalDevice* lDevice1 = LogicalDevice_create("Device", myModel);
	LogicalNode* lln0 = LogicalNode_create("LLN0", lDevice1);
	CDC_ASG_create("varASG", (ModelNode*) lln0, 0, false);
	CDC_SAV_create("varSAV", (ModelNode*) lln0, 0, false);
	CDC_ENG_create("varENG", (ModelNode*) lln0, 0);
	CDC_MV_create("varMV", (ModelNode*) lln0, CDC_OPTION_RANGE, false);
	CDC_LPL_create("varLPL", (ModelNode*) lln0, 0);

	// Adding DO in rw mode
	DataObject* newDA = DataObject_create("editValues", (ModelNode*)lln0 , 0);
	DataAttribute_create("string", (ModelNode*) newDA, IEC61850_VISIBLE_STRING_255, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("boolean", (ModelNode*) newDA, IEC61850_BOOLEAN, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("int32", (ModelNode*) newDA, IEC61850_INT32, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("int16", (ModelNode*) newDA, IEC61850_INT16, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("uint32", (ModelNode*) newDA, IEC61850_INT32U, IEC61850_FC_SP, 0, 0, 0);
	DataAttribute_create("int32[10]", (ModelNode*) newDA, IEC61850_INT32, IEC61850_FC_SP, 0, 10, 0);
	
	iedServer = IedServer_create(myModel);

	IedServer_start(iedServer, port_61850);
	if (!IedServer_isRunning(iedServer)) {
		fprintf(stderr, "Starting IED server failed! Exit.\n");
		IedServer_destroy(iedServer);
		exit(-1);
	}
	printf("Iec61850 test server is ready\n");
	return 1;
}

int main(void)
{
	launchIedTestServer(8102);
	IedClientError error;
	IedConnection con = IedConnection_create();
	IedConnection_connect(con, &error, "localhost", 8102);
	
	assert (error == IED_ERROR_OK);
	std::vector<MmsValueWrapper> varList = getVariableList(con);
	for(std::vector<MmsValueWrapper>::iterator it = varList.begin();it!=varList.end();it++)
	{
		MmsType valType= it->getType();
		it->update(con);
		printf("%s : type is %s\n", it->getName().c_str(), MmsValue_getTypeString(it->getMmsValue()));
		assert((valType!=MMS_STRUCTURE));
	}
	

	IedConnection_close(con);
	IedConnection_destroy(con);
	
	printf("Stoping iec server\n");
	IedServer_stop(iedServer);
	IedServer_destroy(iedServer);

	return (0);




}
