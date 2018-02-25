function {:existential true} b0(m:int, j:int, aval:int, v1: int, v2: int, v3: int): bool;



procedure main()
{
  var size: int;
  var j, k: int;
  var a: [int]int;
  var menor: int;
  var t: int;
  var v1, v2, v3: int;

  //size := 5; 
  j := 0;
  while (j < size)
  invariant b0(menor, j, a[0], v1, v2, v3);
  {
    havoc t;
    a[j] := t;
    if(a[j] <= menor)
    {
      menor := a[j];
    }
    j := j + 1;
    havoc v1;
    havoc v2;
    havoc v3;
  }
  assert size <= 0 || a[0] >= menor;
}

