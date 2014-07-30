#ifndef INPUT_CONNECTION_H
#define INPUT_CONNECTION_H
class InputBit;
class Column;
class Input_connection
{
public:
	Column* column = nullptr;
	InputBit* inputBit = nullptr;
	double strength = 0;

private:

};
#endif
