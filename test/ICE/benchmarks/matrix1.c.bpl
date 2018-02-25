// SMACK-PRELUDE-BEGIN

function {:existential true} b0(m:int, aval:int, j:int): bool;
//function {:existential true} b1(m:int, aval:int, j:int, k:int): bool;


procedure boogie_si_record_int(i: int);

// Integer arithmetic
function $add(p1:int, p2:int) returns (int) {p1 + p2}
function $sub(p1:int, p2:int) returns (int) {p1 - p2}
function $mul(p1:int, p2:int) returns (int) {p1 * p2}
function $sdiv(p1:int, p2:int) returns (int);
function $udiv(p1:int, p2:int) returns (int);
function $srem(p1:int, p2:int) returns (int);
function $urem(p1:int, p2:int) returns (int);
function $and(p1:int, p2:int) returns (int);
axiom $and(0,0) == 0;
axiom $and(0,1) == 0;
axiom $and(1,0) == 0;
axiom $and(1,1) == 1;
function $or(p1:int, p2:int) returns (int);
axiom $or(0,0) == 0;
axiom $or(0,1) == 1;
axiom $or(1,0) == 1;
axiom $or(1,1) == 1;
function $xor(p1:int, p2:int) returns (int);
axiom $xor(0,0) == 0;
axiom $xor(0,1) == 1;
axiom $xor(1,0) == 1;
axiom $xor(1,1) == 0;
function $lshr(p1:int, p2:int) returns (int);
function $ashr(p1:int, p2:int) returns (int);
function $shl(p1:int, p2:int) returns (int);
function $ult(p1:int, p2:int) returns (bool) {p1 < p2}
function $ugt(p1:int, p2:int) returns (bool) {p1 > p2}
function $ule(p1:int, p2:int) returns (bool) {p1 <= p2}
function $uge(p1:int, p2:int) returns (bool) {p1 >= p2}
function $slt(p1:int, p2:int) returns (bool) {p1 < p2}
function $sgt(p1:int, p2:int) returns (bool) {p1 > p2}
function $sle(p1:int, p2:int) returns (bool) {p1 <= p2}
function $sge(p1:int, p2:int) returns (bool) {p1 >= p2}
function $nand(p1:int, p2:int) returns (int);
function $max(p1:int, p2:int) returns (int);
function $min(p1:int, p2:int) returns (int);
function $umax(p1:int, p2:int) returns (int);
function $umin(p1:int, p2:int) returns (int);
function $i2b(i: int) returns (bool);
axiom (forall i:int :: $i2b(i) <==> i != 0);
axiom $i2b(0) == false;
function $b2i(b: bool) returns (int);
axiom $b2i(true) == 1;
axiom $b2i(false) == 0;

// Floating point
type float;
function $fp(a:int) returns (float);
const $ffalse: float;
const $ftrue: float;
function $fadd(f1:float, f2:float) returns (float);
function $fsub(f1:float, f2:float) returns (float);
function $fmul(f1:float, f2:float) returns (float);
function $fdiv(f1:float, f2:float) returns (float);
function $frem(f1:float, f2:float) returns (float);
function $foeq(f1:float, f2:float) returns (bool);
function $foge(f1:float, f2:float) returns (bool);
function $fogt(f1:float, f2:float) returns (bool);
function $fole(f1:float, f2:float) returns (bool);
function $folt(f1:float, f2:float) returns (bool);
function $fone(f1:float, f2:float) returns (bool);
function $ford(f1:float, f2:float) returns (bool);
function $fueq(f1:float, f2:float) returns (bool);
function $fuge(f1:float, f2:float) returns (bool);
function $fugt(f1:float, f2:float) returns (bool);
function $fule(f1:float, f2:float) returns (bool);
function $fult(f1:float, f2:float) returns (bool);
function $fune(f1:float, f2:float) returns (bool);
function $funo(f1:float, f2:float) returns (bool);
function $fp2si(f:float) returns (int);
function $fp2ui(f:float) returns (int);
function $si2fp(i:int) returns (float);
function $ui2fp(i:int) returns (float);

// Memory region declarations: 1
var $M.0: [int] int;

// SMACK Flat Memory Model

function $ptr(obj:int, off:int) returns (int) {obj + off}
function $size(int) returns (int);
function $obj(int) returns (int);
function $off(ptr:int) returns (int) {ptr}

var alloc: [int] bool;
var $CurrAddr:int;

const unique $NULL: int;
axiom $NULL == 0;
const $UNDEF: int;

function $pa(pointer: int, index: int, size: int) returns (int);
function $trunc(p: int) returns (int);
function $p2i(p: int) returns (int);
function $i2p(p: int) returns (int);
function $p2b(p: int) returns (bool);
function $b2p(b: bool) returns (int);

axiom (forall p:int, i:int, s:int :: {$pa(p,i,s)} $pa(p,i,s) == p + i * s);
axiom (forall p:int :: $trunc(p) == p);

axiom $b2p(true) == 1;
axiom $b2p(false) == 0;
axiom (forall i:int :: $p2b(i) <==> i != 0);
axiom $p2b(0) == false;
axiom (forall i:int :: $p2i(i) == i);
axiom (forall i:int :: $i2p(i) == i);
procedure __SMACK_nondet() returns (p: int);
procedure __SMACK_nondetInt() returns (p: int);

procedure $malloc(obj_size: int) returns (new: int);
modifies $CurrAddr, alloc;
requires obj_size > 0;
ensures 0 < old($CurrAddr);
ensures new == old($CurrAddr);
ensures $CurrAddr > old($CurrAddr) + obj_size;
ensures $size(new) == obj_size;
ensures (forall x:int :: new <= x && x < new + obj_size ==> $obj(x) == new);
ensures alloc[new];
ensures (forall x:int :: {alloc[x]} x == new || old(alloc)[x] == alloc[x]);

procedure $free(pointer: int);
modifies alloc;
requires alloc[pointer];
requires $obj(pointer) == pointer;
ensures !alloc[pointer];
ensures (forall x:int :: {alloc[x]} x == pointer || old(alloc)[x] == alloc[x]);

procedure $alloca(obj_size: int) returns (new: int);
modifies $CurrAddr, alloc;
requires obj_size > 0;
ensures 0 < old($CurrAddr);
ensures new == old($CurrAddr);
ensures $CurrAddr > old($CurrAddr) + obj_size;
ensures $size(new) == obj_size;
ensures (forall x:int :: new <= x && x < new + obj_size ==> $obj(x) == new);
ensures alloc[new];
ensures (forall x:int :: {alloc[x]} x == new || old(alloc)[x] == alloc[x]);

// SMACK-PRELUDE-END
// BEGIN SMACK-GENERATED CODE
const unique __VERIFIER_assert: int;
axiom (__VERIFIER_assert == -1024);
const unique main: int;
axiom (main == -2048);
const unique llvm.stacksave: int;
axiom (llvm.stacksave == -3072);
const unique __VERIFIER_nondet_int: int;
axiom (__VERIFIER_nondet_int == -4096);
const unique llvm.stackrestore: int;
axiom (llvm.stackrestore == -5120);

procedure __VERIFIER_assert(cond: int) 
  modifies $M.0, alloc, $CurrAddr;
{
  var $b: bool;
$bb0:
  assume {:sourceloc "matrix_inv1.c", 1, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 2, 0} true;
  $b := (cond != 0);
  assume {:sourceloc "matrix_inv1.c", 2, 0} true;
  goto $bb3, $bb4;
$bb1:
  assume {:sourceloc "matrix_inv1.c", 5, 0} true;
  return;
$bb2:
  assume {:sourceloc "matrix_inv1.c", 2, 0} true;
  goto $bb5;
$bb3:
  assume {:sourceloc "matrix_inv1.c", 2, 0} true;
  assume $b;
  goto $bb1;
$bb4:
  assume {:sourceloc "matrix_inv1.c", 2, 0} true;
  assume !($b);
  goto $bb2;
$bb5:
  assume {:sourceloc "matrix_inv1.c", 3, 0} true;
  goto $bb5;
}

procedure main() 
  returns ($r: int) 
  modifies $M.0, alloc, $CurrAddr;
{
  var $p: int;
  var $p1: int;
  var $p2: int;
  var $p3: int;
  var $p4: int;
  var $p5: int;
  var $b: bool;
  var $p6: int;
  var $p7: int;
  var $p8: int;
  var $p9: int;
  var $b10: bool;
  var $p11: int;
  var $p12: int;
  var $p13: int;
  var $b14: bool;
  var $p15: int;
  var $p16: int;
  var $p17: int;
  var $p18: int;
  var $p19: int;
  var $p20: int;
  var $p21: int;
  var $p22: int;
  var $p23: int;
  var $b24: bool;
  var $p25: int;
  var $p26: int;
  var $p27: int;
  var $p28: int;
  var $p29: int;
  var $p30: int;
$bb0:
  assume {:sourceloc "matrix_inv1.c", 11, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 12, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 14, 0} true;
  call $p := llvm.stacksave();
  assume {:sourceloc "matrix_inv1.c", 14, 0} true;
  $p1 := $mul(1, 1);
  assume {:sourceloc "matrix_inv1.c", 14, 0} true;
  call $p2 := $alloca($mul(4, $p1));
  assume {:sourceloc "matrix_inv1.c", 14, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 16, 0} true;
  havoc $p3;
  assume {:sourceloc "matrix_inv1.c", 16, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  $p4 := 0;
  $p5 := $p3;
  goto $bb1;
$bb1:
  assert b0($p5,$M.0[$p2],$p4);
  

  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  $b := $ult($p4, 1);
  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  goto $bb4, $bb5;
$bb2:
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  $p12 := 0;
  $p13 := $p5;
  goto $bb6;
$bb3:
  assume {:sourceloc "matrix_inv1.c", 28, 0} true;
  $p6 := $mul(0, 1);
  assume {:sourceloc "matrix_inv1.c", 28, 0} true;
  $p7 := $pa($p2, $p6, 4);
  assume {:sourceloc "matrix_inv1.c", 28, 0} true;
  $p8 := $pa($p7, 0, 4);
  assume {:sourceloc "matrix_inv1.c", 28, 0} true;
  $p9 := $M.0[$p8];
  assume {:sourceloc "matrix_inv1.c", 28, 0} true;
  $b10 := $sle($p9, $p5);
  assume {:sourceloc "matrix_inv1.c", 28, 0} true;
  $p11 := $b2p($b10);
  assume {:sourceloc "matrix_inv1.c", 28, 0} true;
  assert $p11 == 1;
  assume {:sourceloc "matrix_inv1.c", 29, 0} true;
  call llvm.stackrestore($p);
  assume {:sourceloc "matrix_inv1.c", 29, 0} true;
  $r := 0;
  return;
$bb4:
  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  assume $b;
  goto $bb2;
$bb5:
  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  assume !($b);
  goto $bb3;
$bb6:
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  assert ($p13 >= $M.0[$p2] || ($p4 == 0 && $p12 == 0));

  $b14 := $ult($p12, 1);
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  goto $bb9, $bb10;
$bb7:
  assume {:sourceloc "matrix_inv1.c", 22, 0} true;
  havoc $p16;
  assume {:sourceloc "matrix_inv1.c", 22, 0} true;
  $p17 := $mul($p4, 1);
  assume {:sourceloc "matrix_inv1.c", 22, 0} true;
  $p18 := $pa($p2, $p17, 4);
  assume {:sourceloc "matrix_inv1.c", 22, 0} true;
  $p19 := $pa($p18, $p12, 4);
  assume {:sourceloc "matrix_inv1.c", 22, 0} true;
  $M.0[$p19] := $p16;
  assume {:sourceloc "matrix_inv1.c", 24, 0} true;
  $p20 := $mul($p4, 1);
  assume {:sourceloc "matrix_inv1.c", 24, 0} true;
  $p21 := $pa($p2, $p20, 4);
  assume {:sourceloc "matrix_inv1.c", 24, 0} true;
  $p22 := $pa($p21, $p12, 4);
  assume {:sourceloc "matrix_inv1.c", 24, 0} true;
  $p23 := $M.0[$p22];
  assume {:sourceloc "matrix_inv1.c", 24, 0} true;
  $b24 := $sge($p23, $p13);
  assume {:sourceloc "matrix_inv1.c", 24, 0} true;
  $p25 := $p13;
  goto $bb14, $bb15;
$bb8:
  assume {:sourceloc "matrix_inv1.c", 26, 0} true;
  goto $bb11;
$bb9:
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  assume $b14;
  goto $bb7;
$bb10:
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  assume !($b14);
  goto $bb8;
$bb11:
  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  $p15 := $add($p4, 1);
  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 17, 0} true;
  $p4 := $p15;
  $p5 := $p13;
  goto $bb1;
$bb12:
  assume {:sourceloc "matrix_inv1.c", 25, 0} true;
  $p27 := $mul($p4, 1);
  assume {:sourceloc "matrix_inv1.c", 25, 0} true;
  $p28 := $pa($p2, $p27, 4);
  assume {:sourceloc "matrix_inv1.c", 25, 0} true;
  $p29 := $pa($p28, $p12, 4);
  assume {:sourceloc "matrix_inv1.c", 25, 0} true;
  $p30 := $M.0[$p29];
  assume {:sourceloc "matrix_inv1.c", 25, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 25, 0} true;
  $p25 := $p30;
  goto $bb13;
$bb13:
  assume {:sourceloc "matrix_inv1.c", 26, 0} true;
  goto $bb16;
$bb14:
  assume {:sourceloc "matrix_inv1.c", 24, 0} true;
  assume $b24;
  goto $bb12;
$bb15:
  assume {:sourceloc "matrix_inv1.c", 24, 0} true;
  assume !($b24);
  goto $bb13;
$bb16:
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  $p26 := $add($p12, 1);
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  // WARNING: ignoring llvm.debug call.
  assume true;
  assume {:sourceloc "matrix_inv1.c", 19, 0} true;
  $p12 := $p26;
  $p13 := $p25;
  goto $bb6;
}

procedure llvm.stacksave() 
  returns ($r: int) ;
  modifies $M.0, alloc, $CurrAddr;


procedure __VERIFIER_nondet_int() 
  returns ($r: int) ;
  modifies $M.0, alloc, $CurrAddr;


procedure llvm.stackrestore(p0: int) ;
  modifies $M.0, alloc, $CurrAddr;

// END SMACK-GENERATED CODE
