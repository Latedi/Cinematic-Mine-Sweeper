#include <windows.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

typedef unsigned char BYTE;
typedef int ( __stdcall *pressField )(unsigned int, unsigned int);

const int BORDER = 0x10;
const int MINE = 0x8A;
const int EMPTY = 0x0F;

struct pos
{
	int x;
	int y;
};
 
INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
	//Run when attached to process
	switch(Reason)
	{
	case DLL_PROCESS_ATTACH:
		//These values were found with Ida Pro
		int *fieldWith = (int *) 0x010056F8;
		int *fieldHeight = (int *) 0x01005A68;
		BYTE *mineField = (BYTE *) 0x1005700;
		pressField func = (pressField) 0x01003144;

		//Vector of empty fields which we can click later
		std::vector<pos> emptyFields;

		//The program runs when this button is pressed
		MessageBox(NULL, "Leerooooy", "Let's do this", MB_OK);
		//Loop through all the possible fields on screen
		for(unsigned int i = 1; i <= *fieldHeight; i++)
		{
			for(unsigned int j = 1; j <= *fieldWith; j++)
			{
				//Check if the value of that field is "empty" and if it is, add to the vector
				//Other possible values are mines, borders, flags and numbers I believe.
				BYTE val = (BYTE) *(mineField + (i << 5) + j);
				if(val == EMPTY)
				{
					pos p = {j, i};
					emptyFields.push_back(p);
				}
			}
		}

		//We will pick the numbers randomly, it looks cooler than doing it linearly
		srand(time(NULL));
		while(emptyFields.size() != 0)
		{
			int i = rand() % emptyFields.size();
			pos p = emptyFields[i];
			emptyFields.erase(emptyFields.begin() + i);
			//Press the field
			func(p.x, p.y);
			//Waiting so it doesn't happen instantly (30ms)
			Sleep(20);
		}
		break;
	}

	return TRUE;
}