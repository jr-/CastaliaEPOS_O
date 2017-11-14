/***********************************************************************************************
 *  Copyright (c) Federal University of Para, brazil - 2012                                    *
 *  Developed at the Research Group on Computer Network and Multimedia Communication (GERCOM)  *
 *  All rights reserved                                                                        *
 *                                                                                             *
 *  Permission to use, copy, modify, and distribute this protocol and its documentation for    *
 *  any purpose, without fee, and without written agreement is hereby granted, provided that   *
 *  the above copyright notice, and the author appear in all copies of this protocol.          *
 *                                                                                             *
 *  Author(s): M. Butt et al.                                                                  *
 *  Reference Paper: link quality-based lexical routing metric for reactive routing protocols  *
 *  in ieee 802.15. 4 networks                                                                 *
 *  Implementarion: Thiago Fernandes <thiago.oliveira@itec.ufpa.br> <thiagofdso.ufpa@gmail.com>* 
 **********************************************************************************************/

#include "LabileTable_rt.h"

using namespace std;

LabileTable::LabileTable(){
    table = new list<Entry>();
}

LabileTable::~LabileTable(){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry &e = *i;
        if ((e.precursor)!=NULL){
            delete (e.precursor);
        }

    }
    delete table;
}

LabileTable::Entry* LabileTable::searchByDest(std::string destination){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destination){
            return &ec;
        }
    }
    return NULL;
}
LabileTable::Entry* LabileTable::searchByDest(const char* destination){
    string s(destination);
    return LabileTable::searchByDest(s);
}

void LabileTable::updateEntry(Entry& e,int HCTreshold){
    Entry* ec = LabileTable::searchByDest(e.destinationAddr);
    if(ec && (!(ec->state==VALID)||(ec->destinationSeqNum < e.destinationSeqNum)||
    		((ec->destinationSeqNum = e.destinationSeqNum)))
         &&((ec->hopCount - e.hopCount)<HCTreshold)&&           //LABILE
	((ec->weakLinks > e.weakLinks)||(ec->weakLinks == e.weakLinks && ec->hopCount > e.hopCount))){
            ec->destinationSeqNum = e.destinationSeqNum;
            ec->hopCount = e.hopCount;
	    ec->weakLinks = e.weakLinks;
            ec->isDestinationValid = e.isDestinationValid;
            ec->nextHopAddr = e.nextHopAddr;
            ec->precursor->merge(*(e.precursor));
            ec->state = e.state;
      } else if(!ec){
        table->push_back(e);
    }

}

void LabileTable::updateEntry(const string& destinationAddr,unsigned long destinationSeqNum,
                               bool isDestinationValid,RoutingFlag state,int hopCount,int weakLinks,
                               const char *nextHopAddr,list<string>* precursor,int HCTreshold){
    Entry e;
    e.destinationAddr = destinationAddr;
    e.destinationSeqNum = destinationSeqNum;
    e.hopCount = hopCount;
    e.weakLinks = weakLinks;
    e.isDestinationValid = isDestinationValid;
    string nextHop(nextHopAddr);
    e.nextHopAddr = nextHop;
    e.precursor = new list<string>();
    if(precursor != NULL){
        e.precursor->merge(*precursor);
    }
    e.state = state;
    LabileTable::updateEntry(e,HCTreshold);
}

void LabileTable::updateEntry(const char *destinationAddr, unsigned long destinationSeqNum,
                               bool isDestinationValid, RoutingFlag state, int hopCount, int weakLinks,
                               const char *nextHopAddr, std::list<std::string> *precursor,int HCTreshold){
    string destAddr(destinationAddr);
    //string nextHop(nextHopAddr);
    LabileTable::updateEntry(destAddr,destinationSeqNum,isDestinationValid,state,hopCount,weakLinks,nextHopAddr,precursor,HCTreshold);
}

void LabileTable::updateState(const string& destAddr,RoutingFlag st){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destAddr){
            ec.state = st;
            break;
        }
    }
}

void LabileTable::updateState(const char* destAddr, RoutingFlag st){
    LabileTable::updateState(string(destAddr), st);
}

const char* LabileTable::getNextHop(const char* destination){
    Entry *e = LabileTable::searchByDest(destination);
    if(e /*&& e->state==VALID*/){
        return e->nextHopAddr.c_str();
    }
    else return NULL;
}

unsigned long LabileTable::getDestSeqNum(const char* destination){
    Entry *e = LabileTable::searchByDest(destination);
    if(e){
        return e->destinationSeqNum;
    }
    else return 0;
}

bool LabileTable::isDestSeqNumValid(const char* destination){
    Entry *e = LabileTable::searchByDest(destination);
    if(e){
        return e->isDestinationValid;
    }
    else return false;
}

RoutingFlag LabileTable::getState(const char* destination){
    Entry *e = LabileTable::searchByDest(destination);
    if(e){
        return e->state;
    }
    else return NOT_DEFINED;
}

int LabileTable::getHopCount(const char* destination){
    Entry *e = LabileTable::searchByDest(destination);
    if(e){
        return e->hopCount;
    }
    else return 0;
}

const std::list<string>& LabileTable::getPrecursors(const char* destination){
    Entry *e = LabileTable::searchByDest(destination);
    if(e){
        return *(e->precursor);
    }
    else return *(new list<string>());
}

void LabileTable::updatePrecursor(const char* destination, const char* precursor){
    string destAddr = string(destination);
    string pre = string(precursor);
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destAddr){
            for(list<string>::iterator j = ec.precursor->begin();j!=ec.precursor->end();++j){
                string curr = *j;
                if(pre==curr) return;
            }
            ec.precursor->push_back(pre);
            break;
        }
    }
}

void LabileTable::deletePrecursor(const char* destination, const char* precursor){
    string destAddr = string(destination);
    string pre = string(precursor);
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destAddr){
            ec.precursor->remove(pre);
            break;
        }
    }
}

bool LabileTable::isRouteValid(const char* destination){
    string destAddr = string(destination);
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destAddr){
            if(ec.state==VALID){
                return true;
            }
        }
    }
    return false;
}

void LabileTable::reportLinkBroken(const char* neib, list<string>* affectedDest, list<string>* affectedPrecursor){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.nextHopAddr.compare(neib)==0 && ec.state==VALID){
            ec.state=INVALID;
        	ec.destinationSeqNum++;
            affectedPrecursor->insert(affectedPrecursor->begin(),ec.precursor->begin(),ec.precursor->end());
            affectedDest->push_front(ec.destinationAddr);
        }
    }
    affectedDest->unique();
    affectedPrecursor->unique();
}

void LabileTable::forwardLinkBroken(const char* src, const list<string>* affectedDest, std::list<std::string>* affectedPrecursor){
    for(list<string>::const_iterator i=affectedDest->begin();i!=affectedDest->end();++i){
        string s = *i;
        Entry* e = searchByDest(s);
        if(e && e->state==VALID && e->nextHopAddr.compare(src)==0){
            e->state=INVALID;
            affectedPrecursor->insert(affectedPrecursor->begin(),e->precursor->begin(),e->precursor->end());
        }
    }
    affectedPrecursor->unique();
}

void LabileTable::updateSeqNum(const char* destination, unsigned long seq){
	Entry *e = LabileTable::searchByDest(destination);
	if(e && e->destinationSeqNum < seq){
	    e->destinationSeqNum = seq;
	}
}




