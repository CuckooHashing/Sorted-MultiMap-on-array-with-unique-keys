#pragma once
#include "SortedMultiMap.h" 

typedef SortedMultiMap Container;

class SMMIterator
{
private:
	const Container& c;	
	int current;
	int currentIndex;

public:
	SMMIterator(const Container& c);
	~SMMIterator();
	//sets the iterator to the first element of the container

	void first();



	//moves the iterator to the next element

	//throws exception if the iterator is not valid

	void next();

	//moves the iterator to k steps back

	//throws exception if the iterator is not valid

	void jumpBackward(int k);


	//checks if the iterator is valid

	bool valid() const;



	//returns the value of the current element from the iterator

	// throws exception if the iterator is not valid

	TElem getCurrent() const;
};

