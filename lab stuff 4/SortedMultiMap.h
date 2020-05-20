#pragma once

#include <vector>



typedef int TKey;

typedef int TValue;



#include <utility>

typedef std::pair<TKey, TValue> TElem;

class SMMIterator;

using namespace std;



typedef bool(*Relation)(TKey, TKey);

struct VNode{
	int next = -1;
	int previous = -1;
	TValue info;
};

struct KNode {
	VNode *dlla;
	int head = -1, tail = -1, firstEmpty = 0, capac = 4;
	int next, previous;
	TKey key;

};

class SortedMultiMap {



private:

	/* representation of the SortedMultiMap */
	int capacity = 4;//capacity of the smm
	int head, tail;// 'main' head and tail. belong to smm
	int firstEmpty;//firstEmpty in sll
	Relation r;
	KNode* nodes;//list of KNodes
	int len;//number of keys
	int key_value = 0;//number of key/value pairs

	//seeks the key in the list. will return its position if found, -1 otherwise
	int instance(TKey searched) const;
	//searches for the position in which the new KValue must be placed in order to maintain the sorted property. -1 if such a position was not found 
	int find_position(TKey searched);
	//creates a new KNode. for the sake of it looking nice. Ok, but add the position from your DLLA where to create it directly
	void newNode(TKey c, TValue v, int pos);
	//resizes a KNode
	void resizeKNode(int pos);
	//global resize
	void resize();


public:


	friend class SMMIterator;

	// constructor

	SortedMultiMap(Relation r);



	//adds a new key value pair to the sorted multi map

	void add(TKey c, TValue v);



	//returns the values belonging to a given key

	vector<TValue> search(TKey c) const;



	//removes a key value pair from the sorted multimap

	//returns true if the pair was removed (it was part of the multimap), false if nothing is removed

	bool remove(TKey c, TValue v);



	//returns the number of key-value pairs from the sorted multimap

	int size() const;


	//verifies if the sorted multi map is empty

	bool isEmpty() const;



	// returns an iterator for the sorted multimap. The iterator will returns the pairs as required by the relation (given to the constructor)	

	SMMIterator iterator() const;



	// destructor

	~SortedMultiMap();

};
