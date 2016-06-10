/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///

#include <libiec61850/hal_thread.h>
#include <signal.h>
#include <stdio.h>
#include <libiec61850/sv_subscriber.h>
#include <sstream>
#include <iostream>
#include <string>


static bool running = true;

void sigint_handler(int signalId)
{
	running = 0;
}


/* Callback handler for received SV messages */
static void svUpdateListener (SVSubscriber subscriber, void* parameter, SVClientASDU asdu)
{

	const char* svID = SVClientASDU_getSvId(asdu);
	char * format = (char*) parameter;
	int i, nbVals, dataSize;
	std::stringstream oss;
	dataSize = SVClientASDU_getDataSize(asdu);
	nbVals = strlen(format);
	printf("SV recv\n");
	printf("  recv %d bytes, unpacking with format %s\n", dataSize, format);

	if (svID != NULL)
		printf("  svID=(%s)\n", svID);

	printf("  smpCnt: %i\n", SVClientASDU_getSmpCnt(asdu));
	printf("  confRev: %u\n", SVClientASDU_getConfRev(asdu));
	int idx=0;
	for(i=0;i<nbVals;i++)
	{
		switch(format[i])
		{
			case 'f':
				oss << SVClientASDU_getFLOAT32(asdu, idx)<<" ";
				idx+=4;
				break;
			case 'd':
				oss << SVClientASDU_getFLOAT64(asdu, idx)<<" ";
				idx+=8;
				break;
		}
	}
	if(idx!=dataSize) std::cerr<<"Error unserializing data : size mismatch"<<std::endl;
	std::cout <<"  DATA: " << oss.str()<<std::endl;
}

int main(int argc, char** argv)
{
	SVReceiver receiver = SVReceiver_create();
	if(argc!=3)
	{
		printf("Usage : svDisp interface format\n");
		printf(" example: svDisp eth0 ff to unpack to float values\n");
		exit(-1);
	}

	SVReceiver_setInterfaceId(receiver, argv[1]);
	printf("Set interface id: %s\n", argv[1]);

	/* Create a subscriber listening to SV messages with APPID 4000h */
	SVSubscriber subscriber = SVSubscriber_create(NULL, 0x4000);

	/* Install a callback handler for the subscriber */
	SVSubscriber_setListener(subscriber, svUpdateListener, argv[2]);

	/* Connect the subscriber to the receiver */
	SVReceiver_addSubscriber(receiver, subscriber);

	/* Start listening to SV messages - starts a new receiver background thread */
	SVReceiver_start(receiver);

	signal(SIGINT, sigint_handler);

	while (running)
		Thread_sleep(1);

	/* Stop listening to SV messages */
	SVReceiver_stop(receiver);

	/* Cleanup and free resources */
	SVReceiver_destroy(receiver);
}
