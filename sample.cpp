#include "InvertedIndex.h"
#include "InvertedIndex.cpp"

int main()
{
	int wordSize = 100;
	std::vector<uint32_t> vectWord;
	vectWord.resize(wordSize);

	for (int i = 0; i < wordSize; i++)
	{
		vectWord[i] = rand() % 10000;
	}

	SInvDoc<uint32_t, int32_t> Doc(1, wordSize, vectWord);


	InvertedIndex<uint32_t, int32_t> invIdx;

	invIdx.addDoc(Doc);
	invIdx.updateDoc(Doc);
	Doc.nID = 10;
	invIdx.addDoc(Doc);
	invIdx.delDoc(1);



	return 0;

}