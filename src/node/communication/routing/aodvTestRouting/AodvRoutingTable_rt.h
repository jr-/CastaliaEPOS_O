/*
 * @Author: Mathieu MICHEL (http://w3.umons.ac.be/staff/Michel.Mathieu/)
 *
 * Copyright (C) 2013  Mathieu MICHEL
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef ROUTINGTABLE_H_
#define ROUTINGTABLE_H_
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <limits>
#include "CastaliaModule.h"


enum RoutingFlag
{
    UNKNOWN = 0,
    VALID = 1,
    INVALID = 2,
    REPAIRABLE = 3,
    BEING_REPAIRED = 4,
};

typedef struct
{
        std::string dstIP;
        unsigned long dstSN;
        bool state;//Valid Destination Sequence Number flag
        RoutingFlag flag;
        int hopCount;
        std::string nextHopAddr;
        std::list<std::string>* precursor;
}Route;

struct RouteTimer
{
    double lifetime;
    string destination;
    bool canceled;
};

struct RouteTimerCompare
{
    bool operator()(const RouteTimer &a,const RouteTimer &b)
    {
        return a.lifetime > b.lifetime ;
    }
};

class AodvRoutingTable
{
private:
        std::list<Route>* table;
        priority_queue <RouteTimer,vector<RouteTimer>,RouteTimerCompare > timers;

public:

        AodvRoutingTable();
        ~AodvRoutingTable();

        Route* searchByDest(std::string destination);
        //if the route exists it will be updated, else a new route is inserted
        void insertRoute(const std::string& dstIP,unsigned long dstSN,bool state,RoutingFlag flag,int hopCount,const std::string& nextHopAddr,std::list<std::string>* precursor, double lifetime);
        void removeRoute(std::string destination);

        unsigned long getDstSN(std::string destination);
        //the method first checks that the newSN is larger than the current SN
        void setDstSN(std::string destination, unsigned long newSN);

        double getLifetime(std::string destination);
        void setLifetime(RouteTimer* timer);

        std::string getNextHop(std::string destination);
        void setNextHop(std::string destination, string newNextHop);

        RoutingFlag getFlag(std::string destination);
        void setFlag(std::string destination, RoutingFlag newFlag);

        int getHopCount(std::string destination);
        void setHopCount(std::string destination, int newCount);

        //return the validty of the DstSN
        bool getState(std::string destination);
        void setState(std::string destination, bool valid);

        bool isRouteValid(std::string destination);
        bool isPartRouteValid();

        const RouteTimer* getNextExpiredRoute();
        void clearTimerExpired();
        int getTimersSize();
        void resetTimer(string dest);

        const std::list<std::string>& getPrecursors(std::string destination);
        void addPrecursor(std::string destination, std::string precursor);
        void deletePrecursor(std::string destination, std::string precursor);

        //this method will update the affectedDst and affectedPre list
        void setLinkFailure(const char* node, std::list<std::string>* affectedDst, std::list<std::string>* affectedPre);
        //this method update the affectedPre list
        void forwardLinkFailure(const char* node, const std::list<std::string>* affectedDst, std::list<std::string>* affectedPre);
};

#endif /* ROUTINGTABLE_H_ */
