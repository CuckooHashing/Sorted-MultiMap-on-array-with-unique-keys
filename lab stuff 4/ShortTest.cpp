#include <assert.h>
#include <iostream>
#include "SortedMultiMap.h"
#include "SMMIterator.h"

#include <exception>
#include <vector>

using namespace std;

bool relation1(TKey cheie1, TKey cheie2) {
	if (cheie1 <= cheie2) {
		return true;
	}
	else {
		return false;
	}
}

void testAll(){
	SortedMultiMap smm = SortedMultiMap(relation1);
	assert(smm.size() == 0);
	assert(smm.isEmpty());
    smm.add(1,2);
    smm.add(1,3);
    assert(smm.size() == 2);
    assert(!smm.isEmpty());
    vector<TValue> v= smm.search(1);
    assert(v.size()==2);
    v= smm.search(3);
    assert(v.size()==0);
   
	smm.add(2, 5);
	smm.add(7, 3);
	smm.add(7, 2);
	smm.add(7, 12);
	SMMIterator back = smm.iterator();
	back.first();
	int count = 0;
	while (count < 4 && back.valid())
	{
		count++;
		back.next();
	}
	back.jumpBackward(count - 1);
	assert(back.valid());
	TElem e = back.getCurrent();
	assert(e.first == 1);
	assert(e.second == 3);
	back.first();
	back.jumpBackward(20);
	assert(back.valid() == false);
	smm.add(-4, 5);
	smm.add(5, 5);
	smm.add(7, 0);
	smm.add(7, 0);
	smm.add(7, 0);
	smm.add(7, 0);
	smm.add(7, 0);
	smm.add(7, 0);
	smm.add(7, 0);
	smm.add(7, 0);
	smm.add(17, 4);
	smm.add(42, 7);
	smm.add(15, 3);
	smm.add(-11, -11);
	smm.add(9, 9);
	cout << smm.size() << endl;
	SMMIterator it = smm.iterator();
	it.first();
    while (it.valid()){
    	TElem e = it.getCurrent();
		cout << e.first << " " << e.second << endl;
    	it.next();
    }
    assert(smm.remove(1, 2) == true);
    assert(smm.remove(1, 3) == true);
    assert(smm.remove(2, 1) == false);
	assert(smm.remove(7, 0) == true);
	assert(smm.remove(7, 0) == true);
	assert(smm.remove(7, 0) == true);
	assert(smm.remove(7, 0) == true);
	assert(smm.remove(7, 0) == true);
	assert(smm.remove(7, 0) == true);
	assert(smm.remove(7, 0) == true);
	assert(smm.remove(7, 0) == true);
	assert(smm.remove(7, 0) == false);
	assert(smm.remove(-11, -11) == true);
	assert(smm.remove(42, 7) == true);
	assert(smm.remove(17, 4) == true);
	cout << "....." << endl;
	SMMIterator it2 = smm.iterator();
	it2.first();
	while (it2.valid()) {
		TElem e = it2.getCurrent();
		cout << e.first << " " << e.second << endl;
		it2.next();
	}
	assert(smm.remove(9, 9) == true);
	assert(smm.remove(5, 5) == true);
	assert(smm.remove(2, 5) == true);
	cout << "....." << endl;	
	it2.first();
	while (it2.valid()) {
		TElem e = it2.getCurrent();
		cout << e.first << " " << e.second << endl;
		it2.next();
	}
	assert(smm.remove(7, 3) == true);
	assert(smm.remove(7, 12) == true);
	assert(smm.remove(15, 3) == true);
	assert(smm.remove(7, 2) == true);
	assert(smm.remove(-4, 5) == true);
	assert(smm.isEmpty());
}

