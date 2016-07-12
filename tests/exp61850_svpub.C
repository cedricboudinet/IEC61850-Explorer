/*
 * IEC 61850 SV publisher example
 *
 */

#include <signal.h>
#include <stdio.h>
#include <libiec61850/hal_thread.h>
#include <libiec61850/sv_publisher.h>
#define NB_FL 8
static bool running = true;

void sigint_handler(int signalId)
{
	running = 0;
}

int main(int argc, char** argv)
{
	SampledValuesPublisher svPublisher = SampledValuesPublisher_create("eth0");
	char svName[] = "mysv";
	int flrefs[NB_FL];
	SV_ASDU asdu = SampledValuesPublisher_addASDU(svPublisher, svName, NULL, 1);
	for(int i =0; i<NB_FL;i++)
		flrefs[i] = SV_ASDU_addFLOAT(asdu);

	SampledValuesPublisher_setupComplete(svPublisher);

	float fls[NB_FL];
	for(int i =0; i<NB_FL;i++)
		fls[i]=i;

	while (running)
	{
		for(int i =0; i<NB_FL;i++)
		{
			SV_ASDU_setFLOAT(asdu, flrefs[i], fls[i]);
			fls[i]+=.1;
		}
		SV_ASDU_increaseSmpCnt(asdu);

		SampledValuesPublisher_publish(svPublisher);

		Thread_sleep(50);
	}

	SampledValuesPublisher_destroy(svPublisher);
}
