  //Output clock .
  PORT->Group[0].PINCFG[PIN_PA14].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[PIN_PA14/2].reg &= 0xF0;
  PORT->Group[0].PMUX[PIN_PA14/2].reg |= 0x07;
  
     PORT->Group[0].PINCFG[PIN_PA14].reg &=~ (uint8_t)(PORT_PINCFG_INEN) ;
     PORT->Group[0].DIRSET.reg |= (uint32_t)(1<<14) ;
