#include "InvertedIndex.h"


#ifndef __INVERTED_CPP__
#define __INVERTED_CPP__

template<class TKeyWord, class TID>
SInvDoc<TKeyWord, TID>::SInvDoc(TID ID, int32_t nSize, std::vector<TKeyWord> &vecWord)
{
	nID = ID;
	nVecWord.resize(nSize);
	memcpy(&nVecWord[0], &vecWord[0], sizeof(TKeyWord)*nSize);

	for (int i = 0; i < nSize; i++)
	{
		auto wordTF = nMapTF.find(nVecWord[i]);
		if (wordTF == nMapTF.end())
			nMapTF[nVecWord[i]] = 1;
		else
			wordTF->second++;
	}
}

template<class TID>
bool operator<(const SIDCount<TID> &a, const SIDCount<TID> &b)
{
	return a.nCount > b.nCount;
}

template<class TKeyWord, class TID>
bool InvertedIndex<TKeyWord, TID>::addDoc(IN SInvDoc<TKeyWord, TID> &Doc)
{
	//! add doc to doc map
	auto newDoc = m_mapDoc.find(Doc.nID);
	if (newDoc == m_mapDoc.end())
		m_mapDoc.insert(std::make_pair(Doc.nID, Doc));
	else
		return false;

	//! add word to word doc map
	auto wordTF = Doc.nMapTF.begin();
	while (wordTF != Doc.nMapTF.end())
	{
		auto newWord = m_mapWordDoc.find(wordTF->first);
		m_mapWordDoc[wordTF->first].push_back(Doc.nID);

		wordTF++;
	}

	return true;
}

template<class TKeyWord, class TID>
bool InvertedIndex<TKeyWord, TID>::delDoc(IN TID nID)
{
	auto newDoc = m_mapDoc.find(nID);
	if (newDoc == m_mapDoc.end())
		return false;

	//! add word to word doc map
	auto wordTF = newDoc->second.nMapTF.begin();
	while (wordTF != newDoc->second.nMapTF.end())
	{
		auto newWord = m_mapWordDoc.find(wordTF->first);
		if (newWord != m_mapWordDoc.end())
		{
			auto ID = newWord->second.begin();
			while (ID != newWord->second.end())
			{
				if (*ID == nID)
				{
					newWord->second.erase(ID++);
					continue;
				}
				ID++;
			}

			if (newWord->second.size() == 0)
			{
				m_mapWordDoc.erase(newWord++);
				continue;
			}

		}

		wordTF++;
	}

	//! delete doc from doc map
	m_mapDoc.erase(newDoc);

	return true;
}

template<class TKeyWord, class TID>
bool InvertedIndex<TKeyWord, TID>::updateDoc(IN SInvDoc<TKeyWord, TID> &Doc)
{
	if (delDoc(Doc.nID) == false)
		return false;
	if (addDoc(Doc) == false)
		return false;

	return true;
}

template<class TKeyWord, class TID>
bool InvertedIndex<TKeyWord, TID>::getMatchID(IN SInvDoc<TKeyWord, TID> &Doc, IN int nTopN, IN MatchType method, OUT TID &nID)
{
	/** get top N */
	std::vector<TID> topDocID = getTopN(Doc, nTopN);
	if (topDocID.size() == 0)
		return false;

	/** match */
	float fMaxSimilar = 0, fSimilar = 0;
	for (int i = 0; i < (int)topDocID.size(); i ++)
	{
		fSimilar = match(topDocID[i], Doc, method);
		if (fSimilar > fMaxSimilar)
		{
			fMaxSimilar = fSimilar;
			nID = topDocID[i];
		}
	}

	return fMaxSimilar > getThrehold(method);
}

template<class TKeyWord, class TID>
std::vector<TID> InvertedIndex<TKeyWord, TID>::getTopN(IN SInvDoc<TKeyWord, TID> &Doc, IN int nTopN)
{
	std::vector<TID> vectResult;
	std::map<TID, int> mapIDCount;

	auto wordTF = Doc.nMapTF.begin();
	while (wordTF != Doc.nMapTF.end())
	{
		auto word = m_mapWordDoc.find(wordTF->first);
		if (word != m_mapWordDoc.end())
		{
			auto itID = word->second.begin();
			while (itID != word->second.end())
			{
				auto itIDCount = mapIDCount.find(*itID);
				if (itIDCount == mapIDCount.end())
				{
					mapIDCount[*itID] = wordTF->second;
				}
				else
				{
					itIDCount->second += wordTF->second;
				}
				itID ++;
			}
		}

		wordTF++;
	}

	std::priority_queue <SIDCount<TID> >  IDCount;
	auto itIDCount = mapIDCount.begin();
	while (itIDCount != mapIDCount.end())
	{
		if ((int32_t)IDCount.size() >= nTopN)
		{
			if (itIDCount->second > IDCount.top().nCount)
			{
				IDCount.pop();
				IDCount.push(SIDCount<TID>(itIDCount->first, itIDCount->second));
			}
		}
		else
		{
			IDCount.push(SIDCount<TID>(itIDCount->first, itIDCount->second));
		}

		itIDCount++;
	}

	while (IDCount.size())
	{
		vectResult.push_back(IDCount.top().nID);
		IDCount.pop();
	}

	return vectResult;
}

template<class TKeyWord, class TID>
float InvertedIndex<TKeyWord, TID>::match(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc, IN MatchType method)
{
	float fSimilar = 0;
	switch (method)
	{
	case MatchType::EDIT_DISTANCE:
		fSimilar = matchByEditDistance(nID, Doc);
		break;
	case MatchType::HAMMING_DISTANCE:
		fSimilar = matchByHammingDistance(nID, Doc);
		break;
	default:
		break;
	}

	return fSimilar;
}

template<class TKeyWord, class TID>
float InvertedIndex<TKeyWord, TID>::matchByEditDistance(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc)
{


	return 0;
}

template<class TKeyWord, class TID>
float InvertedIndex<TKeyWord, TID>::matchByHammingDistance(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc)
{
	auto itDoc = m_mapDoc.find(nID);
	if (itDoc == m_mapDoc.end())
		return 0;

	int nLength = 0, nEqual = 0;
	for (; nLength < (int32_t)Doc.nVecWord.size() && nLength < (int32_t)itDoc->second.nVecWord.size(); nLength++)
	{
		if (Doc.nVecWord[nLength] == itDoc->second.nVecWord[nLength])
		{
			nEqual++;
		}
	}

	return nEqual * 1.0f/ nLength;
}

template<class TKeyWord, class TID>
float InvertedIndex<TKeyWord, TID>::getThrehold(IN MatchType method)
{
	switch (method)
	{
	case MatchType::EDIT_DISTANCE:
		return THREASHOLD_EDIT_DISTANCE;
	case MatchType::HAMMING_DISTANCE:
		return THREASHOLD_HAMMING_DISTANCE;
	default:
		break;
	}

	return 0;
}




#endif