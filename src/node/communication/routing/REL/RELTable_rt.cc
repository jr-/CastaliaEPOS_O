/***********************************************************************************************
 *  Copyright (c) Federal University of Para, brazil - 2012                                    *
 *  Developed at the Research Group on Computer Network and Multimedia Communication (GERCOM)  *
 *  All rights reserved                                                                        *
 *                                                                                             *
 *  Permission to use, copy, modify, and distribute this protocol and its documentation for    *
 *  any purpose, without fee, and without written agreement is hereby granted, provided that   *
 *  the above copyright notice, and the author appear in all copies of this protocol.          *
 *                                                                                             *
 *  Author(s): Kassio Machado <kassio.comp@gmail.com> ,                                        *
 *             Thiago Oliveira <thiagofdso.ufpa@gmail.com>                                     *
 *  Reference Paper: Design of a routing protocol using remaining energy and link quality indicator (REL)
 **********************************************************************************************/

#include "RELTable_rt.h"

using namespace std;

RELTable::RELTable(){
    table = new list<Entry>();
}

RELTable::~RELTable(){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry &e = *i;
        if ((e.precursor)!=NULL){
            delete (e.precursor);
        }

    }
    delete table;
}

RELTable::Entry* RELTable::searchByDest(std::string destination){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destination){
            return &ec;
        }
    }
    return NULL;
}
RELTable::Entry* RELTable::searchByDest(const char* destination){
    string s(destination);
    return RELTable::searchByDest(s);
}
RELTable::Entry* RELTable::searchByDest(const char* destination,const char* nextHop){
    string s(destination);
    string s2(nextHop);
    return RELTable::searchByDest(s,s2);
}
RELTable::Entry* RELTable::searchByDest(std::string destination,std::string nextHop){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destination&&ec.nextHopAddr == nextHop){
            return &ec;
        }
    }
    return NULL;
}
void RELTable::updateEntry(Entry& e){
    Entry* ec = RELTable::searchByDest(e.destinationAddr,e.nextHopAddr);
    //if route exists update else insert the new route - multipath -
    if(ec){
            ec->destinationSeqNum = e.destinationSeqNum;
            ec->hopCount = e.hopCount;
	    ec->linkCost = e.linkCost;
            ec->isDestinationValid = e.isDestinationValid;
            ec->nextHopAddr = e.nextHopAddr;
            ec->precursor->merge(*(e.precursor));
            ec->state = e.state;
	    ec->energy = e.energy;
      } else if(!ec){
        table->push_back(e);
    }

}

void RELTable::updateEntry(const string& destinationAddr,unsigned long destinationSeqNum,
                               bool isDestinationValid,RoutingFlag state,int hopCount,int linkCost,int energy,
                               const string& nextHopAddr,list<string>* precursor){
    Entry e;
    e.destinationAddr = destinationAddr;
    e.destinationSeqNum = destinationSeqNum;
    e.hopCount = hopCount;
    e.linkCost = linkCost;
    e.isDestinationValid = isDestinationValid;
    e.nextHopAddr = nextHopAddr;
    e.precursor = new list<string>();
    e.energy = energy;
    if(precursor != NULL){
        e.precursor->merge(*precursor);
    }
    e.state = state;
    RELTable::updateEntry(e);
}

void RELTable::updateEntry(const char *destinationAddr, unsigned long destinationSeqNum,
                               bool isDestinationValid, RoutingFlag state, int hopCount, int linkCost,int energy,
                               const char *nextHopAddr, std::list<std::string> *precursor){
    string destAddr(destinationAddr);
    string nextHop(nextHopAddr);
    RELTable::updateEntry(destAddr,destinationSeqNum,isDestinationValid,state,hopCount,linkCost,energy,nextHop,precursor);
}

void RELTable::updateEntryEnergy(const char*destinationAddr,int energy){
    Entry* ec = RELTable::searchByDest(destinationAddr);
    if(ec && ec->energy > energy){
      ec->energy = energy;
    }
      
}

void RELTable::updateState(const string& destAddr,const std::string& nextHop,RoutingFlag st){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destAddr&&ec.nextHopAddr==nextHop){
            ec.state = st;
            break;
        }
    }
}


void RELTable::updateState(const char* destAddr,const char* nextHop, RoutingFlag st){
    RELTable::updateState(string(destAddr),string(nextHop), st);
}

const char* RELTable::getNextHop(const char* destination,int HCTreshould){
  Entry *e = RELTable::getBestRoute(destination,HCTreshould);
//    Entry *e = RELTable::searchByDest(destination);
    if(e /*&& e->state==VALID*/){
        return e->nextHopAddr.c_str();
    }
    else return NULL;

}
RELTable::Entry* RELTable::getBestRoute(const char* destination,int HCTreshould){
    string s(destination);
    return RELTable::getBestRoute(s,HCTreshould);
}
//novos
RELTable::Entry* RELTable::getBestRoute(std::string destination,int HCTreshould){
	list<Entry>::iterator best;
	bool primeira = true;
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
	
        if(ec.destinationAddr == destination)
	{	
		if(primeira){
			best = i;
			primeira = false;
		}
		else
            		if(((ec.linkCost/(ec.hopCount*8) < best->linkCost/(best->hopCount*8)) ||((ec.linkCost/(ec.hopCount*8) == best->linkCost/(best->hopCount*8))&& 
			   (ec.energy > best->energy)) || ((ec.linkCost/(ec.hopCount*8) == best->linkCost/(best->hopCount*8))&& 
			   (ec.energy == best->energy)&& ( ec.hopCount < best->hopCount)&&
			       best->hopCount-ec.hopCount)) &&((best->hopCount-ec.hopCount)<HCTreshould ))
		           best=i;
        }
    }
    if(primeira)
      return NULL;
      else{
	Entry& s = *best;
	return &s;
      }

}
unsigned long RELTable::getDestSeqNum(const char* destination){
    Entry *e = RELTable::searchByDest(destination);
    if(e){
        return e->destinationSeqNum;
    }
    else return 0;
}

bool RELTable::isDestSeqNumValid(const char* destination){
    Entry *e = RELTable::searchByDest(destination);
    if(e){
        return e->isDestinationValid;
    }
    else return false;
}
void RELTable::deleteRoute(string destinationAddr,string nextHop){
  list<Entry>::iterator i;
  for(i=table->begin();i!=table->end();++i){
    Entry& ec = *i;
        if(ec.destinationAddr==destinationAddr&&ec.nextHopAddr == nextHop){
	  table->erase(i);
	  break;
	}
  }
  
  
}
RoutingFlag RELTable::getState(const char* destination,const char* nextHop){
    for(list<Entry>::iterator i=table->begin();i!=table->end();++i){
        Entry& ec = *i;
        if(ec.destinationAddr==destination&&ec.nextHopAddr==nextHop){
            return ec.state;
        }
    }
    return NOT_DEFINED;
}
int RELTable::getHopCount(const char* destination){
    Entry *e = RELTable::searchByDest(destination);
    if(e){
        return e->hopCount;
    }
    else return 0;
}

const std::list<string>& RELTable::getPrecursors(const char* destination){
    Entry *e = RELTable::searchByDest(destination);
    if(e){
        return *(e->precursor);
    }
    else return *(new list<string>());
}

void RELTable::updatePrecursor(const char* destination, const char* precursor){
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

void RELTable::deletePrecursor(const char* destination, const char* precursor){
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

bool RELTable::isRouteValid(const char* destination){
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

void RELTable::reportLinkBroken(const char* neib, list<string>* affectedDest, list<string>* affectedPrecursor){
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

void RELTable::forwardLinkBroken(const char* src, const list<string>* affectedDest, std::list<std::string>* affectedPrecursor){
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

void RELTable::updateSeqNum(const char* destination, unsigned long seq){
	Entry *e = RELTable::searchByDest(destination);
	if(e && e->destinationSeqNum < seq){
	    e->destinationSeqNum = seq;
	}
}




