/* Copyright 2023 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#include <stddef.h>
#include <string.h>

#include "driver/eeprom.h"
#include "driver/i2c.h"
#include "driver/system.h"
uint8_t WRITE_SIZE=0;
void EEPROM_ReadBuffer(uint32_t Address, void *pBuffer, uint8_t Size) {
    if(Size==0)return;

        I2C_Start();
      uint8_t  P0 = (Address / 0x10000) << 1;

        I2C_Write(0xA0|P0);

        I2C_Write((Address >> 8) & 0xFF);
        I2C_Write((Address >> 0) & 0xFF);

        I2C_Start();

        I2C_Write((0xA0|P0)+1);

        I2C_ReadBuffer(pBuffer, Size);

        I2C_Stop();

}

void EEPROM_WriteBuffer(uint32_t Address, const void *pBuffer,uint8_t WRITE_SIZE) {
    if (pBuffer == NULL )
        return;
    uint8_t buffer[128];
    EEPROM_ReadBuffer(Address, buffer, WRITE_SIZE);
    if (memcmp(pBuffer, buffer, WRITE_SIZE) != 0) {
        uint8_t P0 = (Address / 0x10000) << 1;
        I2C_Start();
        if(Address<0x10000)  I2C_Write(0xA0);
        else         I2C_Write(0xA0 | P0);

        I2C_Write((Address >> 8) & 0xFF);
        I2C_Write((Address >> 0) & 0xFF);
        I2C_WriteBuffer(pBuffer, WRITE_SIZE);
        I2C_Stop();
    }
    SYSTEM_DelayMs(11);

}


