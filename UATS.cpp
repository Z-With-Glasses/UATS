#include <assert.h>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>

const int amountOfDifferentItems{ 5 };//number of items in each array of items (eg if 5 then 5 different boots/hats/pants etc.)
const int amountOfNFTs{ 2593 };//amount of NFTs to be created		5difitems 5nftitems =2592	(WYZANT X=AODI N=AOINNFT)
const int amountOfItemsInNFT{ 5 };//amount of items in each NFT (eg if 5 then 5 unique items boots/hats/pants/chest/hat)

static double arrayOfItems[amountOfDifferentItems][amountOfItemsInNFT];//X[array#]Y[boots,pants,etc.]
static double NFT[amountOfItemsInNFT]{};
static double arrayOfNFTs[amountOfNFTs][amountOfItemsInNFT]{};//X[array#]Y[boots,pants,etc.]
static double NFTsums[amountOfNFTs];

static int amountOfNFTsCreated{ 0 };

std::mt19937 gen((unsigned)time(0));

int random(int low, int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

void fillArrayOfItems()
{
	static double arrayKey{ 1 };
	for (int outerIt{0}; outerIt < amountOfItemsInNFT; outerIt++)
	{
		double value{ arrayKey };//set value to key initially
		for (int innerIt{ 0 }; innerIt < amountOfItemsInNFT; innerIt++)
		{
			arrayOfItems[innerIt][outerIt] = value;//set element to array
//std::cout << "ITEMARRAY assigning " << value << " to array " << innerIt << " at index " << outerIt << '\n';
			value = value * 10;//set previous value to new value
		}
		
		arrayKey = arrayKey * 2;// double key value for next array
	}
};

void buildNFT()
{
	for (int it{0}; it < amountOfItemsInNFT; it++)
	{
		int randomArray{ random(0, amountOfItemsInNFT - 1 )};//-1 so the RNG generates from 0 to the size of the array - 1, since the array starts at 0 instead of 1
		NFT[it] = arrayOfItems[randomArray][it];
//std::cout << "NFTBUILD assigning " << NFT[it] << " to index " << it << " from random array " << randomArray << '\n';
	}
}

double calculateNFT_Sum()
{
	double NFT_ArrayTotal{};
	for (int it{ 0 }; it < amountOfItemsInNFT; it++)//iterate through NFT, sum the indexes
	{
		NFT_ArrayTotal += NFT[it];
	}
	return NFT_ArrayTotal;
}

bool compareNFT_Sums()
{
	bool passResult{};
	if (amountOfNFTsCreated == 0)
	{
		return 1;
	}
	for (int it{ 0 }; it < amountOfNFTsCreated; it++)
	{
		if (calculateNFT_Sum() == NFTsums[it])
		{
//std::cout << "PASSRESULT 0" << '\n';
			passResult = 0;
			break;
		}
		else {
			passResult = 1;
		}
	}
	return passResult;
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
				for (int innerIt{ 0 }; innerIt < amountOfItemsInNFT; innerIt++)//store new NFT
				{
					arrayOfNFTs[outerIt][innerIt] = NFT[innerIt];
//std::cout << "NFTARRAYARRAY assigning " << NFT[innerIt] << " to array " << outerIt << " at index " << innerIt << '\n';
				}
			}
			else {
				duplicateNFT_Created = true;
//zstd::cout << "NFT# "<< outerIt <<" duplicateNFT created" << '\n';
			}
		}
	}
}
void uniqueGuaranteeCheck()
{
	int totalNonUniqueArrays{};
	for (int it{ 0 }; it < amountOfNFTs-1; it++)
	{
/*
		std::cout << "Comparing sum for NFT #" << it + 1 << " against NFT#" << it + 2 << '\n';
		std::cout << "NFT #" << it+1 << " = " << NFTsums[it] << '\n';
		std::cout << "NFT #" << it+2 << " = " << NFTsums[it+1] << '\n';
*/
		if (NFTsums[it] == NFTsums[it + 1])//iterate through stored and sorted sums, break if duplicate found
		{
			std::cout << "NFT SUMS INDEX#" << it << '=' << NFTsums[it] << " which is the same as " << "NFT SUMS INDEX#" << it + 1 << '=' << NFTsums[it + 1] << '\n';
			totalNonUniqueArrays++;
			assert(NFTsums[it] != NFTsums[it + 1]);
		}
	}
	std::cout << "Non unique NFTs = " << totalNonUniqueArrays << '\n';
}
void printNFT_Sums() 
{
	for (int it{ 0 }; it < amountOfNFTs; it++)
	{
		std::cout << "NFT #" << it + 1 << " SUM: " << NFTsums[it] << '\n';
	}
	std::cout << '\n';
}
void printNFTs()
{
	for (int outerIt{ 0 }; outerIt < amountOfNFTs; outerIt++)
	{
		std::cout << "NFT #" << outerIt+1 << ": ";
		for (int innerIt{ 0 }; innerIt < amountOfItemsInNFT; innerIt++)
		{
			std::cout << arrayOfNFTs[outerIt][innerIt] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}
int main()
{
	fillArrayOfItems();
	fillArrayOfNFTs();

	std::sort(NFTsums, NFTsums + amountOfNFTs);

	//printNFTs();
	//printNFT_Sums();

	uniqueGuaranteeCheck();
	std::cout << "Amount of NFTs created = " << amountOfNFTsCreated << '\n';
	return 0;
}