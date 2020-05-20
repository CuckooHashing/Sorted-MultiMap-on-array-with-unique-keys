#include "SortedMultiMap.h"
#include "SMMIterator.h"


int SortedMultiMap::instance(TKey searched) const
{//returns the position on which the tkey elem was found, -1 if it was not found O(n)
	int i = this->head;
	while (i != -1)
	{
		if (this->nodes[i].key == searched)
			return i;
		i = this->nodes[i].next;
	}
	return -1;
}

int SortedMultiMap::find_position(TKey searched)
{//returns the position after which the the tkey element must be placed. -1 if it is the head
//O(n)
	int i = this->head;
	if (i == -1 || r(this->nodes[i].key, searched) == false)
		return -1;
	while (this->nodes[i].next != -1)
	{
		if (r(this->nodes[i].key, searched) && r(this->nodes[this->nodes[i].next].key, searched) == false)
			return i;
		if (this->nodes[i].next == -1)
			return i;
		//ZSU: should go to next i, otherwise it will cycle forever
		i = this->nodes[i].next;
	}

	//ZSU: return i, otherwise in  case of list with one element it will return -1 which is our special value for something else.
	return i;
}

//ZSU: added position as param. I do not like tha you return nodes...all sorts of copies happend there that can mess up memory
void  SortedMultiMap::newNode(TKey c, TValue v, int pos)
{//creates a new node for the sortedmultimap at position pos 

	this->nodes[pos].dlla = new VNode[this->nodes[pos].capac];
	for (int i = 0; i < this->nodes[pos].capac; i++)
	{
		this->nodes[pos].dlla[i].next = i + 1;
		this->nodes[pos].dlla[i].previous = i - 1;
	}
	//ZSU: set firstEmpty as well
	this->nodes[pos].firstEmpty = 0;
	this->nodes[pos].dlla[this->nodes[pos].capac - 1].next = -1;
	this->nodes[pos].dlla[this->nodes[pos].firstEmpty].info = v;
	this->nodes[pos].head = this->nodes[pos].firstEmpty;
	this->nodes[pos].tail = this->nodes[pos].firstEmpty;
	this->nodes[pos].firstEmpty = this->nodes[pos].dlla[this->nodes[pos].firstEmpty].next;
	//ZSU set ends of the value list to -1	
	this->nodes[pos].dlla[this->nodes[pos].head].next = -1;
	this->nodes[pos].dlla[this->nodes[pos].head].previous = -1;	
	this->nodes[pos].key = c;
	//return ad;
}


void SortedMultiMap::resizeKNode(int pos)
{//resises the dlla of a node from the sortedmultimap
//complexity is theta(n)	
	this->nodes[pos].capac *= 2;
	VNode* aux = new VNode[this->nodes[pos].capac];
	int i = this->nodes[pos].head;
	while (i != -1)
	{
		//ZSU: copy next and prev as well!!!
		aux[i].info = this->nodes[pos].dlla[i].info;
		aux[i].previous = this->nodes[pos].dlla[i].previous;
		aux[i].next = this->nodes[pos].dlla[i].next;
		i = this->nodes[pos].dlla[i].next;
	}
	
	for (i = this->nodes[pos].capac/2; i < this->nodes[pos].capac; i++)
	{
		aux[i].next = i + 1;
		aux[i].previous = i - 1;
	}
	aux[this->nodes[pos].capac - 1].next = -1;
	delete[] this->nodes[pos].dlla;
	this->nodes[pos].dlla = aux;
	this->nodes[pos].firstEmpty = this->nodes[pos].capac / 2;
	//return thing;
}

void SortedMultiMap::resize()
{//resizes the main dlla of the sortedmultimap 
//complexity is theta(n)
	this->capacity *= 2;
	KNode* aux = new KNode[this->capacity];
	int i = this->head;
	while (i != -1)
	{
		aux[i] = this->nodes[i];
		i = this->nodes[i].next;
	}
	for (i = this->len; i < this->capacity; i++)
	{
		aux[i].next = i + 1;
		aux[i].previous = i - 1;
		aux[i].dlla = nullptr;
	}
	aux[this->capacity - 1].next = -1;
	delete[] this->nodes;
	this->nodes = aux;
	this->firstEmpty = this->len;
	this->nodes[this->len].previous = -1;
	
}

SortedMultiMap::SortedMultiMap(Relation r) :r{ r }
{//complexity is theta(n)
	this->head = -1;
	this->tail = -1;
	this->firstEmpty = 0;
	this->nodes = new KNode[this->capacity];
	this->len = 0;
	this->nodes[0].previous = -1;
	for (int i = 0; i < this->capacity; i++)
	{
		this->nodes[i].next = i + 1;
		this->nodes[i].previous = i - 1;
		//ZSU: since you have a  separate function for initializing the value list part, this is not needed. We just need to set them to nullptr then, to make
		//sure everything is fine...plus, we are going to check for nullptr in destructor.
		this->nodes[i].dlla = nullptr;
		//now for i's array! yay!
		/*this->nodes[i].dlla = new VNode[this->nodes[i].capac];
		this->nodes[i].dlla[0].previous = -1;
		for (int j = 0; j < this->nodes[i].capac; j++)
		{
			this->nodes[i].dlla[j].next = i + 1;       //PLS DON'T DO THIS AGAIN
			this->nodes[i].dlla[j].previous = j - 1;
		}
		
		this->nodes[i].dlla[this->nodes[i].capac - 1].next = -1;
		*/
	}
	this->nodes[this->capacity - 1].next = -1;
}


void SortedMultiMap::add(TKey c, TValue v)//<---it messes with firstEmpty
{//complexity is O(n)
	int here = this->instance(c);
	if (here != -1)
	{
		//add a new element to an already existing key
		if (this->nodes[here].firstEmpty == -1)
		{
			//ZSU: pass only the position where changs should happen. I do not trust it when that many copies happen
			this->resizeKNode(here);
		}
		this->nodes[here].dlla[this->nodes[here].firstEmpty].info = v;
		int ver = this->nodes[here].firstEmpty;
		this->nodes[here].firstEmpty = this->nodes[here].dlla[this->nodes[here].firstEmpty].next;
		this->nodes[here].dlla[ver].next = -1;
		this->nodes[here].dlla[ver].previous = this->nodes[here].tail;
		this->nodes[here].dlla[this->nodes[here].tail].next = ver;
		this->nodes[here].tail = ver;
		this->key_value++;
	}
	else
	{
		//add a new key then
		if (this->firstEmpty == -1)
		{
			this->resize();
		}
		here = this->find_position(c);
		//KNode newElem = 
		this->newNode(c, v, this->firstEmpty);
		//this->nodes[this->firstEmpty] = newElem;
		if (here != -1)
		{
			//now for the linkssss
			
				if (here == this->tail)//if best position is tail
				{
					int temp = this->nodes[this->firstEmpty].next;
					this->nodes[this->firstEmpty].previous = this->tail;
					this->nodes[this->firstEmpty].next = -1;
					this->nodes[this->tail].next = this->firstEmpty;
					this->tail = this->firstEmpty;
					this->firstEmpty = temp;
				}
				else
				{//if best position is a random position
					int temp = this->nodes[this->firstEmpty].next;
					this->nodes[this->firstEmpty].previous = here;
					this->nodes[this->firstEmpty].next = this->nodes[here].next;
					this->nodes[this->nodes[here].next].previous = this->firstEmpty;
					this->nodes[here].next = this->firstEmpty;
					this->firstEmpty = temp;
				}
			this->len++;
			this->key_value++;
		}
		else
		{
			if(this->head==-1)//<-----here this->nodes[firstEmpty].next abrupty changes  from 1 to a random value-no idea why
			{
				this->head = this->firstEmpty;
				this->tail = this->firstEmpty;
				this->firstEmpty = this->nodes[this->firstEmpty].next;
				this->nodes[this->head].next = -1;
				this->nodes[this->head].previous = -1;
			}
			else
			{
				int temp = this->nodes[this->firstEmpty].next;
				this->nodes[this->firstEmpty].next = this->head;
				this->nodes[this->firstEmpty].previous = -1;
				this->nodes[this->head].previous = this->firstEmpty;
				this->head = this->firstEmpty;
				this->firstEmpty = temp;
			}
			this->len++;
			this->key_value++;
		}
	}
}

vector<TValue> SortedMultiMap::search(TKey c) const
{//complexity is theta(n)
	int here = this->instance(c);
	vector<TValue> things;
	if (here == -1)
		return things;
	else 
	{
		int i = this->nodes[here].head;
		while (i != -1)
		{
			things.push_back(this->nodes[here].dlla[i].info);
			i = this->nodes[here].dlla[i].next;
		}
	}
	return things;
}


bool SortedMultiMap::remove(TKey c, TValue v)
{//complexity is O(n)
	int here = this->instance(c);
	if (here != -1)
	{
		int i = this->nodes[here].head;
		while (i != -1)
		{
			if (this->nodes[here].dlla[i].info == v)
			{//case i == head == tail
				if (this->nodes[here].head == i && this->nodes[here].tail == i)
				{	
					

					//ZSU. It is correct that you have to remove this whole KNode. But here you again have several cases...
					//ZSU: this is common stuff
					delete[] this->nodes[here].dlla;
					//ZSU: just for safety
					this->nodes[here].dlla = nullptr;
					this->key_value--;
					this->len--;
					
					//here is head and tail		
					if (here == this->head && here == this->tail) {
						this->head = -1;
						this->tail = -1;

						this->nodes[here].next = this->firstEmpty;
						if (this->firstEmpty != -1) {
							this->nodes[this->firstEmpty].previous = here;
						}
						this->firstEmpty = here;
					}
					else if (here == this->head) {
						//here is just head
						this->head = this->nodes[head].next;
						this->nodes[head].previous = -1;
						this->nodes[here].next = this->firstEmpty;
						if (this->firstEmpty != -1) {
							this->nodes[this->firstEmpty].previous = here;
						}
						this->firstEmpty = here;
					}
					else if (here == this->tail) {
						//here is just tail
						this->tail = this->nodes[this->tail].previous;
						this->nodes[this->tail].next = -1;
						this->nodes[here].next = this->firstEmpty;
						if (this->firstEmpty != -1) {
							this->nodes[this->firstEmpty].previous = here;
						}
						this->firstEmpty = here;
					}
					else {
						//here is in the middle -> this is what you implemented
						this->nodes[this->nodes[here].previous].next = this->nodes[here].next;
						this->nodes[this->nodes[here].next].previous = this->nodes[here].previous;

						this->nodes[here].next = this->firstEmpty;
						if (this->firstEmpty != -1) {
							this->nodes[this->firstEmpty].previous = here;
						}
						this->firstEmpty = here;
					}
					return true;
				}
				else
				{
					if (this->nodes[here].head == i)
					{//if the one to be removed is the head
						this->nodes[here].dlla[this->nodes[here].dlla[i].next].previous = -1;
						this->nodes[here].head = this->nodes[here].dlla[i].next;
						this->key_value--;
						this->nodes[here].dlla[i].next = this->nodes[here].firstEmpty;
						if (this->nodes[here].firstEmpty != -1)
						{
							this->nodes[here].dlla[this->nodes[here].firstEmpty].previous = i;
						}
						this->nodes[here].firstEmpty = i;
						return true;
					}
					else
					{
						if (this->nodes[here].tail == i)
						{//if the one to be removed is the tail
							this->nodes[here].dlla[this->nodes[here].dlla[i].previous].next = -1;
							this->nodes[here].tail = this->nodes[here].dlla[i].previous;
							this->key_value--;
							this->nodes[here].dlla[i].next = this->nodes[here].firstEmpty;
							if (this->nodes[here].firstEmpty != -1)
							{
								this->nodes[here].dlla[this->nodes[here].firstEmpty].previous = i;
							}
							this->nodes[here].firstEmpty = i;
							return true;

						}
						else
						{//if the one to be removed is a random node
							this->nodes[here].dlla[this->nodes[here].dlla[i].previous].next = this->nodes[here].dlla[i].next;
							this->nodes[here].dlla[this->nodes[here].dlla[i].next].previous = this->nodes[here].dlla[i].previous;
							this->nodes[here].firstEmpty = i;
							this->key_value--;
							this->nodes[here].dlla[i].next = this->nodes[here].firstEmpty;
							if (this->nodes[here].firstEmpty != -1)
							{
								this->nodes[here].dlla[this->nodes[here].firstEmpty].previous = i;
							}
							this->nodes[here].firstEmpty = i;
							return true;

						}
					}
				}
			}
			i = this->nodes[here].dlla[i].next;
		}
	}
	return false;
}

int SortedMultiMap::size() const
{//theta(1)
	return this->key_value;
}

bool SortedMultiMap::isEmpty() const
{//theta(1)
	return this->len == 0;
}

SMMIterator SortedMultiMap::iterator() const
{//theta(1)
	return SMMIterator(*this);
}

SortedMultiMap::~SortedMultiMap()
{//complexity is theta(n)
	for (int i = 0; i < this->capacity; i++)
		//check to be different from nullptr;
		if (this->nodes[i].dlla != nullptr) {
			delete[] this->nodes[i].dlla;//<-----this triggers a breakpoint in delete_scalar.cpp. Maybe because memory is allocated for a KNode in both the constructor of the smm and in newKNode. program
		}
	delete[] this->nodes;//will not work if either is commented
}
