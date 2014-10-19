int mysinlen = 1*16;
int myamplen = 4*16;

void setup () 
{  
  // sintables
  {
    float ang,res;
    int i,r;
    println();
    println(); print("   ");
    for (i=0;i<mysinlen;i++)
    {
      if ((i%16) == 0) {  println(); print("   ");};
      ang = i * (2.0*3.141592 / (float)mysinlen);
      res = sin(ang);
      r = (int)(res * 125.0); // 125 orig, 112 lower
      print(r + ", ");
    }
    println();
    println();
    for (i=0;i<myamplen;i++)
    {
      if ((i%16) == 0) {  println(); print("   ");};
      ang = i * (3.141592 / (float)myamplen);
      res = cos(ang);
      r = (int)(res * 125.0); // 125 orig, 112 lower
      print(r + ", ");
    }
    println();
  }
}



