#ifndef INPUT_ENTRY_H
#define INPUT_ENTRY_H
#include <string>

//enum e_EntryType{INIT, INT, BOOL, DOUBLE};
enum e_EntryType{INIT, DOUBLE};

class InputEntry
{
public:
	InputEntry();
	void setIdentifier(std::string ident){if(!finalized)identifier = ident;}
	std::string getIdentifier(){return identifier;}
	void setInputWidth(int w){inputWidth = w;}
	int getInputWidth(){return inputWidth;}
	void setNumBuckets(int buckets){if(!finalized)numBuckets = buckets;}
	int getNumBuckets(){return numBuckets;}
	void setBucketWidth(int bw){if(!finalized)bucketWidth = bw;}
	int getBucketWidth(){return bucketWidth;}
	void setStartInputIndex(int index){if(!finalized)startInputIndex = index;}
	int getStartInputIndex(){return startInputIndex;}
	void setType(enum e_EntryType type){entryType = type;}
	enum e_EntryType getType(){return entryType;}
	void finalize(){if(entryType != INIT && startInputIndex >=0 && identifier != "")finalized = true;}
	bool isFinalized(){return finalized;}
	int getLastInputIndex(){return startInputIndex + (numBuckets * bucketWidth) - 1;}
	int getInputSize(){return getLastInputIndex() - startInputIndex + 1;}
	void setRange(double min, double max){range_min = min; range_max = max;}
	double getRangeMax(){return range_max;}	
	double getRangeMin(){return range_min;}	

	bool hasPrediction(){return has_prediction;}
	void setHasPrediction(bool has){has_prediction = has;}
	int getPredictionStep(){return prediction_value_step;}
	void setPredictionStep(int step){prediction_value_step = step;}

	void setRawActive(double value){raw_active_value = value;}
	double getRawActive(){return raw_active_value;}
	void setRawPrediction(double value){raw_prediction_value = value;}
	double getRawPrediction(){return raw_prediction_value;}


private:
	bool finalized = false;
	std::string identifier = "";
	int inputWidth = 0;//changable
	int numBuckets = 0;
	int bucketWidth = 0;//recommend an even number
	int startInputIndex = -1;
	enum e_EntryType entryType = INIT;//changeable
	//
	double range_min = 0;//changable
	double range_max = 1;//changable
	//values
	int prediction_value_step = -2;//only calc raw values once
	bool has_prediction = false;
	double raw_prediction_value = 0;//percentage
	double raw_active_value = 0;//percentage
};
#endif

