void main()
{
  int x = 0;
  int y = 0;
  int i = 0;
  int j = 0;

  while(unknown1())
  {
    while(unknown2())
    {
       if(x==y)
          i++;
       else
          j++;
    }
    if(i>=j)
    {
       x++;
       y++;
    }
    else
       y++;
  }
  if(i <= j-1)
  {goto ERROR; ERROR:__VERIFIER_error();}
}