/**************************************************************************/
/*!
    @file     BLEClientHIDReportCharacteristic.cpp
    @author   Eric Boucher

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2018, Eric Boucher
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#include "bluefruit.h"
#include "BLEClientHIDReportCharacteristic.h"

BLEClientHIDReportCharacteristic::BLEClientHIDReportCharacteristic(void)
  : BLEClientCharacteristic(UUID16_CHR_REPORT)
{
}

/**
 * Destructor
 */
BLEClientHIDReportCharacteristic::~BLEClientHIDReportCharacteristic()
{
}

void BLEClientHIDReportCharacteristic::_readReportRef()
{
  if(_report_ref_handle != BLE_GATT_HANDLE_INVALID && _report_type==0)
  {
    uint8_t buffer[2] = {0};
    _read(_report_ref_handle, buffer, 2);
    _report_id = buffer[0];
    _report_type= buffer[1];
    //delay(10);
  }
}

bool BLEClientHIDReportCharacteristic::_acceptHandle(uint16_t handle)
{
  if(handle == BLE_GATT_HANDLE_INVALID) return false;
  if(handle == _report_ref_handle) return true;
  return BLEClientCharacteristic::_acceptHandle(handle);
}

void BLEClientHIDReportCharacteristic::_processDescriptor(ble_gattc_desc_t* pDesc)
{
  if ((_report_ref_handle==BLE_GATT_HANDLE_INVALID) && 
      (pDesc->uuid.type == BLE_UUID_TYPE_BLE) &&
      (pDesc->uuid.uuid == UUID16_REPORT_REF_DESCR))
  {
    LOG_LV2("DISC", "Found Report Ref: handle = %d", pDesc->handle);
    _report_ref_handle = pDesc->handle;
  }
  else
  {
    BLEClientCharacteristic::_processDescriptor(pDesc);
  }
}

