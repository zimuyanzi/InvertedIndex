#include "InvertedIndex.h"
#include "InvertedIndex.cpp"

int main()
{
	int wordSize = 100;
	std::vector<uint32_t> vectWord;
	vectWord.resize(wordSize);

	for (int i = 0; i < wordSize; i++)
	{
		vectWord[i] = rand() % 1000;
	}

	SInvDoc<uint32_t, int32_t> Doc(1, wordSize, vectWord);


	InvertedIndex<uint32_t, int32_t> invIdx;

	invIdx.addDoc(Doc);
	invIdx.updateDoc(Doc);
	Doc.nID = 10;
	invIdx.addDoc(Doc);
	invIdx.delDoc(1);

	for (int32_t i = 0; i < 100; i++)
	{
		for (int j = 0 ;j < wordSize; j ++)
		{
			vectWord[j] = rand() % 1000;
		}
		SInvDoc<uint32_t, int32_t> newDoc(i, wordSize, vectWord);
		invIdx.addDoc(newDoc);
	}

	int32_t id;
	invIdx.getMatchID(Doc, 5, MatchType::HAMMING_DISTANCE, id);

	return 0;

}