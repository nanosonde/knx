#pragma once

#include <stdint.h>
#include "knx_types.h"
#include "npdu.h"
#include "transport_layer.h"
#include "network_layer_entity.h"
#include "network_layer.h"

class DeviceObject;
class RouterObject;

class NetworkLayerCoupler : public NetworkLayer
{
    friend class NetworkLayerEntity;

  public:
    NetworkLayerCoupler(DeviceObject& deviceObj, TransportLayer& layer);

    virtual NetworkLayerEntity& getEntity(uint8_t interfaceIndex) override;

    void rtObjPrimary(RouterObject& rtObjPrimary);
    void rtObjSecondary(RouterObject& rtObjSecondary);

    // from transport layer
    virtual void dataIndividualRequest(AckType ack, uint16_t destination, HopCountType hopType, Priority priority, TPDU& tpdu) override;
    virtual void dataGroupRequest(AckType ack, uint16_t destination, HopCountType hopType, Priority priority, TPDU& tpdu) override;
    virtual void dataBroadcastRequest(AckType ack, HopCountType hopType, Priority priority, TPDU& tpdu) override;
    virtual void dataSystemBroadcastRequest(AckType ack, HopCountType hopType, Priority priority, TPDU& tpdu) override;

  private:
    enum CouplerType
    {
        LineCoupler,
        BackboneCoupler,
        TP1Bridge,
        TP1Repeater
    };

    // from entities
    virtual void dataIndication(AckType ack, AddressType addType, uint16_t destination, FrameFormat format, NPDU& npdu,
                        Priority priority, uint16_t source) override;
    virtual void dataConfirm(AckType ack, AddressType addrType, uint16_t destination, FrameFormat format, Priority priority,
                     uint16_t source, NPDU& npdu, bool status) override;
    virtual void broadcastIndication(AckType ack, FrameFormat format, NPDU& npdu,
                             Priority priority, uint16_t source) override;
    virtual void broadcastConfirm(AckType ack, FrameFormat format, Priority priority, uint16_t source, NPDU& npdu, bool status) override;
    virtual void systemBroadcastIndication(AckType ack, FrameFormat format, NPDU& npdu,
                                   Priority priority, uint16_t source) override;
    virtual void systemBroadcastConfirm(AckType ack, FrameFormat format, Priority priority, uint16_t source, NPDU& npdu, bool status) override;

    void routeMsgHopCount(AckType ack, AddressType addrType, uint16_t destination, FrameFormat format, NPDU& npdu, Priority priority,
                      SystemBroadcast broadcastType, uint8_t sourceInterfaceIndex);

    // Support a maximum of two physical interfaces for couplers
    NetworkLayerEntity _netLayerEntities[2];

    RouterObject* _rtObjPrimary {nullptr};
    RouterObject* _rtObjSecondary {nullptr};

    CouplerType _couplerType;
};

/*
 * C  hop count value contained in the N-protocol header
 * D  low order octet of the Destination Address, i.e. Device Address part
 * G  Group Address
 * SD low nibble of high order octet plus low order octet, i.e. Line Address + Device Address
 * Z  high nibble of high order octet of the Destination Address, i.e. Area Address
 * ZS high order octet of the Destination Address, i.e. hierarchy information part: Area Address + Line Address
*/
