/*
* FUNCTION   : inverted index
* AUTHOR     : zimuyanzi
* DATE       : 2017-03-30
*
*/

#ifndef __INVERTED_H__
#define __INVERTED_H__

#include <map>
#include <unordered_map>
#include <list>

#define IN
#define OUT

typedef enum _MatchType
{
	EDIT_DISTANCE = 0,
}MatchType;

template<class TKeyWord, class TID>
struct SInvDoc
{
	TID                                           nID;                 /** DOC ID   */
	std::vector<TKeyWord>                         nVecWord;            /** DOC WORD */
	std::unordered_map<TKeyWord, int32_t>              nMapTF;              /** Term Frequency */

	SInvDoc(TID ID, int32_t nSize, std::vector<TKeyWord> &vecWord);
};


template<class TKeyWord, class TID>
class InvertedIndex
{
public:
	bool addDoc(SInvDoc<TKeyWord, TID> &Doc);
	bool delDoc(TID nID);
	bool updateDoc(SInvDoc<TKeyWord, TID> &Doc);
	bool getMatchID(IN SInvDoc<TKeyWord, TID> &Doc, IN int nTopN, IN MatchType method, OUT TID &nID);

private:
	std::vector<TID> getTopN(SInvDoc<TKeyWord, TID> &Doc, int nTopN);
	float match(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc, IN MatchType method);
	float matchByEditDistance(IN TID nID, IN SInvDoc<TKeyWord, TID> &Doc);

private:
	std::unordered_map<TID, SInvDoc<TKeyWord, TID> >           m_mapDoc;
	std::unordered_map<TKeyWord, std::list<TID> >              m_mapWordDoc;
};




#endif