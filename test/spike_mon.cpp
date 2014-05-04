#include <snn.h>

#include "carlsim_tests.h"

// TODO: I should probably use a google tests figure for this to reduce the
// amount of redundant code, but I don't need to do that right now. -- KDC

/// ****************************************************************************
/// Function to read and return a 1D array with time and nid (in that order.
/// ****************************************************************************
void readAndReturnSpikeFile(const std::string fileName, uint*& buffer, long &arraySize){
	FILE* pFile;
	long lSize;
	size_t result;
	pFile = fopen ( fileName.c_str() , "rb" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
		
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell(pFile);
	printf("lSize = %lu\n",lSize);
	printf("lSize = %ld\n",lSize);
	arraySize = lSize/sizeof(uint);
	printf("arraySize = %ld\n",arraySize);
	rewind (pFile);
	uint* AERArray;
	AERArray = new uint[lSize];
	memset(AERArray,0,sizeof(uint)*lSize);
	// allocate memory to contain the whole file:
	buffer = (uint*) malloc (sizeof(uint)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
		
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
		
	/* the whole file is now loaded in the memory buffer. */
	for(int i=0;i<lSize/sizeof(uint);i=i+2){
		AERArray[i]=buffer[i];
	}
	printf("AERARrray[0]=%u\n",AERArray[0]);
	printf("buffer[0]=%u\n",buffer[0]);
	printf("lSize = %ld\n",lSize);

	// terminate
	fclose (pFile);

	return;
};

/// ****************************************************************************
/// Function for reading and printing spike data written to a file
/// ****************************************************************************
void readAndPrintSpikeFile(const std::string fileName){
	FILE * pFile;
	long lSize;
	int* buffer;
	size_t result;
	pFile = fopen ( fileName.c_str() , "rb" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
			
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
		
	// allocate memory to contain the whole file:
	buffer = (int*) malloc (sizeof(int)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
		
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
		
	/* the whole file is now loaded in the memory buffer. */
	for(int i=0;i<lSize/sizeof(int);i=i+2){
		printf("time = %u \n",buffer[i]);
		printf("nid = %u \n",buffer[i+1]);
	}

	// terminate
	fclose (pFile);
	free (buffer);
};

/// ****************************************************************************
/// TESTS FOR SPIKE MONITOR 
/// ****************************************************************************

/*!
 * \brief testing to make sure grpId error is caught in setSpikeMonitor.
 *
 */
TEST(SPIKEMON, grpId){
	CARLsim* sim;
	const int GRP_SIZE = 10;
	
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);
		
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		int g2 = sim->createGroup("g2", GRP_SIZE, EXCITATORY_NEURON, ALL);
		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);
		sim->setNeuronParameters(g2, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);
		
		EXPECT_DEATH(sim->setSpikeMonitor(ALL),"");  // grpId = ALL (-1) and less than 0 
		EXPECT_DEATH(sim->setSpikeMonitor(-4),"");  // less than 0
		EXPECT_DEATH(sim->setSpikeMonitor(2),""); // greater than number of groups
		EXPECT_DEATH(sim->setSpikeMonitor(MAX_GRP_PER_SNN),""); // greater than number of group & and greater than max groups
		
		delete sim;
	}
}

/*!
 * \brief testing to make sure configId error is caught in setSpikeMonitor.
 *
 */
TEST(SPIKEMON, configId){
	CARLsim* sim;
	const int GRP_SIZE = 10;
	
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);
		
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		int g2 = sim->createGroup("g2", GRP_SIZE, EXCITATORY_NEURON, ALL);
		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);
		sim->setNeuronParameters(g2, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);
		
		EXPECT_DEATH(sim->setSpikeMonitor(1,"testSpikes.dat",ALL),"");  // configId = ALL (-1) and less than 0 
		EXPECT_DEATH(sim->setSpikeMonitor(1,"testSpikes.dat",-2),"");  // less than 0
		EXPECT_DEATH(sim->setSpikeMonitor(1,"testSpikes.dat",-100),"");  // less than 0

		delete sim;
	}
}


/*!
 * \brief testing to make sure file name error is caught in setSpikeMonitor.
 *
 */
TEST(SPIKEMON, fname){
	CARLsim* sim;
	const int GRP_SIZE = 10;
	
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);
		
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		int g2 = sim->createGroup("g2", GRP_SIZE, EXCITATORY_NEURON, ALL);
		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);
		sim->setNeuronParameters(g2, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);

		// this directory doesn't exist.
		EXPECT_DEATH(sim->setSpikeMonitor(1,"absentDirectory/testSpikes.dat",0),"");  
		
		delete sim;
	}
}

/// ****************************************************************************
/// TESTS FOR SPIKE-INFO CLASS
/// ****************************************************************************

/*!
 * \brief testing to make sure clear() function works.
 *
 */
TEST(SPIKEINFO, clear){
	CARLsim* sim;
	PoissonRate* input;
	const int GRP_SIZE = 10;
	const int inputTargetFR = 5.0f;
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);

		float COND_tAMPA=5.0, COND_tNMDA=150.0, COND_tGABAa=6.0, COND_tGABAb=150.0;
		int inputGroup = sim->createSpikeGeneratorGroup("Input",GRP_SIZE,EXCITATORY_NEURON);
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		int g2 = sim->createGroup("g2", GRP_SIZE, EXCITATORY_NEURON, ALL);
		
		sim->setConductances(true,COND_tAMPA,COND_tNMDA,COND_tGABAa,COND_tGABAb);
		double initWeight = 0.05f;
		double maxWeight = 4*initWeight;

		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);
		sim->setNeuronParameters(g2, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);

		// input
		input = new PoissonRate(GRP_SIZE);
		for(int i=0;i<GRP_SIZE;i++){
			input->rates[i]=inputTargetFR;
		}
		sim->connect(inputGroup,g1,"random", initWeight, maxWeight, 0.5f, 1, 1, SYN_FIXED);
		sim->connect(inputGroup,g2,"random", initWeight, maxWeight, 0.5f, 1, 1, SYN_FIXED);
		sim->connect(g1,g2,"random", initWeight, maxWeight, 0.5f, 1, 1, SYN_FIXED);

		SpikeInfo* spikeInfoG1 = sim->setSpikeMonitor(g1);
		
		sim->setSpikeRate(inputGroup,input);
		
		spikeInfoG1->startRecording();
		
		int runTime = 1;
		// run the network
		sim->runNetwork(runTime,0);
	
		spikeInfoG1->stopRecording();
		
		// we should have spikes!
		EXPECT_TRUE(spikeInfoG1->getSize() != 0);
		
		// now clear the spikes
		spikeInfoG1->clear();

		// we shouldn't have spikes!
		EXPECT_TRUE(spikeInfoG1->getSize() == 0);
		
		// start recording again
		spikeInfoG1->startRecording();
		
		// run the network again
		sim->runNetwork(runTime,0);
		
		// stop recording
		spikeInfoG1->stopRecording();
		
		// we should have spikes again
		EXPECT_TRUE(spikeInfoG1->getSize() != 0);

		
		delete sim;
		delete input;
	}
}
/*!
 * \brief testing to make sure getGrpFiringRate() function works.
 *
 */
TEST(SPIKEINFO, getGrpFiringRate){
	CARLsim* sim;
	PoissonRate* input;
	const int GRP_SIZE = 1;
	const int inputTargetFR = 5.0f;
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);

		float COND_tAMPA=5.0, COND_tNMDA=150.0, COND_tGABAa=6.0, COND_tGABAb=150.0;
		int inputGroup = sim->createSpikeGeneratorGroup("Input",GRP_SIZE,EXCITATORY_NEURON);
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		
		sim->setConductances(true,COND_tAMPA,COND_tNMDA,COND_tGABAa,COND_tGABAb);
		double initWeight = 0.27f;
		double maxWeight = 4*initWeight;

		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);

		// input
		input = new PoissonRate(GRP_SIZE);
		for(int i=0;i<GRP_SIZE;i++){
			input->rates[i]=inputTargetFR;
		}
		sim->connect(inputGroup,g1,"random", initWeight, maxWeight, 1.0f, 1, 1, SYN_FIXED);

		system("rm -rf spkInputGrp.dat");
		system("rm -rf spkG1Grp.dat");
		SpikeInfo* spikeInfoInput = sim->setSpikeMonitor(inputGroup,"spkInputGrp.dat");
		SpikeInfo* spikeInfoG1 = sim->setSpikeMonitor(g1,"spkG1Grp.dat");
		
		uint* inputArray;
		long inputSize;
		readAndReturnSpikeFile("spkInputGrp.dat",inputArray,inputSize);
		printf("inputSize = %lu\n",inputSize);
		uint* g1Array;
		long g1Size;
		readAndReturnSpikeFile("spkG1Grp.dat",g1Array,g1Size);
		printf("g1Size = %lu\n",g1Size);
		sim->setSpikeRate(inputGroup,input);
		
		spikeInfoInput->startRecording();
		spikeInfoG1->startRecording();
	 		
		int runTime = 10;
		// run the network
		sim->runNetwork(runTime,0);
	
		spikeInfoInput->stopRecording();
		spikeInfoG1->stopRecording();
		
		
		
		// calculate the average firing rate from the spike files
		float inputFR = (float)inputSize/(float)runTime;
		float g1FR = (float)g1Size/(float)runTime;
		printf("inputFR = %f\n",inputFR);
		printf("g1FR = %f\n", g1FR);
	
		// change this
		//EXPECT_FLOAT_EQ(spikeInfoInput->getGrpFiringRate(),0);

		// I can do a more exact calulculation of this by reading the spike files later.

		delete sim;
		delete input;
	}
}

/*!
 * \brief testing to make sure getMaxFiringRate() function works.
 *
 */
TEST(SPIKEINFO, getMaxNeuronFiringRate){
	CARLsim* sim;
	PoissonRate* input;
	const int GRP_SIZE = 1;
	const int inputTargetFR = 5.0f;
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);

		float COND_tAMPA=5.0, COND_tNMDA=150.0, COND_tGABAa=6.0, COND_tGABAb=150.0;
		int inputGroup = sim->createSpikeGeneratorGroup("Input",GRP_SIZE,EXCITATORY_NEURON);
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		
		sim->setConductances(true,COND_tAMPA,COND_tNMDA,COND_tGABAa,COND_tGABAb);
		double initWeight = 0.27f;
		double maxWeight = 4*initWeight;

		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);

		// input
		input = new PoissonRate(GRP_SIZE);
		for(int i=0;i<GRP_SIZE;i++){
			input->rates[i]=inputTargetFR;
		}
		sim->connect(inputGroup,g1,"random", initWeight, maxWeight, 1.0f, 1, 1, SYN_FIXED);

		SpikeInfo* spikeInfoInput = sim->setSpikeMonitor(inputGroup);
		SpikeInfo* spikeInfoG1 = sim->setSpikeMonitor(g1);
		
		sim->setSpikeRate(inputGroup,input);
		
		spikeInfoInput->startRecording();
		spikeInfoG1->startRecording();
	 		
		int runTime = 30;
		// run the network
		sim->runNetwork(runTime,0);
	
		spikeInfoInput->stopRecording();
		spikeInfoG1->stopRecording();
		
		// make sure it's plus or minus 10% of the target firing rate.		
		EXPECT_TRUE(spikeInfoInput->getMaxNeuronFiringRate() < 5.0f+0.1*5.0 &&
								spikeInfoInput->getMaxNeuronFiringRate() > 5.0f-0.1*5.0);

		// I can do a more exact calulculation of this by reading the spike files later.

		delete sim;
		delete input;
	}
}


/*!
 * \brief testing to make sure getMinFiringRate() function works.
 *
 */
TEST(SPIKEINFO, getMinNeuronFiringRate){
	CARLsim* sim;
	PoissonRate* input;
	const int GRP_SIZE = 1;
	const int inputTargetFR = 5.0f;
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);

		float COND_tAMPA=5.0, COND_tNMDA=150.0, COND_tGABAa=6.0, COND_tGABAb=150.0;
		int inputGroup = sim->createSpikeGeneratorGroup("Input",GRP_SIZE,EXCITATORY_NEURON);
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		
		sim->setConductances(true,COND_tAMPA,COND_tNMDA,COND_tGABAa,COND_tGABAb);
		double initWeight = 0.27f;
		double maxWeight = 4*initWeight;

		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);

		// input
		input = new PoissonRate(GRP_SIZE);
		for(int i=0;i<GRP_SIZE;i++){
			input->rates[i]=inputTargetFR;
		}
		sim->connect(inputGroup,g1,"random", initWeight, maxWeight, 1.0f, 1, 1, SYN_FIXED);

		SpikeInfo* spikeInfoInput = sim->setSpikeMonitor(inputGroup);
		SpikeInfo* spikeInfoG1 = sim->setSpikeMonitor(g1);
		
		sim->setSpikeRate(inputGroup,input);
		
		spikeInfoInput->startRecording();
		spikeInfoG1->startRecording();
	 		
		int runTime = 30;
		// run the network
		sim->runNetwork(runTime,0);
	
		spikeInfoInput->stopRecording();
		spikeInfoG1->stopRecording();
		
		// make sure it's plus or minus 10% of the target firing rate.		
		EXPECT_TRUE(spikeInfoInput->getMinNeuronFiringRate() < 5.0f+0.1*5.0 &&
								spikeInfoInput->getMinNeuronFiringRate() > 5.0f-0.1*5.0);

		// I can do a more exact calulculation of this by reading the spike files later.

		delete sim;
		delete input;
	}
}

/*!
 * \brief testing to make sure getNeuronFiringRate() function works.
 *
 */
TEST(SPIKEINFO, getNeuronFiringRate){
	CARLsim* sim;
	PoissonRate* input;
	const int GRP_SIZE = 5;
	const int inputTargetFR = 5.0f;
	vector<float> FR;
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);

		float COND_tAMPA=5.0, COND_tNMDA=150.0, COND_tGABAa=6.0, COND_tGABAb=150.0;
		int inputGroup = sim->createSpikeGeneratorGroup("Input",GRP_SIZE,EXCITATORY_NEURON);
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		
		sim->setConductances(true,COND_tAMPA,COND_tNMDA,COND_tGABAa,COND_tGABAb);
		double initWeight = 0.27f;
		double maxWeight = 4*initWeight;

		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);

		// input
		input = new PoissonRate(GRP_SIZE);
		for(int i=0;i<GRP_SIZE;i++){
			input->rates[i]=inputTargetFR;
		}
		
		sim->connect(inputGroup,g1,"random", initWeight, maxWeight, 1.0f, 1, 1, SYN_FIXED);

		SpikeInfo* spikeInfoInput = sim->setSpikeMonitor(inputGroup);
		SpikeInfo* spikeInfoG1 = sim->setSpikeMonitor(g1);
		
		sim->setSpikeRate(inputGroup,input);
		
		spikeInfoInput->startRecording();
		spikeInfoG1->startRecording();
	 		
		int runTime = 10;
		// run the network
		sim->runNetwork(runTime,0);
	
		spikeInfoInput->stopRecording();
		spikeInfoG1->stopRecording();
		
		// make sure it's plus or minus 10% of the target firing rate.		
		FR=spikeInfoInput->getNeuronFiringRate();
		
		for(int i=0;i<GRP_SIZE;i++){
			EXPECT_TRUE(FR.at(i) > inputTargetFR - 0.25*inputTargetFR &&
									FR.at(i) < inputTargetFR + 0.25*inputTargetFR);
		}
	
		
		delete sim;
		delete input;
	}
}

/*!
 * \brief testing to make sure getNumNeuronsWithFiringRate() function works.
 *
 */
TEST(SPIKEINFO, getNumNeuronsWithFiringRate){
	CARLsim* sim;
	PoissonRate* input;
	const int GRP_SIZE = 5;
	const int inputTargetFR = 5.0f;
	vector<float> FR;
	// use threadsafe version because we have deathtests
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	// loop over both CPU and GPU mode.
	for(int mode=0; mode<=1; mode++){
		// first iteration, test CPU mode, second test GPU mode
		sim = new CARLsim("SNN",mode?GPU_MODE:CPU_MODE,SILENT,0,1,42);

		float COND_tAMPA=5.0, COND_tNMDA=150.0, COND_tGABAa=6.0, COND_tGABAb=150.0;
		int inputGroup = sim->createSpikeGeneratorGroup("Input",GRP_SIZE,EXCITATORY_NEURON);
		int g1 = sim->createGroup("g1", GRP_SIZE, EXCITATORY_NEURON, ALL);
		
		sim->setConductances(true,COND_tAMPA,COND_tNMDA,COND_tGABAa,COND_tGABAb);
		double initWeight = 0.27f;
		double maxWeight = 4*initWeight;

		sim->setNeuronParameters(g1, 0.02f, 0.0f, 0.2f, 0.0f, -65.0f, 0.0f, 8.0f, 0.0f, ALL);

		// input
		input = new PoissonRate(GRP_SIZE);
		for(int i=0;i<GRP_SIZE;i++){
			input->rates[i]=inputTargetFR;
		}
		
		sim->connect(inputGroup,g1,"full", initWeight, maxWeight, 1.0f, 1, 1, SYN_FIXED);

		system("rm -rf spkInput.dat");
		SpikeInfo* spikeInfoInput = sim->setSpikeMonitor(inputGroup,"spkInput.dat");
		SpikeInfo* spikeInfoG1 = sim->setSpikeMonitor(g1);

		sim->setSpikeRate(inputGroup,input);
		
		spikeInfoInput->startRecording();
		spikeInfoG1->startRecording();
	 		
		int runTime = 1;
		// run the network
		sim->runNetwork(runTime,0);
	
		spikeInfoInput->stopRecording();
		spikeInfoG1->stopRecording();
	
		// read the entire spike file
		//readAndPrintSpikeFile("spkInput.dat");
		uint* spikeData;
		int arraySize = 0;
		//readAndReturnSpikeFile("spkInput.dat",spikeData, arraySize);
		
		printf("time 0 = %u\n",spikeData[0]);
		printf("nid 0 = %u\n",spikeData[1]);
		printf("arraySize = %d\n",arraySize);

		// make sure it's plus or minus 10% of the target firing rate.		
		FR=spikeInfoInput->getNeuronFiringRate();
		
		for(int i=0;i<GRP_SIZE;i++){
			EXPECT_TRUE(FR.at(i) > inputTargetFR - 0.25*inputTargetFR &&
									FR.at(i) < inputTargetFR + 0.25*inputTargetFR);
		}
	
		delete spikeData;
		delete sim;
		delete input;
	}
}

/*!
 * \brief testing to make sure getNumSilentNeurons() function works.
 *
 */

/*!
 * \brief testing to make sure getPercentNeuronsWithFiringRate() function works.
 *
 */

/*!
 * \brief testing to make sure getPercentSilentNeurons() function works.
 *
 */

/*!
 * \brief testing to make sure getGrpSize() function works.
 *
 */

/*!
 * \brief testing to make sure getSortedNeuronFiringRate() function works.
 *
 */

/*!
 * \brief testing to make sure isRecording() function works.
 *
 */


/*!
 * \brief testing to make sure print() function works.
 *
 */


/*!
 * \brief testing to make sure startRecording() function works.
 *
 */

/*!
 * \brief testing to make sure stopRecording() function works.
 *
 */


