 //*
 //* @Author: Mathieu MICHEL (http://w3.umons.ac.be/staff/Michel.Mathieu/)
 //*
 //* Copyright (C) 2013  Mathieu MICHEL
 //* This program is free software; you can redistribute it and/or modify
 //* it under the terms of the GNU General Public License as published by
 //* the Free Software Foundation; either version 2 of the License, or
 //* (at your option) any later version.
 //* 
 //* This program is distributed in the hope that it will be useful,
 //* but WITHOUT ANY WARRANTY; without even the implied warranty of
 //* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 //* GNU General Public License for more details.
 //*
 //* You should have received a copy of the GNU General Public License
 //* along with this program; if not, write to the Free Software
 //* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 //*/



cplusplus 
{{
#include "RoutingPacket_m.h"
}}

class RoutingPacket;

packet PacketRREQ extends RoutingPacket 
{
	bool flagJ;
	bool flagR;
	bool flagG;
	bool flagD;
	bool flagU;
	int hopCount;
	int rreqID;
	string dstIP;
	unsigned long dstSN;
	string srcIP;
	unsigned long srcSN;
	double lifetime;
}

packet PacketRREP extends RoutingPacket
{
	bool flagR;
	bool flagA;
	int prefixSz;
	int hopCount;
	string dstIP;
	unsigned long dstSN;
	string originIP;
	double lifetime;
	
	int rreqID;
}

packet PacketHELLO extends RoutingPacket
{
	double lifetime;
}

packet PacketRREPack extends RoutingPacket
{
	double lifetime;
}

packet PacketRERR extends RoutingPacket 
{
	bool flagN;
	int destCount;
	double lifetime;
	string unreachableDstIP[];
	unsigned long unreachableDstSN[];
}








