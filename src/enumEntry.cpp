#include <string>
#include <vector>
#include <cstdlib>

struct enumEntryType
{
	std::string typeName = "";
	int width = 0;
	int representSize = 0;
	int* sdr_mem = nullptr;
};

enumEntryType* createEnumEntryType(std::string typeName, int width, int representSize)
{
	//initial setup of the type
	enumEntryType* newType = (enumEntryType*) malloc(sizeof(enumEntryType));
	newType->typeName = typeName;
	newType->width = width;
	newType->representSize = representSize;

	//create random representation
	int* sdr = (int*) malloc(sizeof(int) * newType->representSize);
	srand(time(NULL));
	for(int i=0;i<newType->representSize;)
	{
		int index = rand() % newType->width;
		//check if it has already been used
		bool good = true;
		for(int j=0;j<i;++j)
		{
			if(index == sdr[i])
			{
				good = false;
				break;
			}	
		}
		if(good)
		{
			sdr[i] = index;
			++i;
		}
	}

	newType->sdr_mem = sdr;
}	

class EnumEntry
{
public:
	EnumEntry()
	{
		
	}

	void addEnumEntry(enumEntryType )
	{
		
	}

	~EnumEntry()
	{
	}
private:
	int entryWidth = 0;
	int representSize = 0;
	int startInputIndex = 0;

	int prediction_value_step = -2;

	int closest_prediction_index = 0;
	std::vector<enumEntryType> typesVec;

	int overlapWeight = 1.0;
};
