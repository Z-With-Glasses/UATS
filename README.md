This project creates an amount of vectors with at least one unique element. This is done by having a set of X numbers where each number in one vector is 10* the previous one and each vector's starting number is doubled from the previous one. Then a vector is created by randomly picking one number from each of the vectors. Uniqueness is checked by comparing the sums of the vectors of randomly chosen numbers.

//Example//
Vector #1 [1,10,100,1000,10000]
Vector #2 [2,20,200,2000,20000]
Vector #3 [4,40,400,4000,40000]

Unique Vector #1 [1,2,400] Sum:412
Unique Vector #2 [1,20000,40] Sum:20041

In this program the vectors of * 10 incrementing values are called vectors of items, the unique vectors are called NFTs.

In the last part of the project a slot machine is run which displays the unique vectors and assigns rarity to each item based on the number of items in each vector.
