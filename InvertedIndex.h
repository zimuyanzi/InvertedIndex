/*
* FUNCTION   : Inverted Index
* AUTHOR     : zimuyanzi
* DATE       : 2017-03-30
*
*/

#ifndef __INVERTED_H__
#define __INVERTED_H__

#include <map>
#include <unordered_map>
#include <list>
#include <queue>

#define IN
#define OUT

#define THREASHOLD_EDIT_DISTANCE             0.3f
#define THREASHOLD_HAMMING_DISTANCE          0.5f

typedef enum _MatchType
{
	EDIT_DISTANCE = 0,
	HAMMING_DISTANCE,
}MatchType;

template<class TKeyWord, class TID>
struct SInvDoc
{
	TID                                           nID;                 /** DOC ID   */
	std::vector<TKeyWord>                         nVecWord;            /** DOC WORD */
	std::unordered_map<TKeyWord, int32_t>         nMapTF;              /** Term Frequency */

	SInvDoc(TID ID, int32_t nSize, std::vector<TKeyWord> &vecWord);
};

template<class TID>
struct SIDCount
{
	TID nID;
	int32_t nCount;

	SIDCount(TID ID, int32_t count) : nID(ID), nCount(count) {}
};

template<class TKeyWord, class TID>
class InvertedIndex
{
public:
	bool addDoc(IN SInvDoc<TKeyWord, TID> &Doc);
	bool delDoc(IN TID nID);
	bool updateDoc(IN SInvDoc<TKeyWord, TID> &Doc);
	bool getMatchID(IN SInvDoc<TKeyWord, TID> &Doc, IN int nTopN, IN MatchType method, OUT TID &nID);

private:
	std::vector<TID> getTopN(IN SInvDoc<TKeyWord, TID> &Doc, IN int nTopN);
	float match(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc, IN MatchType method);
	float matchByEditDistance(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc);
	float matchByHammingDistance(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc);

	float getThrehold(IN MatchType method);
private:
	std::unordered_map<TID, SInvDoc<TKeyWord, TID> >           m_mapDoc;
	std::unordered_map<TKeyWord, std::list<TID> >              m_mapWordDoc;
};




#endif