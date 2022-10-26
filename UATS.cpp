#include <assert.h>
#include <chrono>
#include <cwchar>
#include <iostream>
#include <iomanip>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <Windows.h>

const int typesOfItems{ 5 };//number of types of items in each NFT (eg if 5 then 5 unique items: boots/chest/pants/gloves/hats) (above 150 results in INF)
const int amountOfItems{ 10 };//number of items for each type of item (eg if 10 then 10 items: boots/hats/pants etc.) (above 20 results in overflow)
const int amountOfNFTs{ 90000 };//amount of NFTs to be created (set this to a high value,check for amount of creatable NFTs, then set that value here)
static std::vector<std::vector <double>> vectorOfItems(typesOfItems, std::vector<double>(amountOfItems));
static std::vector <double> NFT(typesOfItems);
static std::vector<std::vector <double>> vectorOfNFTs(amountOfNFTs, std::vector<double>(typesOfItems));
static std::vector <double> NFTsums(amountOfNFTs);
static std::vector <double> itemSums(typesOfItems);//stores the sums of all items of each type to be used in determining item rarity
static int amountOfNFTsCreated{ 0 };
static int amountOfNFTsChosen{ 0 };
bool FAOIdebugging{};
bool BNFTdebugging{};
bool FAONFTdebugging{};
bool UGCdebugging{};
bool oBNFTdebugging{};
bool oFAONFTdebugging{};
bool limitCheck{};

std::mt19937 gen((unsigned)time(0));

int random(int low, int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

bool input()
{
	char input{};
	bool cont{ 1 };
	while (cont)
	{
		std::cin >> input;
		if (input == 'Y' || input == 'y')
		{
			cont = false;
			return true;
		}
		else if (input == 'N' || input == 'n')
		{
			cont = false;
			return false;
		}
		std::cout << "Please enter Y for yes or N for no." << '\n';
	}
	return 0;
}

void fillVectorOfItems()
{
	static double valueKey{ 1 };
	for (int outerIt{ 0 }; outerIt < typesOfItems; outerIt++)
	{
		double value{ valueKey };//set value to key initially
		for (int innerIt{ 0 }; innerIt < amountOfItems; innerIt++)
		{
			vectorOfItems[outerIt][innerIt] = value;//set element to Vector
			if (FAOIdebugging)
				std::cout << "ITEMVECTOR assigning " << value << " to Vector " << outerIt << " at index " << innerIt << '\n';
			value = value * 10;//set previous value to new value
		}
		valueKey = valueKey * 2;// double key value for next Vector
	}
}

void fillItemSums()
{
	for (int outerIt{ 0 }; outerIt < typesOfItems; outerIt++)
	{
		for (int innerIt{ 0 }; innerIt < amountOfItems; innerIt++)
		{
			itemSums[outerIt] += vectorOfItems[outerIt][innerIt];//sum values of item array and assign to itemSums
		}
	}
}

void buildNFT()
{
	for (int it{ 0 }; it < typesOfItems; it++)
	{
		int randomItem{ random(0, amountOfItems - 1) };//-1 so the RNG generates from 0 to the size of the Vector - 1, since the Vector starts at 0 instead of 1
		NFT[it] = vectorOfItems[it][randomItem];
		if (BNFTdebugging)
			std::cout << "buildNFT() assigning " << vectorOfItems[it][randomItem] << " to index " << it << " RNG= " << randomItem << '\n';
	}
}

double calculateNFT_Sum()
{
	double NFT_VectorTotal{};
	for (int it{ 0 }; it < typesOfItems; it++)//iterate through NFT, sum the indexes
	{
		NFT_VectorTotal += NFT[it];
	}
	return NFT_VectorTotal;
}

bool compareNFT_Sums()
{
	if (amountOfNFTsCreated == 0)
		return true;
	for (int it{ 0 }; it < amountOfNFTsCreated; it++)
	{
		if (calculateNFT_Sum() == NFTsums[it])
		{
			return 0;
		}
	}
	return true;
}

void fillVectorOfNFTs()
{
	for (int outerIt{ 0 }; outerIt < amountOfNFTs; outerIt++)
	{
		bool duplicateNFT_Created{ true };
		while (duplicateNFT_Created) {//build new NFT, calculate sum of indexes, compare with stored sums, if 1 unique index then store new NFT
			buildNFT();
			if (compareNFT_Sums())
			{
				duplicateNFT_Created = false;
				amountOfNFTsCreated++;
				NFTsums[outerIt] = calculateNFT_Sum();//store new sum
				if(limitCheck)
				std::cout << amountOfNFTsCreated << " NFTs created." << '\n';
				for (int innerIt{ 0 }; innerIt < typesOfItems; innerIt++)//store new NFT
				{
					vectorOfNFTs[outerIt][innerIt] = NFT[innerIt];
					if (FAONFTdebugging)
						std::cout << "vectorOfNFTs assigning " << NFT[innerIt] << " to Vector " << outerIt << " at index " << innerIt << '\n';
				}
			}
			else {
				duplicateNFT_Created = true;
				if (FAONFTdebugging)
					std::cout << "NFT# " << outerIt + 1 << " duplicateNFT created, trying again" << '\n';//+1 so that the first created is NFT #1
			}
		}
	}
}

void uniqueGuaranteeCheck()
{
	int totalNonUniqueNFTs{};
	for (int it{ 0 }; it < amountOfNFTs - 1; it++)
	{
		if (UGCdebugging)
			std::cout << "Comparing sum for NFT #" << it + 1 << '-' << NFTsums[it] << " against NFT#" << it + 2 << '-' << NFTsums[it + 1] << '\n';
		if (NFTsums[it] == NFTsums[it + 1])//iterate through stored and sorted sums, break if duplicate found
		{
			std::cout << "NFT SUMS INDEX#" << it << '=' << NFTsums[it] << " which is the same as " << "NFT SUMS INDEX#" << it + 1 << '=' << NFTsums[it + 1] << '\n';
			totalNonUniqueNFTs++;
			assert(NFTsums[it] != NFTsums[it + 1]);
		}
	}
	std::cout << "Non unique NFTs = " << totalNonUniqueNFTs << '\n';
}

void print2dVector(std::string string, std::vector<std::vector<double>> vector, int outerItRange, int innerItRange)
{
	for (int outerIt{ 0 }; outerIt < outerItRange; outerIt++)
	{
		std::cout << string << outerIt + 1 << ": [";
		for (int innerIt{ 0 }; innerIt < innerItRange; innerIt++)
		{
			std::cout << vector[outerIt][innerIt];
			if (innerIt != innerItRange -1)
				std::cout << ',';
		}
		std::cout << ']' << '\n';
	}
	std::cout << '\n';
}

void printSums(std::string string, std::vector<double> vector, int itRange)
{
	for (int it{ 0 }; it < itRange; it++)
	{
		std::cout << string << it + 1 << " SUM: " << vector[it] << '\n';//+1 so that the first created is NFT #1
	}
	std::cout << '\n';
}
//___________________CONSOLE MANIPULATION___________________//
void changeColour(int colour)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, colour);
}

int getX()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD                      result;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
	))
		return -1;
	return result.X;
}

int getY()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD                      result;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
	))
		return -1;
	return result.Y;
}

void moveCursorToXY(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}
void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}
//_________________END CONSOLE MANIPULATION_________________//
//_______________________SLOT MACHINE_______________________//
int determineItemRarityColour(double chosenItem, int outerIt)
{
	auto chosenItemIterator = std::find(vectorOfItems[outerIt].begin(), vectorOfItems[outerIt].end(), chosenItem);//returns iterator from vectorOfItems for chosenItem
	float itemIndexLocation = chosenItemIterator - vectorOfItems[outerIt].begin();//index of item from result of find
	float itemRarity = ((itemIndexLocation + 1) / amountOfItems) * 100;//index of item from chosen NFT in arrayOfItems / amount of items * 100 (eg. 1(item index) / 10(amountofItems) =.1 * 100 = 10 = item rarity common)

	if (itemRarity <= 39)//0-39=40% common,gray
		return 0;
	else if (itemRarity <= 69)//40-69=30% uncommon, green
		return 1;
	else if (itemRarity <= 89)//70-89=20% rare, blue
		return 2;
	else if (itemRarity <= 99)//90-99=9% very rare, purple
		return 3;
	else if (itemRarity == 100)//100=1% ultra rare, yellow
		return 4;
	else return 6;//red,error
}
void slotMachineVectorGrab()
{
	const int GREEN = 2;
	const int MAGENTA = 5;
	const int GRAY = 7;
	const int BLUE = 9;
	const int YELLOW = 14;
	const int WHITE = 15;
	const int RED = 12;
	int colour[] = { GRAY, GREEN, BLUE, MAGENTA, YELLOW, WHITE, RED };
	int randomNFT{ random(0, amountOfNFTs - 1 - amountOfNFTsChosen) };//-1 so the RNG generates from 0 to the size of the Vector - 1, since the Vector starts at 0 instead of 1
	int displayedItems{0};
	for (int outerIt{ 0 }; outerIt < typesOfItems; outerIt++)
	{
		
		for (int middleIt{ 0 }; middleIt < 20; middleIt++)
		{
			moveCursorToXY(displayedItems, 0);
			for (int innerIt{ 0 }; innerIt < typesOfItems - displayedItems; innerIt++)
			{
				changeColour(colour[random(0, 4)]);
				std::cout << '*';
			}
			Sleep(100);
			std::cout << "\b";
		}
		changeColour(colour[determineItemRarityColour(vectorOfNFTs[randomNFT][outerIt], outerIt)]);
		moveCursorToXY(displayedItems, 0);
		std::cout << '*';
		displayedItems++;
		moveCursorToXY(0,displayedItems);
		std::cout << "Item #" << outerIt + 1 << ' ' << vectorOfNFTs[randomNFT][outerIt] << '\n';
	}
	changeColour(WHITE);
	std::cout << '\n';
	auto it = vectorOfNFTs.begin() + randomNFT;
	std::rotate(it, it + 1, vectorOfNFTs.end());
	amountOfNFTsChosen++;
}
//_____________________END SLOT MACHINE_____________________//
int main()
{
	std::cout << std::fixed << std::setprecision(0);
	std::cout << "Type Y for Yes or N for No." << '\n';
	std::cout << "Check for maximum amount of creatable NFTs with given parameters?" << '\n';
	if (input())
	{
		limitCheck = true;
	}
	else {
		std::cout << "Run with debugging?" << '\n';
		if (input()) {
			std::cout << "Run with fillVectorOfItems() debugging?" << '\n';
			if (input())
				FAOIdebugging = true;
			std::cout << "Run with buildNFT() debugging?" << '\n';
			if (input())
				BNFTdebugging = true;
			std::cout << "Run with fillVectorOfNFTs() debugging?" << '\n';
			if (input())
				FAONFTdebugging = true;
			std::cout << "Run with uniqueGuaranteeCheck() debugging?" << '\n';
			if (input())
				UGCdebugging = true;
		}
	}

	auto start = std::chrono::steady_clock::now();
	fillVectorOfItems();
	fillVectorOfNFTs();
	fillItemSums();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_taken = end - start;

	std::cout << "Print Item Vectors?" << '\n';
	if (input())
		print2dVector("Item Vector #", vectorOfItems, typesOfItems, amountOfItems);
	std::cout << "Print Item Sums?" << '\n';
	if (input())
		printSums("Item Vector #", itemSums, typesOfItems);//print item sums

	std::cout << "Print NFTs?" << '\n';
	if (input())
	{
		std::cout << "Sorted NFTs? Note that the list of sums will only match the list of NFTs if both are unsorted. " << '\n';
		if (input())
			std::sort(vectorOfNFTs.begin(), vectorOfNFTs.end());

		print2dVector("NFT #", vectorOfNFTs, amountOfNFTs, typesOfItems);
	}

	std::cout << "Print NFT sums?" << '\n';
	if (input())
	{
		std::cout << "Sorted sums? Note that the list of sums will only match the list of NFTs if both are unsorted." << '\n';
		if (input())
			std::sort(NFTsums.begin(), NFTsums.end());

		printSums("NFT #", NFTsums, amountOfNFTs);//print NFT sums
	}

	uniqueGuaranteeCheck();
	std::cout << "Amount of NFTs created = " << amountOfNFTsCreated << '\n' << "Run Time: " << std::setprecision(10) << time_taken.count() << '\n';	
	std::cout << std::fixed << std::setprecision(0);
	//___________________LIGHTS___________________//
	bool spinning{true};
	while (spinning && amountOfNFTsChosen != amountOfNFTs)
	{
		clear();
		slotMachineVectorGrab();
		std::cout << "Spin again?" << '\n';
		if (!input())
			spinning = false;
	}
	if (amountOfNFTsChosen == amountOfNFTs)
		std::cout << "Sorry, all NFTs have been taken! Come back once we've made more!" << '\n';

	return 0;
}