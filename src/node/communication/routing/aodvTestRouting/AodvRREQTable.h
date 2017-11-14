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

#ifndef AODVRREQTABLE_H_
#define AODVRREQTABLE_H_

#include "VirtualRouting.h"
#include "AodvTestRoutingControlPacket_m.h"
#include "AodvTestRoutingDataPacket_m.h"

#include <list>
#include <map>
#include <queue>
#include <math.h>
#include <limits>
#include "CastaliaModule.h"


#endif /* AODVRREQTABLE_H_ */



struct RREQset
{
    string dest;
    int rreqID;
    double lifetime;
};


class AodvRREQTable
{
    private:
        std::list<RREQset>* rreqTable;//the set of RREQ processed by the node
    public:

        AodvRREQTable();
        ~AodvRREQTable();

        RREQset* searchByDest(std::string dest);
        RREQset* searchByID(int id);
        void insertRREQ(std::string dest,double lifetime, int id);
        void removeRREQ(std::string dest);
        void removeRREQ(int id);
        bool isRREQExpired(std::string dest);
        RREQset* getNextExpiredRREQ();
        void clearRREQExpired();
        int getTableSize();
};
