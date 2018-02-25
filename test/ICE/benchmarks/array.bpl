function {:existential true} b0(m:int, j:int, aval:int): bool;



procedure main()
{
  var size: int;
  var j, k: int;
  var a: [int]int;
  var menor: int;
  var t: int;

  size := 5; 
  j := 0;
  while (j < size)
  invariant b0(menor, j, a[0]);
  {
    havoc t;
    a[j] := t;
    if(a[j] <= menor)
    {
      menor := a[j];
    }
    j := j + 1;
  }
  assert a[0] >= menor;
}

