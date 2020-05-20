#include "SMMIterator.h"

SMMIterator::SMMIterator(const Container& c) : c{c}{
	this->currentIndex = this->c.head;
	this->current = this->c.nodes[this->c.head].head;
	//theta(1)
}

SMMIterator::~SMMIterator()
{//theta(1)
}

void SMMIterator::first()
{//theta(1)
	this->currentIndex = this->c.head;
	this->current = this->c.nodes[c.head].head;
}

void SMMIterator::next()
{//theta(1)
	if (this->valid())
	{
		this->current = this->c.nodes[this->currentIndex].dlla[this->current].next;
		if (this->current == -1)
		{
			this->currentIndex = this->c.nodes[this->currentIndex].next;
			//ZSU: do this only if currentIndex is not -1
			if (this->currentIndex != -1) {
				this->current = this->c.nodes[this->currentIndex].head;
			}
		}
	}
	else
		throw exception();
}

bool SMMIterator::valid() const
{//theta(1)
	if(this->currentIndex == -1)
		return false;
	return true;
}

void SMMIterator::jumpBackward(int k)
{//complexity O(n)

	int i = 0;
	if (this->valid())
	{
		while (this->valid() && i < k)
		{
			i++;
			this->current = this->c.nodes[this->currentIndex].dlla[this->current].previous;
			if (this->current == -1)
			{
				this->currentIndex = this->c.nodes[this->currentIndex].previous;
				if (this->currentIndex != -1)
					this->current = this->c.nodes[this->currentIndex].tail;
			}

		}
		if (i < k)
			this->currentIndex = -1;
	}
	else
		throw exception();
}

TElem SMMIterator::getCurrent() const
{//theta(1)
	TElem thing (this->c.nodes[this->currentIndex].key, this->c.nodes[this->currentIndex].dlla[this->current].info);
	return thing;
}
