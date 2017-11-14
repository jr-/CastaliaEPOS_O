//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package node.application.geoSync;

simple GeoSync like node.application.iApplication {
 parameters:
 	string applicationID = default ("geoSync"); 	
	bool collectTraceInfo = default (false);
	int priority = default (1);
	string nextRecipient = default ("0"); // Destination for data packets sent by this node. 
	int packetHeaderOverhead = default (0);
	int constantDataPayload = default (0);
	
	double startupDelay = default (0s) @unit(s); // delay before the app starts producing packets
	double startupRandomDelay = default (0s) @unit(s); // random delay before the app starts producing packets
	
	double packetTimeLimit = default (0s) @unit(s); 
	int dataPacketLimit = default (0);
	int hecopsPacketLimit = default (0);
	int ptpSyncPacketLimit = default (0);
	
	int dataPacketLength = default (4);
	int ptpSyncPacketLength = default (8 + 4);
    int ptpDelayReqPacketLength = default (8 + 4);
    int ptpDelayRespPacketLength = default (8 + 4);
    int hecopsPacketLength = default (8 + 4);
	
	double ptpPeriod @unit(s);
	double hecopsPeriod @unit(s);
	double dataPeriod @unit(s);
	
	double dataPacketGenerationChance = default (1.0);
	
	bool ptpMaster = default (false);	
	
	string LISHAIoTSeriesName = default("010000002805382c2b2d85289c630000");
	
gates:
 	output toCommunicationModule;
	output toSensorDeviceManager;
	input fromCommunicationModule;
	input fromSensorDeviceManager;
	input fromResourceManager;
}
