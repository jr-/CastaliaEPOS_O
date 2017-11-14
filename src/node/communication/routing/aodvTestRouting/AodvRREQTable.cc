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


#include "AodvRREQTable.h"

using namespace std;

AodvRREQTable::AodvRREQTable()
{
    rreqTable = new list<RREQset>();//table is empty at the beginning
}

AodvRREQTable::~AodvRREQTable()
{
    delete rreqTable;
}


RREQset* AodvRREQTable::searchByDest(string destination)
{
    //return the first RREQ in the list
	  if(destination=="")
	           return NULL;
    for(list<RREQset>::iterator i=rreqTable->begin();i!=rreqTable->end();++i)
    {
        RREQset &p = *i;
        if(p.dest.compare(destination)==0)
        {
                return &p;
        }
    }
    return NULL;
}

RREQset* AodvRREQTable::searchByID(int id)
{
    //return the first RREQ in the list

    for(list<RREQset>::iterator i=rreqTable->begin();i!=rreqTable->end();++i)
    {
        RREQset &p = *i;
        if(p.rreqID==id)
        {
                return &p;
        }
    }
    return NULL;
}

void AodvRREQTable::removeRREQ(string destination)
{
    //when a RREP is received we can remove all the RREQ for that destination
    for(list<RREQset>::iterator i=rreqTable->begin();i!=rreqTable->end();)
        {
            RREQset &r = *i;
            if(r.dest.compare(destination)==0)
            {
                //delete &r;
                i=rreqTable->erase(i);
            }
            else
                i++;
        }
}

void AodvRREQTable::removeRREQ(int id)
{
    //when a RREP is received we can remove all the RREQ for that destination
    for(list<RREQset>::iterator i=rreqTable->begin();i!=rreqTable->end();)
        {
            RREQset &r = *i;
            if(r.rreqID==id)
            {
                //delete &r;
                i=rreqTable->erase(i);
            }
            else
                i++;
        }
}

void AodvRREQTable::insertRREQ(std::string dest, double lifetime, int id)
{
    if(searchByDest(dest))
        return;
    RREQset newRREQ;
    newRREQ.dest = dest;
    newRREQ.lifetime = lifetime;
    newRREQ.rreqID = id;
    rreqTable->push_back(newRREQ);
}


bool AodvRREQTable::isRREQExpired(string destination)
{
    RREQset *p = searchByDest(destination);
    //a rreq is expired if the rreq doesn't exist or the time is too short
    if(p && (p->lifetime - simTime().dbl())<=0)
            return true;
    else
        return false;
}



void AodvRREQTable::clearRREQExpired()
{
    for(list<RREQset>::iterator i=rreqTable->begin();i!=rreqTable->end();)
    {
        RREQset &r = *i;
        if(isRREQExpired(r.dest))
        {
            //delete &r;
            i=rreqTable->erase(i);
        }
        else
            i++;
    }
}

RREQset* AodvRREQTable::getNextExpiredRREQ()
{
    double minLifetime=numeric_limits<double>::max( );
    string dst="";
    for(list<RREQset>::iterator i=rreqTable->begin();i!=rreqTable->end();++i)
    {
        RREQset& r = *i;
        if(r.lifetime < minLifetime)
        {
                minLifetime = r.lifetime;
                dst=r.dest;
        }
    }
    return this->searchByDest(dst);
}



int AodvRREQTable::getTableSize()
{
    return rreqTable->size();
}

