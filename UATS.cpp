#include <assert.h>
#include <chrono>//comment out if not testing run speed
#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>

//DO NOT SET numberOfItems ABOVE 300 IF YOUR COMPILER'S LONG DOUBLE RESOLVES TO DOUBLE
const int typesOfItems{ 5 };//(X)number of types of items in each NFT (eg if 5 then 5 unique items boots/chest/pants/gloves/hats
const int numberOfItems{ 300 };//(N)number of items for each type of item (eg if 10 then 10 item boots/hats/pants etc.) !!!!INF IF ABOVE 300, long double limit for some compilers is 1e+309!!!!
const int amountOfNFTs{ typesOfItems * numberOfItems };//amount of NFTs to be created, tOI * nOI is not the max but it is safe
static double arrayOfItems[typesOfItems][numberOfItems]{};//X[array#]Y[boots,pants,etc.]
static double NFT[typesOfItems]{};
static double arrayOfNFTs[amountOfNFTs][typesOfItems]{};//X[array#]Y[boots,pants,etc.]
static double NFTsums[amountOfNFTs]{};
static int amountOfNFTsCreated{ 0 };

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
			arrayOfItems[outerIt][innerIt] = value;//set element to array
//std::cout << "ITEMARRAY assigning " << value << " to array " << outerIt << " at index " << innerIt << '\n';
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
		NFT[it] = arrayOfItems[it][randomItem];
//std::cout << "buildNFT() assigning " << arrayOfItems[it][randomItem] << " to index " << it  << " RNG= " << randomItem << '\n';
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
				for (int innerIt{ 0 }; innerIt < typesOfItems; innerIt++)//store new NFT
				{
					arrayOfNFTs[outerIt][innerIt] = NFT[innerIt];
//std::cout << "NFTARRAYARRAY assigning " << NFT[innerIt] << " to array " << outerIt << " at index " << innerIt << '\n';
				}
			}
			else {
				duplicateNFT_Created = true;
//std::cout << "NFT# "<< outerIt + 1 <<" duplicateNFT created, trying again" << '\n';//+1 so that the first created is NFT #1
			}
		}
	}
}
void uniqueGuaranteeCheck()
{
	int totalNonUniqueNFTs{};
	for (int it{ 0 }; it < amountOfNFTs - 1; it++)
	{
//std::cout << "Comparing sum for NFT #" << it + 1 << '-' << NFTsums[it] << " against NFT#" << it + 2 << '-' << NFTsums[it+1] << '\n';
		if (NFTsums[it] == NFTsums[it + 1])//iterate through stored and sorted sums, break if duplicate found
		{
			std::cout << "NFT SUMS INDEX#" << it << '=' << NFTsums[it] << " which is the same as " << "NFT SUMS INDEX#" << it + 1 << '=' << NFTsums[it + 1] << '\n';
			totalNonUniqueNFTs++;
			assert(NFTsums[it] != NFTsums[it + 1]);
		}
	}
	std::cout << "Non unique NFTs = " << totalNonUniqueNFTs << '\n';
}
void printItemArrays()
{
	for (int outerIt{ 0 }; outerIt < typesOfItems; outerIt++)
	{
		std::cout << "Item Array #" << outerIt + 1 << ": ";
		for (int innerIt{ 0 }; innerIt < numberOfItems; innerIt++)
		{
			std::cout << arrayOfItems[outerIt][innerIt] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}
void printNFTs()
{
	for (int outerIt{ 0 }; outerIt < amountOfNFTs; outerIt++)
	{
		std::cout << "NFT #" << outerIt + 1 << ": ";
		for (int innerIt{ 0 }; innerIt < typesOfItems; innerIt++)
		{
			std::cout << arrayOfNFTs[outerIt][innerIt] << ' ';
		}
		std::cout << '\n';
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
int main()
{
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	fillArrayOfItems();
	fillArrayOfNFTs();
	std::sort(NFTsums, NFTsums + amountOfNFTs);

	//printItemArrays();
	//printNFTs();
	//printNFT_Sums();

	uniqueGuaranteeCheck();

	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

	std::cout << "Amount of NFTs created = " << amountOfNFTsCreated << '\n' << "Run Time: " << time_span.count() << '\n';
	std::cout << "Run again? Y/N" << '\n';
	
	bool cont{1};
	while (cont)
	{
		char input{};
		std::cin >> input;
		if (input == 'Y' || input == 'y')
		{
			main();
		}
		else if (input == 'N' || input == 'n')
		{
			return 0;
		}
		std::cout << "Enter Y to continue or N to exit" << '\n';
	}
}