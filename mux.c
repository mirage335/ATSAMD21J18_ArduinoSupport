  //Output clock .
  PORT->Group[0].PINCFG[PIN_PA14].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[PIN_PA14/2].reg &= 0xF0;
  PORT->Group[0].PMUX[PIN_PA14/2].reg |= 0x07;