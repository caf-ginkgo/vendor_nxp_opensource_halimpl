/******************************************************************************
 *
 *  Copyright 2018-2020 NXP
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <log/log.h>

#include "NxpNfc.h"
#include "eSEClientExtns.h"
#include "eSEClientIntf.h"
#include "phNxpNciHal.h"
#include "phNxpNciHal_Adaptation.h"
#include "phNxpNciHal_IoctlOperations.h"

extern bool nfc_debug_enabled;

namespace vendor {
namespace nxp {
namespace nxpnfc {
namespace V2_0 {
namespace implementation {

Return<void>
NxpNfc::getVendorParam(const ::android::hardware::hidl_string &key,
                          getVendorParam_cb _hidl_cb) {
  string val = phNxpNciHal_getSystemProperty(key);
  _hidl_cb(val);
  return Void();
}

Return<bool>
NxpNfc::setVendorParam(const ::android::hardware::hidl_string &key,
                          const ::android::hardware::hidl_string &value) {
  return phNxpNciHal_setSystemProperty(key, value);
}

Return<bool>
NxpNfc::resetEse(uint64_t resetType) {
  NFCSTATUS status = NFCSTATUS_FAILED;
  bool ret = false;
  ALOGD("NxpNfc::resetEse Entry");
  if((uint64_t)Constants::HAL_NFC_ESE_HARD_RESET == resetType) {
    status = phNxpNciHal_resetEse();
    if(NFCSTATUS_SUCCESS == status) {
      ret = true;
      status = NFCSTATUS_SUCCESS;
      ALOGD("HAL_NFC_ESE_HARD_RESET completed");
      } else {
        ALOGD("HAL_NFC_ESE_HARD_RESET failed");
      }
    } else {
      ALOGD("reset called with %lu type",resetType);
    }
  ALOGD("NxpNfc::resetEse Exit");
  return ret;
}

Return<bool>
NxpNfc::setEseUpdateState(NxpNfcHalEseState eSEState) {
  int ret = -1;
  bool status = false;

  ALOGD("NxpNfc::setEseUpdateState Entry");
  ret = phNxpNciHal_nfcTriggerSavedCb((uint8_t)NxpNfcEvents::HAL_NFC_HCI_RESET);
  if(ret == 0){
    status = true;
  }

  if(eSEState == NxpNfcHalEseState::HAL_NFC_ESE_JCOP_UPDATE_COMPLETED
  || eSEState == NxpNfcHalEseState::HAL_NFC_ESE_LS_UPDATE_COMPLETED)
  {
    ALOGD("NxpNfc::setEseUpdateState state == HAL_NFC_ESE_JCOP_UPDATE_COMPLETED");
    seteSEClientState((uint8_t)eSEState);
    eSEClientUpdate_NFC_Thread();
  }
  if (eSEState == NxpNfcHalEseState::HAL_NFC_ESE_UPDATE_COMPLETED) {
        phNxpNciHal_nfcTriggerSavedCb((uint8_t)NxpNfcEvents::HAL_NFC_RESTART);
  }
  ALOGD("NxpNfc::setEseUpdateState Exit");
  return status;
}

Return<bool>
NxpNfc::setNxpTransitConfig(const ::android::hardware::hidl_string &strval)
{
  bool status = true;
  ALOGD("NxpNfc::setNxpTransitConfig Entry");

  status = phNxpNciHal_setNxpTransitConfig((char *)strval.c_str());

  ALOGD("NxpNfc::setNxpTransitConfig Exit");
  return status;
}

Return<bool>
NxpNfc::isJcopUpdateRequired()
{
    bool status = 0;
    ALOGD("NxpNfc::isJcopUpdateRequired Entry");

    status = getJcopUpdateRequired();

    ALOGD("NxpNfc::isJcopUpdateRequired Exit");
    return status;

}

Return<bool>
NxpNfc::isLsUpdateRequired()
{
    bool status = 0;
    ALOGD("NxpNfc::isLsUpdateRequired Entry");

    status = getLsUpdateRequired();

    ALOGD("NxpNfc::isLsUpdateRequired Exit");
    return status;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace nxpnfc
}  // namespace nxp
}  // namespace vendor