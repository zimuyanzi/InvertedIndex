#include "InvertedIndex.h"


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

template<class TKeyWord, class TID>
bool InvertedIndex<TKeyWord, TID>::addDoc(SInvDoc<TKeyWord, TID> &Doc)
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
bool InvertedIndex<TKeyWord, TID>::delDoc(TID nID)
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
bool InvertedIndex<TKeyWord, TID>::updateDoc(SInvDoc<TKeyWord, TID> &Doc)
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
	float fMaxSimilar = 0;
	TID nMostFimilarID;
	for (int i = 0; i < topDocID.size(); i ++)
	{

	}

	return true;
}

template<class TKeyWord, class TID>
std::vector<TID> InvertedIndex<TKeyWord, TID>::getTopN(SInvDoc<TKeyWord, TID> &Doc, int nTopN)
{
	std::vector<TID> vectResult;

	return vectResult;
}

template<class TKeyWord, class TID>
float match(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc, IN MatchType method)
{
	float fSimilar = 0;
	switch (method)
	{
	case MatchType::EDIT_DISTANCE:
		fSimilar = matchByEditDistance(nID, Doc);
		break;
	default:
		break;
	}

	return fSimilar;
}

template<class TKeyWord, class TID>
float matchByEditDistance(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc)
{
	return 0;
}