#pragma once

#include <stdint.h>
#include "knx_types.h"
#include "npdu.h"
#include "transport_layer.h"
#include "network_layer_entity.h"
#include "network_layer.h"

class DeviceObject;

class NetworkLayerDevice : public NetworkLayer
{
    friend class NetworkLayerEntity;

  public:
    NetworkLayerDevice(DeviceObject& deviceObj, TransportLayer& layer);

    virtual NetworkLayerEntity& getEntity(uint8_t interfaceIndex) override;

    // from transport layer
    virtual void dataIndividualRequest(AckType ack, uint16_t destination, HopCountType hopType, Priority priority, TPDU& tpdu) override;
    virtual void dataGroupRequest(AckType ack, uint16_t destination, HopCountType hopType, Priority priority, TPDU& tpdu) override;
    virtual void dataBroadcastRequest(AckType ack, HopCountType hopType, Priority priority, TPDU& tpdu) override;
    virtual void dataSystemBroadcastRequest(AckType ack, HopCountType hopType, Priority priority, TPDU& tpdu) override;

  private:
    // from entities
    virtual void dataIndication(AckType ack, AddressType addType, uint16_t destination, FrameFormat format, NPDU& npdu,
                        Priority priority, uint16_t source) override;
    virtual void dataConfirm(AckType ack, AddressType addressType, uint16_t destination, FrameFormat format, Priority priority,
                     uint16_t source, NPDU& npdu, bool status) override;
    virtual void broadcastIndication(AckType ack, FrameFormat format, NPDU& npdu,
                             Priority priority, uint16_t source) override;
    virtual void broadcastConfirm(AckType ack, FrameFormat format, Priority priority, uint16_t source, NPDU& npdu, bool status) override;
    virtual void systemBroadcastIndication(AckType ack, FrameFormat format, NPDU& npdu,
                                   Priority priority, uint16_t source) override;
    virtual void systemBroadcastConfirm(AckType ack, FrameFormat format, Priority priority, uint16_t source, NPDU& npdu, bool status) override;

    // Support only physical interface for normal devices
    NetworkLayerEntity _netLayerEntities[1];
};