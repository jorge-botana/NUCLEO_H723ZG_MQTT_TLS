// ------------------------------------------------------------------------------
// ETHERNETIF
//
// - Archivo de cabecera.
//
// ------------------------------------------------------------------------------
// Copyright (c) 2017 STMicroelectronics.
// All rights reserved.
//
// This software component is licensed by ST under BSD 3-Clause license, the
// "License"; You may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
//                                           opensource.org/licenses/BSD-3-Clause
// ------------------------------------------------------------------------------

#ifndef   ETHERNETIF_H
#define   ETHERNETIF_H

#include  HAL_DRIVER

extern ETH_HandleTypeDef heth;

int ETH_Link_Status(void);

#endif // ETHERNETIF_H
