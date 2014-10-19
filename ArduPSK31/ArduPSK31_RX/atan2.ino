
// supposede to give -pi to +pi values.
// instead in 2pii/16 parts deg
//  1 =   22.5,  3 =   87.5,  5 =  112.5,  7 =  157.5 
// -1 =  -22.5, -3 =  -87.5, -5 = -112.5, -7 = -157.5 

int8_t _atan2(int8_t I, int8_t Q)  
{
  if (I>0)
  {
    if (Q>0)
    {
      if (Q>I)   return  1;
      else       return  3;
    }
    else // Q < 0
    {
      if (I>-Q)  return  5;
      else       return  7;
    }
  }
  else // I < 0
  {
    if (Q>0)
    {
      if (Q>-I)  return -1;
      else       return -3;
    }
    else // Q < 0
    {
      if (-I>-Q) return -5;
      else       return -7;
    }
  }
}
  

