
int8_t _atan2(int8_t I, int8_t Q)
{
  int8_t r;
  if (I>0)
  {
    if (Q>0)
    {
      if (Q>I)   return  1;
      else       return  2;
    }
    else // Q < 0
    {
      if (I>-Q)  return  3;
      else       return  4;
    }
  }
  else // I < 0
  {
    if (Q>0)
    {
      if (Q>-I)  return  8;
      else       return  7;
    }
    else // Q < 0
    {
      if (-I>-Q) return  6;
      else       return  5;
    }
  }
}
  

