#ifndef INPUT_ENTRY_H
#define INPUT_ENTRY_H
#include <string>

enum e_EntryType{INIT, INT, BOOL, DOUBLE};

class InputEntry
{
public:
	//InputEntry();
	void setIdentifier(std::string ident){identifier = ident;}
	std::string getIdentifier(){return identifier;}
	void setInputWidth(int w){inputWidth = w;}
	int getInputWidth(){return inputWidth;}
	void setNumBuckets(int buckets){numBuckets = buckets;}
	int getNumBuckets(){return numBuckets;}
	void setBucketWidth(int bw){bucketWidth = bw;}
	int getBucketWidth(){return bucketWidth;}
	void setStartInputIndex(int index){startInputIndex = index;}
	int getStartInputIndex(){return startInputIndex;}
	//void finalize();
	int getLastInputIndex(){return startInputIndex + (numBuckets * bucketWidth);}
	int getInputSize(){return getLastInputIndex() - startInputIndex + 1;}
	void setRange(double min, double max){range_min = min; range_max = max;}
	
	

private:
	//bool finalized = false;
	std::string identifier = "";
	int inputWidth = 0;
	int numBuckets = 0;
	int bucketWidth = 0;//recommend an even number
	int startInputIndex = 0;
	enum e_EntryType entryType = INIT;
	//
	double range_min = 0;
	double range_max = 1;
	//values
	double raw_prediction_value = 0;
	double raw_active_value = 0;
	
	

	
	

};
#endif

