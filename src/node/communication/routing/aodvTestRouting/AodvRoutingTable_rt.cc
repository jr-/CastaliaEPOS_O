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

#include "AodvRoutingTable_rt.h"

using namespace std;

AodvRoutingTable::AodvRoutingTable()
{
    table = new list<Route>();//table is empty at the beginning
}

AodvRoutingTable::~AodvRoutingTable()
{
    for(list<Route>::iterator i=table->begin();i!=table->end();++i)
    {
        Route &r = *i;
        if ((r.precursor)!=NULL)
        {
            delete (r.precursor);
        }
    }
    delete table;
}

Route* AodvRoutingTable::searchByDest(string destination)
{
    if(destination=="")
           return NULL;
    for(list<Route>::iterator i=table->begin();i!=table->end();++i)
    {
        Route &r = *i;
        if(r.dstIP.compare(destination)==0)
        {
                return &r;
        }
    }
    return NULL;
}
void AodvRoutingTable::insertRoute(const std::string& dstIP,unsigned long dstSN,bool state,RoutingFlag flag,int hopCount,const std::string& nextHopAddr,std::list<std::string>* precursor, double lifetime)
{
    Route* r = searchByDest(dstIP);
    if(r) // a route for that dst exists already
    {
            //the new route SN is higher or the same but with a lower hopcount
            if(r->flag!=VALID || r->dstSN < dstSN || (r->dstSN == dstSN && r->hopCount > (hopCount + 1)))
            {
                r->dstSN = dstSN;
                r->hopCount = hopCount;
                r->flag = flag;
                r->nextHopAddr = nextHopAddr;
                /*if(r->precursor==NULL)
                    r->precursor=new list<string>();
                if(precursor!=NULL)
                    r->precursor->merge(*precursor);*/
                r->state = state;
            }
        else //the route is not valid anymore
        {
            if(r->flag == BEING_REPAIRED)
            {

            }
        }
    }
    else //no route for that dst exists already
    {
        Route nr;
        nr.dstSN = dstSN;
        nr.hopCount = hopCount;
        nr.flag = flag;
        nr.nextHopAddr = nextHopAddr;
        nr.precursor = new list<string>();
        /*if(precursor!=NULL)
            nr.precursor->merge(*precursor);*/
        nr.state = state;
        nr.dstIP = dstIP;
        //route.lifetime = active_route_timeout
        table->push_back(nr);
    }
}

void AodvRoutingTable::removeRoute(string destination)
{
    for(list<Route>::iterator i=table->begin();i!=table->end();)
    {
        Route &r = *i;
        if(r.dstIP.compare(destination)==0)
        {
            //delete &r;
            i=table->erase(i);
        }
        else
            i++;
    }
}

string AodvRoutingTable::getNextHop(string destination)
{
    Route* r = searchByDest(destination);
    if(r)
    {
        return r->nextHopAddr;
    }
    else
        return "";
}

void AodvRoutingTable::setNextHop(string destination, string newNextHop)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        r->nextHopAddr = newNextHop;
    }
}

unsigned long AodvRoutingTable::getDstSN(string destination)
{
    Route* r = searchByDest(destination);
    if(r)
    {
        return r->dstSN;
    }
    else
        return 0;
}

void AodvRoutingTable::setDstSN(string destination, unsigned long newSN)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        if(r->dstSN < newSN)
            r->dstSN = newSN;
    }
}

double AodvRoutingTable::getLifetime(string destination)
{
    RouteTimer* timer;
    double x=0;
    if(!timers.empty())
    {
        timer = (RouteTimer*)&(timers.top());
        for(unsigned int i = 0; i < timers.size(); i++)
        {
				if(!timer[i].canceled && timer[i].destination.compare(destination)==0 && timer[i].lifetime>x)
				{
					x = timer[i].lifetime;
				}
        }
    }
    return x;
}

void AodvRoutingTable::setLifetime(RouteTimer* timer)
{
    Route *r = searchByDest(timer->destination);
    if(r)
    {
        timers.push(*timer);
    }
}

RoutingFlag AodvRoutingTable::getFlag(string destination)
{
    Route* r = searchByDest(destination);
    if(r)
    {
        return r->flag;
    }
    else
        return UNKNOWN;
}

void AodvRoutingTable::setFlag(string destination, RoutingFlag newFlag)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        r->flag = newFlag;
    }
}

int AodvRoutingTable::getHopCount(string destination)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        return r->hopCount;
    }
    else
        return 0;
}

void AodvRoutingTable::setHopCount(string destination, int newCount)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        r->hopCount = newCount;
    }
}

bool AodvRoutingTable::getState(string destination)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        return r->state;
    }
    else
        return false;
}

void AodvRoutingTable::setState(string destination, bool valid)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        r->state = valid;
    }
}

bool AodvRoutingTable::isRouteValid(string destination)
{
    Route *r = searchByDest(destination);
    if(r && r->flag==VALID)
            return true;
    else
        return false;
}

//check if the node is part of a valid route
bool AodvRoutingTable::isPartRouteValid()
{
    for(list<Route>::iterator i=table->begin();i!=table->end();++i)
    {
        Route &r = *i;
        if(isRouteValid(r.dstIP) && r.precursor->size()!=0)
        {
                return true;
        }
    }
    return false;
}

void AodvRoutingTable::clearTimerExpired()
{
   timers.pop();
}
void AodvRoutingTable::resetTimer(string dest)
{
    RouteTimer* timer;
    if(!timers.empty())
    {
        timer = (RouteTimer*)&(timers.top());
        for(unsigned int i = 0; i < timers.size(); i++)
        {
            if(timer[i].destination.compare(dest)==0)
            {
                timer[i].canceled = true;
                return;
            }
        }
    }

}

int AodvRoutingTable::getTimersSize()
{
    return timers.size();
}

const RouteTimer* AodvRoutingTable::getNextExpiredRoute()
{
   return &(timers.top());
}

const std::list<string>& AodvRoutingTable::getPrecursors(string destination)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        return *(r->precursor);
    }
    else
        return *(new list<string>());
}

void AodvRoutingTable::addPrecursor(string destination, string precursor)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        if(r->precursor==NULL)
            r->precursor=new list<string>();

        for(list<string>::iterator i = r->precursor->begin();i!=r->precursor->end();++i)
        {
            string temp = *i;
            if(temp==precursor)
                    return;
        }
        r->precursor->push_back(precursor);
    }
}

void AodvRoutingTable::deletePrecursor(string destination, string precursor)
{
    Route *r = searchByDest(destination);
    if(r)
    {
        r->precursor->remove(precursor);
    }
}

void AodvRoutingTable::setLinkFailure(const char* node, list<string>* affectedDst, list<string>* affectedPre)
{
    //we look for all the routes which have node has next-hop
    for(list<Route>::iterator i=table->begin();i!=table->end();++i)
    {
        Route &r = *i;
        if(r.nextHopAddr.compare(string(node)) == 0 && r.flag==VALID)
        {
            r.flag=INVALID;
            r.dstSN++;
            affectedPre->insert(affectedPre->begin(),r.precursor->begin(),r.precursor->end());
            affectedDst->push_front(r.dstIP);
        }
    }
    affectedDst->unique();
    affectedPre->unique();
}

void AodvRoutingTable::forwardLinkFailure(const char* node, const list<string>* affectedDst, list<string>* affectedPre)
{
    //we look for all the routes which have node has next-hop
    for(list<string>::const_iterator i=affectedDst->begin();i!=affectedDst->end();++i)
    {
        Route *r = searchByDest(*i);
        if(r && (r->nextHopAddr.compare(string(node))) == 0 && r->flag==VALID)
        {
            r->flag=INVALID;
            affectedPre->insert(affectedPre->begin(),r->precursor->begin(),r->precursor->end());
        }
    }
    affectedPre->unique();
}
