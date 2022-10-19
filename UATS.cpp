#include <assert.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <vector>

const int typesOfItems{ 5 };//number of types of items in each NFT (eg if 5 then 5 unique items boots/chest/pants/gloves/hats (above 150 results in INF)
const int numberOfItems{ 10 };//number of items for each type of item (eg if 10 then 10 item boots/hats/pants etc.) (above 22 results in overflow)
const int amountOfNFTs{ 90500 };//amount of NFTs to be created
static std::vector<std::vector <double>> vectorOfItems(typesOfItems, std::vector<double>(numberOfItems));
static double NFT[typesOfItems]{};
static std::vector<std::vector <double>> vectorOfNFTs(amountOfNFTs, std::vector<double>(typesOfItems));
static double NFTsums[amountOfNFTs]{};
static int amountOfNFTsCreated{ 0 };
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

void fillArrayOfItems()
{
	static double valueKey{ 1 };
	for (int outerIt{ 0 }; outerIt < typesOfItems; outerIt++)
	{
		double value{ valueKey };//set value to key initially
		for (int innerIt{ 0 }; innerIt < numberOfItems; innerIt++)
		{
			vectorOfItems[outerIt][innerIt] = value;//set element to array
			if (FAOIdebugging)
				std::cout << "ITEMARRAY assigning " << value << " to array " << outerIt << " at index " << innerIt << '\n';
			value = value * 10;//set previous value to new value
		}
		valueKey = valueKey * 2;// double key value for next array
	}
};

void buildNFT()
{
	for (int it{ 0 }; it < typesOfItems; it++)
	{
		int randomItem{ random(0, numberOfItems - 1) };//-1 so the RNG generates from 0 to the size of the array - 1, since the array starts at 0 instead of 1
		NFT[it] = vectorOfItems[it][randomItem];
		if (BNFTdebugging)
			std::cout << "buildNFT() assigning " << vectorOfItems[it][randomItem] << " to index " << it << " RNG= " << randomItem << '\n';
	}
}

double calculateNFT_Sum()
{
	double NFT_ArrayTotal{};
	for (int it{ 0 }; it < typesOfItems; it++)//iterate through NFT, sum the indexes
	{
		NFT_ArrayTotal += NFT[it];
	}
	return NFT_ArrayTotal;
}

bool compareNFT_Sums()
{
	if (amountOfNFTsCreated == 0)
		return 1;
	for (int it{ 0 }; it < amountOfNFTsCreated; it++)
	{
		if (calculateNFT_Sum() == NFTsums[it])
		{
			return 0;
		}
	}
	return 1;
}

void fillArrayOfNFTs()
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
						std::cout << "NFTARRAYARRAY assigning " << NFT[innerIt] << " to array " << outerIt << " at index " << innerIt << '\n';
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
void printNFT_Sums()
{
	for (int it{ 0 }; it < amountOfNFTs; it++)
	{
		std::cout << "NFT #" << it + 1 << " SUM: " << NFTsums[it] << '\n';//+1 so that the first created is NFT #1
	}
	std::cout << '\n';
}
int input()
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
}
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
			std::cout << "Run with fillArrayOfItems() debugging?" << '\n';
			if (input())
				FAOIdebugging = true;
			std::cout << "Run with buildNFT() debugging?" << '\n';
			if (input())
				BNFTdebugging = true;
			std::cout << "Run with fillArrayOfNFTs() debugging?" << '\n';
			if (input())
				FAONFTdebugging = true;
			std::cout << "Run with uniqueGuaranteeCheck() debugging?" << '\n';
			if (input())
				UGCdebugging = true;
		}
	}
	auto start = std::chrono::steady_clock::now();
	fillArrayOfItems();
	fillArrayOfNFTs();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_taken = end - start;

	std::cout << "Print Item Arrays?" << '\n';
	if (input())
		print2dVector("Item Array #", vectorOfItems, typesOfItems, numberOfItems);
	std::cout << "Print NFTs?" << '\n';
	if (input())
	{
		std::cout << "Sorted NFTs? Note that the list of sums will only match the list of NFTs if both are unsorted. " << '\n';
		if (input())
		{
				sort(vectorOfNFTs.begin(), vectorOfNFTs.end());
		}
		print2dVector("NFT #", vectorOfNFTs, amountOfNFTs, typesOfItems);
	}
	std::cout << "Print NFT sums?" << '\n';
	if (input())
	{
		std::cout << "Sorted sums? Note that the list of sums will only match the list of NFTs if both are unsorted." << '\n';
		if (input())
		{
			std::sort(NFTsums, NFTsums + amountOfNFTs);
		}
		printNFT_Sums();
	}

	uniqueGuaranteeCheck();
	std::cout << "Amount of NFTs created = " << amountOfNFTsCreated << '\n' << "Run Time: " << std::setprecision(10) << time_taken.count() << '\n';
}