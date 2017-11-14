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

#ifndef RELTABLE_RT_H
#define RELTABLE_RT_H

#endif // RELTABLE_H
#include <string>
#include <list>
#include "CastaliaModule.h"

enum RoutingFlag {
    NOT_DEFINED = 0,
    VALID = 1,
    INVALID = 2,
    REPAIRABLE = 3,
    BEING_REPAIRED = 4,
};


class RELTable{
    //expiration of enties managed by timers
    typedef struct _ROW {
            std::string destinationAddr;
            unsigned long destinationSeqNum;
            bool isDestinationValid;
            RoutingFlag state;
            int hopCount;
            int linkCost;
            int energy; //in percentage
            std::string nextHopAddr;
            std::list<std::string>* precursor;

    }Entry;
private:
    std::list<Entry>* table;
    Entry* searchByDest(const char* destination);
    Entry* searchByDest(std::string destination);
    Entry* searchByDest(const char* destination,const char* nextHop);
    Entry* searchByDest(std::string destination,std::string nextHop);      
    Entry* getBestRoute(const char* destination,int HCTreshould);
    Entry* getBestRoute(std::string destination,int HCTreshould);
public:
    RELTable();
    ~RELTable();
    /* the function will first search an entry row by destination address in the routing
     * table. If it finds one, the function updates the entry with other parameters,
     * otherwise it adds a new entry to th int linkCost ,e table.
     */
    void updateEntry(Entry& e);
    void updateEntry(const std::string& destinationAddr,unsigned long destinationSeqNum,
                     bool isDestinationValid,RoutingFlag state,int hopCount, int linkCost ,int energy,
                     const std::string& nextHopAddr,std::list<std::string>* precursor);
    void updateEntry(const char* destinationAddr,unsigned long destinationSeqNum,
                     bool isDestinationValid,RoutingFlag state,int hopCount, int linkCost ,int energy,
                     const char* nextHopAddr,std::list<std::string>* precursor);
    void updateEntryEnergy(const char* destinationAddr,int energy);
    /* the function will first search an entry row by destination address in the routing
     * table. If it finds one, the function updates the state field in the entry,
     * otherwise it does nothing.
     */
    void deleteRoute(string destinationAddr,string nextHop);
    void updateState(const std::string& destAddr,const std::string& nextHop, RoutingFlag st);
    void updateState(const char* destAddr,const char* nextHop, RoutingFlag st);

    /* the function will first search an entry row by destination address in the routing
     * table. If it finds one, the function return the nexthop address recorded in the entry,
     * otherwise it returns NULL.
     */
    const char* getNextHop(const char* destination,int HCTreshould);

    /* the function will first search an entry row by destination address in the routing
     * table. If it finds one, the function return the destination sequence number recorded
     * in the entry, otherwise it returns 0.
     */
    unsigned long getDestSeqNum(const char* destination);

    bool isDestSeqNumValid(const char* destination);
    RoutingFlag getState(const char* destination,const char* nextHop);
    int getHopCount(const char* destination);
    const std::list<std::string>& getPrecursors(const char* destination);
    void updatePrecursor(const char* destination, const char* precursor);
    void deletePrecursor(const char* destination, const char* precursor);
    void updateSeqNum(const char* destination, unsigned long seq);

    bool isRouteValid(const char* destination);

    void reportLinkBroken(const char* neib, std::list<std::string>* affectedDest, std::list<std::string>* affectedPrecursor);
    void forwardLinkBroken(const char* src, const std::list<std::string>* affectedDest, std::list<std::string>* affectedPrecursor);
};


