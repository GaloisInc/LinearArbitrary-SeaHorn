/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*		Definitions used in C5.0				 */
/*              ------------------------				 */
/*									 */
/*************************************************************************/


#define	 RELEASE	"2.07 GPL Edition"

				/*  Uncomment following line to enable
				    sample estimates for large datasets.
				    This can lead to some variablility,
				    especially when used with SMP  */
//#define	SAMPLE_ESTIMATES

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>

#include "text.i"

// Pranav
#include "cpp_wrapper/cmap.h"


/*************************************************************************/
/*									 */
/*		Definitions dependent on cc options			 */
/*									 */
/*************************************************************************/


#ifdef	VerbOpt
#define Goodbye(x)		{Cleanup(); exit(x);}
#else
#define Goodbye(x)		exit(x)
#endif

#ifdef	VerbOpt
#include <assert.h>
#define	Verbosity(d,s)		if(VERBOSITY >= d) {s;}
#else
#define	 assert(x)
#define Verbosity(d,s)
#endif


/*  Alternative random number generator  */
/* Pranav: Redefine AltRandom
#define AltRandom		drand48()
#define	AltSeed(x)		srand48(x)
*/
#define AltRandom		(double)((rand()%RAND_MAX)/RAND_MAX)

#define Free(x)			{free(x); x=0;}


/*************************************************************************/
/*									 */
/*		Constants, macros etc.					 */
/*									 */
/*************************************************************************/

#define	 THEORYFRAC	0.23	/* discount rate for estimated coding cost */

#define	 Nil	   0		/* null pointer */
#define	 false	   0
#define	 true	   1
#define	 None	   -1
#define NegInfinity -1E38 
#define	 Epsilon   1E-4
#define	 MinLeaf   0.05		/* minimum weight for non-null leaf */
#define	 Width	   80		/* approx max width of output */

#define  EXCLUDE   1		/* special attribute status: do not use */
#define  SKIP	   2		/* do not use in classifiers */
#define  DISCRETE  4		/* ditto: collect values as data read */
#define  ORDERED   8		/* ditto: ordered discrete values */
#define  DATEVAL   16		/* ditto: YYYY/MM/DD or YYYY-MM-DD */
#define  STIMEVAL  32		/* ditto: HH:MM:SS */
#define	 TSTMPVAL  64		/* date time */

#define	 CMINFO	   1		/* generate confusion matrix */
#define	 USAGEINFO 2		/* print usage information */

				/* unknown and N/A values are represented by
				   unlikely floating-point numbers
				   (octal 01600000000 and 01) */
#define	 UNKNOWN   01600000000	/* 1.5777218104420236e-30 */
#define	 NA	   01		/* 1.4012984643248171e-45 */

#define	 BrDiscr   1
#define	 BrThresh  2
#define	 BrSubset  3

#define  Plural(n)		((n) != 1 ? "s" : "")

#define  AllocZero(N,T)		(T *) Pcalloc(N, sizeof(T))
#define  Alloc(N,T)		AllocZero(N,T) /* for safety */
#define  Realloc(V,N,T)		V = (T *) Prealloc(V, (N)*sizeof(T))

#define	 Max(a,b)               ((a)>(b) ? (a) : (b))
#define	 Min(a,b)               ((a)<(b) ? (a) : (b))

#define	 Log2			0.69314718055994530942
#define	 Log(x)			((x) <= 0 ? 0.0 : log((double)x) / Log2)

#define	 Bit(b)			(1 << (b))
#define	 In(b,s)		((s[(b) >> 3]) & Bit((b) & 07))
#define	 ClearBits(n,s)		memset(s,0,n)
#define	 CopyBits(n,f,t)	memcpy(t,f,n)
#define	 SetBit(b,s)		(s[(b) >> 3] |= Bit((b) & 07))
#define	 ResetBit(b,s)		(s[(b) >> 3] ^= Bit((b) & 07))

#define	 ForEach(v,f,l)		for(v=f ; v<=l ; ++v)

#define	 CountCases(f,l)	(UnitWeights ? (l-(f)+1.0) : SumWeights(f,l))

#define	 StatBit(a,b)		(SpecialStatus[a]&(b))
#define	 Exclude(a)		StatBit(a,EXCLUDE)
#define	 Skip(a)		StatBit(a,EXCLUDE|SKIP)
#define  Discrete(a)		(MaxAttVal[a] || StatBit(a,DISCRETE))
#define  Continuous(a)		(! MaxAttVal[a] && ! StatBit(a,DISCRETE))
#define	 Ordered(a)		StatBit(a,ORDERED)
#define	 DateVal(a)		StatBit(a,DATEVAL)
#define	 TimeVal(a)		StatBit(a,STIMEVAL)
#define	 TStampVal(a)		StatBit(a,TSTMPVAL)

#define  FreeUnlessNil(p)	if((p)!=Nil) Free(p)

#define  CheckClose(f)		if(f) {fclose(f); f=Nil;}

#define	 Int(x)			((int)(x+0.5))

#define  Space(s)	(s == ' ' || s == '\n' || s == '\r' || s == '\t')
#define  SkipComment	while ( ( c = InChar(f) ) != '\n' && c != EOF )

#define	 P1(x)		(rint((x)*10) / 10)

#define	 No(f,l)	((l)-(f)+1)

#define	 EmptyNA(T)	(T->Branch[1]->Cases < 0.01)

#define  Before(n1,n2)  (n1->Tested < n2->Tested ||\
			n1->Tested == n2->Tested && n1->Cut < n2->Cut)

#define	 SwapWithImplications(a,b)	{DataRec xab;\
			                assert(a >= 0 && a <= MaxCase &&\
			                b >= 0 && b <= MaxCase);\
			                xab = Case[a]; Case[a] = Case[b]; Case[b] = xab;\
                                        }

#define	 Swap(a,b)	{assert (false);}

#define	 NOFILE		 0
#define	 BADCLASSTHRESH	 1
#define	 LEQCLASSTHRESH	 2
#define	 BADATTNAME	 3
#define	 EOFINATT	 4
#define	 SINGLEATTVAL	 5
#define	 BADATTVAL	 6
#define	 BADNUMBER	 7
#define	 BADCLASS	 8
#define	 BADCOSTCLASS	 9
#define	 BADCOST	10
#define	 NOMEM		11
#define	 TOOMANYVALS	12
#define	 BADDISCRETE	13
#define	 NOTARGET	14
#define	 BADCTARGET	15
#define	 BADDTARGET	16
#define	 LONGNAME	17
#define	 HITEOF		18
#define	 MISSNAME	19
#define	 BADDATE	20
#define	 BADTIME	21
#define	 BADTSTMP	22
#define	 DUPATTNAME	23
#define	 UNKNOWNATT	24
#define	 BADDEF1	25
#define	 BADDEF2	26
#define	 BADDEF3	27
#define	 BADDEF4	28
#define	 SAMEATT	29
#define	 MODELFILE	30
#define	 CWTATTERR	31

#define	 READDATA	 1
#define	 WINNOWATTS	 2
#define	 FORMTREE	 3
#define	 SIMPLIFYTREE	 4
#define	 FORMRULES	 5
#define	 SIFTRULES	 6
#define	 EVALTRAIN	 7
#define	 READTEST	 8
#define	 EVALTEST	 9
#define	 CLEANUP	10
#define	 ALLOCTABLES	11
#define	 RESULTS	12
#define	 READXDATA	13


/*************************************************************************/
/*									 */
/*		Type definitions					 */
/*									 */
/*************************************************************************/


typedef  unsigned char	Boolean, BranchType, *Set, Byte;
typedef	 char		*String;

typedef  int	CaseNo;		/* data case number */
typedef  float	CaseCount;	/* count of (partial) cases */

typedef  int	ClassNo,	/* class number, 1..MaxClass */
		DiscrValue,	/* discrete attribute value */
		Attribute;	/* attribute number, 1..MaxAtt */

#ifdef USEDOUBLE
typedef	 double	ContValue;	/* continuous attribute value */
#define	 PREC	14		/* precision */
#else
typedef	 float	ContValue;	/* continuous attribute value */
#define	 PREC	 7		/* precision */
#endif


typedef  union	 _def_val
	 {
	    String	_s_val;		/* att val for comparison */
	    ContValue	_n_val;		/* number for arith */
	 }
	 DefVal;

typedef  struct  _def_elt
	 {
	    short	_op_code;	/* type of element */
	    DefVal	_operand;	/* string or numeric value */
	 }
	 DefElt, *Definition;

typedef  struct  _elt_rec
	 {
	    int		Fi,		/* index of first char of element */
			Li;		/* last ditto */
	    char	Type;		/* 'B', 'S', or 'N' */
	 }
	 EltRec;

#define	 DefOp(DE)	DE._op_code
#define	 DefSVal(DE)	DE._operand._s_val
#define	 DefNVal(DE)	DE._operand._n_val

#define	 OP_ATT			 0	/* opcodes */
#define	 OP_NUM			 1
#define	 OP_STR			 2
#define	 OP_MISS		 3
#define	 OP_AND			10
#define	 OP_OR			11
#define	 OP_EQ			20
#define	 OP_NE			21
#define	 OP_GT			22
#define	 OP_GE			23
#define	 OP_LT			24
#define	 OP_LE			25
#define	 OP_SEQ			26
#define	 OP_SNE			27
#define	 OP_PLUS		30
#define	 OP_MINUS		31
#define	 OP_UMINUS		32
#define	 OP_MULT		33
#define	 OP_DIV			34
#define	 OP_MOD			35
#define	 OP_POW			36
#define	 OP_SIN			40
#define	 OP_COS			41
#define	 OP_TAN			42
#define	 OP_LOG			43
#define	 OP_EXP			44
#define	 OP_INT			45
#define	 OP_END			99


typedef  union  _attribute_value
	 {
	    DiscrValue	_discr_val;
	    ContValue	_cont_val;
	 }
	 AttValue, *DataRec;

typedef	 struct _sort_rec
	 {
	    ContValue	V;
	    ClassNo	C;
	    float	W;
	 }
	 SortRec;

#define  CVal(Case,Att)		Case[Att]._cont_val
#define  DVal(Case,Att)		Case[Att]._discr_val
#define  XDVal(Case,Att)	(Case[Att]._discr_val & 077777777)
#define  SVal(Case,Att)		Case[Att]._discr_val
#define  Class(Case)		(*Case)._discr_val
#define  Weight(Case)		(*(Case-1))._cont_val

#define	 Unknown(Case,Att)	(DVal(Case,Att)==UNKNOWN)
#define	 UnknownVal(AV)		(AV._discr_val==UNKNOWN)
#define	 NotApplic(Case,Att)	(Att != ClassAtt && DVal(Case,Att)==NA)
#define	 NotApplicVal(AV)	(AV._discr_val==NA)

#define	 RelCWt(Case)		( Unknown(Case,CWtAtt)||\
				  NotApplic(Case,CWtAtt)||\
			  	  CVal(Case,CWtAtt)<=0 ? 1 :\
				  CVal(Case,CWtAtt)/AvCWt )

typedef  struct _treerec	*Tree;
typedef  struct _treerec
	 {
	    BranchType	NodeType;
	    ClassNo	Leaf;		/* best class at this node */
	    CaseCount	Cases,		/* no of cases at this node */
			*ClassDist,	/* class distribution of cases */
	    		Errors;		/* est or resub errors at this node */
	    Attribute	Tested; 	/* attribute referenced in test */
	    int		Forks,		/* number of branches at this node */
			Leaves;		/* number of non-empty leaves in tree */
	    ContValue	Cut,		/* threshold for continuous attribute */
		  	Lower,		/* lower limit of soft threshold */
		  	Upper,		/* upper limit ditto */
			Mid;		/* midpoint for soft threshold */
	    Set         *Subset;	/* subsets of discrete values  */
	    Tree	*Branch,	/* Branch[x] = subtree for outcome x */
			Parent;		/* node above this one */
	 }
	 TreeRec;


typedef	 struct _environment
	 {
	    CaseNo	Xp, Ep;			/* start and end of scan  */
	    double	Cases,			/* total cases */
			KnownCases,		/* ditto less missing values */
			ApplicCases,		/* cases with numeric values */
			HighCases, LowCases,	/* cases above/below cut */
			NAInfo,			/* info for N/A values */
			FixedSplitInfo,		/* split info for ?, N/A */
			BaseInfo,		/* info before split */
			UnknownRate,		/* proportion of ? values */
			MinSplit,		/* min cases before/after cut */
			**Freq,			/* local Freq[4][class] */
			*ClassFreq,		/* local class frequencies */
			*ValFreq;		/* cases with val i */
	    ClassNo	HighClass, LowClass;	/* class after/before cut */
	    ContValue	HighVal, LowVal;	/* values after/before cut */
	    SortRec	*SRec;			/* for Cachesort() */
	    Set		**Subset,		/* Subset[att][number] */
			*WSubset;		/* working subsets */
	    int		*Subsets,		/* no of subsets for att */
			Blocks,			/* intermediate no of subsets */
			Bytes,			/* size of each subset */
			ReasonableSubsets;
	    double	*SubsetInfo,		/* subset info */
			*SubsetEntr,		/* subset entropy */
			**MergeInfo,		/* info of merged subsets i,j */
			**MergeEntr;		/* entropy ditto */

            struct cmap     *Implications;
            struct array    *permutation;
            struct array    *classAttr;
	 }
	 EnvRec;


typedef  int	RuleNo;			/* rule number */

typedef  struct _condrec
	 {
	    BranchType	NodeType;	/* test type (see tree nodes) */
	    Attribute	Tested;		/* attribute tested */
	    ContValue	Cut;		/* threshold (if relevant) */
	    Set		Subset;		/* subset (if relevant) */
	    int		TestValue,	/* specified outcome of test */
			TestI;		/* rule tree index of this test */
	 }
	 CondRec, *Condition;


typedef  struct _rulerec
	 {
	    RuleNo	RNo;		/* rule number */
	    int		TNo,		/* trial number */
	    		Size;		/* number of conditions */
	    Condition	*Lhs;		/* conditions themselves */
	    ClassNo	Rhs;		/* class given by rule */
	    CaseCount	Cover,		/* number of cases covered by rule */
			Correct;	/* number on which correct */
	    float	Prior;		/* prior probability of RHS */
	    int		Vote;		/* unit = 0.001 */
	 }
	 RuleRec, *CRule;


typedef  struct _ruletreerec *RuleTree;
typedef  struct _ruletreerec
	 {
	    RuleNo	*Fire;		/* rules matched at this node */
	    Condition	CondTest;	/* new test */
	    int		Forks;		/* number of branches */
	    RuleTree	*Branch;	/* subtrees */
	 }
	 RuleTreeRec;


typedef struct _rulesetrec
	 {
	    RuleNo	SNRules;	/* number of rules */
	    CRule	*SRule;		/* rules */
	    ClassNo	SDefault;	/* default class for this ruleset */
	    RuleTree	RT;		/* rule tree (see ruletree.c) */
	 }
	 RuleSetRec, *CRuleSet;


/*************************************************************************/
/*									 */
/*		Function prototypes					 */
/*									 */
/*************************************************************************/

	/* c50.c */

int	    main(int, char *[]);
void	    FreeClassifier(int Trial);

	/* construct.c */

void	    ConstructClassifiers(void);
void	    InitialiseWeights(void);
void	    SetAvCWt(void);
void	    Evaluate(int Flags);
void	    EvaluateSingle(int Flags);
void	    EvaluateBoost(int Flags);
void	    RecordAttUsage(DataRec Case, int *Usage);

	/* getnames.c */

Boolean	    ReadName(FILE *f, String s, int n, char ColonOpt);
void	    GetNames(FILE *Nf);
void	    ExplicitAtt(FILE *Nf);
int	    Which(String Val, String *List, int First, int Last);
void	    ListAttsUsed(void);
void	    FreeNames(void);
int	    InChar(FILE *f);

	/* implicitatt.c */

void	    ImplicitAtt(FILE *Nf);
void	    ReadDefinition(FILE *f);
void	    Append(char c);
Boolean	    Expression(void);
Boolean	    Conjunct(void);
Boolean	    SExpression(void);
Boolean	    AExpression(void);
Boolean	    Term(void);
Boolean	    Factor(void);
Boolean	    Primary(void);
Boolean	    Atom(void);
Boolean	    Find(String S);
int	    FindOne(String *Alt);
Attribute   FindAttName(void);
void	    DefSyntaxError(String Msg);
void	    DefSemanticsError(int Fi, String Msg, int OpCode);
void	    Dump(char OpCode, ContValue F, String S, int Fi);
void	    DumpOp(char OpCode, int Fi);
Boolean	    UpdateTStack(char OpCode, ContValue F, String S, int Fi);
AttValue    EvaluateDef(Definition D, DataRec Case);

	/* getdata.c */

void	    GetData(FILE *Df, Boolean Train, Boolean AllowUnknownClass);
DataRec	    GetDataRec(FILE *Df, Boolean Train);
CaseNo	    CountData(FILE *Df);
int	    StoreIVal(String s);
void	    FreeData(void);
void	    CheckValue(DataRec Case, Attribute Att);
void 		GetImplications(const char * extension); // Daniel
void		GetIntervals(const char * extension); // Daniel

	/* mcost.c */

void	    GetMCosts(FILE *f);

	/* attwinnow.c */

void	    WinnowAtts(void);
float	    TrialTreeCost(Boolean FirstTime);
float	    ErrCost(Tree T, CaseNo Fp, CaseNo Lp);
void	    ScanTree(Tree T, Boolean *Used);

	/* formtree.c */

void	    InitialiseTreeData(void);
void	    FreeTreeData(void);
void	    SetMinGainThresh(void);
// Daniel's Changes
//void	    FormTree(CaseNo, CaseNo, int, Tree *);
void	    FormTree(CaseNo, CaseNo, int, Tree *, Attribute, Attribute);
void	    MyFormTree(CaseNo, CaseNo, int, Tree *);
//Attribute   ChooseSplit(CaseNo Fp, CaseNo Lp, CaseCount Cases, Boolean Sampled);
Attribute   ChooseSplit(CaseNo Fp, CaseNo Lp, CaseCount Cases, Boolean Sampled, Attribute, Attribute);
//void	    Divide(Tree Node, CaseNo Fp, CaseNo Lp, int Level);
void	    Divide(Tree Node, CaseNo Fp, CaseNo Lp, int Level, Attribute, Attribute);
// end
void	    SampleEstimate(CaseNo Fp, CaseNo Lp, CaseCount Cases);
void	    Sample(CaseNo Fp, CaseNo Lp, CaseNo N);
void	    ProcessQueue(CaseNo WFp, CaseNo WLp, CaseCount WCases);
//Attribute   FindBestAtt(CaseCount Cases);
Attribute   FindBestAttNew(CaseCount Cases, Attribute upperBound, Attribute lowerBound);
void	    EvalDiscrSplit(Attribute Att, CaseCount Cases);
CaseNo	    Group(DiscrValue, CaseNo, CaseNo, Tree);
CaseCount   SumWeights(CaseNo, CaseNo);
CaseCount   SumNocostWeights(CaseNo, CaseNo);
void	    FindClassFreq(double [], CaseNo, CaseNo);
void	    FindAllFreq(CaseNo, CaseNo);

/* Pranav: */
int         assignClass(int caseNo, int classValue);


	/* discr.c */

void	    EvalDiscreteAtt(Attribute Att, CaseCount Cases);
void	    EvalOrderedAtt(Attribute Att, CaseCount Cases);
void	    SetDiscrFreq(Attribute Att);
double	    DiscrKnownBaseInfo(CaseCount KnownCases, DiscrValue MaxVal);
void	    DiscreteTest(Tree Node, Attribute Att);

	/* contin.c */

void	    EvalContinuousAtt(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    EvalContinuousAttAlg1(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    EvalContinuousAttAlg2(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    EvalContinuousAttAlg3(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    EvalContinuousAttAlg4(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    EvalContinuousAttAlg5(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    EvalContinuousAttAlg6(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    EstimateMaxGR(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    PrepareForContin(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    PrepareForContinAlg1(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    PrepareForContinAlg2(Attribute Att, CaseNo Fp, CaseNo Lp);
void	    PrepareForContinAlg3(Attribute Att, CaseNo Fp, CaseNo Lp);
CaseNo	    PrepareForScan(CaseNo Lp);
void	    ContinTest(Tree Node, Attribute Att);
void	    AdjustAllThresholds(Tree T);
void	    AdjustThresholds(Tree T, Attribute Att, CaseNo *Ep);
ContValue   GreatestValueBelow(ContValue Th, CaseNo *Ep);

	/* info.c */

double	    ComputeGain(double BaseInfo, float UnknFrac, DiscrValue MaxVal,
			CaseCount TotalCases);
double	    TotalInfo(double V[], DiscrValue MinVal, DiscrValue MaxVal);
double	    ImplicationInfo(double pos, double neg, double implications);
void	    PrintDistribution(Attribute Att, DiscrValue MinVal,
			DiscrValue MaxVal, double **Freq, double *ValFreq,
			Boolean ShowNames);

	/* subset.c */

void	    InitialiseBellNumbers(void);
void	    EvalSubset(Attribute Att, CaseCount Cases);
void	    Merge(DiscrValue x, DiscrValue y, CaseCount Cases);
void	    EvaluatePair(DiscrValue x, DiscrValue y, CaseCount Cases);
void	    PrintSubset(Attribute Att, Set Ss);
void	    SubsetTest(Tree Node, Attribute Att);
Boolean	    SameDistribution(DiscrValue V1, DiscrValue V2);
void	    AddBlock(DiscrValue V1, DiscrValue V2);
void	    AddBlock(DiscrValue V1, DiscrValue V2);
void	    MoveBlock(DiscrValue V1, DiscrValue V2);

	/* prune.c */

void	    Prune(Tree T);
void	    EstimateErrs(Tree T, CaseNo Fp, CaseNo Lp, int Sh, int Flags);
void	    GlobalPrune(Tree T);
void	    FindMinCC(Tree T);
void	    InsertParents(Tree T, Tree P);
void	    CheckSubsets(Tree T, Boolean);
void	    InitialiseExtraErrs(void);
float	    ExtraErrs(CaseCount N, CaseCount E, ClassNo C);
float	    RawExtraErrs(CaseCount N, CaseCount E);
void	    RestoreDistribs(Tree T);
void	    CompressBranches(Tree T);
void	    SetGlobalUnitWeights(int LocalFlag);

	/* p-thresh.c */

void	    SoftenThresh(Tree T);
void	    ResubErrs(Tree T, CaseNo Fp, CaseNo Lp);
void	    FindBounds(Tree T, CaseNo Fp, CaseNo Lp);

	/* classify.c */

ClassNo	    TreeClassify(DataRec Case, Tree DecisionTree);
void	    FollowAllBranches(DataRec Case, Tree T, float Fraction);
ClassNo	    RuleClassify(DataRec Case, CRuleSet RS);
int	    FindOutcome(DataRec Case, Condition OneCond);
Boolean	    Matches(CRule R, DataRec Case);
void	    CheckActiveSpace(int N);
void	    MarkActive(RuleTree RT, DataRec Case);
void	    SortActive(void);
void	    CheckUtilityBand(int *u, RuleNo r, ClassNo Actual, ClassNo Default);
ClassNo	    BoostClassify(DataRec Case, int MaxTrial);
ClassNo	    SelectClass(ClassNo Default, Boolean UseCosts);
ClassNo	    Classify(DataRec Case);
float	    Interpolate(Tree T, ContValue Val);

	/* special case for dual-purpose routines  */

void	    FindLeaf(DataRec Case, Tree T, Tree PT, float Wt);
Boolean	    Satisfies(DataRec Case, Condition OneCond);

	/* sort.c */

void	    Quicksort(CaseNo Fp, CaseNo Lp, Attribute Att);
void 		QuicksortWithImplications(CaseNo Fp, CaseNo Lp, Attribute Att, struct array * permutation); // Daniel
void	    Cachesort(CaseNo Fp, CaseNo Lp, SortRec *SRec);

	/* trees.c */

void	    FindDepth(Tree T);
void	    PrintTree(Tree T, String Title);
void	    Show(Tree T, int Sh);
void	    ShowBranch(int Sh, Tree T, DiscrValue v, DiscrValue BrNo);
DiscrValue  Elements(Attribute Att, Set S, DiscrValue *Last);
int	    MaxLine(Tree SubTree);
void	    Indent(int Sh, int BrNo);
void	    FreeTree(Tree T);
Tree	    Leaf(double *Freq, ClassNo NodeClass, CaseCount Cases,
		 CaseCount Errors);
void	    Sprout(Tree T, DiscrValue Branches);
void	    UnSprout(Tree T);
int	    TreeSize(Tree T);
int	    ExpandedLeafCount(Tree T);
int	    TreeDepth(Tree T);
Tree	    CopyTree(Tree T);

	/* utility.c */

void	    PrintHeader(String Title);
char	    ProcessOption(int Argc, char **Argv, char *Str);
void	    *Pmalloc(size_t Bytes);
void	    *Prealloc(void *Present, size_t Bytes);
void	    *Pcalloc(size_t Number, unsigned int Size);
void	    FreeVector(void **V, int First, int Last);
DataRec	    NewCase(void);
void	    FreeCases(void);
void	    FreeLastCase(DataRec Case);
double	    KRandom(void);
void	    ResetKR(int KRInit);
void	    Error(int ErrNo, String S1, String S2);
String	    CaseLabel(CaseNo N);
FILE *	    GetFile(String Extension, String RW);
double	    ExecTime(void);
int	    Denominator(ContValue Val);
int	    GetInt(String S, int N);
int	    DateToDay(String DS);
void	    DayToDate(int DI, String Date);
int	    TimeToSecs(String TS);
void	    SecsToTime(int Secs, String Time);
void	    SetTSBase(int y);
int	    TStampToMins(String TS);
void	    Check(float Val, float Low, float High);
void	    CValToStr(ContValue CV, Attribute Att, String DS);
double	    rint(double v);
void	    Cleanup(void);
#ifdef UTF8
int	    UTF8CharWidth(unsigned char *U);
int	    wcwidth(wchar_t ucs);
int	    wcswidth(const wchar_t *pwcs, size_t n);
#endif

	/* confmat.c */

void	    PrintConfusionMatrix(CaseNo *ConfusionMat);
void	    PrintErrorBreakdown(CaseNo *ConfusionMat);
void	    PrintUsageInfo(CaseNo *Usage);

	/* formrules.c */

CRuleSet    FormRules(Tree T);
void	    Scan(Tree T);
void	    SetupNCost(void);
void	    PushCondition(void);
void	    PopCondition(void);
void	    PruneRule(Condition Cond[], ClassNo TargetClass);
void	    ProcessLists(void);
void	    AddToList(CaseNo *List, CaseNo N);
void	    DeleteFromList(CaseNo *Before, CaseNo N);
int	    SingleFail(CaseNo i);
void	    Increment(int d, CaseNo i, double *Total, double *Errors);
void	    FreeFormRuleData(void);

	/* rules.c */

Boolean	    NewRule(Condition Cond[], int NConds, ClassNo TargetClass,
		    Boolean *Deleted, CRule Existing,
		    CaseCount Cover, CaseCount Correct, float Prior);
void	    ListSort(int *L, int Fp, int Lp);
Byte	    *Compress(int *L);
void	    Uncompress(Byte *CL, int *UCL);
Boolean	    SameRule(RuleNo r, Condition Cond[], int NConds,
		     ClassNo TargetClass);
void	    FreeRule(CRule R);
void	    FreeRules(CRuleSet RS);
void	    PrintRules(CRuleSet, String);
void	    PrintRule(CRule R);
void	    PrintCondition(Condition C);

	/* siftrules.c */

void	    SiftRules(float EstErrRate);
void	    InvertFires(void);
void	    FindTestCodes(void);
float	    CondBits(Condition C);
void	    SetInitialTheory(void);
void	    CoverClass(ClassNo Target);
int	    MessageLength(RuleNo NR, double RuleBits, float Errs);
void	    HillClimb(void);
void	    InitialiseVotes(void);
void	    CountVotes(CaseNo i);
void	    UpdateDeltaErrs(CaseNo i, double Delta, RuleNo Toggle);
CaseCount   CalculateDeltaErrs(void);
void	    PruneSubsets(void);
void	    SetDefaultClass(void);
void	    SwapRule(RuleNo A, RuleNo B);
int	    OrderByUtility(void);
int	    OrderByClass(void);
void	    OrderRules(void);
void	    GenerateLogs(int MaxN);
void	    FreeSiftRuleData(void);

	/* ruletree.c */

void	    ConstructRuleTree(CRuleSet RS);
void	    SetTestIndex(Condition C);
RuleTree    GrowRT(RuleNo *RR, int RRN, CRule *Rule);
int	    DesiredOutcome(CRule R, int TI);
int	    SelectTest(RuleNo *RR, int RRN, CRule *Rule);
void	    FreeRuleTree(RuleTree RT);

	/* modelfiles.c */

void	    CheckFile(String Extension, Boolean Write);
void	    WriteFilePrefix(String Extension);
void	    ReadFilePrefix(String Extension);
void	    SaveDiscreteNames(void);
void	    SaveTree(Tree T, String Extension);
void	    OutTree(Tree T);
void	    SaveRules(CRuleSet RS, String Extension);
void	    AsciiOut(String Pre, String S);
void	    ReadHeader(void);
Tree	    GetTree(String Extension);
Tree	    InTree(void);
CRuleSet    GetRules(String Extension);
CRuleSet    InRules(void);
CRule	    InRule(void);
Condition   InCondition(void);
int	    ReadProp(char *Delim);
String	    RemoveQuotes(String S);
Set	    MakeSubset(Attribute Att);
void	    StreamIn(String S, int n);

	/* update.c (Unix) or winmain.c (WIN32) */

void	    NotifyStage(int);
void	    Progress(float);

	/* xval.c */

void	    CrossVal(void);
void	    Prepare(void);
void	    Shuffle(int *Vec);
void	    Summary(void);
float	    SE(float sum, float sumsq, int no);

	/* Daniel: debug.c */
int verify (Tree, DataRec *, struct cmap *, int);

int are_equal (DataRec *, DataRec *, int, int);
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*		General data for C5.0					 */
/*		---------------------					 */
/*									 */
/*************************************************************************/

#include "cpp_wrapper/cmap.h" // Daniel

/*************************************************************************/
/*									 */
/*		Parameters etc						 */
/*									 */
/*************************************************************************/

int		VERBOSITY=0,	/* verbosity level (0 = none) */
		TRIALS=1,	/* number of trees to be grown */
		FOLDS=10,	/* crossvalidation folds */
		UTILITY=0;	/* rule utility bands */

Boolean		SUBSET=0,	/* subset tests allowed */
		BOOST=0,	/* boosting invoked */
		PROBTHRESH=0,	/* to use soft thresholds */
		RULES=0,	/* rule-based classifiers */
		XVAL=0,		/* perform crossvalidation */
		NOCOSTS=0,	/* ignoring costs */
		WINNOW=0,	/* attribute winnowing */
		GLOBAL=1;	/* use global pruning for trees */

CaseCount	MINITEMS=2,	/* minimum cases each side of a cut */
		LEAFRATIO=0;	/* leaves per case for boosting */

float		CF=0.25,	/* confidence limit for tree pruning */
		SAMPLE=0.0;	/* sample training proportion */

Boolean		LOCK=false;	/* sample locked */


/*************************************************************************/
/*									 */
/*		Attributes and data					 */
/*									 */
/*************************************************************************/

Attribute	ClassAtt=0,	/* attribute to use as class */
		LabelAtt=0,	/* attribute to use as case ID */
		CWtAtt=0;	/* attribute to use for case weight */

double		AvCWt;		/* average case weight */

String		*ClassName=0,	/* class names */
		*AttName=0,	/* att names */
		**AttValName=0;	/* att value names */

char		*IgnoredVals=0;	/* values of labels and atts marked ignore */
int		IValsSize=0,	/* size of above */
		IValsOffset=0;	/* index of first free char */

int		MaxAtt,		/* max att number */
		MinAtt = 2, /* Daniel: min att number (assuming that PC is attribute #1) */
		MaxClass,	/* max class number */
		MaxDiscrVal=3,	/* max discrete values for any att */
		MaxLabel=0,	/* max characters in case label */
		LineNo=0,	/* input line number */
		ErrMsgs=0,	/* errors found */
		AttExIn=0,	/* attribute exclusions/inclusions */
		TSBase=0;	/* base day for time stamps */

DiscrValue	*MaxAttVal=0;	/* number of values for each att */

char		*SpecialStatus=0;/* special att treatment */

Definition	*AttDef=0;	/* definitions of implicit atts */
Attribute	**AttDefUses=0;	/* list of attributes used by definition */

Boolean		*SomeMiss=Nil,	/* att has missing values */
		*SomeNA=Nil,	/* att has N/A values */
		Winnowed=0;	/* atts have been winnowed */

ContValue	*ClassThresh=0;	/* thresholded class attribute */

CaseNo		MaxCase=-1;	/* max data case number */

DataRec		*Case=0;	/* data cases */

DataRec		*SaveCase=0;

String		FileStem="undefined";

// Daniel
struct cmap	* Implications = 0;
struct array * IntervalsUpperBounds = 0;
struct array * IntervalsLowerBounds = 0;

#ifdef DEBUG
struct cmap	* DebugImplications = 0;

DataRec * DebugCase = 0;
#endif


/*************************************************************************/
/*									 */
/*		Trees							 */
/*									 */
/*************************************************************************/

Tree		*Raw=0,		/* unpruned trees */
		*Pruned=0,	/* pruned trees */
		WTree=0;	/* winnow tree */

float		Confidence,	/* set by classify() */
		SampleFrac=1,	/* fraction used when sampling */
		*Vote=0,	/* total votes for classes */
		*BVoteBlock=0,	/* boost voting block */
		**MCost=0,	/* misclass cost [pred][real] */
		**NCost=0,	/* normalised MCost used for rules */
		*WeightMul=0;	/* prior adjustment factor */

CRule		*MostSpec=0;	/* most specific rule for each class */

Boolean		UnitWeights=1,	/* all weights are 1.0 */
		CostWeights=0;	/* reweight cases for costs */

int		Trial,		/* trial number for boosting */
		MaxTree=0;	/* max tree grown */

ClassNo		*TrialPred=0;	/* predictions for each boost trial */

double		*ClassFreq=0,	/* ClassFreq[c] = # cases of class c */
		**DFreq=0;	/* DFreq[a][c*x] = Freq[][] for attribute a */

float		*Gain=0,	/* Gain[a] = info gain by split on att a */
		*Info=0,	/* Info[a] = max info from split on att a */
		*EstMaxGR=0,	/* EstMaxGR[a] = est max GR from folit on a */
		*ClassSum=0;	/* class weights during classification */

ContValue	*Bar=0;		/* Bar[a]  = best threshold for contin att a */

double		GlobalBaseInfo,	/* base information before split */
		**Bell=0;	/* table of Bell numbers for subsets */

Byte		*Tested=0;	/* Tested[a] = att a already tested */

Set		**Subset=0;	/* Subset[a][s] = subset s for att a */
int		*Subsets=0;	/* Subsets[a] = no. subsets for att a */

EnvRec		GEnv;		/* environment block */

/*************************************************************************/
/*									 */
/*		Rules							 */
/*									 */
/*************************************************************************/

CRule		*Rule=0;	/* current rules */

RuleNo		NRules,		/* number of rules */
		RuleSpace;	/* space currently allocated for rules */

CRuleSet	*RuleSet=0;	/* rulesets */

ClassNo		Default;	/* default class associated with ruleset or
				   boosted classifier */

Byte		**Fires=Nil,	/* Fires[r][*] = cases covered by rule r */
		*CBuffer=Nil;	/* buffer for compressing lists */

int		*CovBy=Nil,	/* entry numbers for Fires inverse */
		*List=Nil;	/* temporary list of cases or rules */

float		AttTestBits,	/* average bits to encode tested attribute */
		*BranchBits=0;	/* ditto attribute value */
int		*AttValues=0,	/* number of attribute values in the data */
		*PossibleCuts=0;/* number of thresholds for an attribute */

double		*LogCaseNo=0,	/* LogCaseNo[i] = log2(i) */
		*LogFact=0;	/* LogFact[i] = log2(i!) */

int		*UtilErr=0,	/* error by utility band */
		*UtilBand=0;	/* last rule in each band */
double		*UtilCost=0;	/* cost ditto */


/*************************************************************************/
/*									 */
/*		Misc							 */
/*									 */
/*************************************************************************/

int		KRInit=0,	/* KRandom initializer for SAMPLE */
		Now=0;		/* current stage */

FILE		*TRf=0;		/* file pointer for tree and rule i/o */
char		Fn[500];	/* file name */

FILE  		*Of=0;		/* output file */

/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/


/*************************************************************************/
/*									 */
/*	Main routine, C5.0						 */
/*	------------------						 */
/*									 */
/*************************************************************************/



#include "cpp_wrapper/cmap.h"

//#include <signal.h>

//#include <sys/unistd.h>
//#include <sys/time.h>
//#include <sys/resource.h>

#define SetFOpt(V)	V = strtod(OptArg, &EndPtr);\
			if ( ! EndPtr || *EndPtr != '\00' ) break;\
			ArgOK = true
#define SetIOpt(V)	V = strtol(OptArg, &EndPtr, 10);\
			if ( ! EndPtr || *EndPtr != '\00' ) break;\
			ArgOK = true

// Daniel
int select_rhs_if_lhs_is_positive(int lhs, int rhs) {
	assert (0 <= lhs && lhs <= MaxCase && 0 <= rhs && rhs <= MaxCase);
	assert (Class(Case[lhs]) == DVal(Case[lhs], ClassAtt));
	assert (Class(Case[rhs]) == DVal(Case[rhs], ClassAtt));

	// Consistency check of the sample
	if (Class(Case[lhs]) == 1 && Class(Case[rhs]) == 2) {
		fprintf(Of, "Found inconsistency in sample, exiting");
		exit (1);
	}

	return (Class(Case[lhs]) == 1 && Class(Case[rhs]) == 0);

}

int select_rhs_if_lhs_is_negative(int lhs, int rhs) {
	assert (0 <= lhs && lhs <= MaxCase && 0 <= rhs && rhs <= MaxCase);
	assert (Class(Case[lhs]) == DVal(Case[lhs], ClassAtt));
	assert (Class(Case[rhs]) == DVal(Case[rhs], ClassAtt));

	// Consistency check of the sample
	if (Class(Case[lhs]) == 1 && Class(Case[rhs]) == 2) {
		fprintf(Of, "Found inconsistency in sample, exiting");
		exit (1);
	}

	return (Class(Case[lhs]) == 2 && Class(Case[rhs]) == 0);
}
// End Daniel


int main(int Argc, char *Argv[])
/*  ----  */
{
    int			o;
    extern String	OptArg, Option;
    char		*EndPtr;
    Boolean		FirstTime=true, ArgOK;
    double		StartTime;
    FILE		*F;
    CaseNo		SaveMaxCase;
    Attribute		Att;

    // Pranav:
    // struct rlimit RL;

    /*  Make sure there is a largish runtime stack  */

    /* Pranav:
    getrlimit(RLIMIT_STACK, &RL);

    RL.rlim_cur = Max(RL.rlim_cur, 20 * 1024 * 1024);

    if ( RL.rlim_max > 0 )	// -1 if unlimited 
    {
	RL.rlim_cur = Min(RL.rlim_max, RL.rlim_cur);
    }

    setrlimit(RLIMIT_STACK, &RL);
    */

    /*  Check for output to be saved to a file  */

    if ( Argc > 2 && ! strcmp(Argv[Argc-2], "-o") )
    {
	Of = fopen(Argv[Argc-1], "w");
	Argc -= 2;
    }

    if ( ! Of )
    {
	Of = stdout;
    }

    KRInit = time(0) & 07777;

    PrintHeader("");

    /*  Process options  */

    while ( (o = ProcessOption(Argc, Argv, "f+bpv+t+sm+c+S+I+ru+egX+wh")) )
    {
	if ( FirstTime )
	{
	    fprintf(Of, T_OptHeader);
	    FirstTime = false;
	}

	ArgOK = false;

	switch (o)
	{
	case 'f':   FileStem = OptArg;
		    fprintf(Of, T_OptApplication, FileStem);
		    ArgOK = true;
		    break;
	case 'b':   BOOST = true;
		    fprintf(Of, T_OptBoost);
		    if ( TRIALS == 1 ) TRIALS = 10;
		    ArgOK = true;
		    break;
	case 'p':   PROBTHRESH = true;
		    fprintf(Of, T_OptProbThresh);
		    ArgOK = true;
		    break;
#ifdef VerbOpt
	case 'v':   SetIOpt(VERBOSITY);
		    fprintf(Of, "\tVerbosity level %d\n", VERBOSITY);
		    ArgOK = true;
		    break;
#endif
	case 't':   SetIOpt(TRIALS);
		    fprintf(Of, T_OptTrials, TRIALS);
		    Check(TRIALS, 3, 1000);
		    BOOST = true;
		    break;
	case 's':   SUBSET = true;
		    fprintf(Of, T_OptSubsets);
		    ArgOK = true;
		    break;
	case 'm':   SetFOpt(MINITEMS);
		    fprintf(Of, T_OptMinCases, MINITEMS);
		    Check(MINITEMS, 1, 1000000);
		    break;
	case 'c':   SetFOpt(CF);
		    fprintf(Of, T_OptCF, CF);
		    Check(CF, 0, 100);
		    CF /= 100;
		    break;
	case 'r':   RULES = true;
		    fprintf(Of, T_OptRules);
		    ArgOK = true;
		    break;
	case 'S':   SetFOpt(SAMPLE);
		    fprintf(Of, T_OptSampling, SAMPLE);
		    Check(SAMPLE, 0.1, 99.9);
		    SAMPLE /= 100;
		    break;
	case 'I':   SetIOpt(KRInit);
		    fprintf(Of, T_OptSeed, KRInit);
		    KRInit = KRInit & 07777;
		    break;
	case 'u':   SetIOpt(UTILITY);
		    fprintf(Of, T_OptUtility, UTILITY);
		    Check(UTILITY, 2, 10000);
		    RULES = true;
		    break;
	case 'e':   NOCOSTS = true;
		    fprintf(Of, T_OptNoCosts);
		    ArgOK = true;
		    break;
	case 'w':   WINNOW = true;
		    fprintf(Of, T_OptWinnow);
		    ArgOK = true;
		    break;
	case 'g':   GLOBAL = false;
		    fprintf(Of, T_OptNoGlobal);
		    ArgOK = true;
		    break;
	case 'X':   SetIOpt(FOLDS);
		    fprintf(Of, T_OptXval, FOLDS);
		    Check(FOLDS, 2, 1000);
		    XVAL = true;
		    break;
	}

	if ( ! ArgOK )
	{
	    if ( o != 'h' )
	    {
		fprintf(Of, T_UnregnizedOpt,
			    Option,
			    ( ! OptArg || OptArg == Option+2 ? "" : OptArg ));
		fprintf(Of, T_SummaryOpts);
	    }
	    fprintf(Of, T_ListOpts);
	    Goodbye(1);
	}
    }

    if ( UTILITY && BOOST )
    {
	fprintf(Of, T_UBWarn);
    }

    StartTime = ExecTime();

    /*  Get information on training data  */

    if ( ! (F = GetFile(".names", "r")) ) Error(NOFILE, "", "");
    GetNames(F);

    if ( ClassAtt )
    {
	fprintf(Of, T_ClassVar, AttName[ClassAtt]);
    }

    NotifyStage(READDATA);
    Progress(-1.0);

    /*  Allocate space for SomeMiss[] and SomeNA[] */

    SomeMiss = AllocZero(MaxAtt+1, Boolean);
    SomeNA   = AllocZero(MaxAtt+1, Boolean);

    /*  Read data file  */

    if ( ! (F = GetFile(".data", "r")) ) Error(NOFILE, "", "");
#if false
    // Pranav: Now allow even the class attribute to be unknown
    GetData(F, true, false);
#else
    GetData(F, true, true);
#endif
    fprintf(Of, TX_ReadData(MaxCase+1, MaxAtt, FileStem));

    #if false
    {
	int i = 62;
	printf("datapoint in Case %i: %f,%f,%f,%d\n", i, Case[i][1]._cont_val, Case[i][2]._cont_val, Case[i][3]._cont_val, Case[i][ClassAtt]._discr_val);

    }
    #endif

	// Daniel: Read implications
	GetImplications(".implications");
	fprintf( Of, "Read %d implications\n", cmap_number_of_implications (Implications));
	//cmap_print(Implications);

	// Daniel: Read intervals
	// Needs to be called after reading the data!
	GetIntervals(".intervals");
	if (IntervalsUpperBounds) {
		fprintf( Of, "Read %d intervals\n", IntervalsUpperBounds->size);

#ifdef DEBUG
		int i;
		for (i = 0; i<IntervalsUpperBounds->size; i++) {
			printf ("Bounds for attribute value %s (%d): [%d, %d]\n", AttValName[1][i+2], (i+2), IntervalsLowerBounds->entries[i], IntervalsUpperBounds->entries[i]);
		}
#endif
		
	}

#ifdef DEBUG
	// Save implications and original data for later verification
	DebugImplications = cmap_copy (Implications);
	fprintf( Of, "Copied %d implications\n", cmap_number_of_implications (DebugImplications));

    DebugCase = (DataRec *) malloc((MaxCase + 1) * sizeof (DataRec));
	int my_MaxCase = MaxCase;
	int my_MaxAtt = MaxAtt;
	int loop;
	ForEach (loop, 0, MaxCase) {

		DebugCase[loop] = (DataRec)malloc((MaxAtt + 1) * sizeof (AttValue));

		int k;
		ForEach (k, 0, MaxAtt) {
			memcpy (&(DebugCase[loop][k]), &(Case[loop][k]), sizeof (AttValue));
		}

	}

	if (! are_equal (Case, DebugCase, my_MaxCase, my_MaxAtt)) {
		printf ("Copy was not successful!\n");
		return 1;
	} else {
		printf ("Copying of Cases successful\n");
	}
#endif

	// Daniel: Initial implication propagation
	// Forward
	struct array * forward = cmap_select_rhs (Implications, & select_rhs_if_lhs_is_positive);
	cmap_propagate (forward, Implications, 1, & assignClass);
	delete_array (forward);
	forward = NULL;
	// Backward
	struct array * backward = cmap_select_lhs (Implications, & select_rhs_if_lhs_is_negative);
	cmap_propagate (backward, Implications, 2, & assignClass);
	delete_array (backward);
	backward = NULL;
	// End

    if ( XVAL && (F = GetFile(".test", "r")) )
    {
	assert (false);
	SaveMaxCase = MaxCase;
	GetData(F, false, false);
	fprintf(Of, TX_ReadTest(MaxCase-SaveMaxCase, FileStem));
    }

    /*  Check whether case weight attribute appears  */

    if ( CWtAtt )
    {
	fprintf(Of, T_CWtAtt);
    }

    if ( ! NOCOSTS && (F = GetFile(".costs", "r")) )
    {
	assert (false);
	GetMCosts(F);
	if ( MCost )
	{
	    fprintf(Of, T_ReadCosts, FileStem);
	}
    }

    /*  Note any attribute exclusions/inclusions  */

    if ( AttExIn )
    {
	assert (false);
	fprintf(Of, "%s", ( AttExIn == -1 ? T_AttributesOut : T_AttributesIn ));

	ForEach(Att, 1, MaxAtt)
	{
	    if ( Att != ClassAtt &&
		 Att != CWtAtt &&
		 ( StatBit(Att, SKIP) > 0 ) == ( AttExIn == -1 ) )
	    {
		fprintf(Of, "    %s\n", AttName[Att]);
	    }
	}
    }

    /*  Build decision trees  */

    if ( ! BOOST )
    {
	TRIALS = 1;
    }

    InitialiseTreeData();
    if ( RULES )
    {
	assert (false);
	RuleSet = AllocZero(TRIALS+1, CRuleSet);
    }

    if ( WINNOW )
    {
	assert (false);
	NotifyStage(WINNOWATTS);
	Progress(-MaxAtt);
	WinnowAtts();
    }

    if ( XVAL )
    {
	assert (false);
	CrossVal();
    }
    else
    {
	ConstructClassifiers();


	/*  Evaluation  */

	fprintf(Of, T_EvalTrain, MaxCase+1);

	NotifyStage(EVALTRAIN);
	Progress(-TRIALS * (MaxCase+1.0));

	Evaluate(CMINFO | USAGEINFO);

	if ( (F = GetFile(( SAMPLE ? ".data" : ".test" ), "r")) )
	{
	    assert (false);
	    NotifyStage(READTEST);
	    fprintf(Of, "\n");

	    FreeData();
	    GetData(F, false, false);

	    fprintf(Of, T_EvalTest, MaxCase+1);

	    NotifyStage(EVALTEST);
	    Progress(-TRIALS * (MaxCase+1.0));

	    Evaluate(CMINFO);
	}
    }

    fprintf(Of, T_Time, ExecTime() - StartTime);

#ifdef DEBUG
	if (! are_equal (Case, DebugCase, my_MaxCase, my_MaxAtt)) {
		printf ("There has been reorganization of data in Cases!\n");
	} else {
		printf ("There has been NO reorganization of data in Cases\n");
	}
#endif

#ifdef VerbOpt
    Cleanup();
#endif

	// Daniel
	delete_cmap (Implications);
	if (IntervalsUpperBounds) {
		delete_array (IntervalsUpperBounds);
	}
	if (IntervalsLowerBounds) {
		delete_array (IntervalsLowerBounds);
	}

#ifdef DEBUG
	delete_cmap (DebugImplications);
	ForEach (loop, 0, my_MaxCase) {
		free (DebugCase[loop]);
	}
	free (DebugCase);
#endif

    return 0;
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Manage construction of classifiers, including boosting		 */
/*	------------------------------------------------------		 */
/*									 */
/*	C5.0 uses a modified form of boosting as follows:		 */
/*	*  Multiplicative weight adjustment is used for cases that are	 */
/*	   classified correctly, but misclassified cases use a form	 */
/*	   of additive weight adjustment.				 */
/*	*  In later boosting trials, cases that cannot possibly be	 */
/*	   classified correctly are dropped.  (This follows Freund's	 */
/*	   "Brown-Boost" approach, since the number of trials is known.) */
/*	*  The voting weight of a boosted classifier is determined by	 */
/*	   the confidence of its classification, based on the boosted	 */
/*	   weights of the training cases.				 */
/*									 */
/*	Variable misclassification costs are also supported.  When	 */
/*	there are two classes, the misclassification costs are used	 */
/*	to reweight the training cases, and the reweighting is reversed	 */
/*	after the classifier is constructed.  When classifying a case,	 */
/*	the estimated class probabilities and cost matrix are used to	 */
/*	determine the predicted class with lowest expected cost.	 */
/*									 */
/*************************************************************************/



#include "cpp_wrapper/tree_serializer.h"

/*************************************************************************/
/*									 */
/*	Grow single tree or sequence of boosted trees			 */
/*									 */
/*************************************************************************/


void ConstructClassifiers()
/*   --------------------  */
{
    CaseNo	i, Errs, Cases, Bp, Excl=0;
    double	ErrWt, ExclWt=0, OKWt, ExtraErrWt, NFact, MinWt=1.0, a, b;
    ClassNo	c, Pred, Real, Best;
    static	ClassNo	*Wrong=Nil;
    int		BaseLeaves;
    Boolean	NoStructure, CheckExcl;
    float	*BVote;

    /*  Clean up after possible interrupt  */

    FreeUnlessNil(Wrong);

    Wrong = Alloc(MaxCase+1, ClassNo);

#if false
    // Pranav: Some debug output
    // MaxClass = 2 {0: unknown, 1: true, 2: false}
    printf("maxclass: %d\n", MaxClass);
    // MaxCase: 24; from 0..24 i.e., total 25 case if there are 25 data points
    printf("maxcase: %d\n", MaxCase);
    // boost = 0
    printf("boost: %d\n", BOOST);
    // trials = 1
    printf("trial: %d\n", TRIALS);
    // CWtAtt = 0
    printf("cwtatt: %d\n", CWtAtt);
    // minitems = -1/0/2
    printf("minitems: %d\n", MINITEMS);
    // probthresh = 0
    printf("pobthresh: %d\n", PROBTHRESH);
    // mcost = 0
    printf("mcost: %d\n", MCost);
    // rules = 0/1 depending upon whether -r flag is used or not.
    printf("ruleS: %d\n", RULES);
    // leafration = 0/-1 
    printf("leafratio: %d\n", LEAFRATIO);
#endif

    if ( TRIALS > 1 )
    {
	assert (false);

	/*  BVoteBlock contains each case's class votes  */

	BVoteBlock = AllocZero((MaxCase+1) * (MaxClass+1), float);
    }

    /*  Preserve original case order  */

    SaveCase = Alloc(MaxCase+1, DataRec);
    memcpy(SaveCase, Case, (MaxCase+1) * sizeof(DataRec));

    /*  If using case weighting, find average  */

    if ( CWtAtt )
    {
	assert (false);
	SetAvCWt();
    }

    InitialiseWeights();

    /*  Adjust minimum weight if using cost weighting  */

    if ( CostWeights )
    {
	ForEach(c, 1, MaxClass)
	{
	    if ( WeightMul[c] < MinWt ) MinWt = WeightMul[c];
	}
    }

    LEAFRATIO = Bp = 0;
    SetMinGainThresh();

    /*  Main loop for growing the sequence of boosted classifiers  */

    ForEach(Trial, 0, TRIALS-1 )
    {
	if ( TRIALS > 1 )
	{
	    fprintf(Of, "\n-----  " F_Trial " %d:  -----\n", Trial);
	}

	NotifyStage(FORMTREE);
	Progress(-(MaxCase+1.0));

	/*  Update count here in case tree construction is interrupted  */

	MaxTree = Trial;
	Raw[MaxTree] = Pruned[MaxTree] = Nil;
	if ( RULES ) RuleSet[MaxTree] = Nil;

	memset(Tested, 0, MaxAtt+1);		/* reset tested attributes */

	// Daniel: Use the new version of formtree (the one that splits PC first)
	MyFormTree(Bp, MaxCase, 0, &Raw[Trial]);

	/*  Prune the raw tree to minimise expected misclassification cost  */

	Verbosity(1, if ( ! RULES ) PrintTree(Raw[Trial], "Before pruning:"))

	NotifyStage(SIMPLIFYTREE);
	Progress(-(MaxCase+1));

	/*  If still need raw tree, copy it; otherwise set initial
	    pruned tree to raw tree  */

	if ( VERBOSITY && ! RULES )
	{
	    Pruned[Trial] = CopyTree(Raw[Trial]);
	    if ( MCost )
	    {
		RestoreDistribs(Raw[Trial]);
	    }
	}
	else
	{
	    Pruned[Trial] = Raw[Trial];
	    Raw[Trial] = Nil;
	}

	memcpy(Case, SaveCase, (MaxCase+1) * sizeof(DataRec)); /* restore */

	// Pranav: Turn off pruning
	//Prune(Pruned[Trial]);

	AdjustAllThresholds(Pruned[Trial]);

	/*  Record tree parameters for later  */

	if ( ! Trial )
	{
	    BaseLeaves = ( RULES || SUBSET ? TreeSize(Pruned[0]) :
					     ExpandedLeafCount(Pruned[0]) );
	}
	NoStructure = ! Pruned[Trial]->NodeType;

	if ( PROBTHRESH )
	{
	    assert (false);
	    SoftenThresh(Pruned[Trial]);
	}

	memcpy(Case, SaveCase, (MaxCase+1) * sizeof(DataRec)); /* restore */

	if ( RULES )
	{
	    RuleSet[Trial] = FormRules(Pruned[Trial]);
	    NoStructure |= ! RuleSet[Trial]->SNRules;

	    PrintRules(RuleSet[Trial], T_Rules);
	    fprintf(Of, "\n" T_Default_class ": %s\n",
			ClassName[RuleSet[Trial]->SDefault]);

	    FreeTree(Pruned[Trial]);			Pruned[Trial] = Nil;
	}
	else
	{
	    PrintTree(Pruned[Trial], T_Tree);
	}

#if DEBUG
	printf ("\n========== DEBUG ==========\n");
	verify (Pruned[Trial], DebugCase, DebugImplications, MaxCase);
	printf ("Tree passed!\n");
	printf ("========== END ==========\n\n");
#endif

	// Daniel
	// Serialize tree to JSON
	const char * ser = serialize_to_JSON (Pruned[Trial]);

	FILE * JSONfp;
	if ( ! (JSONfp = GetFile(".json", "w")) )
    	{
            Error(NOFILE, "", "");
        }

	fprintf (JSONfp, ser);
	fclose(JSONfp);
	free (ser);

	
	if ( Trial == TRIALS-1 ) continue;

	/*  Check errors, adjust boost voting, and shift dropped cases
	    to the front  */

	assert (false);

	ErrWt = Errs = OKWt = Bp = 0;
	CheckExcl = ( Trial+1 > TRIALS / 2.0 );

	ForEach(i, 0, MaxCase)
	{
	    /*  Has this case been dropped already?  */

	    if ( Weight(Case[i]) <= 0 )
	    {
		Case[i]  = Case[Bp];
		Wrong[i] = Wrong[Bp];
		Bp++;
		continue;
	    }

	    Pred = ( RULES ? RuleClassify(Case[i], RuleSet[Trial]) :
			     TreeClassify(Case[i], Pruned[Trial]) );

	    Real = Class(Case[i]);

	    /*  Update boosting votes for this case.  (Note that cases
		must have been reset to their original order.)  */

	    BVote = BVoteBlock + i * (MaxClass+1);
	    BVote[Pred] += Confidence;

	    Best = BVote[0];
	    if ( BVote[Pred] > BVote[Best] ) BVote[0] = Best = Pred;

	    if ( CheckExcl )
	    {
		/*  Check whether this case should be dropped because
		    the vote for the correct class cannot be increased
		    sufficiently in the remaining trials  */

		if ( BVote[Best] > BVote[Real] + (TRIALS-1) - Trial )
		{
		    Excl++;
		    ExclWt += Weight(Case[i]);

		    Weight(Case[i]) = 0;
		    Case[i]  = Case[Bp];
		    Wrong[i] = Wrong[Bp];
		    Bp++;

		    continue;
		}
	    }

	    if ( Pred != Real )
	    {
		Wrong[i] = Pred;
		ErrWt   += Weight(Case[i]);
		Errs++;
	    }
	    else
	    {
		Wrong[i] = 0;
		OKWt    += Weight(Case[i]);
	    }
	}

	Cases  = (MaxCase+1) - Excl;

	/*  Special termination conditions  */

	if ( ErrWt < 0.1 )
	{
	    TRIALS = Trial + 1;
	    fprintf(Of, TX_Reduced1(TRIALS), TRIALS);
	}
	else
	if ( Trial && NoStructure || ErrWt / Cases >= 0.49 )
	{
	    TRIALS = ( Trial ? Trial : 1 );
	    fprintf(Of, TX_Reduced2(TRIALS), TRIALS);
	}
	else
	{
	    /*  Adjust case weights.  Total weight of misclassified cases
		set to midway between current weight and half total weight.
		Take account of any dropped cases  */

	    ExtraErrWt = 0.25 * (OKWt - ErrWt);		/* half */
	    a = (OKWt - ExtraErrWt) / OKWt;
	    b = ExtraErrWt / Errs;

	    /*  Normalise so that sum of weights equals number of cases  */

	    NFact = Cases / (OKWt + ErrWt);

	    MinWt *= a * NFact;

	    ForEach(i, Bp, MaxCase)
	    {
		if ( Wrong[i] )
		{
		    Weight(Case[i]) = NFact * (Weight(Case[i]) + b);
		}
		else
		{
		    Weight(Case[i]) *= NFact * a;

		    /*  Necessary for accumulated arithmetic errors  */

		    if ( Weight(Case[i]) < 1E-3 ) Weight(Case[i]) = 1E-3;
		}
	    }

	    /*  Set the leaf ratio for subsequent boosting trials.
		The idea is to constrain the tree to roughly the size
		of the initial tree by limiting the number of leaves
		per training case.  This limitation is not strict
		since even a tiny number of cases can give a leaf  */

	    if ( Trial == 0 )
	    {
		LEAFRATIO = 1.1 * BaseLeaves / (MaxCase + 1.0);
	    }

	    /*  Trim cases for larger datasets  */

	    if ( MaxCase > 4000 && MinWt <= 0.2 )
	    {
		a = 0;
		ForEach(i, Bp, MaxCase)
		{
		    if ( Weight(Case[i]) <= MinWt + 1E-3 )
		    {
			a += Weight(Case[i]);
			Swap(i, Bp);
			Bp++;
		    }
		}
	    }
	}

	UnitWeights = false;
    }

    FreeUnlessNil(SaveCase);				SaveCase = Nil;

    /*  Decide whether boosting should be abandoned  */

    if ( BOOST && TRIALS <= 2 )
    {
	fprintf(Of, T_Abandoned);
	TRIALS = 1;
    }

    /*  Save trees or rulesets  */

    if ( ! XVAL )
    {
	if ( ! RULES )
	{
	    ForEach(Trial, 0, TRIALS-1)
	    {
		SaveTree(Pruned[Trial], ".tree");
	    }
	}
	else
	{
	    ForEach(Trial, 0, TRIALS-1)
	    {
		SaveRules(RuleSet[Trial], ".rules");
	    }
	}

	fclose(TRf);
    }
    TRf = 0;

    Free(Wrong);					Wrong = Nil;
    FreeUnlessNil(BVoteBlock);				BVoteBlock = Nil;
}



/*************************************************************************/
/*								 	 */
/*	Initialise the weight of each case				 */
/*								 	 */
/*************************************************************************/


void InitialiseWeights()
/*   -----------------  */
{
    CaseNo	i;

    if ( CostWeights )
    {
	/*  Make weights proportional to average error cost  */

	ForEach(i, 0, MaxCase)
	{
	    Weight(Case[i]) = WeightMul[Class(Case[i])];
	}
	UnitWeights = false;
    }
    else
    {
	ForEach(i, 0, MaxCase)
	{
	    Weight(Case[i]) = 1.0;
	}
	UnitWeights = true;
    }

    /*  Adjust when using case weights  */

    if ( CWtAtt )
    {
	ForEach(i, 0, MaxCase)
	{
	    Weight(Case[i]) *= RelCWt(Case[i]);
	}
	UnitWeights = false;
    }
}



/*************************************************************************/
/*								 	 */
/*	Determine average case weight, ignoring cases with unknown,	 */
/*	non-applicable, or negative values of CWtAtt.			 */
/*								 	 */
/*************************************************************************/


void SetAvCWt()
/*   --------  */
{
    CaseNo	i, NCWt=0;
    ContValue	CWt;

    AvCWt = 0;
    ForEach(i, 0, MaxCase)
    {
	if ( ! NotApplic(Case[i], CWtAtt) && ! Unknown(Case[i], CWtAtt) &&
	     (CWt = CVal(Case[i], CWtAtt)) > 0 )
	{
	    NCWt++;
	    AvCWt += CWt;
	}
    }

    AvCWt = ( NCWt > 0 ? AvCWt / NCWt : 1 );
}



/*************************************************************************/
/*									 */
/*	Print report of errors for each of the trials			 */
/*									 */
/*************************************************************************/

char *Multi[]  = {	F_Trial,
			F_UTrial,
			"" },

     *StdR[]   = {	"   Before Pruning   ",
			"  ----------------  ",
			"  " F_SizeErrors "  " },

     *StdP[]   = {	"  " F_DecisionTree16 "  ",
			"  ----------------  ",
			"  " F_SizeErrors "  " },

     *StdPC[]  = {	"  " F_DecisionTree23 "  ",
			"  -----------------------  ",
			"  " F_SizeErrorsCost "  " },

     *Extra[]  = {	"  " F_Rules16,
			"  ----------------",
			"  " F_NoErrors },

     *ExtraC[] = {	"  " F_Rules23,
			"  -----------------------",
			"  " F_NoErrorsCost };


void Evaluate(int Flags)
/*   --------  */
{
    if ( TRIALS == 1 )
    {
	EvaluateSingle(Flags);
    }
    else
    {
	EvaluateBoost(Flags);
    }
}



void EvaluateSingle(int Flags)
/*   --------------  */
{
    ClassNo	RealClass, PredClass;
    int		x, u, SaveUtility;
    CaseNo	*ConfusionMat, *Usage, i, RawErrs=0, Errs=0;
    double	ECost=0, Tests;
    Boolean	CMInfo, UsageInfo;

    CMInfo    = Flags & CMINFO;
    UsageInfo = Flags & USAGEINFO;

    if ( CMInfo )
    {
	ConfusionMat = AllocZero((MaxClass+1)*(MaxClass+1), CaseNo);
    }

    if ( UsageInfo )
    {
	Usage = AllocZero(MaxAtt+1, CaseNo);
    }

    Tests = Max(MaxCase+1, 1);	/* in case no useful test data! */

    if ( UTILITY && RULES )
    {
	assert (false);
	SaveUtility = UTILITY;

	UTILITY = Min(UTILITY, RuleSet[0]->SNRules);

	UtilErr  = AllocZero(UTILITY, int);
	UtilBand = Alloc(UTILITY, int);
	if ( MCost )
	{
	    UtilCost = AllocZero(UTILITY, double);
	}

	ForEach(u, 1, UTILITY-1)
	{
	    UtilBand[u] = rint(RuleSet[0]->SNRules * u / (float) UTILITY);
	}
    }
	    
    fprintf(Of, "\n");
    ForEach(x, 0, 2)
    {
	putc('\t', Of);
	if ( RULES )
	{
	    fprintf(Of, "%s", ( MCost ? ExtraC[x] : Extra[x] ));
	}
	else
	{
	    Verbosity(1, fprintf(Of, "%s", StdR[x]))
	    fprintf(Of, "%s", ( MCost ? StdPC[x] : StdP[x] ));
	}
	putc('\n', Of);
    }
    putc('\n', Of);

    ForEach(i, 0, MaxCase)
    {
	RealClass = Class(Case[i]);
	// TODO Pranav: Uncomment the following comment after the code that propogates 
	// class for UNKNOWN points has been intergrated.
	//assert(RealClass > 0 && RealClass <= MaxClass);

	memset(Tested, 0, MaxAtt+1);	/* for usage */

	if ( RULES )
	{
	    PredClass = RuleClassify(Case[i], RuleSet[0]);
	}
	else
	{
	    Verbosity(1,
		PredClass = TreeClassify(Case[i], Raw[0]);
		// Pranav: Error only if a data point with True/False class value has a different class value 
		// after construction of the tree. RealClass and PredClass can differ if RealClass was UNKNOWN.
		if ( RealClass > 0 && PredClass != RealClass )
		{
		    // Pranav: Debug output in case of errors
                    printf("Erroneous point: %f, %f, %f\n", Case[i][1]._cont_val, Case[i][2]._cont_val, Case[i][3]._cont_val);
                    printf("RealClass: %d PredClass: %d\n\n", RealClass, PredClass);
		    RawErrs++;
		})

	    PredClass = TreeClassify(Case[i], Pruned[0]);
	}
	assert(PredClass > 0 && PredClass <= MaxClass);

	if ( RealClass > 0 && PredClass != RealClass )
	{
	    Errs++;
	    if ( MCost ) ECost += MCost[PredClass][RealClass];
	}

	if ( CMInfo )
	{
	    ConfusionMat[RealClass*(MaxClass+1)+PredClass]++;
	}

	if ( UsageInfo )
	{
	    RecordAttUsage(Case[i], Usage);
	}
    }

    putc('\t', Of);

    if ( RULES )
    {
	fprintf(Of, "  %4d %4d(%4.1f%%)",
	       RuleSet[0]->SNRules, Errs, 100 * Errs / Tests);
    }
    else
    {
	/*  Results for unpruned tree  */

	Verbosity(1,
	{
	    fprintf(Of, " %3d %3d %4d(%4.1f%%)  ",
		   TotalTreeSize(Raw[0]), TreeSize(Raw[0]), RawErrs, 100 * RawErrs / Tests);
	})

	/*  Results for pruned tree  */

	int total_tree_size = TotalTreeSize(Pruned[0]);
	fprintf(Of, " %3d %3d %4d(%4.1f%%)",
	       total_tree_size, TreeSize(Pruned[0]), Errs, 100 * Errs / Tests);

	FILE *F = GetFile(".out", "a");
	assert(F != NULL);
	fprintf(F, "%d\n", total_tree_size);
	fclose(F);


	// Pranav: Debug outut in case there are errors.
        //printf("RawErrs: %d  Errs: %d\n", RawErrs, Errs);
        assert (RawErrs <= 0);
        assert (Errs <= 0);

    }

    if ( MCost )
    {
	fprintf(Of, "%7.2f", ECost / Tests);
    }

    fprintf(Of, "   <<\n");

    if ( CMInfo )
    {
	PrintConfusionMatrix(ConfusionMat);
	Free(ConfusionMat);
    }

    if ( UsageInfo )
    {
	PrintUsageInfo(Usage);
	Free(Usage);
    }

    if ( UtilErr )
    {
	if ( ! XVAL )
	{
	    fprintf(Of, "\n" T_Rule_utility_summary ":\n\n"
			"\t" F_Rules "\t      " F_Errors "%s\n"
			"\t" F_URules "\t      " F_UErrors "%s\n",
			    ( MCost ? "   " F_Cost : "" ),
			    ( MCost ? "   " F_UCost : "" ));

	    ForEach(u, 1, UTILITY-1)
	    {
		fprintf(Of, "\t%s%d\t %4d(%4.1f%%)",
			    ( UtilBand[u] == 1 ? "" : "1-" ), UtilBand[u],
			    UtilErr[u], 100 * UtilErr[u] / Tests);
		if ( MCost )
		{
		    fprintf(Of, "%7.2f", UtilCost[u] / Tests);
		}
		fprintf(Of, "\n");
	    }
	}

	Free(UtilErr);					UtilErr = Nil;
	FreeUnlessNil(UtilCost);			UtilCost = Nil;
	Free(UtilBand);					UtilBand = Nil;

	UTILITY = SaveUtility;
    }
}



void EvaluateBoost(int Flags)
/*   -------------  */
{
    ClassNo	RealClass, PredClass;
    int		t;
    CaseNo	*ConfusionMat, *Usage, i, *Errs, BoostErrs=0;
    double	*ECost, BoostECost=0, Tests;
    Boolean	CMInfo, UsageInfo;

    CMInfo    = Flags & CMINFO;
    UsageInfo = Flags & USAGEINFO;

    if ( CMInfo )
    {
	ConfusionMat = AllocZero((MaxClass+1)*(MaxClass+1), CaseNo);
    }

    if ( UsageInfo )
    {
	Usage = AllocZero(MaxAtt+1, CaseNo);
    }

    Tests = Max(MaxCase+1, 1);	/* in case no useful test data! */
    Errs = AllocZero(TRIALS, CaseNo);
    ECost = AllocZero(TRIALS, double);

    fprintf(Of, "\n");
    ForEach(t, 0, 2)
    {
	fprintf(Of, "%s\t", Multi[t]);
	if ( RULES )
	{
	    fprintf(Of, "%s", ( MCost ? ExtraC[t] : Extra[t] ));
	}
	else
	{
	    fprintf(Of, "%s", ( MCost ? StdPC[t] : StdP[t] ));
	}
	putc('\n', Of);
    }
    putc('\n', Of);

    /*  Set global default class for boosting  */

    Default = ( RULES ? RuleSet[0]->SDefault : Pruned[0]->Leaf );

    ForEach(i, 0, MaxCase)
    {
	RealClass = Class(Case[i]);

	memset(Tested, 0, MaxAtt+1);	/* for usage */

	PredClass = BoostClassify(Case[i], TRIALS-1);
	if ( PredClass != RealClass )
	{
	    BoostErrs++;
	    if ( MCost ) BoostECost += MCost[PredClass][RealClass];
	}

	if ( CMInfo )
	{
	    ConfusionMat[RealClass*(MaxClass+1)+PredClass]++;
	}

	if ( UsageInfo )
	{
	    RecordAttUsage(Case[i], Usage);
	}

	/*  Keep track of results for each trial  */

	ForEach(t, 0, TRIALS-1)
	{
	    if ( TrialPred[t] != RealClass )
	    {
		Errs[t]++;
		if ( MCost ) ECost[t] += MCost[TrialPred[t]][RealClass];
	    }
	}
    }

    /*  Print results for individual trials  */

    ForEach(t, 0, TRIALS-1)
    {
	fprintf(Of, "%4d\t", t);

	if ( RULES )
	{
	    fprintf(Of, "  %4d %4d(%4.1f%%)",
		   RuleSet[t]->SNRules, Errs[t], 100 * Errs[t] / Tests);
	}
	else
	{
	    fprintf(Of, "  %4d %4d(%4.1f%%)",
		   TreeSize(Pruned[t]), Errs[t], 100 * Errs[t] / Tests);
	}

	if ( MCost )
	{
	    fprintf(Of, "%7.2f", ECost[t] / Tests);
	}

	putc('\n', Of);
    }

    /*  Print boosted results  */

    if ( RULES )
    {
	fprintf(Of, F_Boost "\t  %9d(%4.1f%%)",
	    BoostErrs, 100 * BoostErrs / Tests);
    }
    else
    {
	fprintf(Of, F_Boost "\t       %4d(%4.1f%%)",
		BoostErrs, 100 * BoostErrs / Tests);
    }

    if ( MCost )
    {
	fprintf(Of, "%7.2f", BoostECost / Tests);
    }

    fprintf(Of, "   <<\n");

    if ( CMInfo )
    {
	PrintConfusionMatrix(ConfusionMat);
	Free(ConfusionMat);
    }

    if ( UsageInfo )
    {
	PrintUsageInfo(Usage);
	Free(Usage);
    }

    Free(Errs);
    Free(ECost);
}



/*************************************************************************/
/*								 	 */
/*	Record atts used when classifying last case			 */
/*								 	 */
/*************************************************************************/


void RecordAttUsage(DataRec Case, int *Usage)
/*   --------------  */
{
    Attribute	Att;
    int		i;

    /*  Scan backwards to allow for information from defined attributes  */

    for ( Att = MaxAtt ; Att > 0 ; Att-- )
    {
	if ( Tested[Att] && ! Unknown(Case, Att) )
	{
	    Usage[Att]++;

	    if ( AttDef[Att] )
	    {
		ForEach(i, 1, AttDefUses[Att][0])
		{
		    Tested[AttDefUses[Att][i]] = true;
		}
	    }
	}
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*								 	 */
/*    Central tree-forming algorithm					 */
/*    ------------------------------					 */
/*								 	 */
/*************************************************************************/



#include "cpp_wrapper/cmap.h"

Boolean		MultiVal,	/* all atts have many values */
		Subsample;	/* use subsampling */
float		AvGainWt,	/* weight of average gain in gain threshold */
		MDLWt;		/* weight of MDL threshold ditto */

Attribute	*DList=Nil;	/* list of discrete atts */
int		NDList;		/* number in list */

DiscrValue	MaxLeaves;	/* target maximum tree size */

#define		SAMPLEUNIT	2000

float		ValThresh;	/* minimum GR when evaluating sampled atts */
Boolean		Sampled;	/* true if sampling used */

Attribute	*Waiting=Nil,	/* attribute wait list */
		NWaiting=0;




/*************************************************************************/
/*								 	 */
/*	Allocate space for tree tables				 	 */
/*								 	 */
/*************************************************************************/


void InitialiseTreeData()
/*   ------------------  */
{
    DiscrValue	v;
    Attribute	Att;
    DiscrValue	vMax;

    Raw	     = AllocZero(TRIALS+1, Tree);
    Pruned   = AllocZero(TRIALS+1, Tree);

    Tested   = AllocZero(MaxAtt+1, Byte);

    Gain     = AllocZero(MaxAtt+1, float);
    Info     = AllocZero(MaxAtt+1, float);
    Bar      = AllocZero(MaxAtt+1, ContValue);

    EstMaxGR = AllocZero(MaxAtt+1, float);

    /*  Data for subsets  */

    if ( SUBSET )
    {
	assert (false);

	InitialiseBellNumbers();
	Subset = Alloc(MaxAtt+1, Set *);

	ForEach(Att, 1, MaxAtt)
	{
	    if ( Discrete(Att) && Att != ClassAtt && ! Skip(Att) )
	    {
		Subset[Att] = AllocZero(MaxAttVal[Att]+1, Set);
		ForEach(v, 0, MaxAttVal[Att])
		{
		    Subset[Att][v] = Alloc((MaxAttVal[Att]>>3)+1, Byte);
		}
	    }
	}
	Subsets = AllocZero(MaxAtt+1, int);
    }

    DList  = Alloc(MaxAtt, Attribute);
    NDList = 0;

    DFreq = AllocZero(MaxAtt+1, double *);
    ForEach(Att, 1, MaxAtt)
    {
	if ( Att == ClassAtt || Skip(Att) || ! Discrete(Att) ) continue;

	DList[NDList++] = Att;

	DFreq[Att] = Alloc(MaxClass * (MaxAttVal[Att]+1), double);
    }

    ClassFreq = AllocZero(MaxClass+1, double);
    ClassSum  = Alloc(MaxClass+1, float);

    if ( BOOST )
    {
	Vote      = Alloc(MaxClass+1, float);
	TrialPred = Alloc(TRIALS, ClassNo);
    }

    if ( RULES )
    {
	MostSpec     = Alloc(MaxClass+1, CRule);
	PossibleCuts = Alloc(MaxAtt+1, int);
    }

    /*  Check whether all attributes have many discrete values  */

    MultiVal = true;
    if ( ! SUBSET )
    {
	for ( Att = 1 ; MultiVal && Att <= MaxAtt ; Att++ )
	{
	    if ( ! Skip(Att) && Att != ClassAtt )
	    {
		MultiVal = MaxAttVal[Att] >= 0.3 * (MaxCase + 1);
	    }
	}
    }

    /*  See whether there are continuous attributes for subsampling  */

    Subsample = false;

    /*  Set parameters for RawExtraErrs() */

    InitialiseExtraErrs();

    /*  Set up environment  */

    Waiting = Alloc(MaxAtt+1, Attribute);

    vMax = Max(3, MaxDiscrVal+1);

    GEnv.Freq = Alloc(vMax+1, double *);
    ForEach(v, 0, vMax)
    {
	GEnv.Freq[v] = Alloc(MaxClass+1, double);
    }

    GEnv.ValFreq = Alloc(vMax, double);

    GEnv.ClassFreq = Alloc(MaxClass+1, double);

    GEnv.SRec = Alloc(MaxCase+1, SortRec);

    // Pranav: Allocate the new *Implications-related* data structures.
    GEnv.Implications = 0;
    GEnv.permutation = new_array(MaxCase+1);
    GEnv.classAttr = new_array(MaxCase+1);

    if ( SUBSET )
    {
	GEnv.SubsetInfo = Alloc(MaxDiscrVal+1, double);
	GEnv.SubsetEntr = Alloc(MaxDiscrVal+1, double);

	GEnv.MergeInfo = Alloc(MaxDiscrVal+1, double *);
	GEnv.MergeEntr = Alloc(MaxDiscrVal+1, double *);
	GEnv.WSubset   = Alloc(MaxDiscrVal+1, Set);
	ForEach(v, 1, MaxDiscrVal)
	{
	    GEnv.MergeInfo[v] = Alloc(MaxDiscrVal+1, double);
	    GEnv.MergeEntr[v] = Alloc(MaxDiscrVal+1, double);
	    GEnv.WSubset[v]   = Alloc((MaxDiscrVal>>3)+1, Byte);
	}
    }
}


void FreeTreeData()
/*   ------------  */
{
    Attribute	Att;
    DiscrValue	vMax;

    FreeUnlessNil(Raw);					Raw = Nil;
    FreeUnlessNil(Pruned);				Pruned = Nil;

    FreeUnlessNil(Tested);				Tested = Nil;

    FreeUnlessNil(Gain);				Gain = Nil;
    FreeUnlessNil(Info);				Info = Nil;
    FreeUnlessNil(Bar);					Bar = Nil;

    FreeUnlessNil(EstMaxGR);				EstMaxGR = Nil;

    if ( SUBSET )
    {
	FreeVector((void **) Bell, 1, MaxDiscrVal);	Bell = Nil;

	if ( Subset )
	{
	    ForEach(Att, 1, MaxAtt)
	    {
		if ( Subset[Att] )
		{
		    FreeVector((void **) Subset[Att], 0, MaxAttVal[Att]);
		}
	    }
	    Free(Subset);				Subset = Nil;
	    Free(Subsets);				Subsets = Nil;
	}
    }

    FreeUnlessNil(DList);				DList = Nil;

    if ( DFreq )
    {
	ForEach(Att, 1, MaxAtt)
	{
	    FreeUnlessNil(DFreq[Att]);
	}

	Free(DFreq);					DFreq = Nil;
    }

    FreeUnlessNil(ClassFreq);				ClassFreq = Nil;
    FreeUnlessNil(ClassSum);				ClassSum = Nil;

    FreeUnlessNil(Vote);				Vote = Nil;
    FreeUnlessNil(TrialPred);				TrialPred = Nil;

    FreeUnlessNil(MostSpec);				MostSpec = Nil;
    FreeUnlessNil(PossibleCuts);			PossibleCuts = Nil;

    vMax = Max(3, MaxDiscrVal+1);
    FreeVector((void **) GEnv.Freq, 0, vMax);
    Free(GEnv.ValFreq);
    Free(GEnv.ClassFreq);
    FreeUnlessNil(GEnv.SRec);

    delete_cmap(GEnv.Implications);
    delete_array(GEnv.permutation);
    delete_array(GEnv.classAttr);

    if ( GEnv.SubsetInfo )
    {
	Free(GEnv.SubsetInfo);
	Free(GEnv.SubsetEntr);
	FreeVector((void **) GEnv.MergeInfo, 1, MaxDiscrVal);
	FreeVector((void **) GEnv.MergeEntr, 1, MaxDiscrVal);
	FreeVector((void **) GEnv.WSubset, 1, MaxDiscrVal);
    }

    FreeUnlessNil(Waiting);				Waiting = Nil;
}



/*************************************************************************/
/*									 */
/*	Set threshold on minimum gain as follows:			 */
/*	  * when forming winnowing tree: no minimum			 */
/*	  * for small problems, AvGain (usual Gain Ratio)		 */
/*	  * for large problems, discounted MDL				 */
/*	  * for intermediate problems, interpolated			 */
/*									 */
/*************************************************************************/


void SetMinGainThresh()
/*   ----------------  */
{
    float	Frac;

    /*  Set AvGainWt and MDLWt  */

    if ( Now == WINNOWATTS )
    {
	AvGainWt = MDLWt = 0.0;
    }
    else
    if ( (MaxCase+1) / MaxClass <= 500 )
    {
	AvGainWt = 1.0;
	MDLWt    = 0.0;
    }
    else
    if ( (MaxCase+1) / MaxClass >= 1000 )
    {
	AvGainWt = 0.0;
	MDLWt    = 0.9;
    }
    else
    {
	Frac = ((MaxCase+1) / MaxClass - 500) / 500.0;

	AvGainWt = 1 - Frac;
	MDLWt    = 0.9 * Frac;
    }
}


/*
 * Defers the construction of the tree by one new root node which splits on the first attribute
 * (assuming that it is discrete). If no intervals are given, this method falls back to the
 * original FormTree method.
 */
void MyFormTree(CaseNo Fp, CaseNo Lp, int Level, Tree * Result)
{

	// Sanity check
	if (Fp > Lp) {
	
		printf ("This version of C5 does not support constructing a tree from empty data!\n");
		exit (EXIT_FAILURE);
	
	}

	/*
	 * If no intervals are give, fall back to original C5
	 */
	if (IntervalsUpperBounds == NULL || IntervalsUpperBounds->size == 0) {

		Verbosity(1, 
			fprintf(Of, "No intervals given, falling back to original C5 ICE"));

		FormTree(Fp, Lp, Level, Result, MaxAtt, 1);

	}

	/*
	 * Otherwise, create new root and call original C5 on children
	 */
	else {

		Attribute FirstSplitAttr = 1;

		int i;	
		
		// Make sure the first split attribute is a discrete attribute
		if (!Discrete (FirstSplitAttr)) {

			printf ("The first attribute has to be a discrete attribute!\n");
			exit (EXIT_FAILURE);

		}		

		// Make sure that there are as many intervals as discrete attribute values
		if (IntervalsLowerBounds->size != MaxAttVal[FirstSplitAttr] - 1) {

			printf ("The number of discrete attribute (%d) values and the number of intervals (%d) does not match!\n", MaxAttVal[FirstSplitAttr] - 1, IntervalsLowerBounds->size);
			exit (EXIT_FAILURE);

		}
		
	
		/*
		 * Sort cases on FirstSplitAttr
		 */
		struct array * permutation = new_array (MaxCase + 1);
		ForEach (i, 0, MaxCase) {
			permutation->entries[i] = i;
		}
		QuicksortWithImplications(Fp, Lp, FirstSplitAttr, permutation);
		struct array * inverse_permutation = array_invert(permutation);
		struct cmap * sortedImplications = cmap_copy_and_rename(Implications, inverse_permutation);
		delete_cmap (Implications);
		Implications = sortedImplications;
		delete_array (permutation);
		delete_array (inverse_permutation);
		
		/*
		 * Store upper and lower index of cases with given PC (index 0 is ?, we don't want to use that)
		 */
		 CaseNo LowerCase[MaxCase + 1];
		memset (LowerCase, 0, (MaxAttVal[FirstSplitAttr] + 1)  * sizeof(CaseNo));
		CaseNo UpperCase[MaxAttVal[FirstSplitAttr]];
		memset (UpperCase, 0, (MaxAttVal[FirstSplitAttr] + 1)  * sizeof(CaseNo));
		char AttValueFound[MaxAttVal[FirstSplitAttr]];
		memset (AttValueFound, 0, (MaxAttVal[FirstSplitAttr] + 1) * sizeof(char));

		DiscrValue cur = DVal(Case[Fp], FirstSplitAttr);
		AttValueFound[cur] = 1;
		LowerCase[cur] = Fp;
		ForEach (i, Fp + 1, Lp) {

			assert (i > 0 ? DVal(Case[i], FirstSplitAttr) >= DVal(Case[i - 1], FirstSplitAttr) : 1);

			DiscrValue v = DVal(Case[i], FirstSplitAttr);

			if (v != cur) {

				assert (!AttValueFound[v]);
				
				AttValueFound[v] = 1;
				LowerCase[v] = i;
				UpperCase[cur] = i - 1;
				cur = v;

			}

		}
		UpperCase[cur] = Lp;

#ifdef DEBUG
		ForEach (i, 1, MaxAttVal[FirstSplitAttr]) {
		
			printf ("%s=%d (%s), enabled=%s: [%d, %d]\n", AttName[FirstSplitAttr], i, AttValName[FirstSplitAttr][i], AttValueFound[i] ? "yes" : "no", LowerCase[i], UpperCase[i]);
		
		}
#endif

		/*
		 * Construct tree
		 */
		// Let us create a new root node
		Tree Node;
		*Result = Node = Leaf(Nil, 0, 0.0, 0.0);
		DiscreteTest (Node, FirstSplitAttr);
		
		// Recursively construct tree
		ForEach(i, 1, Node->Forks) {

			// Only construct subtree if attribute value is present
			if (AttValueFound[i]) {
				FormTree(LowerCase[i], UpperCase[i], Level + 1, &Node->Branch[i], IntervalsUpperBounds->entries[i - 2], IntervalsLowerBounds->entries[i - 2]);
			}
			
			// Create a leaf with classified with the first attribute vale (whatever that might be)
			else {
				Node->Branch[i] = Leaf(Nil, 1, 0.0, 0.0);
			}

		}

	}

}


/*************************************************************************/
/*								 	 */
/*	Build a decision tree for the cases Fp through Lp	 	 */
/*								 	 */
/*    - if all cases are of the same class, the tree is a leaf labelled	 */
/*	with this class						 	 */
/*								 	 */
/*    - for each attribute, calculate the potential information provided */
/*	by a test on the attribute (based on the probabilities of each	 */
/*	case having a particular value for the attribute), and the gain	 */
/*	in information that would result from a test on the attribute	 */
/*	(based on the probabilities of each case with a particular	 */
/*	value for the attribute being of a particular class)		 */
/*								 	 */
/*    - on the basis of these figures, and depending on the current	 */
/*	selection criterion, find the best attribute to branch on. 	 */
/*	Note:  this version will not allow a split on an attribute	 */
/*	unless two or more subsets have at least MINITEMS cases 	 */
/*								 	 */
/*    - try branching and test whether the resulting tree is better than */
/*	forming a leaf						 	 */
/*								 	 */
/*************************************************************************/
void FormTree(CaseNo Fp, CaseNo Lp, int Level, Tree *Result, Attribute upper, Attribute lower)
/*   --------  */
{
    CaseCount	Cases=0, TreeErrs=0;
    Attribute	BestAtt;
    ClassNo	c, BestLeaf=1, Least=1;
    Tree	Node;
    DiscrValue	v;


    assert(Fp >= 0 && Lp >= Fp && Lp <= MaxCase);

    /*  Make a single pass through the cases to determine class frequencies
	and value/class frequencies for all discrete attributes  */

    FindAllFreq(Fp, Lp);

    /*  Choose the best leaf and the least prevalent class  */

    ForEach(c, 2, MaxClass)
    {
	if ( ClassFreq[c] > ClassFreq[BestLeaf] )
	{
	    BestLeaf = c;
	}
	else
	if ( ClassFreq[c] > 0.1 && ClassFreq[c] < ClassFreq[Least] )
	{
	    Least = c;
	}
    }

    /*  Ignoring the cases which have UNKNOWN class values while computing
        the total Cases.  */
    ForEach(c, 1, MaxClass)
    {
	Cases += ClassFreq[c];
    }

    MaxLeaves = ( LEAFRATIO > 0 ? rint(LEAFRATIO * Cases) : 1E6 );

    *Result = Node =
	Leaf(ClassFreq, BestLeaf, Cases, Cases - ClassFreq[BestLeaf]);

    Verbosity(1,
    	fprintf(Of, "\n<%d> %d cases", Level, No(Fp,Lp));
	if ( fabs(No(Fp,Lp) - Cases) >= 0.1 )
	{
	    fprintf(Of, ", total weight %.1f", Cases);
	}
	fprintf(Of, "\n"))

    /*  Do not try to split if:
	- all cases are of the same class
	- there are not enough cases to split  */

    // Pranav: Return only if there are no errors; otherwise split further. 
    //if ( ClassFreq[BestLeaf] >= 0.999 * Cases  ||
    if ( ClassFreq[BestLeaf] >= Cases  ||
	 Cases < 2 * MINITEMS ||
	 MaxLeaves < 2 )
    {
	if ( Now == FORMTREE ) Progress(Cases);

	// Pranav: Propagate the classification at the leaf to even nodes labeled unknown.

	int numAllCases = Lp - Fp + 1;
	int numUnknownClassifiedCases = numAllCases - Cases ;

	if ( numUnknownClassifiedCases > 0 )
	{

	    struct array * unknownClassifiedCases = new_array(numUnknownClassifiedCases);

	    int i, j = 0;

            ForEach(i, Fp, Lp)
            {
		#if false
		printf("Class Attr of Case %d: %d\n", i, Class(Case[i]));
		printf("Class Attr of Case %d: %d\n", i, DVal(Case[i], ClassAtt));
		#endif
	    	if ( Class(Case[i]) == 0 )	
	    	{
		    assert(DVal(Case[i], ClassAtt) == 0);
		    unknownClassifiedCases->entries[j] = i;
		    j++;
	        }
		else
		    assert(DVal(Case[i], ClassAtt) != 0);
	    }

	    assert (j == numUnknownClassifiedCases) ;

	    int success = cmap_propagate(unknownClassifiedCases, Implications, BestLeaf, & assignClass);

	    assert (success > 0);

	    delete_array(unknownClassifiedCases);
	}

	return;
    }

    /*  Calculate base information  */

    GlobalBaseInfo = TotalInfo(ClassFreq, 1, MaxClass) / Cases;

    /*  Perform preliminary evaluation if using subsampling.
	Must expect at least 10 of least prevalent class  */

    ValThresh = 0;
    if ( Subsample && No(Fp, Lp) > 5 * MaxClass * SAMPLEUNIT &&
	 (ClassFreq[Least] * MaxClass * SAMPLEUNIT) / No(Fp, Lp) >= 10 )
    {
	assert (false);

	SampleEstimate(Fp, Lp, Cases);
	Sampled   = true;
    }
    else
    {
	Sampled = false;
    }

    BestAtt = ChooseSplit(Fp, Lp, Cases, Sampled, upper, lower);
	assert (lower <= BestAtt && BestAtt <= upper);
	
    /*  Decide whether to branch or not  */

    if ( BestAtt == None )
    {
	Verbosity(1, fprintf(Of, "\tno sensible splits\n"))
	if ( Now == FORMTREE ) Progress(Cases);
    }
    else
    {
	Verbosity(1,
	    fprintf(Of, "\tbest attribute %s", AttName[BestAtt]);
	    if ( Continuous(BestAtt) )
	    {
		fprintf(Of, " cut %.3f", Bar[BestAtt]);
	    }
	    fprintf(Of, " inf %.3f gain %.3f val %.3f\n",
		   Info[BestAtt], Gain[BestAtt], Gain[BestAtt] / Info[BestAtt]))

	/*  Build a node of the selected test  */

	if ( Discrete(BestAtt) )
	{
	    printf("BestAtt is: %d %s\n", BestAtt, AttName[BestAtt]);
	    assert (false);

	    if ( SUBSET && MaxAttVal[BestAtt] > 3 && ! Ordered(BestAtt) )
	    {
		SubsetTest(Node, BestAtt);
	    }
	    else
	    {
		DiscreteTest(Node, BestAtt);
	    }
	}
	else
	{
	    ContinTest(Node, BestAtt);
	}

	/*  Carry out the recursive divide-and-conquer  */

	++Tested[BestAtt];

	Divide(Node, Fp, Lp, Level, upper, lower);

	--Tested[BestAtt];

	/*  See whether we would have been no worse off with a leaf  */

	ForEach(v, 1, Node->Forks)
	{
	    TreeErrs += Node->Branch[v]->Errors;
	}

	if ( TreeErrs >= 0.999 * Node->Errors )
	{
	    assert (false);

	    Verbosity(1,
		fprintf(Of, "<%d> Collapse tree for %d cases to leaf %s\n",
			    Level, No(Fp,Lp), ClassName[BestLeaf]))

	    UnSprout(Node);
	}
	else
	{
	    Node->Errors = TreeErrs;
	}
    }
}



/*************************************************************************/
/*								 	 */
/*	Estimate Gain[] and Info[] using sample				 */
/*								 	 */
/*************************************************************************/


void SampleEstimate(CaseNo Fp, CaseNo Lp, CaseCount Cases)
/*   --------------  */
{
    CaseNo	SLp, SampleSize;
    CaseCount	NewCases;
    Attribute	Att;
    float	GR;

    /*  Phase 1: evaluate all discrete attributes and record best GR  */

    ForEach(Att, 1, MaxAtt)
    { 
	Gain[Att] = None;

	if ( Discrete(Att) )
	{
	    EvalDiscrSplit(Att, Cases);

	    if ( Info[Att] > Epsilon &&
		 (GR = Gain[Att] / Info[Att]) > ValThresh )
	    {
		ValThresh = GR;
	    }
	}
    }

    /*  Phase 2: generate sample  */

    SampleSize = MaxClass * SAMPLEUNIT;
    Sample(Fp, Lp, SampleSize);
    SLp = Fp + SampleSize - 1;

    /*  Phase 3: evaluate continuous attributes using sample  */

    NewCases   = CountCases(Fp, SLp);
    SampleFrac = NewCases / Cases;
    NWaiting   = 0;

    ForEach(Att, 1, MaxAtt) 
    { 
	if ( Continuous(Att) )
	{
	    Waiting[NWaiting++] = Att;
	}
    } 

    ProcessQueue(Fp, SLp, NewCases);

    SampleFrac = 1.0;
}



/*************************************************************************/
/*								 	 */
/*	Sample N cases from cases Fp through Lp				 */
/*								 	 */
/*************************************************************************/


void Sample(CaseNo Fp, CaseNo Lp, CaseNo N)
/*   ------  */
{
    CaseNo	i, j;
    double	Interval;

    Interval = No(Fp, Lp) / (double) N;

    ForEach(i, 0, N-1)
    {
	j = (i + 0.5) * Interval;

	assert(j >= 0 && Fp + j <= Lp);

	Swap(Fp + i, Fp + j);
    }
}



/*************************************************************************/
/*								 	 */
/*	Evaluate splits and choose best attribute to split on.		 */
/*	If Sampled, Gain[] and Info[] have been estimated on		 */
/*	sample and unlikely candidates are not evaluated on all cases	 */
/*							 	 */
/*************************************************************************/
// Daniel: upper and lower define an interval of attributes from which the split can be performed  

Attribute ChooseSplit(CaseNo Fp, CaseNo Lp, CaseCount Cases, Boolean Sampled, Attribute upper, Attribute lower)
/*        -----------  */
{
    Attribute	Att;
    int		i, j;


    /*  For each available attribute, find the information and gain  */

    NWaiting = 0;

    if ( Sampled )
    {
	assert (false);

	/*  If samples have been used, do not re-evaluate discrete atts
	    or atts that have low GR  */

	for ( Att = MaxAtt ; Att > 0 ; Att-- )
	{
	    if ( ! Continuous(Att) ) continue;

	    if ( EstMaxGR[Att] >= ValThresh )
	    {
		/*  Add attributes in reverse order of estimated max GR  */

		for ( i = 0 ;
		      i < NWaiting && EstMaxGR[Waiting[i]] < EstMaxGR[Att] ;
		      i++ )
		    ;

		for ( j = NWaiting-1 ; j >= i ; j-- )
		{
		    Waiting[j+1] = Waiting[j];
		}
		NWaiting++;

		Waiting[i] = Att;
	    }
	    else
	    {
		/*  Don't use -- attribute hasn't been fully evaluated.
		    Leave Gain unchanged to get correct count for Possible  */

		Info[Att] = -1E6;	/* negative so GR also negative */
	    }
	}
    }
    else
    {

	// Daniel: We only want to consider attributes within the allowed interval of attributes
	//for ( Att = MaxAtt ; Att > 0 ; Att-- )
	for ( Att = upper ; Att >= lower ; Att-- )
	{
	    Gain[Att] = None;

	    if ( Skip(Att) || Att == ClassAtt )
	    {
		continue;
	    }

		//printf ("Adding %s (%d) to worklist; lower is %d, upper is %d\n", AttName[Att], Att, lower, upper);
	    Waiting[NWaiting++] = Att;
	}
    }

    ProcessQueue(Fp, Lp, Cases);

    Attribute BestAttr = FindBestAttNew(Cases, upper, lower);
    assert (BestAttr != None);

    /*
    if(BestAttr == None)
    {
    	ProcessQueue(Fp, Lp, Cases);
	BestAttr = FindBestAttNew(Cases, upper, lower);
    }
    assert (BestAttr != None);
    */
    return BestAttr;
}


/*  Pranav: WCases is the total number of cases classified as TRUE or FALSE. Does not 
    include cases with UNKNOWN class attribute  */
void ProcessQueue(CaseNo WFp, CaseNo WLp, CaseCount WCases)
/*   ------------  */
{
    Attribute	Att;
    float	GR;

    for ( ; NWaiting > 0 ; )
    {
	Att = Waiting[--NWaiting];

	if ( Discrete(Att) )
	{
	    assert (false);
	    EvalDiscrSplit(Att, WCases);
	}
	else
	if ( SampleFrac < 1 )
	{
	    assert (false);
	    EstimateMaxGR(Att, WFp, WLp);
	}
	else
	if ( Sampled )
	{
	    assert (false);

	    Info[Att] = -1E16;

	    if ( EstMaxGR[Att] > ValThresh )
	    {
		EvalContinuousAtt(Att, WFp, WLp);

		if ( Info[Att] > Epsilon &&
		     (GR = Gain[Att] / Info[Att]) > ValThresh )
		{
		    if ( GR > ValThresh ) ValThresh = GR;
		}
	    }
	}
	else
	{
	    if (0)
	    	/* Ignores the implication data points while computing the entropy/gain 
	       	   for a particular cut. */
	        EvalContinuousAttAlg1(Att, WFp, WLp);

	    else if (0)

		/* Penalty for the number of Implications cut. */
	        EvalContinuousAttAlg2(Att, WFp, WLp);

#ifndef PENALTY
	    else if (1)

		/* Entropy for the Implication sample */
	        EvalContinuousAttAlg3(Att, WFp, WLp);
#endif

	    else if (0)
	        EvalContinuousAttAlg4(Att, WFp, WLp);
	
	    else if (0)
		/* Alg 2 + 3 */
	        EvalContinuousAttAlg5(Att, WFp, WLp);

#ifdef PENALTY
	    else
		/* Penalty for Implications cut but not those from neg -> pos */
	        EvalContinuousAttAlg6(Att, WFp, WLp);

#endif
	}
    }
}

/*************************************************************************/
/*								 	 */
/*	Adjust each attribute's gain to reflect choice and		 */
/*	select att with maximum GR					 */
/*								 	 */
/*************************************************************************/

#if false
Attribute FindBestAtt(CaseCount Cases)
/*	  -----------  */
{
    double	BestVal, Val, MinGain=1E6, AvGain=0, MDL;
    Attribute	Att, BestAtt, Possible=0;
    DiscrValue	NBr, BestNBr=MaxDiscrVal+1;

    ForEach(Att, 1, MaxAtt)
    {
	/*  Update the number of possible attributes for splitting and
	    average gain (unless very many values)  */

	// TODO Pranav: consider the gain regardless of the value of Gain, MaxAttVal, etc.
	if ( Gain[Att] >= Epsilon &&
	     ( MultiVal || MaxAttVal[Att] < 0.3 * (MaxCase + 1) ) )
	{
	    Possible++;
	    AvGain += Gain[Att];
	}
	else
	{
	    Gain[Att] = None;
	}
    }

    /*  Set threshold on minimum gain  */

    if ( ! Possible ) return None;

    AvGain /= Possible;
    MDL     = Log(Possible) / Cases;
    MinGain = AvGain * AvGainWt + MDL * MDLWt;

    Verbosity(2,
	fprintf(Of, "\tav gain=%.3f, MDL (%d) = %.3f, min=%.3f\n",
		    AvGain, Possible, MDL, MinGain))

    /*  Find best attribute according to Gain Ratio criterion subject
	to threshold on minimum gain  */

    // TODO Pranav: Set BestVal to -LargeVal to encourage it to be even negative.
    BestVal = -Epsilon;
    BestAtt = None;
    ForEach(Att, 1, MaxAtt)
    {
	// Pranav: Dont introduce errors if all splits have gain < MinGain.
	//if ( Gain[Att] >= 0.999 * MinGain && Info[Att] > 0 )
	if ( Info[Att] > 0 )
	{
	    Val = Gain[Att] / Info[Att];
	    NBr = ( MaxAttVal[Att] <= 3 || Ordered(Att) ? 3 :
		    SUBSET ? Subsets[Att] : MaxAttVal[Att] );

	    if ( Val > BestVal ||
		 Val > 0.999 * BestVal &&
		 ( NBr < BestNBr ||
		   NBr == BestNBr && Gain[Att] > Gain[BestAtt] ) )
	    {
		BestAtt = Att;
		BestVal = Val;
		BestNBr = NBr;
	    }
	}
    }

    return BestAtt;
}
#endif


/*************************************************************************/
/*								 	 */
/*	Adjust each attribute's gain to reflect choice and		 */
/*	select att with maximum GR					 */
/*								 	 */
/*************************************************************************/


Attribute FindBestAttNew(CaseCount Cases, Attribute upperBound, Attribute lowerBound)
/*	  -----------  */
{
    double	BestVal, Val, MinGain=1E6, AvGain=0, MDL;
    Attribute	Att, BestAtt, Possible=0;
    DiscrValue	NBr, BestNBr=MaxDiscrVal+1;

    ForEach(Att, lowerBound, upperBound)
    {
	/*  Update the number of possible attributes for splitting and
	    average gain (unless very many values)  */

	// Pranav: consider the gain regardless of the value of Gain, MaxAttVal, etc.
	if ( Gain[Att] >= Epsilon &&
	     ( MultiVal || MaxAttVal[Att] < 0.3 * (MaxCase + 1) ) )
	{
	    Possible++;
	    AvGain += Gain[Att];
	}
    }

    /*  Set threshold on minimum gain  */

    AvGain /= Possible;
    MDL     = Log(Possible) / Cases;
    MinGain = AvGain * AvGainWt + MDL * MDLWt;

    Verbosity(2,
	fprintf(Of, "\tav gain=%.3f, MDL (%d) = %.3f, min=%.3f\n",
		    AvGain, Possible, MDL, MinGain))

    /*  Find best attribute according to Gain Ratio criterion subject
	to threshold on minimum gain  */

    BestVal = NegInfinity;
    BestAtt = None;

    ForEach(Att, lowerBound, upperBound)
    {
	if ( Skip(Att) || Att == ClassAtt )
	{
	    continue;
	}
	// Pranav: Dont introduce errors if all splits have gain < MinGain.
	//if ( Gain[Att] >= 0.999 * MinGain && Info[Att] > 0 )
	if ( Info[Att] > 0 )
	{
	    Val = Gain[Att] / Info[Att];
	    NBr = ( MaxAttVal[Att] <= 3 || Ordered(Att) ? 3 :
		    SUBSET ? Subsets[Att] : MaxAttVal[Att] );

	    if ( Val > BestVal ||
	         (Val == BestVal && fabs(Bar[Att]) < fabs(Bar[BestAtt])) || 
		  Val > 0.999 * BestVal &&
		   ( NBr < BestNBr ||
		      NBr == BestNBr && Gain[Att] > Gain[BestAtt] ) ) 
	    {

		BestAtt = Att;
		BestVal = Val;
		BestNBr = NBr;
	    }
	    /*
	    else if (Val == BestVal && fabs(Bar[Att]) < fabs(Bar[BestAtt]))
	    {
		BestAtt = Att;
		BestVal = Val;
		BestNBr = NBr;	
	    }*/
	}
    }

    if (BestAtt != None)
        return BestAtt;

    printf("Info is probably zero for all attributes\n");
    BestVal = NegInfinity;
    ForEach(Att, lowerBound, upperBound)
    {
	if ( Skip(Att) || Att == ClassAtt )
	{
	    continue;
	}

	// Pranav: Dont introduce errors if all splits have gain < MinGain.
	//if ( Gain[Att] >= 0.999 * MinGain && Info[Att] > 0 )
	    Val = Gain[Att] ;
	    NBr = ( MaxAttVal[Att] <= 3 || Ordered(Att) ? 3 :
		    SUBSET ? Subsets[Att] : MaxAttVal[Att] );

	    if ( Val > BestVal ||
		 Val > 0.999 * BestVal &&
		 ( NBr < BestNBr ||
		   NBr == BestNBr && Gain[Att] > Gain[BestAtt] ) )
	    {
		BestAtt = Att;
		BestVal = Val;
		BestNBr = NBr;
	    }
    }

    return BestAtt;

}



/*************************************************************************/
/*								 	 */
/*	Evaluate split on Att						 */
/*								 	 */
/*************************************************************************/


void EvalDiscrSplit(Attribute Att, CaseCount Cases)
/*   --------------  */
{
    DiscrValue	v, NBr;

    Gain[Att] = None;

    if ( Skip(Att) || Att == ClassAtt ) return;

    if ( Ordered(Att) )
    {
	EvalOrderedAtt(Att, Cases);
	NBr = ( GEnv.ValFreq[1] > 0.5 ? 3 : 2 );
    }
    else
    if ( SUBSET && MaxAttVal[Att] > 3 )
    {
	EvalSubset(Att, Cases);
	NBr = Subsets[Att];
    }
    else
    if ( ! Tested[Att] )
    {
	EvalDiscreteAtt(Att, Cases);

	NBr = 0;
	ForEach(v, 1, MaxAttVal[Att])
	{
	    if ( GEnv.ValFreq[v] > 0.5 ) NBr++;
	}
    }
    else
    {
	NBr = 0;
    }

    /*  Check that this test will not give too many leaves  */

    if ( NBr > MaxLeaves + 1 )
    {
	Verbosity(2,
	    fprintf(Of, "\t(cancelled -- %d leaves, max %d)\n", NBr, MaxLeaves))

	Gain[Att] = None;
    }
}



/*************************************************************************/
/*								 	 */
/*	Form the subtrees for the given node				 */
/*								 	 */
/*************************************************************************/


void Divide(Tree T, CaseNo Fp, CaseNo Lp, int Level, Attribute upper, Attribute lower)
/*   ------  */
{
    CaseNo	Bp, Ep, Missing, Cases, i;
    CaseCount	KnownCases, MissingCases, BranchCases;
    Attribute	Att;
    double	Factor;
    DiscrValue	v;
    Boolean	PrevUnitWeights;

    PrevUnitWeights = UnitWeights;

    Att = T->Tested;
    Missing = (Ep = Group(0, Fp, Lp, T)) - Fp + 1;

    KnownCases = T->Cases - (MissingCases = CountCases(Fp, Ep));

    if ( Missing )
    {
	assert (false);

	UnitWeights = false;

	/*  If using costs, must adjust branch factors to undo effects of
	    reweighting cases  */

	if ( CostWeights )
	{
	    KnownCases = SumNocostWeights(Ep+1, Lp);
	}

	/*  If there are many cases with missing values and many branches,
	    skip cases whose weight < 0.1  */

	if ( (Cases = No(Fp,Lp)) > 1000 &&
	     Missing > 0.5 * Cases &&
	     T->Forks >= 10 )
	{
	    ForEach(i, Fp, Ep)
	    {
		if ( Weight(Case[i]) < 0.1 )
		{
		    Missing--;
		    MissingCases -= Weight(Case[i]);
		    Swap(Fp, i);
		    Fp++;
		}
	    }

	    assert(Missing >= 0);
	}
    }

    Bp = Fp;
    ForEach(v, 1, T->Forks)
    {
	Ep = Group(v, Bp + Missing, Lp, T);

	assert(Bp + Missing <= Lp+1 && Ep <= Lp);

	/*  Bp -> first value in missing + remaining values
	    Ep -> last value in missing + current group  */

        /* Pranav: BranchCases counts the total number of points flowing to the particular branch under 
	   consideration. It also counts the points participating in implications. */
	BranchCases = CountCases(Bp + Missing, Ep);

	Factor = ( ! Missing ? 0 :
		   ! CostWeights ? BranchCases / KnownCases :
		   SumNocostWeights(Bp + Missing, Ep) / KnownCases );

	if ( BranchCases + Factor * MissingCases >= MinLeaf )
	{
	    if ( Missing )
	    {
		/*  Adjust weights of cases with missing values  */

		ForEach(i, Bp, Bp + Missing - 1)
		{
		    Weight(Case[i]) *= Factor;
		}
	    }

	    FormTree(Bp, Ep, Level+1, &T->Branch[v], upper, lower);

	    /*  Restore weights if changed  */

	    if ( Missing )
	    {
		for ( i = Ep ; i >= Bp ; i-- )
		{
		    if ( Unknown(Case[i], Att) )
		    {
			Weight(Case[i]) /= Factor;
			Swap(i, Ep);
			Ep--;
		    }
		}
	    }

	    Bp = Ep+1;
	}
	else
	{
	    T->Branch[v] = Leaf(Nil, T->Leaf, 0.0, 0.0);
	}
    }

    UnitWeights = PrevUnitWeights;
}



/*************************************************************************/
/*								 	 */
/*	Group together the cases corresponding to branch V of a test 	 */
/*	and return the index of the last such			 	 */
/*								 	 */
/*	Note: if V equals zero, group the unknown values	 	 */
/*								 	 */
/*************************************************************************/


CaseNo Group(DiscrValue V, CaseNo Bp, CaseNo Ep, Tree TestNode)
/*     -----  */
{
    CaseNo	i;
    Attribute	Att;
    ContValue	Thresh;
    Set		SS;

    Att = TestNode->Tested;

    if ( ! V )
    {
	/*  Group together unknown values (if any)  */

	if ( SomeMiss[Att] )
	{
	    //assert (false);

	    ForEach(i, Bp, Ep)
	    {
		if ( Unknown(Case[i], Att) )
		{
		    Swap(Bp, i);
		    Bp++;
		}
	    }
	}
    }
    else				/* skip non-existant N/A values */
    if ( V != 1 || TestNode->NodeType == BrSubset || SomeNA[Att] )
    {
	/*  Group cases on the value of attribute Att, and depending
	    on the type of branch  */

	switch ( TestNode->NodeType )
	{
	    case BrDiscr:

		assert (false);	

		ForEach(i, Bp, Ep)
		{
		    if ( DVal(Case[i], Att) == V )
		    {
			Swap(Bp, i);
			Bp++;
		    }
		}
		break;

	    case BrThresh:

		Thresh = TestNode->Cut;

		// Daniel: Also swap the implications
		struct array * permutation = new_array(MaxCase+1);
		int tmp = 0;
		for(; tmp <= MaxCase; tmp++) {
			permutation->entries[tmp] = tmp;
		}

		ForEach(i, Bp, Ep)
		{
		    if ( V == 1 ? NotApplic(Case[i], Att) :
			 (CVal(Case[i], Att) <= Thresh) == (V == 2) )
		    {
			SwapWithImplications(Bp, i);

			// Daniel: Swap entries
			tmp = permutation->entries[Bp]; 
			permutation->entries[Bp] = permutation->entries[i];
			permutation->entries[i] = tmp;

			Bp++;
		    }
		}

		// Daniel: Swap and clean up
	        struct array * inverse_permutation = array_invert(permutation); 
		struct cmap * renamed_implications = cmap_copy_and_rename(Implications, inverse_permutation);
		delete_cmap(Implications);
		Implications = renamed_implications;
		renamed_implications = NULL;
		delete_array(permutation);
		delete_array(inverse_permutation);

		break;

	    case BrSubset:

		assert (false);	

		SS = TestNode->Subset[V];
		ForEach(i, Bp, Ep)
		{
		    if ( In(XDVal(Case[i], Att), SS) )
		    {
			Swap(Bp, i);
			Bp++;
		    }
		}
		break;
	}
    }

    return Bp - 1;
}



/*************************************************************************/
/*								 	 */
/*	Return the total weight of cases from Fp to Lp		 	 */
/*								 	 */
/*************************************************************************/


CaseCount SumWeights(CaseNo Fp, CaseNo Lp)
/*        ----------  */
{
    double	Sum=0.0;
    CaseNo	i;

    assert(Fp >= 0 && Lp >= Fp-1 && Lp <= MaxCase);

    ForEach(i, Fp, Lp)
    {
	Sum += Weight(Case[i]);
    }

    return Sum;
}



/*************************************************************************/
/*								 	 */
/*	Special version to undo the weightings associated with costs	 */
/*								 	 */
/*************************************************************************/


CaseCount SumNocostWeights(CaseNo Fp, CaseNo Lp)
/*        ----------------  */
{
    double	Sum=0.0;
    CaseNo	i;

    assert(Fp >= 0 && Lp >= Fp-1 && Lp <= MaxCase);

    ForEach(i, Fp, Lp)
    {
	Sum += Weight(Case[i]) / WeightMul[Class(Case[i])];
    }

    return Sum;
}



/*************************************************************************/
/*                                                               	 */
/*	Generate class frequency distribution				 */
/*									 */
/*************************************************************************/


void FindClassFreq(double *CF, CaseNo Fp, CaseNo Lp)
/*   -------------  */
{
    ClassNo	c;
    CaseNo	i;

    assert(Fp >= 0 && Lp >= Fp && Lp <= MaxCase);

    ForEach(c, 0, MaxClass)
    {
	CF[c] = 0;
    }

    ForEach(i, Fp, Lp)
    {
	assert(Class(Case[i]) >= 1 && Class(Case[i]) <= MaxClass);

	CF[ Class(Case[i]) ] += Weight(Case[i]);
    }
}



/*************************************************************************/
/*                                                               	 */
/*	Find all discrete frequencies					 */
/*									 */
/*************************************************************************/


void FindAllFreq(CaseNo Fp, CaseNo Lp)
/*   -----------  */
{
    ClassNo	c;
    CaseNo	i;
    Attribute	Att, a;
    CaseCount	w;
    int		x;

    /*  Zero all values  */

    ForEach(c, 0, MaxClass)
    {
	ClassFreq[c] = 0;
    }

    for ( a = 0 ; a < NDList ; a++ )
    {
		//assert (false);

	Att = DList[a];
	for ( x = MaxClass * (MaxAttVal[Att]+1) - 1 ; x >= 0 ; x-- )
	{
	    DFreq[Att][x] = 0;
	}
    }

    /*  Scan cases  */

    ForEach(i, Fp, Lp)
    {
	ClassFreq[ (c=Class(Case[i])) ] += (w=Weight(Case[i]));

	for ( a = 0 ; a < NDList ; a++ )
	{
    	    //assert (false);

	    Att = DList[a];
	    DFreq[Att][ MaxClass * XDVal(Case[i], Att) + (c-1) ] += w;
	}
    }
}


/* Given a Case number and a value for the class attribute, assigns the class for the given case
   by the given class value. */
int assignClass(int caseNo, int classValue)
{

    assert (0 <= caseNo && caseNo <= MaxCase) ;
    assert (1 <= classValue && classValue <= MaxClass) ;

    assert (Class(Case[caseNo]) == 0 || Class(Case[caseNo]) == classValue);
    assert (DVal(Case[caseNo], ClassAtt) == 0 || DVal(Case[caseNo], ClassAtt) == classValue);

    Class(Case[caseNo]) = classValue ;
    DVal(Case[caseNo], ClassAtt) = classValue ;
   
    return 1;
}



/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Calculate information, information gain, and print dists	 */
/*	--------------------------------------------------------	 */
/*									 */
/*************************************************************************/




/*************************************************************************/
/*									 */
/*	Given Freq[][] and ValFreq[], compute the information gain.	 */
/*									 */
/*************************************************************************/


double ComputeGain(double BaseInfo, float UnknFrac, DiscrValue MaxVal,
		   CaseCount TotalCases)
/*     -----------  */
{
    DiscrValue	v;
    double	ThisInfo=0.0;

    /*  Check whether all values are unknown or the same  */

    if ( ! TotalCases ) return None;

    /*  Compute total info after split, by summing the
	info of each of the subsets formed by the test  */

    ForEach(v, 1, MaxVal)
    {
	ThisInfo += TotalInfo(GEnv.Freq[v], 1, MaxClass);
    }
    ThisInfo /= TotalCases;

    /*  Set the gain in information for all cases, adjusted for unknowns  */

    return ( BaseInfo <= ThisInfo ? 0.0 :
	     (1 - UnknFrac) * (BaseInfo - ThisInfo) );

}



/*************************************************************************/
/*									 */
/*	Compute the total information in V[ MinVal..MaxVal ]		 */
/*									 */
/*************************************************************************/

// Return Value = (P+N)*log(P+N) - P*log(P) - N*log(N)
double TotalInfo(double V[], DiscrValue MinVal, DiscrValue MaxVal)
/*     ---------  */
{
    DiscrValue	v;
    double	Sum=0.0, TotalCases=0;
    CaseCount	N;

    ForEach(v, MinVal, MaxVal)
    {
	N = V[v];

	Sum += N * Log(N);
	TotalCases += N;
    }

    return TotalCases * Log(TotalCases) - Sum;
}

/*************************************************************************/
/*									 */
/*	Compute the information in an Implication Sample 		 */
/*									 */
/*************************************************************************/

double ImplicationInfo(double numPositive, double numNegative, double numImplications)
/*     ---------  */
{
    double probabilityPos = 0.0, probabilityNeg = 0.0;

    if (numImplications == 0.0)
    {
	probabilityPos = numPositive / ( numPositive + numNegative );
	probabilityNeg = 1.0 - probabilityPos;

    }
    else
    {

    	probabilityPos = ( pow(pow(numPositive + numNegative - numImplications, 2) + 4 * numImplications * numPositive, 0.5)
				- (numPositive + numNegative - numImplications) ) / ( 2 * numImplications ) ;

    	probabilityNeg = 1.0 - probabilityPos;
    }

    #if false
    double a = Log(2);
    double b = Log(probabilityPos);
    double c = Log(probabilityNeg);
    double d = Log(0.5);
    #endif

    double result1 = (-1 * probabilityPos * Log(probabilityPos));
    double result2 = (-1 * probabilityNeg * Log(probabilityNeg)) ;
    double result = (result1 + result2);
    return result;

}




/*************************************************************************/
/*									 */
/*	Print distribution table for given attribute			 */
/*									 */
/*************************************************************************/


void PrintDistribution(Attribute Att, DiscrValue MinVal, DiscrValue MaxVal,
		       double **Freq, double *ValFreq, Boolean ShowNames)
/*   -----------------  */
{
    DiscrValue v;
    ClassNo c;
    String Val;

    fprintf(Of, "\n\t\t\t ");
    ForEach(c, 1, MaxClass)
    {
	fprintf(Of, "%7.6s", ClassName[c]);
    }
    fprintf(Of, "\n");

    ForEach(v, MinVal, MaxVal)
    {
	if ( ShowNames )
	{
	    Val = ( ! v ? "unknown" :
		    MaxAttVal[Att] ? AttValName[Att][v] :
		    v == 1 ? "N/A" :
		    v == 2 ? "below" : "above" );
	    fprintf(Of, "\t\t[%-7.7s:", Val);
	}
	else
	{
	    fprintf(Of, "\t\t[%-7d:", v);
	}

	ForEach(c, 1, MaxClass)
	{
	    fprintf(Of, " %6.1f", Freq[v][c]);
	}

	fprintf(Of, "]\n");
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Evaluation of a test on a discrete valued attribute		 */
/*      ---------------------------------------------------		 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Set Info[] and Gain[] for discrete partition of cases		 */
/*									 */
/*************************************************************************/


void EvalDiscreteAtt(Attribute Att, CaseCount Cases)
/*   ---------------  */
{
    CaseCount	KnownCases;
    int		ReasonableSubsets=0;
    DiscrValue	v;
    double	BaseInfo;

    SetDiscrFreq(Att);
    KnownCases = Cases - GEnv.ValFreq[0];

    /*  Check reasonable subsets  */

    ForEach(v, 1, MaxAttVal[Att])
    {
	if ( GEnv.ValFreq[v] >= MINITEMS ) ReasonableSubsets++;
    }

    if ( ReasonableSubsets < 2 )
    {
	Verbosity(2, fprintf(Of, "\tAtt %s: poor split\n", AttName[Att]))
	return;
    }

    BaseInfo = ( ! GEnv.ValFreq[0] ? GlobalBaseInfo :
		     DiscrKnownBaseInfo(KnownCases, MaxAttVal[Att]) );

    Gain[Att] = ComputeGain(BaseInfo, GEnv.ValFreq[0] / Cases, MaxAttVal[Att],
			    KnownCases);
    Info[Att] = TotalInfo(GEnv.ValFreq, 0, MaxAttVal[Att]) / Cases;

    Verbosity(2,
    {
    	fprintf(Of, "\tAtt %s", AttName[Att]);
    	Verbosity(3,
	    PrintDistribution(Att, 0, MaxAttVal[Att], GEnv.Freq, GEnv.ValFreq,
			      true))
    	fprintf(Of, "\tinf %.3f, gain %.3f\n", Info[Att], Gain[Att]);
    })
}



/*************************************************************************/
/*									 */
/*	Set Info[] and Gain[] for ordered split on cases		 */
/*									 */
/*************************************************************************/


void EvalOrderedAtt(Attribute Att, CaseCount Cases)
/*   --------------  */
{
    CaseCount	KnownCases;
    double	*HoldFreqRow, SplitFreq[4];
    ClassNo	c;
    int		Tries=0;
    DiscrValue	v, BestV;
    double	BaseInfo, ThisGain, BestInfo, BestGain=None;

    SetDiscrFreq(Att);
    KnownCases = Cases - GEnv.ValFreq[0];

    BaseInfo = ( ! GEnv.ValFreq[0] ? GlobalBaseInfo :
		     DiscrKnownBaseInfo(KnownCases, MaxAttVal[Att]) );

    Verbosity(2, fprintf(Of, "\tAtt %s", AttName[Att]))
    Verbosity(3, PrintDistribution(Att, 0, MaxAttVal[Att], GEnv.Freq,
				   GEnv.ValFreq, true))

    /*  Move elts of Freq[] starting with the third up one place
	and aggregate class frequencies  */

    HoldFreqRow = GEnv.Freq[MaxAttVal[Att]+1];
    ForEach(c, 1, MaxClass)
    {
	HoldFreqRow[c] = 0;
    }
    SplitFreq[0] = GEnv.ValFreq[0];
    SplitFreq[1] = GEnv.ValFreq[1];
    SplitFreq[2] = GEnv.ValFreq[2];
    SplitFreq[3] = 0;

    for ( v = MaxAttVal[Att] ; v > 2 ; v-- )
    {
	GEnv.Freq[v+1] = GEnv.Freq[v];
	ForEach(c, 1, MaxClass)
	{
	    HoldFreqRow[c] += GEnv.Freq[v][c];
	}
	SplitFreq[3] += GEnv.ValFreq[v];
    }

    GEnv.Freq[3] = HoldFreqRow;

    /*  Try various cuts, saving the one with maximum gain  */

    ForEach(v, 3, MaxAttVal[Att])
    {
	if ( GEnv.ValFreq[v] > 0 &&
	     SplitFreq[2] >= MINITEMS && SplitFreq[3] >= MINITEMS )
	{
	    Tries++;
	    ThisGain =
		ComputeGain(BaseInfo, GEnv.ValFreq[0] / Cases, 3, KnownCases);

	    if ( ThisGain > BestGain )
	    {
		BestGain = ThisGain;
		BestInfo = TotalInfo(SplitFreq, 0, 3) / Cases;
		BestV    = v-1;
	    }

	    Verbosity(3,
	    {   fprintf(Of, "\t\tFrom %s (gain %.3f)",
			AttValName[Att][v], ThisGain);
		PrintDistribution(Att, 0, 3, GEnv.Freq, GEnv.ValFreq, false);
	    })
	}

	/*  Move val v from right branch to left branch  */

	ForEach(c, 1, MaxClass)
	{
	    GEnv.Freq[2][c] += GEnv.Freq[v+1][c];
	    GEnv.Freq[3][c] -= GEnv.Freq[v+1][c];
	}
	SplitFreq[2] += GEnv.ValFreq[v];
	SplitFreq[3] -= GEnv.ValFreq[v];
    }

    if ( Tries > 1 ) BestGain -= Log(Tries) / Cases;

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */

    if ( BestGain <= 0 )
    {
	Verbosity(2, fprintf(Of, "\tno gain\n"))
    }
    else
    {
	Gain[Att] = BestGain;
	Info[Att] = BestInfo;
	Bar[Att]  = BestV;

	Verbosity(2,
	    fprintf(Of, "\tcut=%g, inf %.3f, gain %.3f\n",
		   Bar[Att], Info[Att], Gain[Att]))
    }
}



/*************************************************************************/
/*									 */
/*	Compute frequency tables Freq[][] and ValFreq[] for attribute	 */
/*	Att for current cases						 */
/*									 */
/*************************************************************************/


void SetDiscrFreq(Attribute Att)
/*   ------------  */
{
    ClassNo	c;
    DiscrValue	v;
    int		x;

    /*  Determine the frequency of each possible value for the
	given attribute  */

    ForEach(v, 0, MaxAttVal[Att])
    {
	GEnv.ValFreq[v] = 0;

	x = v * MaxClass;
	ForEach(c, 1, MaxClass)
	{
	    GEnv.ValFreq[v] += (GEnv.Freq[v][c] = DFreq[Att][x + (c-1)]);
	}
    }
}



/*************************************************************************/
/*									 */
/*	Return the base info for cases with known values of a discrete	 */
/*	attribute, using the frequency table Freq[][]			 */
/*	 								 */
/*************************************************************************/


double DiscrKnownBaseInfo(CaseCount KnownCases, DiscrValue MaxVal)
/*     ------------------  */
{
    ClassNo	c;
    CaseCount	ClassCount;
    DiscrValue	v;

    if ( KnownCases < 1E-5 ) return 0.0;

    ForEach(c, 1, MaxClass)
    {
	ClassCount = 0;
	ForEach(v, 1, MaxVal)
	{
	    ClassCount += GEnv.Freq[v][c];
	}
	GEnv.ClassFreq[c] = ClassCount;
    }

    return TotalInfo(GEnv.ClassFreq, 1, MaxClass) / KnownCases;
}



/*************************************************************************/
/*									 */
/*	Construct and return a node for a test on a discrete attribute	 */
/*									 */
/*************************************************************************/


void DiscreteTest(Tree Node, Attribute Att)
/*   ------------  */
{

    int		S, Bytes;
    DiscrValue	v, CutV;

    if ( Ordered(Att) )
    {
	Sprout(Node, 3);

	Node->NodeType	= BrSubset;
	Node->Tested	= Att;

	Bytes = (MaxAttVal[Att]>>3) + 1;
	Node->Subset = AllocZero(4, Set);

	ForEach(S, 1, 3)
	{
	    Node->Subset[S] = AllocZero(Bytes, Byte);
	}

	Node->Cut = CutV = Bar[Att] + 0.1;

	SetBit(1, Node->Subset[1]);
	ForEach(v, 2, MaxAttVal[Att])
	{
	    S = ( v <= CutV ? 2 : 3 );
	    SetBit(v, Node->Subset[S]);
	}
    }
    else
    {
	Sprout(Node, MaxAttVal[Att]);

	Node->NodeType = BrDiscr;
	Node->Tested   = Att;
    }

}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*                                                                	 */
/*	Evaluation of a test on a continuous valued attribute	  	 */
/*	-----------------------------------------------------	  	 */
/*								  	 */
/*************************************************************************/


#define	PartInfo(n) (-(n)*Log((n)/GEnv.Cases))

#define IntrinsicValueImplications(n, T)  ( -(n/T)*Log( n/T ) )

/*************************************************************************/
/*								  	 */
/*	Continuous attributes are treated as if they have possible	 */
/*	values 0 (unknown), 1 (not applicable), 2 (less than cut) and	 */
/*	3 (greater than cut).						 */
/*	This routine finds the best cut for cases Fp through Lp and	 */
/*	sets Info[], Gain[] and Bar[]					 */
/*								  	 */
/*************************************************************************/


void EvalContinuousAtt(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   -----------------  */
{
    CaseNo	i, j, BestI, Tries=0;
    double	LowInfo, LHInfo, LeastInfo=1E38,
		w, BestGain, BestInfo, ThreshCost=1;
    ClassNo	c;
    ContValue	Interval;

    Verbosity(3, fprintf(Of, "\tAtt %s\n", AttName[Att]))

    Gain[Att] = None;
    PrepareForContin(Att, Fp, Lp);

    /*  Special case when very few known values  */

    if ( GEnv.ApplicCases < 2 * MINITEMS )
    {
	Verbosity(2,
	    fprintf(Of, "\tAtt %s\tinsufficient cases with known values\n",
			AttName[Att]))
	return;
    }

    /*  Try possible cuts between cases i and i+1, and determine the
	information and gain of the split in each case  */

    GEnv.MinSplit = 0.10 * GEnv.KnownCases / MaxClass;
    if ( GEnv.MinSplit > 25 ) GEnv.MinSplit = 25;
    if ( GEnv.MinSplit < MINITEMS ) GEnv.MinSplit = MINITEMS;

    /*	Find first possible cut point and initialise scan parameters  */

    i = PrepareForScan(Lp);

    /*  Repeatedly check next possible cut  */

    for ( ; i <= GEnv.Ep ; i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	assert(c >= 1 && c <= MaxClass);

	GEnv.LowCases   += w;
	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;

	GEnv.HighVal = GEnv.SRec[i+1].V;
	if ( GEnv.HighVal > GEnv.LowVal )
	{
	    Tries++;

	    GEnv.LowClass  = GEnv.HighClass;
	    GEnv.HighClass = GEnv.SRec[i+1].C;
	    for ( j = i+2 ;
		  GEnv.HighClass && j <= GEnv.Ep && GEnv.SRec[j].V == GEnv.HighVal ;
		  j++ )
	    {
		if ( GEnv.SRec[j].C != GEnv.HighClass ) GEnv.HighClass = 0;
	    }

	    if ( ! GEnv.LowClass || GEnv.LowClass != GEnv.HighClass || j > GEnv.Ep )
	    {
		LowInfo = TotalInfo(GEnv.Freq[2], 1, MaxClass);

		/*  If cannot improve on best so far, count remaining
		    possible cuts and break  */

		if ( LowInfo >= LeastInfo )
		{
		    for ( i++ ; i <= GEnv.Ep ; i++ )
		    {
			if ( GEnv.SRec[i+1].V > GEnv.SRec[i].V )
			{
			    Tries++;
			}
		    }
		    break;
		}

		LHInfo = LowInfo + TotalInfo(GEnv.Freq[3], 1, MaxClass);
		if ( LHInfo < LeastInfo )
		{
		    LeastInfo = LHInfo;
		    BestI     = i;

		    BestInfo = (GEnv.FixedSplitInfo
				+ PartInfo(GEnv.LowCases)
				+ PartInfo(GEnv.ApplicCases - GEnv.LowCases))
			       / GEnv.Cases;
		}

		Verbosity(3,
		{
		    fprintf(Of, "\t\tCut at %.3f  (gain %.3f):",
			   (GEnv.LowVal + GEnv.HighVal) / 2,
			   (1 - GEnv.UnknownRate) *
			   (GEnv.BaseInfo - (GEnv.NAInfo + LHInfo) / GEnv.KnownCases));
		    PrintDistribution(Att, 2, 3, GEnv.Freq, GEnv.ValFreq, true);
		})
	    }

	    GEnv.LowVal = GEnv.HighVal;
	}
    }

    BestGain = (1 - GEnv.UnknownRate) *
	       (GEnv.BaseInfo - (GEnv.NAInfo + LeastInfo) / GEnv.KnownCases);

    /*  The threshold cost is the lesser of the cost of indicating the
	cases to split between or the interval containing the split  */

    if ( BestGain > 0 )
    {
	Interval = (GEnv.SRec[Lp].V - GEnv.SRec[GEnv.Xp].V) /
		   (GEnv.SRec[BestI+1].V - GEnv.SRec[BestI].V);
	ThreshCost = ( Interval < Tries ? Log(Interval) : Log(Tries) )
		     / GEnv.Cases;
    }

    BestGain -= ThreshCost;

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */

    if ( BestGain <= 0 )
    {
	Verbosity(2, fprintf(Of, "\tAtt %s\tno gain\n", AttName[Att]))
    }
    else
    {
	Gain[Att] = BestGain;
	Info[Att] = BestInfo;

	GEnv.LowVal  = GEnv.SRec[BestI].V;
	GEnv.HighVal = GEnv.SRec[BestI+1].V;

	/*  Set threshold, making sure that rounding problems do not
	    cause it to reach upper value  */

	if ( (Bar[Att] = (ContValue) (0.5 * (GEnv.LowVal + GEnv.HighVal)))
	     >= GEnv.HighVal )
	{
	    Bar[Att] = GEnv.LowVal;
	}

	Verbosity(2,
	    fprintf(Of, "\tAtt %s\tcut=%.3f, inf %.3f, gain %.3f\n",
		   AttName[Att], Bar[Att], Info[Att], Gain[Att]))
    }
}

/*************************************************************************/
/*								  	 */
/*	Continuous attributes are treated as if they have possible	 */
/*	values 0 (unknown), 1 (not applicable), 2 (less than cut) and	 */
/*	3 (greater than cut).						 */
/*	This routine finds the best cut for cases Fp through Lp and	 */
/*	sets Info[], Gain[] and Bar[]					 */
/*      The algorithm ignores the implication data points while		 */
/*	computing the entropy/gain for a particular cut			 */				
/*				  	 				 */
/*************************************************************************/

// Pranav: Simplified the method-- sans optimizations
void EvalContinuousAttAlg1(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   -----------------  */
{
    CaseNo	i, j, BestI = -1, Tries=0;
    double	LowInfo, LHInfo, LeastInfo=1E38,
		w, BestGain=NegInfinity, BestInfo=1E38, ThreshCost=1;
    ClassNo	c;
    ContValue	Interval;

    Verbosity(3, fprintf(Of, "\tAtt %s\n", AttName[Att]))

    Gain[Att] = NegInfinity;
    PrepareForContinAlg1(Att, Fp, Lp);

    /*  Special case when very few known values  */

    if ( GEnv.ApplicCases < 2 * MINITEMS )
    {
	Verbosity(2,
	    fprintf(Of, "\tAtt %s\tinsufficient cases with known values\n",
			AttName[Att]))
	return;
    }

    /*  Try possible cuts between cases i and i+1, and determine the
	information and gain of the split in each case  */

    /*  Pranav: We have to be wary of splitting a small number of cases off one end,
	as this has little predictive power.  The minimum split GEnv.MinSplit is
	the maximum of MINITEMS or (the minimum of 25 and 10% of the cases
	per class).
	Set MinSplit to MINITEMS.  */

    GEnv.MinSplit = MINITEMS;

    /*  Repeatedly check next possible cut  */

    for (i = GEnv.Xp ; i < GEnv.Ep ; i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	// Pranav: class attribute can be zero!
	//assert(c >= 1 && c <= MaxClass);

	// Pranav: Add to LowCases only those points whose class attribute is not zero!
	if (c > 0)
	{
	    GEnv.LowCases   += w;
	}
	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;

	//GEnv.HighVal = GEnv.SRec[i+1].V;
	if ( i == GEnv.Ep || GEnv.SRec[i+1].V > GEnv.SRec[i].V )
	{
	    Tries++;
	    
	    GEnv.LowVal = GEnv.SRec[i].V;
	    GEnv.HighVal = (i == GEnv.Ep) ? GEnv.LowVal : GEnv.SRec[i+1].V;

	    LowInfo = TotalInfo(GEnv.Freq[2], 1, MaxClass);

	    /*  If cannot improve on best so far, count remaining
	        possible cuts and break  */

	    /* Cannot ascertain the corectness in the presence of UNKNOWNS, so ignore.
	    if ( LowInfo >= LeastInfo )
	    {
	        for ( i++ ; i <= GEnv.Ep ; i++ )
	        {
	    	    if ( GEnv.SRec[i+1].V > GEnv.SRec[i].V )
		    {
		        Tries++;
		    }
		}
	        break;
	    }*/

	    LHInfo = LowInfo + TotalInfo(GEnv.Freq[3], 1, MaxClass);
	    if ( LHInfo < LeastInfo )
	    {
	        LeastInfo = LHInfo;
	        BestI     = i;

	        BestInfo = (GEnv.FixedSplitInfo
	    		    + PartInfo(GEnv.LowCases)
			    + PartInfo(GEnv.ApplicCases - GEnv.LowCases))
			    / GEnv.Cases;
	    }

	    Verbosity(3,
	    {
	        fprintf(Of, "\t\tCut at %.3f  (gain %.3f):",
	        	(GEnv.LowVal + GEnv.HighVal) / 2,
			(1 - GEnv.UnknownRate) *
			(GEnv.BaseInfo - (GEnv.NAInfo + LHInfo) / GEnv.KnownCases));
		    PrintDistribution(Att, 2, 3, GEnv.Freq, GEnv.ValFreq, true);
		})
	}
    }

    if (BestI < 0)
  	return;



    BestGain = (1 - GEnv.UnknownRate) *
	       (GEnv.BaseInfo - (GEnv.NAInfo + LeastInfo) / GEnv.KnownCases);

    /*  The threshold cost is the lesser of the cost of indicating the
	cases to split between or the interval containing the split  */

    if ( BestGain > 0 )
    {
	if ( BestI < GEnv.Ep )
	{
	    Interval = (GEnv.SRec[Lp].V - GEnv.SRec[GEnv.Xp].V) /
	    	       (GEnv.SRec[BestI+1].V - GEnv.SRec[BestI].V);
	    ThreshCost = ( Interval < Tries ? Log(Interval) : Log(Tries) )
		         / GEnv.Cases;
	}	
	else
	{
	    ThreshCost = Log(Tries) / GEnv.Cases ;
	}
    }

    BestGain -= ThreshCost;

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */

    Gain[Att] = BestGain;
    Info[Att] = BestInfo;

    if ( BestI < GEnv.Ep )
    {
        GEnv.LowVal  = GEnv.SRec[BestI].V;
    	GEnv.HighVal = GEnv.SRec[BestI+1].V;
    }
    else
    {
        GEnv.LowVal  = GEnv.HighVal = GEnv.SRec[BestI].V;
    }

    /*  Set threshold, making sure that rounding problems do not
    cause it to reach upper value  */

    if ( (Bar[Att] = (ContValue) (0.5 * (GEnv.LowVal + GEnv.HighVal)))
         >= GEnv.HighVal )
    {
        Bar[Att] = GEnv.LowVal;
    }

    Verbosity(2,
        fprintf(Of, "\tAtt %s\tcut=%.3f, inf %.3f, gain %.3f\n",
    	   AttName[Att], Bar[Att], Info[Att], Gain[Att]))
}


/*************************************************************************/
/*								  	 */
/*	This routine finds the best cut for cases Fp through Lp and	 */
/*	sets Info[], Gain[] and Bar[]					 */
/*      The algorithm counts the number of implications cut and 	 */
/*	penalizes the entropy/gain accordingly				 */				
/*				  	 				 */
/*************************************************************************/

void EvalContinuousAttAlg2(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   -----------------  */
{
    CaseNo	i, j, BestI = -1, Tries=0;
    double	LowInfo, LHInfo, LeastInfo=1E38,
		w, BestGain=NegInfinity, BestInfo=1E38, ThreshCost=1, penalty = 0;
    ClassNo	c;
    ContValue	Interval;
    int		numImplicationsCut = 0, totalImplications;


    Verbosity(3, fprintf(Of, "\tAtt %s\n", AttName[Att]))

    Gain[Att] = NegInfinity;
    PrepareForContinAlg2(Att, Fp, Lp);

    /*  Special case when very few known values  */

    if ( GEnv.ApplicCases < 2 * MINITEMS )
    {
	Verbosity(2,
	    fprintf(Of, "\tAtt %s\tinsufficient cases with known values\n",
			AttName[Att]))
	return;
    }

    /* Find totalImplications that lie entirely between GEnv.Xp and GEnv.Ep */	
    //totalImplications = computeTotalImplications(GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr);	
    //printf("total Implications: %d\n", totalImplications);


    /*  Try possible cuts between cases i and i+1, and determine the
	information and gain of the split in each case  */

    /*  Pranav: We have to be wary of splitting a small number of cases off one end,
	as this has little predictive power.  The minimum split GEnv.MinSplit is
	the maximum of MINITEMS or (the minimum of 25 and 10% of the cases
	per class).
	Set MinSplit to MINITEMS.  */

    GEnv.MinSplit = MINITEMS;

    /*	Find first possible cut point and initialise scan parameters  */

    /*  Repeatedly check next possible cut  */

    for (i = GEnv.Xp ; i < GEnv.Ep ; i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	// Pranav: class attribute can be zero!
	//assert(c >= 1 && c <= MaxClass);

	// Pranav: Add to LowCases only those points whose class attribute is not zero!
	if (c > 0)
	{
	    GEnv.LowCases   += w;
	}
	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;

	numImplicationsCut += numImplicationsCutAt(i, GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr);

	if ( i == GEnv.Ep || GEnv.SRec[i+1].V > GEnv.SRec[i].V )
	{
	    //printf("numImplications cut = %d\n", numImplicationsCut);	
	    Tries++;

	    GEnv.LowVal = GEnv.SRec[i].V;
	    GEnv.HighVal = (i == GEnv.Ep) ? GEnv.LowVal : GEnv.SRec[i+1].V;


	    LowInfo = TotalInfo(GEnv.Freq[2], 1, MaxClass);

	    LHInfo = (LowInfo + TotalInfo(GEnv.Freq[3], 1, MaxClass)) / GEnv.KnownCases;

	    /* Calculating the penalty depending upon the number of implications cut. */
    	    //printf("number of Implications Cut: %d, Cases: %.3f\n", numImplicationsCut, GEnv.Cases);
	    //penalty = Log(1 + numImplicationsCut) / GEnv.Cases;
	    //penalty = numImplicationsCut / GEnv.Cases;
	    penalty = (2 * numImplicationsCut) / (GEnv.Cases + 2 * numImplicationsCut);

	    LHInfo = LHInfo + 1 * penalty ;
	
	    if ( LHInfo < LeastInfo )
	    {
	        LeastInfo = LHInfo;
	        BestI     = i;

	        BestInfo = (GEnv.FixedSplitInfo
	    		    + PartInfo(GEnv.LowCases)
			    + PartInfo(GEnv.ApplicCases - GEnv.LowCases))
			    / GEnv.Cases;
	    }

	    Verbosity(3,
	    {
	        fprintf(Of, "\t\tCut at %.3f  (gain %.3f) (penalty %.3f):",
	        	(GEnv.LowVal + GEnv.HighVal) / 2,
			(1 - GEnv.UnknownRate) *
			GEnv.BaseInfo - (GEnv.NAInfo + LHInfo) , penalty);
		    PrintDistribution(Att, 2, 3, GEnv.Freq, GEnv.ValFreq, true);
		})
	}
    }

    if (BestI < 0)
        return;


    BestGain = (1 - GEnv.UnknownRate) *
	       (GEnv.BaseInfo - (GEnv.NAInfo + LeastInfo)) ;

    /*  The threshold cost is the lesser of the cost of indicating the
	cases to split between or the interval containing the split  */

    if ( BestGain > 0 )
    {
	if ( BestI < GEnv.Ep )
	{
	    Interval = (GEnv.SRec[Lp].V - GEnv.SRec[GEnv.Xp].V) /
	    	       (GEnv.SRec[BestI+1].V - GEnv.SRec[BestI].V);
	    ThreshCost = ( Interval < Tries ? Log(Interval) : Log(Tries) )
		         / GEnv.Cases;
	}	
	else
	{
	    ThreshCost = Log(Tries) / GEnv.Cases ;
	}
    }

    BestGain -= ThreshCost;

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */

    Gain[Att] = BestGain;
    Info[Att] = BestInfo;

    if ( BestI < GEnv.Ep )
    {
        GEnv.LowVal  = GEnv.SRec[BestI].V;
    	GEnv.HighVal = GEnv.SRec[BestI+1].V;
    }
    else
    {
        GEnv.LowVal  = GEnv.HighVal = GEnv.SRec[BestI].V;
    }

    /*  Set threshold, making sure that rounding problems do not
    cause it to reach upper value  */

    if ( (Bar[Att] = (ContValue) (0.5 * (GEnv.LowVal + GEnv.HighVal)))
         >= GEnv.HighVal )
    {
        Bar[Att] = GEnv.LowVal;
    }

    Verbosity(2,
        fprintf(Of, "\tAtt %s\tcut=%.3f, inf %.3f, gain %.3f, penalty %.3f\n",
    	   AttName[Att], Bar[Att], Info[Att], Gain[Att], penalty))
}


/*************************************************************************/
/*								  	 */
/*	This routine finds the best cut for cases Fp through Lp and	 */
/*	sets Info[], Gain[] and Bar[]					 */
/*      The algorithm counts the number of implications cut and 	 */
/*	penalizes the entropy/gain accordingly				 */				
/*      Implications that go from negative to positive are not penalized */
/*				  	 				 */
/*************************************************************************/

void EvalContinuousAttAlg6(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   -----------------  */
{
    CaseNo	i, j, BestI = -1, Tries=0;
    double	LowInfo, LHInfo, LeastInfo=1E38,
		w, BestGain=NegInfinity, BestInfo=1E38, ThreshCost=1, penalty = 0;
    ClassNo	c;
    ContValue	Interval;
    int		numImplicationsCut = 0, totalImplications;
    int 	numImplicationsCutLeft2Right = 0, numImplicationsCutRight2Left = 0;
    double	reductionL2R, reductionR2L;

    Verbosity(3, fprintf(Of, "\tAtt %s\n", AttName[Att]))

    Gain[Att] = NegInfinity;
    PrepareForContinAlg2(Att, Fp, Lp);

    /*  Special case when very few known values  */

    if ( GEnv.ApplicCases < 2 * MINITEMS )
    {
	Verbosity(2,
	    fprintf(Of, "\tAtt %s\tinsufficient cases with known values\n",
			AttName[Att]))
	return;
    }

    /* Find totalImplications that lie entirely between GEnv.Xp and GEnv.Ep */	
    //totalImplications = computeTotalImplications(GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr);	
    //printf("total Implications: %d\n", totalImplications);


    /*  Try possible cuts between cases i and i+1, and determine the
	information and gain of the split in each case  */

    /*  Pranav: We have to be wary of splitting a small number of cases off one end,
	as this has little predictive power.  The minimum split GEnv.MinSplit is
	the maximum of MINITEMS or (the minimum of 25 and 10% of the cases
	per class).
	Set MinSplit to MINITEMS.  */

    GEnv.MinSplit = MINITEMS;

    /*	Find first possible cut point and initialise scan parameters  */

    /*  Repeatedly check next possible cut  */

    for (i = GEnv.Xp ; i < GEnv.Ep ; i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	// Pranav: class attribute can be zero!
	//assert(c >= 1 && c <= MaxClass);

	// Pranav: Add to LowCases only those points whose class attribute is not zero!
	if (c > 0)
	{
	    GEnv.LowCases   += w;
	}
	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;

	numImplicationsCutAtLeftRightSep(i, GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr, &numImplicationsCutLeft2Right, &numImplicationsCutRight2Left);

	/*if (GEnv.SRec[i].V > 10 || GEnv.SRec[i].V < -10)
		continue;
	*/


	if ( i == GEnv.Ep || GEnv.SRec[i+1].V > GEnv.SRec[i].V )
	{
	    //printf("numImplications cut = %d\n", numImplicationsCut);	
	    Tries++;

	    GEnv.LowVal = GEnv.SRec[i].V;
	    GEnv.HighVal = (i == GEnv.Ep) ? GEnv.LowVal : GEnv.SRec[i+1].V;


	    LowInfo = TotalInfo(GEnv.Freq[2], 1, MaxClass);

	    LHInfo = (LowInfo + TotalInfo(GEnv.Freq[3], 1, MaxClass)) / GEnv.KnownCases;

	    /* Calculating the penalty depending upon the number of implications cut. */
    	    //printf("number of Implications Cut: %d, Cases: %.3f\n", numImplicationsCut, GEnv.Cases);
	    //penalty = Log(1 + numImplicationsCut) / GEnv.Cases;
	    //penalty = numImplicationsCut / GEnv.Cases;

	    double nleft = GEnv.Freq[2][2] == 0 ? 0 : GEnv.Freq[2][2] / (GEnv.Freq[2][1] + GEnv.Freq[2][2]);
	    double pleft = GEnv.Freq[2][1] == 0 ? 0 : GEnv.Freq[2][1] / (GEnv.Freq[2][1] + GEnv.Freq[2][2]);
	    double nright = GEnv.Freq[3][2] == 0 ? 0 : GEnv.Freq[3][2] / (GEnv.Freq[3][1] + GEnv.Freq[3][2]);
	    double pright = GEnv.Freq[3][1] == 0 ? 0 : GEnv.Freq[3][1] / (GEnv.Freq[3][1] + GEnv.Freq[3][2]);

	    penalty = (1 - nleft * pright) * numImplicationsCutLeft2Right + (1 - nright * pleft) * numImplicationsCutRight2Left;
	    //penalty = penalty / GEnv.Cases;
	    penalty = (2 * penalty) / (GEnv.Cases + 2 * (numImplicationsCutLeft2Right + numImplicationsCutRight2Left));

	    LHInfo = LHInfo + 1 * penalty ;
	
	    if ( LHInfo < LeastInfo )
	    {
	        LeastInfo = LHInfo;
	        BestI     = i;

	        BestInfo = (GEnv.FixedSplitInfo
	    		    + PartInfo(GEnv.LowCases)
			    + PartInfo(GEnv.ApplicCases - GEnv.LowCases))
			    / GEnv.Cases;
	    }

	    Verbosity(3,
	    {
	        fprintf(Of, "\t\tCut at %.3f  (gain %.3f) (penalty %.3f):",
	        	(GEnv.LowVal + GEnv.HighVal) / 2,
			(1 - GEnv.UnknownRate) *
			GEnv.BaseInfo - (GEnv.NAInfo + LHInfo) , penalty);
		    PrintDistribution(Att, 2, 3, GEnv.Freq, GEnv.ValFreq, true);
		})
	}
    }

    if (BestI < 0)
        return;


    BestGain = (1 - GEnv.UnknownRate) *
	       (GEnv.BaseInfo - (GEnv.NAInfo + LeastInfo)) ;

    /*  The threshold cost is the lesser of the cost of indicating the
	cases to split between or the interval containing the split  */

    if ( BestGain > 0 )
    {
	if ( BestI < GEnv.Ep )
	{
	    Interval = (GEnv.SRec[Lp].V - GEnv.SRec[GEnv.Xp].V) /
	    	       (GEnv.SRec[BestI+1].V - GEnv.SRec[BestI].V);
	    ThreshCost = ( Interval < Tries ? Log(Interval) : Log(Tries) )
		         / GEnv.Cases;
	}	
	else
	{
	    ThreshCost = Log(Tries) / GEnv.Cases ;
	}
    }

    BestGain -= ThreshCost;

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */

    Gain[Att] = BestGain;
    Info[Att] = BestInfo;

    if ( BestI < GEnv.Ep )
    {
        GEnv.LowVal  = GEnv.SRec[BestI].V;
    	GEnv.HighVal = GEnv.SRec[BestI+1].V;
    }
    else
    {
        GEnv.LowVal  = GEnv.HighVal = GEnv.SRec[BestI].V;
    }

    /*  Set threshold, making sure that rounding problems do not
    cause it to reach upper value  */

    if ( (Bar[Att] = (ContValue) (0.5 * (GEnv.LowVal + GEnv.HighVal)))
         >= GEnv.HighVal )
    {
        Bar[Att] = GEnv.LowVal;
    }

    Verbosity(2,
        fprintf(Of, "\tAtt %s\tcut=%.3f, inf %.3f, gain %.3f, penalty %.3f\n",
    	   AttName[Att], Bar[Att], Info[Att], Gain[Att], penalty))
}



/*************************************************************************/
/*								  	 */
/*	This routine finds the best cut for cases Fp through Lp and	 */
/*	sets Info[], Gain[] and Bar[]					 */
/*      The algorithm calculates the entropy of the implication-based 	 */
/*	sample directly. 						 */				
/*				  	 				 */
/*************************************************************************/

void EvalContinuousAttAlg3(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   -----------------  */
{
    CaseNo	i, j, BestI = -1, Tries=0;
    double	LowInfo, HighInfo, LHInfo, LeastInfo=1E38,
		w, BestGain=NegInfinity, BestInfo=1E38, ThreshCost=1;
    ClassNo	c;
    ContValue	Interval;
    double	totalImplications = 0, numImplicationsOnLeft = 0, numImplicationsOnRight = 0, numImplicationsCut = 0;
    double	numPointsOnLeft = 0, numPointsOnRight = 0, totalPoints = 0;
    int 	right2cut, cut2left;  /* counters to track implications as they flow from the 
					 right partition to the left*/

    Verbosity(3, fprintf(Of, "\tAtt %s\n", AttName[Att]))

    Gain[Att] = NegInfinity;
    Info[Att] = 0;
    PrepareForContinAlg3(Att, Fp, Lp);

    /*  Special case when very few known values  */

    if ( GEnv.ApplicCases < 2 * MINITEMS )
    {
	Verbosity(2,
	    fprintf(Of, "\tAtt %s\tinsufficient cases with known values\n",
			AttName[Att]))
	return;
    }

    //printf("Info: %.3f\n", Info[Att]);

    /* Find totalImplications that lie entirely between GEnv.Xp and GEnv.Ep */	
    numImplicationsOnRight = totalImplications = computeTotalImplications(GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr);
    totalPoints = GEnv.Freq[3][1] + GEnv.Freq[3][2] + 2 * totalImplications;
    GEnv.BaseInfo = ImplicationInfo(GEnv.Freq[3][1], GEnv.Freq[3][2], totalImplications);
    //printf("total Implications: %f\n", totalImplications);


    /*  Try possible cuts between cases i and i+1, and determine the
	information and gain of the split in each case  */

    /*  Pranav: We have to be wary of splitting a small number of cases off one end,
	as this has little predictive power.  The minimum split GEnv.MinSplit is
	the maximum of MINITEMS or (the minimum of 25 and 10% of the cases
	per class).
	Set MinSplit to MINITEMS.  */

    GEnv.MinSplit = MINITEMS;

    /*	Find first possible cut point and initialise scan parameters  */

    /*  Repeatedly check next possible cut  */

    for (i = GEnv.Xp ; i < GEnv.Ep ; i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	// Pranav: class attribute can be zero!
	//assert(c >= 1 && c <= MaxClass);

	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;

 	processImplicationsAt(i, GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr, &right2cut, &cut2left);
	numImplicationsOnRight = numImplicationsOnRight - right2cut;
	numImplicationsCut = numImplicationsCut + right2cut - cut2left;
	numImplicationsOnLeft = numImplicationsOnLeft + cut2left;	

	assert (numImplicationsOnRight + numImplicationsCut + numImplicationsOnLeft == totalImplications);

	if ( i == GEnv.Ep || GEnv.SRec[i+1].V > GEnv.SRec[i].V )
	{
	    Tries++;

	    /*if (GEnv.SRec[i].V < -10 || GEnv.SRec[i].V > 10)
		    continue;
*/
	    GEnv.LowVal = GEnv.SRec[i].V;
	    GEnv.HighVal = (i == GEnv.Ep) ? GEnv.LowVal : GEnv.SRec[i+1].V;

	    // TODO: Find how many chains are there in our samples usually? Are we overcounting numImplications by too much!
	
	    numPointsOnLeft = GEnv.Freq[2][1] + GEnv.Freq[2][2] + 2 * numImplicationsOnLeft;
	    numPointsOnRight = GEnv.Freq[3][1] + GEnv.Freq[3][2] + 2 * numImplicationsOnRight;

	    LowInfo = ImplicationInfo(GEnv.Freq[2][1], GEnv.Freq[2][2], numImplicationsOnLeft);
	    HighInfo = ImplicationInfo(GEnv.Freq[3][1], GEnv.Freq[3][2], numImplicationsOnRight);

	    LHInfo = (numPointsOnLeft * LowInfo + numPointsOnRight * HighInfo) / ( numPointsOnLeft + numPointsOnRight ) ;

	    if ( LHInfo < LeastInfo )
	    {
	        LeastInfo = LHInfo;
	        BestI     = i;

		BestInfo = IntrinsicValueImplications(numPointsOnLeft, totalPoints)
				+ IntrinsicValueImplications(numPointsOnRight, totalPoints) ;
	    }

	    Verbosity(3,
	    {
	        fprintf(Of, "\t\tCut at %.3f  (gain %.3f):",
	        	(GEnv.LowVal + GEnv.HighVal) / 2, GEnv.BaseInfo - LHInfo);
		    PrintDistribution(Att, 2, 3, GEnv.Freq, GEnv.ValFreq, true);
		})
	}
    }

    // No threshold set
    if (BestI < 0)
        return;


    BestGain = GEnv.BaseInfo - LeastInfo ;

    /*
    Verbosity(2,
        fprintf(Of, "\tAtt %s\tcut=%.3f, baseInfo %.3f, leastInfo %.3f\n",
    	   AttName[Att], Bar[Att], GEnv.BaseInfo, LeastInfo))
    */

    /*  The threshold cost is the lesser of the cost of indicating the
	cases to split between or the interval containing the split  */

    if ( BestGain > 0 )
    {
	if ( BestI < GEnv.Ep )
	{
	    Interval = (GEnv.SRec[Lp].V - GEnv.SRec[GEnv.Xp].V) /
	    	       (GEnv.SRec[BestI+1].V - GEnv.SRec[BestI].V);
	    ThreshCost = ( Interval < Tries ? Log(Interval) : Log(Tries) )
		         / totalPoints;
	}	
	else
	{
	    ThreshCost = Log(Tries) / totalPoints ;
	}
    }

    //TODO: check if this helps??
    BestGain -= ThreshCost;

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */

    Gain[Att] = BestGain;
    Info[Att] = BestInfo;

    if ( BestI < GEnv.Ep )
    {
        GEnv.LowVal  = GEnv.SRec[BestI].V;
    	GEnv.HighVal = GEnv.SRec[BestI+1].V;
    }
    else
    {
        GEnv.LowVal  = GEnv.HighVal = GEnv.SRec[BestI].V;
    }

    /*  Set threshold, making sure that rounding problems do not
    cause it to reach upper value  */

    if ( (Bar[Att] = (ContValue) (0.5 * (GEnv.LowVal + GEnv.HighVal)))
         >= GEnv.HighVal )
    {
        Bar[Att] = GEnv.LowVal;
    }

    Verbosity(2,
        fprintf(Of, "\tAtt %s\tcut=%.3f, inf %.3f, gain %.3f\n",
    	   AttName[Att], Bar[Att], Info[Att], Gain[Att]))
}





/*************************************************************************/
/*								  	 */
/*	This routine finds the best cut for cases Fp through Lp and	 */
/*	sets Info[], Gain[] and Bar[]					 */
/*      The algorithm calculates the entropy of the implication-based 	 */
/*	sample directly using Madhu's method. 						 */				
/*				  	 				 */
/*************************************************************************/

void EvalContinuousAttAlg4(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   -----------------  */
{
    CaseNo	i, j, BestI = -1, Tries=0;
    double	LowInfo, HighInfo, LHInfo, WeightedAvgInfo, LeastInfo=1E38,
		w, BestGain=NegInfinity, BestInfo=1E38, ThreshCost=1, curBestGain;
		
    ClassNo	c;
    ContValue	Interval;
    double	totalImplications = 0, numImplicationsOnLeft = 0, numImplicationsOnRight = 0, numImplicationsCut = 0;
    double	numPointsOnLeft = 0, numPointsOnRight = 0, totalPoints = 0;
    int 	right2cut, cut2left;  /* counters to track implications as they flow from the 
					 right partition to the left*/


    Verbosity(3, fprintf(Of, "\tAtt %s\n", AttName[Att]))

    Gain[Att] = NegInfinity;
    Info[Att] = 0;
    PrepareForContinAlg3(Att, Fp, Lp);

    /*  Special case when very few known values  */

    if ( GEnv.ApplicCases < 2 * MINITEMS )
    {
	Verbosity(2,
	    fprintf(Of, "\tAtt %s\tinsufficient cases with known values\n",
			AttName[Att]))
	return;
    }

    /*  Repeatedly check next possible cut  */

    for (i = GEnv.Xp ; i < GEnv.Ep ; i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	// Pranav: class attribute can be zero!
	//assert(c >= 1 && c <= MaxClass);

	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;

	if ( i == GEnv.Ep || GEnv.SRec[i+1].V > GEnv.SRec[i].V )
	{
	    Tries++;

	    int t = 0, r = 0, s = 0, w = 0;

	    /* Compute number of implications of the different types. */
	    int p, q;
	    for(p = GEnv.Xp; p <= i; p++)
	    {
		struct array * rhs = cmap_get(GEnv.Implications, p);
		for(q = 0; q < rhs->size; q++)
		{
		    /* Counting the implications completely on the lhs of the cut. */
		    if( rhs->entries[q] >= GEnv.Xp && rhs->entries[q] <= i)
			t++;

		    /* Counting the implications from left to right across the cut. */
		    else if( rhs->entries[q] > i && rhs->entries[q] <= GEnv.Ep)
			r++;

		}
		delete_array(rhs);

	     }

	    for(p = i + 1; p <= GEnv.Ep; p++)
	    {
		struct array * rhs = cmap_get(GEnv.Implications, p);
		for(q = 0; q < rhs->size; q++)
		{
		    /* Counting the implications from right to left across the cut. */
		    if( rhs->entries[q] >= GEnv.Xp && rhs->entries[q] <= i)
			s++;

		    /* Counting the implications completely on the right of the cut. */
		    else if( rhs->entries[q] > i && rhs->entries[q] <= GEnv.Ep)
			w++;
		}
		delete_array(rhs);
	     }

	     curBestGain = NegInfinity;	     
	     /* If var = 0, then the implication is (-,-). If var = 1, implication is (+,+). */
	     /* a corresponds to type of implications completely on lhs of the cut. */
	     /* b corresponds to implications from left to right of the cut. */
	     /* c corresponds to implications from right to left of the cut. */
	     /* d corresponds to type of implications completely on rhs of the cut. */
	     int a, b, c, d;
	     for(a = 0; a < 2; a++)
	     {
		 for(b = 0; b < 2; b++)
		 {
		     for(c = 0; c < 2; c++)
		     {
			for(d = 0; d < 2; d++)
			{

			    /* Computing frequencies of the positive and negative points on the lhs/rhs of the cut. */
			    int curPl = GEnv.Freq[2][1];
			    int curNl = GEnv.Freq[2][2];

			    int curPr = GEnv.Freq[3][1];
			    int curNr = GEnv.Freq[3][2];

			    if(a == 0)
			    {
				curNl += 2 * t;
			    }
			    else
			    { 
				curPl += 2 * t;
			    }


			    if(b == 0)
			    {
				curNl += r;
				curNr += r;
			    }
			    else
			    { 
				curPl += r;
				curPr += r;
			    }

			    if(c == 0)
			    {
				curNl += s;
				curNr += s;
			    }
			    else
			    { 
				curPl += s;
				curPr += s;
			    }

			    if(d == 0)
			    {
				curNr += 2 * w;
			    }
			    else
			    { 
				curPr += 2 * w;
			    }


			    /* Computing the entropy for the particular choice of r,s,t,w. */
	    		    LowInfo = ImplicationInfo(curPl, curNl, 0);
	    		    HighInfo = ImplicationInfo(curPr, curNr, 0);
			    totalPoints = curPl + curNl + curPr + curNr;

	    		    WeightedAvgInfo = ((curPl + curNl) * LowInfo + (curPr + curNr) * HighInfo) / ( totalPoints ) ;
			   
			    if (WeightedAvgInfo < LeastInfo)
			    { 
			   	LeastInfo = WeightedAvgInfo; 
				BestI = i;
			        BestInfo = IntrinsicValueImplications((curPl + curNl), totalPoints) 
		                                + IntrinsicValueImplications((curPr + curNr), totalPoints) ;
			    }
			}
		    }
	     	} 
	     } 

	    GEnv.LowVal = GEnv.SRec[i].V;
	    GEnv.HighVal = (i == GEnv.Ep) ? GEnv.LowVal : GEnv.SRec[i+1].V;

	}
    }

    // No threshold set
    if (BestI < 0)
        return;

    BestGain = -1 * LeastInfo ;

    /*  The threshold cost is the lesser of the cost of indicating the
	cases to split between or the interval containing the split  */

    if ( BestI < GEnv.Ep )
    {
     	Interval = (GEnv.SRec[Lp].V - GEnv.SRec[GEnv.Xp].V) /
	   	       (GEnv.SRec[BestI+1].V - GEnv.SRec[BestI].V);
	ThreshCost = ( Interval < Tries ? Log(Interval) : Log(Tries) )
		         / totalPoints;
    }	
    else
    {
    	ThreshCost = Log(Tries) / totalPoints ;
    }

    BestGain -= ThreshCost;

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */

    Gain[Att] = BestGain;
    Info[Att] = BestInfo;

    if ( BestI < GEnv.Ep )
    {
        GEnv.LowVal  = GEnv.SRec[BestI].V;
    	GEnv.HighVal = GEnv.SRec[BestI+1].V;
    }
    else
    {
        GEnv.LowVal  = GEnv.HighVal = GEnv.SRec[BestI].V;
    }

    /*  Set threshold, making sure that rounding problems do not
    cause it to reach upper value  */

    if ( (Bar[Att] = (ContValue) (0.5 * (GEnv.LowVal + GEnv.HighVal)))
         >= GEnv.HighVal )
    {
        Bar[Att] = GEnv.LowVal;
    }

    Verbosity(2,
        fprintf(Of, "\tAtt %s\tcut=%.3f, inf %.3f, gain %.3f\n",
    	   AttName[Att], Bar[Att], Info[Att], Gain[Att]))
}




/*************************************************************************/
/*								  	 */
/*	This routine finds the best cut for cases Fp through Lp and	 */
/*	sets Info[], Gain[] and Bar[]					 */
/* 						 */				
/* Combines algorithms 2 and 3 (i.e., ...) */
/*				  	 				 */
/*************************************************************************/

void EvalContinuousAttAlg5(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   -----------------  */
{
    CaseNo	i, j, BestI = -1, Tries=0;
    double	LowInfo, HighInfo, LHInfo, LeastInfo=1E38,
		w, BestGain=NegInfinity, BestInfo=1E38, ThreshCost=1;
    ClassNo	c;
    ContValue	Interval;
    double	totalImplications = 0, numImplicationsOnLeft = 0, numImplicationsOnRight = 0, numImplicationsCut = 0;
    double	numPointsOnLeft = 0, numPointsOnRight = 0, totalPoints = 0;
    int 	right2cut, cut2left;  /* counters to track implications as they flow from the 
					 right partition to the left*/
	int numImplicationsCutAccordingToAlg2 = 0;


    Verbosity(3, fprintf(Of, "\tAtt %s\n", AttName[Att]))

    Gain[Att] = NegInfinity;
    Info[Att] = 0;
    PrepareForContinAlg3(Att, Fp, Lp);

    /*  Special case when very few known values  */

    if ( GEnv.ApplicCases < 2 * MINITEMS )
    {
	Verbosity(2,
	    fprintf(Of, "\tAtt %s\tinsufficient cases with known values\n",
			AttName[Att]))
	return;
    }

    //printf("Info: %.3f\n", Info[Att]);

    /* Find totalImplications that lie entirely between GEnv.Xp and GEnv.Ep */	
    numImplicationsOnRight = totalImplications = computeTotalImplications(GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr);
    totalPoints = GEnv.Freq[3][1] + GEnv.Freq[3][2] + 2 * totalImplications;
    GEnv.BaseInfo = ImplicationInfo(GEnv.Freq[3][1], GEnv.Freq[3][2], totalImplications);
    //printf("total Implications: %f\n", totalImplications);


    /*  Try possible cuts between cases i and i+1, and determine the
	information and gain of the split in each case  */

    /*  Pranav: We have to be wary of splitting a small number of cases off one end,
	as this has little predictive power.  The minimum split GEnv.MinSplit is
	the maximum of MINITEMS or (the minimum of 25 and 10% of the cases
	per class).
	Set MinSplit to MINITEMS.  */

    GEnv.MinSplit = MINITEMS;

    /*	Find first possible cut point and initialise scan parameters  */

    /*  Repeatedly check next possible cut  */

    for (i = GEnv.Xp ; i < GEnv.Ep ; i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	// Pranav: class attribute can be zero!
	//assert(c >= 1 && c <= MaxClass);

	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;

 	processImplicationsAt(i, GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr, &right2cut, &cut2left);
	numImplicationsOnRight = numImplicationsOnRight - right2cut;
	numImplicationsCut = numImplicationsCut + right2cut - cut2left;
	numImplicationsOnLeft = numImplicationsOnLeft + cut2left;	

	assert (numImplicationsOnRight + numImplicationsCut + numImplicationsOnLeft == totalImplications);

	// Alg 2
	numImplicationsCutAccordingToAlg2 += numImplicationsCutAt(i, GEnv.Xp, GEnv.Ep, GEnv.Implications, GEnv.classAttr);
	// end

	if ( i == GEnv.Ep || GEnv.SRec[i+1].V > GEnv.SRec[i].V )
	{
	    Tries++;

	    /*if (GEnv.SRec[i].V < -10 || GEnv.SRec[i].V > 10)
		    continue;
*/
	    GEnv.LowVal = GEnv.SRec[i].V;
	    GEnv.HighVal = (i == GEnv.Ep) ? GEnv.LowVal : GEnv.SRec[i+1].V;

	    // TODO: Find how many chains are there in our samples usually? Are we overcounting numImplications by too much!
	
	    numPointsOnLeft = GEnv.Freq[2][1] + GEnv.Freq[2][2] + 2 * numImplicationsOnLeft;
	    numPointsOnRight = GEnv.Freq[3][1] + GEnv.Freq[3][2] + 2 * numImplicationsOnRight;

	    LowInfo = ImplicationInfo(GEnv.Freq[2][1], GEnv.Freq[2][2], numImplicationsOnLeft);
	    HighInfo = ImplicationInfo(GEnv.Freq[3][1], GEnv.Freq[3][2], numImplicationsOnRight);

	    LHInfo = (numPointsOnLeft * LowInfo + numPointsOnRight * HighInfo) / ( numPointsOnLeft + numPointsOnRight ) ;

		// Add penalty according to Alg 2
		LHInfo += 1 * (((double) numImplicationsCutAccordingToAlg2) / ((double) GEnv.Cases));
		// End

	    if ( LHInfo < LeastInfo )
	    {
	        LeastInfo = LHInfo;
	        BestI     = i;

		BestInfo = IntrinsicValueImplications(numPointsOnLeft, totalPoints)
				+ IntrinsicValueImplications(numPointsOnRight, totalPoints) ;
	    }

	    Verbosity(3,
	    {
	        fprintf(Of, "\t\tCut at %.3f  (gain %.3f):",
	        	(GEnv.LowVal + GEnv.HighVal) / 2, GEnv.BaseInfo - LHInfo);
		    PrintDistribution(Att, 2, 3, GEnv.Freq, GEnv.ValFreq, true);
		})
	}
    }

    // No threshold set
    if (BestI < 0)
        return;


    BestGain = GEnv.BaseInfo - LeastInfo ;

    /*
    Verbosity(2,
        fprintf(Of, "\tAtt %s\tcut=%.3f, baseInfo %.3f, leastInfo %.3f\n",
    	   AttName[Att], Bar[Att], GEnv.BaseInfo, LeastInfo))
    */

    /*  The threshold cost is the lesser of the cost of indicating the
	cases to split between or the interval containing the split  */

    if ( BestGain > 0 )
    {
	if ( BestI < GEnv.Ep )
	{
	    Interval = (GEnv.SRec[Lp].V - GEnv.SRec[GEnv.Xp].V) /
	    	       (GEnv.SRec[BestI+1].V - GEnv.SRec[BestI].V);
	    ThreshCost = ( Interval < Tries ? Log(Interval) : Log(Tries) )
		         / totalPoints;
	}	
	else
	{
	    ThreshCost = Log(Tries) / totalPoints ;
	}
    }

    //TODO: check if this helps??
    BestGain -= ThreshCost;

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */

    Gain[Att] = BestGain;
    Info[Att] = BestInfo;

    if ( BestI < GEnv.Ep )
    {
        GEnv.LowVal  = GEnv.SRec[BestI].V;
    	GEnv.HighVal = GEnv.SRec[BestI+1].V;
    }
    else
    {
        GEnv.LowVal  = GEnv.HighVal = GEnv.SRec[BestI].V;
    }

    /*  Set threshold, making sure that rounding problems do not
    cause it to reach upper value  */

    if ( (Bar[Att] = (ContValue) (0.5 * (GEnv.LowVal + GEnv.HighVal)))
         >= GEnv.HighVal )
    {
        Bar[Att] = GEnv.LowVal;
    }

    Verbosity(2,
        fprintf(Of, "\tAtt %s\tcut=%.3f, inf %.3f, gain %.3f\n",
    	   AttName[Att], Bar[Att], Info[Att], Gain[Att]))
}







/*************************************************************************/
/*                                                                	 */
/*	Estimate max gain ratio available from any cut, using sample	 */
/*	of SampleFrac of all cases					 */
/*                                                                	 */
/*************************************************************************/


void EstimateMaxGR(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   -------------  */
{
    CaseNo	i, j;
    double	LHInfo, w, SplitInfo, ThisGain, GR;
    ClassNo	c;

    EstMaxGR[Att] = 0;

    if ( Skip(Att) || Att == ClassAtt ) return;

    PrepareForContin(Att, Fp, Lp);

    /*  Special case when very few known values  */

    if ( GEnv.ApplicCases < 2 * MINITEMS * SampleFrac )
    {
	return;
    }

    /*  Try possible cuts between cases i and i+1.  Use conservative
	value of GEnv.MinSplit to allow for sampling  */

    GEnv.MinSplit = 0.10 * GEnv.KnownCases / MaxClass;
    if ( GEnv.MinSplit > 25 ) GEnv.MinSplit = 25;
    if ( GEnv.MinSplit < MINITEMS ) GEnv.MinSplit = MINITEMS;

    GEnv.MinSplit *= SampleFrac * 0.33;

    i = PrepareForScan(Lp);

    /*  Repeatedly check next possible cut  */

    for ( ; i <= GEnv.Ep ; i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	assert(c >= 1 && c <= MaxClass);

	GEnv.LowCases   += w;
	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;

	GEnv.HighVal = GEnv.SRec[i+1].V;
	if ( GEnv.HighVal > GEnv.LowVal )
	{
	    GEnv.LowClass  = GEnv.HighClass;
	    GEnv.HighClass = GEnv.SRec[i+1].C;
	    for ( j = i+2 ;
		  GEnv.HighClass && j <= GEnv.Ep && GEnv.SRec[j].V == GEnv.HighVal ;
		  j++ )
	    {
		if ( GEnv.SRec[j].C != GEnv.HighClass ) GEnv.HighClass = 0;
	    }

	    if ( ! GEnv.LowClass || GEnv.LowClass != GEnv.HighClass || j > GEnv.Ep )
	    {
		LHInfo = TotalInfo(GEnv.Freq[2], 1, MaxClass)
			 + TotalInfo(GEnv.Freq[3], 1, MaxClass);

		SplitInfo = (GEnv.FixedSplitInfo
			    + PartInfo(GEnv.LowCases)
			    + PartInfo(GEnv.ApplicCases - GEnv.LowCases)) / GEnv.Cases;

		ThisGain = (1 - GEnv.UnknownRate) *
			   (GEnv.BaseInfo - (GEnv.NAInfo + LHInfo) / GEnv.KnownCases);
		if ( ThisGain > Gain[Att] ) Gain[Att] = ThisGain;

		/*  Adjust GR to make it more conservative upper bound  */

		GR = (ThisGain + 1E-5) / SplitInfo;
		if ( GR > EstMaxGR[Att] )
		{
		    EstMaxGR[Att] = GR;
		}

		Verbosity(3,
		{
		    fprintf(Of, "\t\tCut at %.3f  (gain %.3f):",
			   (GEnv.LowVal + GEnv.HighVal) / 2, ThisGain);
		    PrintDistribution(Att, 2, 3, GEnv.Freq, GEnv.ValFreq, true);
		})
	    }

	    GEnv.LowVal = GEnv.HighVal;
	}
    }

    Verbosity(2,
	fprintf(Of, "\tAtt %s: max GR estimate %.3f\n",
		    AttName[Att], EstMaxGR[Att]))
}



/*************************************************************************/
/*								  	 */
/*	Routine to set some preparatory values used by both		 */
/*	EvalContinuousAtt and EstimateMaxGR				 */
/*								  	 */
/*************************************************************************/


void PrepareForContin(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   ----------------  */
{
    CaseNo	i;
    ClassNo	c;
    DiscrValue	v;

    /*  Reset frequency tables  */

    ForEach(v, 0, 3)
    {
	// Pranav: Set Freq also when c = 0
	ForEach(c, 0, MaxClass)
	{
	    GEnv.Freq[v][c] = 0;
	}
	GEnv.ValFreq[v] = 0;
    }

    /*  Omit and count unknown and N/A values */

    GEnv.Cases = 0;

    if ( SomeMiss[Att] || SomeNA[Att] )
    {
	assert (false);

	GEnv.Xp = Lp+1;

	ForEach(i, Fp, Lp)
	{
	    assert(Class(Case[i]) >= 1 && Class(Case[i]) <= MaxClass);

	    GEnv.Cases += Weight(Case[i]);

	    if ( Unknown(Case[i], Att) )
	    {
		GEnv.Freq[ 0 ][ Class(Case[i]) ] += Weight(Case[i]);
	    }
	    else
	    if ( NotApplic(Case[i], Att) )
	    {
		GEnv.Freq[ 1 ][ Class(Case[i]) ] += Weight(Case[i]);
	    }
	    else
	    {
		GEnv.Freq[ 3 ][ Class(Case[i]) ] += Weight(Case[i]);
		GEnv.Xp--;
		GEnv.SRec[GEnv.Xp].V = CVal(Case[i], Att);
		GEnv.SRec[GEnv.Xp].W = Weight(Case[i]);
		GEnv.SRec[GEnv.Xp].C = Class(Case[i]);
	    }
	}

	ForEach(c, 1, MaxClass)
	{
	    GEnv.ValFreq[0] += GEnv.Freq[0][c];
	    GEnv.ValFreq[1] += GEnv.Freq[1][c];
	}

	GEnv.NAInfo = TotalInfo(GEnv.Freq[1], 1, MaxClass);
	GEnv.FixedSplitInfo = PartInfo(GEnv.ValFreq[0]) + PartInfo(GEnv.ValFreq[1]);

	Verbosity(3, PrintDistribution(Att, 0, 1, GEnv.Freq, GEnv.ValFreq, true))
    }
    else
    {
	GEnv.Xp = Fp;

	ForEach(i, Fp, Lp)
	{
	    GEnv.SRec[i].V = CVal(Case[i], Att);
	    GEnv.SRec[i].W = Weight(Case[i]);
	    GEnv.SRec[i].C = Class(Case[i]);

	    GEnv.Freq[3][Class(Case[i])] += Weight(Case[i]);
	}

	ForEach(c, 1, MaxClass)
	{
	    GEnv.Cases += GEnv.Freq[3][c];
	}

	GEnv.NAInfo = GEnv.FixedSplitInfo = 0;
    }

    /*  GEnv.Cases is only capturing the number of cases who have known class 
        attribute. Also GEnv.ValFreq[v] = 0 for all v.  */

    GEnv.KnownCases  = GEnv.Cases - GEnv.ValFreq[0];
    GEnv.ApplicCases = GEnv.KnownCases - GEnv.ValFreq[1];

    GEnv.UnknownRate = 1.0 - GEnv.KnownCases / GEnv.Cases;

    assert (GEnv.UnknownRate == 0);

    Cachesort(GEnv.Xp, Lp, GEnv.SRec);

    /*  If unknowns or using sampling, must recompute base information  */

    if ( GEnv.ValFreq[0] > 0 || SampleFrac < 1 )
    {
	assert (false);

	/*  Determine base information using GEnv.Freq[0] as temp buffer  */

	ForEach(c, 1, MaxClass)
	{
	    GEnv.Freq[0][c] = GEnv.Freq[1][c] + GEnv.Freq[3][c];
	}

	GEnv.BaseInfo = TotalInfo(GEnv.Freq[0], 1, MaxClass) / GEnv.KnownCases;
    }
    else
    {
	GEnv.BaseInfo = GlobalBaseInfo;
    }
}


/*************************************************************************/
/*								  	 */
/*	Routine to set some preparatory values used by both		 */
/*	EvalContinuousAtt and EstimateMaxGR				 */
/*								  	 */
/*************************************************************************/


void PrepareForContinAlg1(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   ----------------  */
{
    CaseNo	i;
    ClassNo	c;
    DiscrValue	v;

    /*  Reset frequency tables  */

    ForEach(v, 0, 3)
    {
	// Pranav: Set Freq also when c = 0
	ForEach(c, 0, MaxClass)
	{
	    GEnv.Freq[v][c] = 0;
	}
	GEnv.ValFreq[v] = 0;
    }

    /*  Omit and count unknown and N/A values */

    GEnv.Cases = 0;

    if ( SomeMiss[Att] || SomeNA[Att] )
    {
	assert (false);

	GEnv.Xp = Lp+1;

	ForEach(i, Fp, Lp)
	{
	    assert(Class(Case[i]) >= 1 && Class(Case[i]) <= MaxClass);

	    GEnv.Cases += Weight(Case[i]);

	    if ( Unknown(Case[i], Att) )
	    {
		GEnv.Freq[ 0 ][ Class(Case[i]) ] += Weight(Case[i]);
	    }
	    else
	    if ( NotApplic(Case[i], Att) )
	    {
		GEnv.Freq[ 1 ][ Class(Case[i]) ] += Weight(Case[i]);
	    }
	    else
	    {
		GEnv.Freq[ 3 ][ Class(Case[i]) ] += Weight(Case[i]);
		GEnv.Xp--;
		GEnv.SRec[GEnv.Xp].V = CVal(Case[i], Att);
		GEnv.SRec[GEnv.Xp].W = Weight(Case[i]);
		GEnv.SRec[GEnv.Xp].C = Class(Case[i]);
	    }
	}

	ForEach(c, 1, MaxClass)
	{
	    GEnv.ValFreq[0] += GEnv.Freq[0][c];
	    GEnv.ValFreq[1] += GEnv.Freq[1][c];
	}

	GEnv.NAInfo = TotalInfo(GEnv.Freq[1], 1, MaxClass);
	GEnv.FixedSplitInfo = PartInfo(GEnv.ValFreq[0]) + PartInfo(GEnv.ValFreq[1]);

	Verbosity(3, PrintDistribution(Att, 0, 1, GEnv.Freq, GEnv.ValFreq, true))
    }
    else
    {
	GEnv.Xp = Fp;

	ForEach(i, Fp, Lp)
	{
	    GEnv.SRec[i].V = CVal(Case[i], Att);
	    GEnv.SRec[i].W = Weight(Case[i]);
	    GEnv.SRec[i].C = Class(Case[i]);

	    GEnv.Freq[3][Class(Case[i])] += Weight(Case[i]);
	}

	ForEach(c, 1, MaxClass)
	{
	    GEnv.Cases += GEnv.Freq[3][c];
	}

	GEnv.NAInfo = GEnv.FixedSplitInfo = 0;
    }
        
    GEnv.LowCases = 0;
    GEnv.Ep = Lp ;

    /*  GEnv.Cases is only capturing the number of cases who have known class 
        attribute. Also GEnv.ValFreq[v] = 0 for all v.  */

    GEnv.KnownCases  = GEnv.Cases - GEnv.ValFreq[0];
    GEnv.ApplicCases = GEnv.KnownCases - GEnv.ValFreq[1];

    GEnv.UnknownRate = 1.0 - GEnv.KnownCases / GEnv.Cases;

    assert (GEnv.UnknownRate == 0);

    Cachesort(GEnv.Xp, Lp, GEnv.SRec);

    /*  If unknowns or using sampling, must recompute base information  */

    if ( GEnv.ValFreq[0] > 0 || SampleFrac < 1 )
    {
	assert (false);

	/*  Determine base information using GEnv.Freq[0] as temp buffer  */

	ForEach(c, 1, MaxClass)
	{
	    GEnv.Freq[0][c] = GEnv.Freq[1][c] + GEnv.Freq[3][c];
	}

	GEnv.BaseInfo = TotalInfo(GEnv.Freq[0], 1, MaxClass) / GEnv.KnownCases;
    }
    else
    {
	GEnv.BaseInfo = GlobalBaseInfo;
    }
}


/*************************************************************************/
/*								  	 */
/*	Routine to set some preparatory values used by both		 */
/*	EvalContinuousAtt and EstimateMaxGR				 */
/*								  	 */
/*************************************************************************/


void PrepareForContinAlg2(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   ----------------  */
{
    CaseNo	i;
    ClassNo	c;
    DiscrValue	v;

    /*  Reset frequency tables  */

    ForEach(v, 0, 3)
    {
	// Pranav: Set Freq also when c = 0
	ForEach(c, 0, MaxClass)
	{
	    GEnv.Freq[v][c] = 0;
	}
	GEnv.ValFreq[v] = 0;
    }

    /*  Omit and count unknown and N/A values */

    GEnv.Cases = 0;
    GEnv.LowCases = 0;

    GEnv.Ep = Lp ;
    GEnv.Xp = Fp;

    ForEach(i, Fp, Lp)
    {
        GEnv.SRec[i].V = CVal(Case[i], Att);
        GEnv.SRec[i].W = Weight(Case[i]);
        GEnv.SRec[i].C = Class(Case[i]);

        GEnv.Freq[3][Class(Case[i])] += Weight(Case[i]);
    }

    ForEach(c, 1, MaxClass)
    {
        GEnv.Cases += GEnv.Freq[3][c];
    }


    GEnv.NAInfo = GEnv.FixedSplitInfo = 0;

    /*  GEnv.Cases is only capturing the number of cases who have known class 
        attribute. Also GEnv.ValFreq[v] = 0 for all v.  */

    GEnv.KnownCases  = GEnv.Cases - GEnv.ValFreq[0];
    GEnv.ApplicCases = GEnv.KnownCases - GEnv.ValFreq[1];

    GEnv.UnknownRate = 1.0 - GEnv.KnownCases / GEnv.Cases;

    assert (GEnv.UnknownRate == 0);


    // Initialize permutation array
    ForEach(i, 0, MaxCase)
    {
        GEnv.permutation->entries[i] = i;
    }

    #if false
    array_print(GEnv.permutation);
    #endif

    CachesortWithImplications(GEnv.Xp, Lp, GEnv.SRec, GEnv.permutation);
    struct array * inverse_permutation = array_invert(GEnv.permutation);

    delete_cmap(GEnv.Implications);
    GEnv.Implications = cmap_copy_and_rename(Implications, inverse_permutation);

    #if false
    cmap_print(Implications);
    array_print(GEnv.permutation);
    array_print(inverse_permutation);
    cmap_print(GEnv.Implications);
    #endif

    delete_array(inverse_permutation);

    // Initialize classAttr array
    ForEach(i, 0, MaxCase)
    {
	if (i >= Fp && i <= Lp)
	{
            GEnv.classAttr->entries[i] = GEnv.SRec[i].C;
	}
	else
	{ 
	    GEnv.classAttr->entries[i] = 0;
	}
    }

    /*  If unknowns or using sampling, must recompute base information  */

    if ( GEnv.ValFreq[0] > 0 || SampleFrac < 1 )
    {
	assert (false);

	/*  Determine base information using GEnv.Freq[0] as temp buffer  */

	ForEach(c, 1, MaxClass)
	{
	    GEnv.Freq[0][c] = GEnv.Freq[1][c] + GEnv.Freq[3][c];
	}

	GEnv.BaseInfo = TotalInfo(GEnv.Freq[0], 1, MaxClass) / GEnv.KnownCases;
    }
    else
    {
	GEnv.BaseInfo = GlobalBaseInfo;
    }
}


/*************************************************************************/
/*								  	 */
/*	Routine to set some preparatory values used by both		 */
/*	EvalContinuousAtt and EstimateMaxGR				 */
/*								  	 */
/*************************************************************************/


void PrepareForContinAlg3(Attribute Att, CaseNo Fp, CaseNo Lp)
/*   ----------------  */
{
    CaseNo	i;
    ClassNo	c;
    DiscrValue	v;

    /*  Reset frequency tables  */

    ForEach(v, 0, 3)
    {
	// Pranav: Set Freq also when c = 0
	ForEach(c, 0, MaxClass)
	{
	    GEnv.Freq[v][c] = 0;
	}
	GEnv.ValFreq[v] = 0;
    }

    /*  Omit and count unknown and N/A values */

    GEnv.Cases = 0;
    GEnv.LowCases = 0;

    GEnv.Ep = Lp ;
    GEnv.Xp = Fp;

    ForEach(i, Fp, Lp)
    {
        GEnv.SRec[i].V = CVal(Case[i], Att);
        GEnv.SRec[i].W = Weight(Case[i]);
        GEnv.SRec[i].C = Class(Case[i]);

        GEnv.Freq[3][Class(Case[i])] += Weight(Case[i]);
    }

    ForEach(c, 1, MaxClass)
    {
        GEnv.Cases += GEnv.Freq[3][c];
    }

    GEnv.NAInfo = GEnv.FixedSplitInfo = 0;

    /*  GEnv.Cases is only capturing the number of cases who have known class 
        attribute. Also GEnv.ValFreq[v] = 0 for all v.  */

    GEnv.KnownCases  = GEnv.Cases - GEnv.ValFreq[0];
    GEnv.ApplicCases = GEnv.KnownCases - GEnv.ValFreq[1];

    GEnv.UnknownRate = 1.0 - GEnv.KnownCases / GEnv.Cases;

    assert (GEnv.UnknownRate == 0);


    // Initialize permutation array
    ForEach(i, 0, MaxCase)
    {
        GEnv.permutation->entries[i] = i;
    }

    #if false
    array_print(GEnv.permutation);
    #endif

    CachesortWithImplications(GEnv.Xp, Lp, GEnv.SRec, GEnv.permutation);
    struct array * inverse_permutation = array_invert(GEnv.permutation);

    delete_cmap(GEnv.Implications);
    GEnv.Implications = cmap_copy_and_rename(Implications, inverse_permutation);
    

    #if false
    cmap_print(Implications);
    array_print(GEnv.permutation);
    array_print(inverse_permutation);
    cmap_print(GEnv.Implications);
    #endif

    delete_array(inverse_permutation);
    
    // Initialize classAttr array
    ForEach(i, 0, MaxCase)
    {
	if (i >= Fp && i <= Lp)
	{
            GEnv.classAttr->entries[i] = GEnv.SRec[i].C;
	}
	else
	{ 
	    GEnv.classAttr->entries[i] = 0;
	}
    }
    #if false
    array_print(GEnv.classAttr);
    cmap_print(GEnv.Implications);
    #endif

    GEnv.BaseInfo = 0;
}


/*************************************************************************/
/*								  	 */
/*	Set low and high bounds for scan and initial class		 */
/*	(used by EvalContinuousAtt and EstimateMaxGR)			 */
/*								  	 */
/*************************************************************************/


CaseNo PrepareForScan(CaseNo Lp)
/*     --------------  */
{
    CaseNo	i, j;
    ClassNo	c;
    double	w;

    /*  Find last possible split  */

    GEnv.HighCases = GEnv.LowCases = 0;

    for ( GEnv.Ep = Lp ; GEnv.Ep >= GEnv.Xp && GEnv.HighCases < GEnv.MinSplit ; GEnv.Ep-- )
    {
	GEnv.HighCases += GEnv.SRec[GEnv.Ep].W;
    }

    /*  Skip cases before first possible cut  */

    for ( i = GEnv.Xp ;
	  i <= GEnv.Ep &&
	  ( GEnv.LowCases + GEnv.SRec[i].W < GEnv.MinSplit - 1E-5 ||
	    GEnv.SRec[i].V == GEnv.SRec[i+1].V ) ;
	  i++ )
    {
	c = GEnv.SRec[i].C;
	w = GEnv.SRec[i].W;
	// assert(c >= 1 && c <= MaxClass);

	GEnv.LowCases   += w;
	GEnv.Freq[2][c] += w;
	GEnv.Freq[3][c] -= w;
    }

    /*  Find the class key for the first interval  */

    GEnv.HighClass = GEnv.SRec[i].C;
    for ( j = i-1; GEnv.HighClass && j >= GEnv.Xp ; j-- )
    {
	if ( GEnv.SRec[j].C != GEnv.HighClass ) GEnv.HighClass = 0;
    }
    assert(GEnv.HighClass <= MaxClass);
    assert(j+1 >= GEnv.Xp);

    GEnv.LowVal = GEnv.SRec[i].V;

    return i;
}


/*************************************************************************/
/*                                                                	 */
/*	Change a leaf into a test on a continuous attribute           	 */
/*                                                                	 */
/*************************************************************************/


void ContinTest(Tree Node, Attribute Att)
/*   ----------  */
{
    Sprout(Node, 3);

    Node->NodeType = BrThresh;
    Node->Tested   = Att;
    Node->Cut 	   =
    Node->Lower	   =
    Node->Upper    = Bar[Att];
}



/*************************************************************************/
/*                                                                	 */
/*	Adjust thresholds of all continuous attributes so that cuts	 */
/*	are values that appear in the data				 */
/*                                                                	 */
/*************************************************************************/


void AdjustAllThresholds(Tree T)
/*   -------------------  */
{
    Attribute	Att;
    CaseNo	Ep;

    ForEach(Att, 1, MaxAtt)
    {
	if ( Continuous(Att) )
	{
	    Ep = -1;
	    AdjustThresholds(T, Att, &Ep);
	}
    }
}



void AdjustThresholds(Tree T, Attribute Att, CaseNo *Ep)
/*   ----------------  */
{
    DiscrValue	v;
    CaseNo	i;

    if ( T->NodeType == BrThresh && T->Tested == Att )
    {
	if ( *Ep == -1 )
	{
	    ForEach(i, 0, MaxCase)
	    {
		if ( ! Unknown(Case[i], Att) && ! NotApplic(Case[i], Att) )
		{
		    (&GEnv)->SRec[++(*Ep)].V = CVal(Case[i], Att);
		}
	    }
	    Cachesort(0, *Ep, (&GEnv)->SRec);

	    if ( PossibleCuts && Trial == 0 )
	    {
		int Cuts=0;

		ForEach(i, 1, *Ep)
		{
		    if ( (&GEnv)->SRec[i].V != (&GEnv)->SRec[i-1].V ) Cuts++;
		}
		PossibleCuts[Att] = Cuts;
	    }
	}

	T->Cut = T->Lower = T->Upper = GreatestValueBelow(T->Cut, Ep);
    }

    if ( T->NodeType )
    {
	ForEach(v, 1, T->Forks)
	{
	    AdjustThresholds(T->Branch[v], Att, Ep);
	}
    }
}



/*************************************************************************/
/*                                                                	 */
/*	Return the greatest value of attribute Att below threshold Th  	 */
/*	(Assumes values of Att have been sorted.)			 */
/*                                                                	 */
/*************************************************************************/


ContValue GreatestValueBelow(ContValue Th, CaseNo *Ep)
/*	  ------------------  */
{
    CaseNo	Low, Mid, High;

    Low  = 0;
    High = *Ep;

    while ( Low < High )
    {
	Mid = (Low + High + 1) / 2;

	if ( (&GEnv)->SRec[Mid].V > Th )
	{
	    High = Mid - 1;
	}
	else
	{
	    Low = Mid;
	}
    }

    return (&GEnv)->SRec[Low].V;
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*      Evaluation of discrete attribute subsets			 */
/*      ----------------------------------------			 */
/*									 */
/*************************************************************************/





/*************************************************************************/
/*									 */
/*	Set up tables for subsets					 */
/*									 */
/*************************************************************************/


void InitialiseBellNumbers()
/*   ---------------------  */
{
    DiscrValue	 n, k;

    /*  Table of Bell numbers (used for subset test penalties)  */

    Bell = AllocZero(MaxDiscrVal+1, double *);
    ForEach(n, 1, MaxDiscrVal)
    {
	Bell[n] = AllocZero(n+1, double);
	ForEach(k, 1, n)
	{
	    Bell[n][k] = ( k == 1 || k == n ? 1 :
			   Bell[n-1][k-1] + k * Bell[n-1][k] );
	}
    }
}



/*************************************************************************/
/*									 */
/*	Evaluate subsetting a discrete attribute and form the chosen	 */
/*	subsets Subset[Att][], setting Subsets[Att] to the number of	 */
/*	subsets, and the Info[] and Gain[] of a test on the attribute	 */
/*									 */
/*************************************************************************/


void EvalSubset(Attribute Att, CaseCount Cases)
/*   ----------  */
{
    DiscrValue	V1, V2, V3, BestV1, BestV2, InitialBlocks, First=1, Prelim=0;
    ClassNo	c;
    double	BaseInfo, ThisGain, ThisInfo, Penalty, UnknownRate,
		Val, BestVal, BestGain, BestInfo, PrevGain, PrevInfo;
    int		MissingValues=0;
    CaseCount	KnownCases;
    Boolean	Better;

    /*  First compute Freq[][], ValFreq[], base info, and the gain
	and total info of a split on discrete attribute Att  */

    SetDiscrFreq(Att);

    GEnv.ReasonableSubsets = 0;
    ForEach(c, 1, MaxAttVal[Att])
    {
	if ( GEnv.ValFreq[c] >= MINITEMS ) GEnv.ReasonableSubsets++;
    }

    if ( ! GEnv.ReasonableSubsets )
    {
	Verbosity(2,
	    fprintf(Of, "\tAtt %s: poor initial split\n", AttName[Att]))

	return;
    }

    KnownCases  = Cases - GEnv.ValFreq[0];
    UnknownRate = GEnv.ValFreq[0] / Cases;

    BaseInfo = ( ! GEnv.ValFreq[0] ? GlobalBaseInfo :
		     DiscrKnownBaseInfo(KnownCases, MaxAttVal[Att]) );

    PrevGain = ComputeGain(BaseInfo, UnknownRate, MaxAttVal[Att], KnownCases);
    PrevInfo = TotalInfo(GEnv.ValFreq, 0, MaxAttVal[Att]) / Cases;
    BestVal  = PrevGain / PrevInfo;

    Verbosity(2, fprintf(Of, "\tAtt %s", AttName[Att]))
    Verbosity(3, PrintDistribution(Att, 0, MaxAttVal[Att], GEnv.Freq,
				   GEnv.ValFreq, true))
    Verbosity(2,
	fprintf(Of, "\tinitial inf %.3f, gain %.3f, val=%.3f\n",
		PrevInfo, PrevGain, BestVal))

    /*  Eliminate unrepresented attribute values from Freq[] and ValFreq[]
	and form a separate subset for each represented attribute value.
	Unrepresented N/A values are ignored  */

    GEnv.Bytes = (MaxAttVal[Att]>>3) + 1;
    ClearBits(GEnv.Bytes, Subset[Att][0]);

    GEnv.Blocks = 0;
    ForEach(V1, 1, MaxAttVal[Att])
    {
	if ( GEnv.ValFreq[V1] > Epsilon || V1 == 1 && SomeNA[Att] )
	{
	    if ( ++GEnv.Blocks < V1 )
	    {
		GEnv.ValFreq[GEnv.Blocks] = GEnv.ValFreq[V1];
		ForEach(c, 1, MaxClass)
		{
		    GEnv.Freq[GEnv.Blocks][c] = GEnv.Freq[V1][c];
		}
	    }
	    ClearBits(GEnv.Bytes, GEnv.WSubset[GEnv.Blocks]);
	    SetBit(V1, GEnv.WSubset[GEnv.Blocks]);
	    CopyBits(GEnv.Bytes, GEnv.WSubset[GEnv.Blocks],
		     Subset[Att][GEnv.Blocks]);

	    /*  Cannot merge N/A values with other blocks  */

	    if ( V1 == 1 ) First = 2;
	}
	else
	if ( V1 != 1 )
	{
	    SetBit(V1, Subset[Att][0]);
	    MissingValues++;
	}
    }

    /*  Set n-way branch as initial test  */

    Gain[Att]    = PrevGain;
    Info[Att]    = PrevInfo;
    Subsets[Att] = InitialBlocks = GEnv.Blocks;

    /*  As a preliminary step, merge values with identical distributions  */

    ForEach(V1, First, GEnv.Blocks-1)
    {
	ForEach(V2, V1+1, GEnv.Blocks)
	{
	    if ( SameDistribution(V1, V2) )
	    {
		Prelim = V1;
		AddBlock(V1, V2);
	    }
	}

	/*  Eliminate any merged values  */

	if ( Prelim == V1 )
	{
	    V3 = V1;

	    ForEach(V2, V1+1, GEnv.Blocks)
	    {
		if ( GEnv.ValFreq[V2] && ++V3 != V2 )
		{
		    MoveBlock(V3, V2);
		}
	    }

	    GEnv.Blocks = V3;
	}
    }

    if ( Prelim )
    {
	PrevInfo = TotalInfo(GEnv.ValFreq, 0, GEnv.Blocks) / Cases;

	Penalty  = ( finite(Bell[InitialBlocks][GEnv.Blocks]) ?
			Log(Bell[InitialBlocks][GEnv.Blocks]) :
			(InitialBlocks-GEnv.Blocks+1) * Log(GEnv.Blocks) );

	Val = (PrevGain - Penalty / Cases) / PrevInfo;
	Better = ( GEnv.Blocks >= 2 && GEnv.ReasonableSubsets >= 2 &&
		   Val >= BestVal );

	Verbosity(2,
	{
	    fprintf(Of, "\tprelim merges -> inf %.3f, gain %.3f, val %.3f%s%s",
			PrevInfo, PrevGain, Val,
		        ( Better ? " **" : "" ),
			(VERBOSITY > 2 ? "" : "\n" ));
	    Verbosity(3, PrintDistribution(Att, 0, GEnv.Blocks, GEnv.Freq,
					   GEnv.ValFreq, false))
	})

	if ( Better )
	{
	    Subsets[Att] = GEnv.Blocks;

	    ForEach(V1, 1, GEnv.Blocks)
	    {
		CopyBits(GEnv.Bytes, GEnv.WSubset[V1], Subset[Att][V1]);
	    }

	    Info[Att] = PrevInfo;
	    Gain[Att] = PrevGain - Penalty / KnownCases;
	    BestVal   = Val;
	}
    }
		
    /*  Determine initial information and entropy values  */

    ForEach(V1, 1, GEnv.Blocks)
    {
	GEnv.SubsetInfo[V1] = -GEnv.ValFreq[V1] * Log(GEnv.ValFreq[V1] / Cases);
	GEnv.SubsetEntr[V1] = TotalInfo(GEnv.Freq[V1], 1, MaxClass);
    }

    ForEach(V1, First, GEnv.Blocks-1)
    {
	ForEach(V2, V1+1, GEnv.Blocks)
	{
	    EvaluatePair(V1, V2, Cases);
	}
    }

    /*  Examine possible pair mergers and hill-climb  */

    while ( GEnv.Blocks > 2 )
    {
	BestV1 = 0;
	BestGain = -Epsilon;

	/*  For each possible pair of values, calculate the gain and
	    total info of a split in which they are treated as one.
	    Keep track of the pair with the best gain.  */

	ForEach(V1, First, GEnv.Blocks-1)
	{
	    ForEach(V2, V1+1, GEnv.Blocks)
	    {
		if ( GEnv.ReasonableSubsets == 2 &&
		     GEnv.ValFreq[V1] >= MINITEMS-Epsilon &&
		     GEnv.ValFreq[V2] >= MINITEMS-Epsilon )
		{
		    continue;
		}

		ThisGain = PrevGain -
			   ((1-UnknownRate) / KnownCases) *
			     (GEnv.MergeEntr[V1][V2] -
			       (GEnv.SubsetEntr[V1] + GEnv.SubsetEntr[V2]));
		ThisInfo = PrevInfo + (GEnv.MergeInfo[V1][V2] -
			   (GEnv.SubsetInfo[V1] + GEnv.SubsetInfo[V2])) / Cases;
		Verbosity(3,
		    fprintf(Of, "\t    combine %d %d info %.3f gain %.3f\n",
			    V1, V2, ThisInfo, ThisGain))

		/*  See whether this merge has the best gain so far  */

		if ( ThisGain > BestGain+Epsilon )
		{
		    BestGain = ThisGain;
		    BestInfo = ThisInfo;
		    BestV1   = V1;
		    BestV2   = V2;
		}
	    }
	}

	if ( ! BestV1 ) break;

	PrevGain = BestGain;
	PrevInfo = BestInfo;

	/*  Determine penalty as log of Bell number.  If number is too
	    large, use an approximation of log  */

	Penalty  = ( finite(Bell[InitialBlocks][GEnv.Blocks-1]) ?
			Log(Bell[InitialBlocks][GEnv.Blocks-1]) :
			(InitialBlocks-GEnv.Blocks+1) * Log(GEnv.Blocks-1) );

	Val = (BestGain - Penalty / Cases) / BestInfo;

	Merge(BestV1, BestV2, Cases);

	Verbosity(2,
	    fprintf(Of, "\tform subset ");
	    PrintSubset(Att, GEnv.WSubset[BestV1]);
	    fprintf(Of, ": %d subsets, inf %.3f, gain %.3f, val %.3f%s\n",
		   GEnv.Blocks, BestInfo, BestGain, Val,
		   ( Val > BestVal ? " **" : "" ));
	    Verbosity(3,
		PrintDistribution(Att, 0, GEnv.Blocks, GEnv.Freq, GEnv.ValFreq,
				  false))
	    )

	if ( Val >= BestVal )
	{
	    Subsets[Att] = GEnv.Blocks;

	    ForEach(V1, 1, GEnv.Blocks)
	    {
		CopyBits(GEnv.Bytes, GEnv.WSubset[V1], Subset[Att][V1]);
	    }

	    Info[Att] = BestInfo;
	    Gain[Att] = BestGain - Penalty / KnownCases;
	    BestVal   = Val;
	}
    }

    /*  Add missing values as another branch  */

    if ( MissingValues )
    {
	Subsets[Att]++;
	CopyBits(GEnv.Bytes, Subset[Att][0], Subset[Att][Subsets[Att]]);
    }

    Verbosity(2,
	fprintf(Of, "\tfinal inf %.3f, gain %.3f, val=%.3f\n",
		Info[Att], Gain[Att], Gain[Att] / (Info[Att] + 1E-3)))
}



/*************************************************************************/
/*									 */
/*	Combine the distribution figures of subsets x and y.		 */
/*	Update Freq, ValFreq, SubsetInfo, SubsetEntr, MergeInfo, and	 */
/*	MergeEntr.							 */
/*									 */
/*************************************************************************/


void Merge(DiscrValue x, DiscrValue y, CaseCount Cases)
/*   -----  */
{
    ClassNo	c;
    double	Entr=0;
    CaseCount	KnownCases=0;
    int		R, C;

    AddBlock(x, y);

    ForEach(c, 1, MaxClass)
    {
	Entr -= GEnv.Freq[x][c] * Log(GEnv.Freq[x][c]);
	KnownCases += GEnv.Freq[x][c];
    }

    GEnv.SubsetInfo[x] = - GEnv.ValFreq[x] * Log(GEnv.ValFreq[x] / Cases);
    GEnv.SubsetEntr[x] = Entr + KnownCases * Log(KnownCases);

    /*  Eliminate y from working blocks  */

    ForEach(R, y, GEnv.Blocks-1)
    {
	MoveBlock(R, R+1);

	GEnv.SubsetInfo[R] = GEnv.SubsetInfo[R+1];
	GEnv.SubsetEntr[R] = GEnv.SubsetEntr[R+1];

	ForEach(C, 1, GEnv.Blocks)
	{
	    GEnv.MergeInfo[R][C] = GEnv.MergeInfo[R+1][C];
	    GEnv.MergeEntr[R][C] = GEnv.MergeEntr[R+1][C];
	}
    }

    ForEach(C, y, GEnv.Blocks-1)
    {
	ForEach(R, 1, GEnv.Blocks-1)
	{
	    GEnv.MergeInfo[R][C] = GEnv.MergeInfo[R][C+1];
	    GEnv.MergeEntr[R][C] = GEnv.MergeEntr[R][C+1];
	}
    }
    GEnv.Blocks--;

    /*  Update information for newly-merged block  */

    ForEach(C, 1, GEnv.Blocks)
    {
	if ( C != x ) EvaluatePair(x, C, Cases);
    }
}



/*************************************************************************/
/*									 */
/*	Calculate the effect of merging subsets x and y			 */
/*									 */
/*************************************************************************/


void EvaluatePair(DiscrValue x, DiscrValue y, CaseCount Cases)
/*   ------------  */
{
    ClassNo	c;
    double	Entr=0;
    CaseCount	KnownCases=0, F;

    if ( y < x )
    {
	c = y;
	y = x;
	x = c;
    }

    F = GEnv.ValFreq[x] + GEnv.ValFreq[y];
    GEnv.MergeInfo[x][y] = - F * Log(F / Cases);

    ForEach(c, 1, MaxClass)
    {
	F = GEnv.Freq[x][c] + GEnv.Freq[y][c];
	Entr -= F * Log(F);
	KnownCases += F;
    }
    GEnv.MergeEntr[x][y] = Entr + KnownCases * Log(KnownCases);
}



/*************************************************************************/
/*									 */
/*	Check whether two values have same class distribution		 */
/*									 */
/*************************************************************************/


Boolean SameDistribution(DiscrValue V1, DiscrValue V2)
/*	----------------  */
{
    ClassNo	c;
    CaseCount	D1, D2;

    D1 = GEnv.ValFreq[V1];
    D2 = GEnv.ValFreq[V2];

    ForEach(c, 1, MaxClass)
    {
	if ( fabs(GEnv.Freq[V1][c] / D1 - GEnv.Freq[V2][c] / D2) > 0.001 )
	{
	    return false;
	}
    }

    return true;
}



/*************************************************************************/
/*									 */
/*	Add frequency and subset information from block V2 to V1	 */
/*									 */
/*************************************************************************/


void AddBlock(DiscrValue V1, DiscrValue V2)
/*   --------  */
{
    ClassNo	c;
    int		b;

    if ( GEnv.ValFreq[V1] >= MINITEMS-Epsilon &&
	 GEnv.ValFreq[V2] >= MINITEMS-Epsilon )
    {
	GEnv.ReasonableSubsets--;
    }
    else
    if ( GEnv.ValFreq[V1] < MINITEMS-Epsilon &&
	 GEnv.ValFreq[V2] < MINITEMS-Epsilon &&
	 GEnv.ValFreq[V1] + GEnv.ValFreq[V2] >= MINITEMS-Epsilon )
    {
	GEnv.ReasonableSubsets++;
    }

    ForEach(c, 1, MaxClass)
    {
	GEnv.Freq[V1][c] += GEnv.Freq[V2][c];
    }
    GEnv.ValFreq[V1] += GEnv.ValFreq[V2];
    GEnv.ValFreq[V2] = 0;
    ForEach(b, 0, GEnv.Bytes-1)
    {
	GEnv.WSubset[V1][b] |= GEnv.WSubset[V2][b];
    }
}



/*************************************************************************/
/*									 */
/*	Move frequency and subset information from block V2 to V1	 */
/*									 */
/*************************************************************************/


void MoveBlock(DiscrValue V1, DiscrValue V2)
/*   ---------  */
{
    ClassNo	c;

    ForEach(c, 1, MaxClass)
    {
	GEnv.Freq[V1][c] = GEnv.Freq[V2][c];
    }
    GEnv.ValFreq[V1] = GEnv.ValFreq[V2];
    CopyBits(GEnv.Bytes, GEnv.WSubset[V2], GEnv.WSubset[V1]);
}



/*************************************************************************/
/*									 */
/*	Print the values of attribute Att which are in the subset Ss	 */
/*									 */
/*************************************************************************/


void PrintSubset(Attribute Att, Set Ss)
/*   -----------  */
{
    DiscrValue	V1;
    Boolean	First=true;

    ForEach(V1, 1, MaxAttVal[Att])
    {
	if ( In(V1, Ss) )
	{
	    if ( First )
	    {
		First = false;
	    }
	    else
	    {
		fprintf(Of, ", ");
	    }

	    fprintf(Of, "%s", AttValName[Att][V1]);
	}
    }
}



/*************************************************************************/
/*									 */
/*	Construct and return a node for a test on a subset of values	 */
/*									 */
/*************************************************************************/


void SubsetTest(Tree Node, Attribute Att)
/*   -----------  */
{
    int	S, Bytes;

    Sprout(Node, Subsets[Att]);

    Node->NodeType = BrSubset;
    Node->Tested   = Att;

    Bytes = (MaxAttVal[Att]>>3) + 1;
    Node->Subset = AllocZero(Subsets[Att]+1, Set);
    ForEach(S, 1, Node->Forks)
    {
	Node->Subset[S] = Alloc(Bytes, Byte);
	CopyBits(Bytes, Subset[Att][S], Node->Subset[S]);
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Prune a decision tree and predict its error rate		 */
/*	------------------------------------------------		 */
/*									 */
/*************************************************************************/




#define	  LocalVerbosity(x,s)	if (Sh >= 0) {Verbosity(x,s)}
#define	  Intab(x)		Indent(x, 0)

#define	  UPDATE		1	/* flag: change tree */
#define	  REGROW		2	/*       regrow branches */
#define	  REPORTPROGRESS	4	/*	 original tree */
#define	  UNITWEIGHTS		8	/*	 UnitWeights is true*/

Set		*PossibleValues;

double		MaxExtraErrs,		/* limit for global prune */
		TotalExtraErrs;		/* extra errors from ties */
Tree		*XT;			/* subtrees with lowest cost comp */
int		NXT;			/* number ditto */
float		MinCC;			/* cost compexity for XT */
Boolean		RecalculateErrs;	/* if missing values */




/*************************************************************************/
/*									 */
/*	Prune tree T							 */
/*									 */
/*************************************************************************/


void Prune(Tree T)
/*   -----  */
{
    Attribute	Att;
    int		i, Options;
    Boolean	Regrow;

    Verbosity(2, fprintf(Of, "\n"))

    Regrow = ( Trial == 0 || Now == WINNOWATTS );

    /*  Local pruning phase  */


    Options = ( Now == WINNOWATTS ? (UPDATE|REGROW) :
		Regrow ? (UPDATE|REGROW|REPORTPROGRESS) :
			 (UPDATE|REPORTPROGRESS) );
    if ( UnitWeights ) Options |= UNITWEIGHTS;

    EstimateErrs(T, 0, MaxCase, 0, Options);

    if ( MCost )
    {
	/*  Remove any effects of WeightMul and reset leaf classes  */

	RestoreDistribs(T);
    }
    else
    {
	/*  Insert information on parents and recalculate errors, noting
	    whether fractional cases might have appeared (for GlobalPrune)  */

	RecalculateErrs = false;
	InsertParents(T, Nil);

	/*  Possible global pruning phase  */

	if ( GLOBAL && Now != WINNOWATTS )
	{
	    GlobalPrune(T);
	}
    }

    /*  Remove impossible values from subsets and ordered splits.
	First record possible values for discrete attributes  */

    PossibleValues = AllocZero(MaxAtt+1, Set);
    ForEach(Att, 1, MaxAtt)
    {
	if ( Ordered(Att) || Discrete(Att) && SUBSET )
	{
	    PossibleValues[Att] = AllocZero((MaxAttVal[Att]>>3)+1, Byte);
	    ForEach(i, 1, MaxAttVal[Att])
	    {
		SetBit(i, PossibleValues[Att]);
	    }
	}
    }

    CheckSubsets(T, true);

    FreeVector((void **) PossibleValues, 1, MaxAtt);	PossibleValues = Nil;

    /*  For multibranch splits, merge non-occurring values.  For trees
	(first boosting trial only), also merge leaves of same class  */

    if ( ! SUBSET )
    {
	CompressBranches(T);
    }
}



/*************************************************************************/
/*									 */
/*	Estimate the errors in a given subtree				 */
/*									 */
/*************************************************************************/


void EstimateErrs(Tree T, CaseNo Fp, CaseNo Lp, int Sh, int Flags)
/*   ------------  */
{
    CaseNo	i, Bp, Ep, Missing;
    CaseCount	Cases=0, KnownCases, *BranchCases, MissingCases,
		*SmallBranches, SmallBranchCases=0,
		Errs, SaveErrs, TreeErrs, LeafErrs, ExtraLeafErrs=0, BestBrErrs;
    double	Factor, *LocalClassDist;
    DiscrValue	v, BestBr=0;
    ClassNo	c, BestClass=1;
    int		UnitWeights;			/* local value */
    Tree	Br;
    Attribute	Att;


    if ( Fp > Lp ) return;

    UnitWeights = (Flags & UNITWEIGHTS);

    LocalClassDist = Alloc(MaxClass+1, double);

    FindClassFreq(LocalClassDist, Fp, Lp);

    /*  Record new class distribution if updating the tree  */

    if ( (Flags & UPDATE) )
    {
	ForEach(c, 1, MaxClass)
	{
	    T->ClassDist[c] = LocalClassDist[c];
	    Cases += LocalClassDist[c];

	    if ( LocalClassDist[c] > LocalClassDist[BestClass] ) BestClass = c;
	}
    }
    else
    {
	ForEach(c, 1, MaxClass)
	{
	    Cases += LocalClassDist[c];

	    if ( LocalClassDist[c] > LocalClassDist[BestClass] ) BestClass = c;
	}
    }

    LeafErrs = Cases - LocalClassDist[BestClass];
    ExtraLeafErrs = ExtraErrs(Cases, LeafErrs, BestClass);

    Free(LocalClassDist);

    if ( (Flags & UPDATE) )
    {
	T->Cases = Cases;
	T->Leaf  = BestClass;
    }

    if ( ! T->NodeType )	/*  leaf  */
    {
	if ( (Flags & UPDATE) && (Flags & REPORTPROGRESS) &&
	     Now == SIMPLIFYTREE &&
	     T->Cases > 0 )
	{
	    Progress(T->Cases);
	}

	T->Errors = LeafErrs + ExtraLeafErrs;

	if ( (Flags & UPDATE) )
	{
	    if ( Sh >= 0 )
	    {
		LocalVerbosity(3,
		    Intab(Sh);
		    fprintf(Of, "%s (%.2f:%.2f/%.2f)\n", ClassName[T->Leaf],
			    T->Cases, LeafErrs, T->Errors))
	    }
	}

	return;
    }

    /*  Estimate errors for each branch  */

    Att = T->Tested;
    Missing = (Ep = Group(0, Fp, Lp, T)) - Fp + 1;

    if ( CostWeights )
    {
	MissingCases = SumNocostWeights(Fp, Ep);
	KnownCases   = SumNocostWeights(Ep+1, Lp);
    }
    else
    {
	MissingCases = CountCases(Fp, Ep);
	KnownCases   = Cases - MissingCases;
    }

    SmallBranches = AllocZero(MaxClass+1, CaseCount);
    BranchCases   = Alloc(T->Forks+1, CaseCount);

    if ( Missing ) UnitWeights = 0;

    TreeErrs = 0;
    Bp = Fp;

    ForEach(v, 1, T->Forks)
    {
	Ep = Group(v, Bp + Missing, Lp, T);

	/*  Bp -> first value in missing + remaining values
	    Ep -> last value in missing + current group  */

	BranchCases[v] = CountCases(Bp + Missing, Ep);

	Factor = ( ! Missing ? 0 :
		   ! CostWeights ? BranchCases[v] / KnownCases :
		   SumNocostWeights(Bp + Missing, Ep) / KnownCases );

	if ( (BranchCases[v] += Factor * MissingCases) >= MinLeaf )
	{
	    if ( Missing )
	    {
		ForEach(i, Bp, Bp + Missing - 1)
		{
		    Weight(Case[i]) *= Factor;
		}
	    }

	    EstimateErrs(T->Branch[v], Bp, Ep, Sh+1, ((Flags&7) | UnitWeights));

	    /*  Group small branches together for error estimation  */

	    if ( BranchCases[v] < MINITEMS )
	    {
		ForEach(i, Bp, Ep)
		{
		    SmallBranches[ Class(Case[i]) ] += Weight(Case[i]);
		}

		SmallBranchCases += BranchCases[v];
	    }
	    else
	    {
		TreeErrs += T->Branch[v]->Errors;
	    }

	    /*  Restore weights if changed  */

	    if ( Missing )
	    {
		for ( i = Ep ; i >= Bp ; i-- )
		{
		    if ( Unknown(Case[i], Att) )
		    {
			Weight(Case[i]) /= Factor;
			Swap(i, Ep);
			Ep--;
		    }
		}
	    }

	    Bp = Ep+1;
	}
    }

    /*  Add error estimate from small branches, if any  */

    if ( SmallBranchCases )
    {
	BestClass = 1;
	ForEach(c, 2, MaxClass)
	{
	    if ( SmallBranches[c] > SmallBranches[BestClass] ) BestClass = c;
	}

	Errs = SmallBranchCases - SmallBranches[BestClass];
	TreeErrs += Errs + ExtraErrs(SmallBranchCases, Errs, BestClass);
    }
    Free(SmallBranches);
    Free(BranchCases);

    if ( ! (Flags & UPDATE) )
    {
	T->Errors = TreeErrs;
	return;
    }

    /*  See how the largest candidate branch would perform.  A branch
	is a candidate if it is not a leaf, contains at least 10% of
	the cases, and does not test the same continuous attribute.
	This test is skipped for boosted trees  */

    ForEach(v, 1, T->Forks)
    {
	if ( ! T->Branch[v]->NodeType ||
	     T->Branch[v]->Cases < 0.1 * T->Cases ||
	     T->Branch[v]->Tested == Att && Continuous(Att) )
	{
	    continue;
	}

	if ( ! BestBr || T->Branch[v]->Cases > T->Branch[BestBr]->Cases )
	{
	    BestBr = v;
	}
    }

    if ( BestBr )
    {
	SaveErrs = T->Branch[BestBr]->Errors;
	EstimateErrs(T->Branch[BestBr], Fp, Lp, -1, 0);
	BestBrErrs = T->Branch[BestBr]->Errors;
	T->Branch[BestBr]->Errors = SaveErrs;
    }
    else
    {
	BestBrErrs = MaxCase+1;
    }

    LocalVerbosity(2,
	Intab(Sh);
	fprintf(Of, "%s:  [%d%%  N=%.2f  tree=%.2f  leaf=%.2f+%.2f",
		AttName[T->Tested],
		(int) ((TreeErrs * 100) / (T->Cases + 0.001)),
		T->Cases, TreeErrs, LeafErrs, ExtraLeafErrs);
	if ( BestBr )
	{
	    fprintf(Of, "  br[%d]=%.2f", BestBr, BestBrErrs);
	}
	fprintf(Of, "]\n"))

    /*  See whether tree should be replaced with leaf or best branch  */

    if ( LeafErrs + ExtraLeafErrs <= BestBrErrs + 0.1 &&
	 LeafErrs + ExtraLeafErrs <= TreeErrs + 0.1 )
    {
	LocalVerbosity(2,
	    Intab(Sh);
	    fprintf(Of, "Replaced with leaf %s\n", ClassName[T->Leaf]))

	UnSprout(T);
	T->Errors = LeafErrs + ExtraLeafErrs;
    }
    else
    if ( BestBrErrs <= TreeErrs + 0.1 )
    {
	LocalVerbosity(2,
	    Intab(Sh);
	    fprintf(Of, "Replaced with branch %d\n", BestBr))

	/*  Free unused bits of tree  */

	ForEach(v, 1, T->Forks)
	{
	    if ( v != BestBr ) FreeTree(T->Branch[v]);
	}
	Br = T->Branch[BestBr];
	Free(T->Branch);
	Free(T->ClassDist);
	if ( T->NodeType == BrSubset )
	{
	    FreeVector((void **) T->Subset, 1, T->Forks);
	}

	/*  Copy the branch up  */

	memcpy((char *) T, (char *) Br, sizeof(TreeRec));
	Free(Br);

	Factor = T->Cases / Cases;
	T->Cases = Cases;

	/*  If not within a rebuilt tree, not a cascaded test, and
	    sufficient new cases to justify the effort, rebuild the branch  */

	if ( T->NodeType && (Flags & REGROW) && Factor < 0.95 )
	{
	    ForEach(v, 1, T->Forks)
	    {
		FreeTree(T->Branch[v]);			T->Branch[v] = Nil;
	    }

	    SetGlobalUnitWeights(Flags & UNITWEIGHTS);

		// Daniel: Remove this part of code as it conflicts with the modifications
		assert(false);
		exit(1);
	    //Divide(T, Fp, Lp, 0);
	}

	EstimateErrs(T, Fp, Lp, Sh, UPDATE);
    }
    else
    {
	T->Errors = TreeErrs;
    }
}



/*************************************************************************/
/*									 */
/*	Phase 2 (global) pruning.					 */
/*	Prune minimum cost complexity subtrees until "error"		 */
/*	(measured by sum of branch errors) increases by 1SE		 */
/*									 */
/*************************************************************************/


void GlobalPrune(Tree T)
/*   -----------  */
{
    int		DeltaLeaves, x;
    double	BaseErrs, DeltaErrs;
    CaseNo	i;
    Tree	ST;

    /*  If fractional cases may have been used, calculate errors
	directly by checking training data  */

    if ( RecalculateErrs )
    {
	BaseErrs = 0;
	ForEach(i, 0, MaxCase)
	{
	    if ( TreeClassify(Case[i], T) != Class(Case[i]) )
	    {
		BaseErrs += Weight(Case[i]);
	    }
	}
    }
    else
    {
	BaseErrs = T->Errors;
    }

    XT = Alloc(T->Leaves, Tree);

    /*  Additional error limit set at 1SE  */

    MaxExtraErrs = sqrt(BaseErrs * (1 - BaseErrs / (MaxCase + 1)));

    while ( MaxExtraErrs > 0 )
    {
	TotalExtraErrs = 0;

	MinCC = 1E38;
	NXT   = 0;

	/*  Find all subtrees with lowest cost complexity  */

	FindMinCC(T);

	Verbosity(2,
	    if ( NXT > 0 && TotalExtraErrs > MaxExtraErrs )
		fprintf(Of, "%d tied with MinCC=%.3f; total extra errs %.1f\n",
			NXT, MinCC, TotalExtraErrs))

	if ( ! NXT || TotalExtraErrs > MaxExtraErrs ) break;

	/*  Make subtree into a leaf  */

	ForEach(x, 0, NXT-1)
	{
	    ST = XT[x];

	    UnSprout(ST);

	    /*  Update errors and leaves for ST and ancestors  */

	    DeltaErrs   = (ST->Cases - ST->ClassDist[ST->Leaf]) - ST->Errors;
	    DeltaLeaves = 1 - ST->Leaves;
	    while ( ST )
	    {
		ST->Errors += DeltaErrs;
		ST->Leaves += DeltaLeaves;
		ST = ST->Parent;
	    }

	    MaxExtraErrs -= DeltaErrs;

	    Verbosity(2,
		fprintf(Of, "global: %d leaves, %.1f errs\n",
			DeltaLeaves, DeltaErrs))
	}
	Verbosity(2, fprintf(Of, "\tremaining=%.1f\n", MaxExtraErrs))
    }

    Free(XT);
}



/*************************************************************************/
/*									 */
/*	Scan tree computing cost complexity of each subtree and		 */
/*	record lowest in global variable XT				 */
/*									 */
/*************************************************************************/


void FindMinCC(Tree T)
/*   ---------  */
{
    DiscrValue	v;
    double	ExtraErrs, CC, SaveMinCC, SaveTotalExtraErrs;
    int		SaveNXT;

    if ( T->NodeType )
    {
	/*  Save current situation  */

	SaveTotalExtraErrs = TotalExtraErrs;
	SaveMinCC          = MinCC;
	SaveNXT		   = NXT;

	/*  Scan subtrees  */

	ForEach(v, 1, T->Forks)
	{
	    if ( T->Branch[v]->Cases > 0.1 )
	    {
		FindMinCC(T->Branch[v]);
	    }
	}
	
	/*  Compute CC for this subtree and check whether minimum  */

	ExtraErrs = (T->Cases - T->ClassDist[T->Leaf]) - T->Errors;

	CC = ExtraErrs / (T->Leaves - 1);

	if ( ExtraErrs <= MaxExtraErrs )
	{
	    /*  Have to be careful of ties in descendants, because
		they would inflate TotalExtraErrs.  Any such ties
		should be discarded  */

	    if ( CC < MinCC ||
		 CC <= MinCC && CC < SaveMinCC /* changed by descendants */ )
	    {
		/*  This is the first of a possible group of ties  */

		MinCC = CC;
		NXT   = 1;
		XT[0] = T;
		TotalExtraErrs = ExtraErrs;
	    }
	    else
	    if ( CC <= MinCC )
	    {
		/*  This is a tie.  Discard any ties among descendants  */

		if ( NXT > SaveNXT )
		{
		    TotalExtraErrs = SaveTotalExtraErrs;
		    NXT		   = SaveNXT;
		}

		XT[NXT++] = T;
		TotalExtraErrs += ExtraErrs;
	    }
	}
    }
}



/*************************************************************************/
/*									 */
/*	Annotate tree with information on parent and leaves		 */
/*									 */
/*************************************************************************/


void InsertParents(Tree T, Tree P)
/*   -------------  */
{
    DiscrValue	v;

    T->Parent = P;
    T->Errors = T->Leaves = 0;

    if ( T->NodeType )
    {
	ForEach(v, 1, T->Forks)
	{
	    InsertParents(T->Branch[v], T);
	    T->Errors += T->Branch[v]->Errors;
	    T->Leaves += T->Branch[v]->Leaves;
	}

	if ( SomeMiss[T->Tested] ) RecalculateErrs = true;
    }
    else
    if ( T->Cases > 1E-3 )
    {
	T->Errors = T->Cases - T->ClassDist[T->Leaf];
	T->Leaves = 1;
    }
}



/*************************************************************************/
/*									 */
/*	Remove unnecessary subset tests on missing values		 */
/*									 */
/*************************************************************************/


void CheckSubsets(Tree T, Boolean PruneDefaults)
/*   ------------  */
{
    Set		HoldValues;
    int		v, vv, x, Bytes, b, First, Any=0;
    Attribute	A;
    Tree	LeafBr;
    ClassNo	c;

    if ( T->NodeType == BrSubset )
    {
	A = T->Tested;

	Bytes = (MaxAttVal[A]>>3) + 1;
	HoldValues = Alloc(Bytes, Byte);

	/*  For non-ordered attributes the last (default) branch contains
	    all values that do not appear in the data.  See whether this
	    branch can be simplified or omitted  */

	if ( ! Ordered(A) && PruneDefaults )
	{
	    ForEach(b, 0, Bytes-1)
	    {
		T->Subset[T->Forks][b] &= PossibleValues[A][b];
		Any |= T->Subset[T->Forks][b];
	    }

	    if ( ! Any )
	    {
		FreeTree(T->Branch[T->Forks]);
		Free(T->Subset[T->Forks]);
		T->Forks--;
	    }
	}

	/*  Process each subtree, leaving only values in branch subset  */

	CopyBits(Bytes, PossibleValues[A], HoldValues);

	ForEach(v, 1, T->Forks)
	{
	    /*  Remove any impossible values from ordered subsets  */

	    if ( Ordered(A) )
	    {
		ForEach(vv, 1, MaxAttVal[A])
		{
		    if ( In(vv, T->Subset[v]) && ! In(vv, HoldValues) )
		    {
			ResetBit(vv, T->Subset[v]);
		    }
		}
	    }

	    CopyBits(Bytes, T->Subset[v], PossibleValues[A]);

	    CheckSubsets(T->Branch[v], PruneDefaults);
	}

	CopyBits(Bytes, HoldValues, PossibleValues[A]);

	Free(HoldValues);

	/*  See whether branches other than N/A can be merged.
	    This cannot be done for ordered attributes since the
	    values in the subset represent an interval  */

	if ( ! Ordered(A) )
	{
	    First = ( In(1, T->Subset[1]) ? 2 : 1 );
	    for ( v = First ; v < T->Forks ; v++ )
	    {
		if ( T->Branch[v]->NodeType ) continue;
		LeafBr = T->Branch[v];

		/*  Consider branches vv that could be merged with branch v  */

		for ( vv = v+1 ; vv <= T->Forks ;  )
		{
		    if ( ! T->Branch[vv]->NodeType &&
			 T->Branch[vv]->Leaf == LeafBr->Leaf &&
			 ( PruneDefaults || T->Branch[vv]->Cases > 0 ) )
		    {
			/*  Branch vv can be merged with branch v  */

			if ( T->Branch[vv]->Cases )
			{
			    /*  Add class distribution from branch vv,
				or replace if branch v has no cases  */

			    ForEach(c, 1, MaxClass)
			    {
				if ( ! LeafBr->Cases )
				{
				    LeafBr->ClassDist[c] =
					T->Branch[vv]->ClassDist[c];
				}
				else
				{
				    LeafBr->ClassDist[c] +=
					T->Branch[vv]->ClassDist[c];
				}
			    }
			    LeafBr->Cases  += T->Branch[vv]->Cases;
			    LeafBr->Errors += T->Branch[vv]->Errors;
			}

			/*  Merge values and free branch vv  */

			ForEach(b, 0, Bytes-1)
			{
			    T->Subset[v][b] |= T->Subset[vv][b];
			}
			FreeTree(T->Branch[vv]);
			Free(T->Subset[vv]);

			T->Forks--;
			ForEach(x, vv, T->Forks)
			{
			    T->Branch[x] = T->Branch[x+1];
			    T->Subset[x] = T->Subset[x+1];
			}
		    }
		    else
		    {
			vv++;
		    }
		}
	    }
	}
    }
    else
    if ( T->NodeType )
    {
	ForEach(v, 1, T->Forks)
	{
	    CheckSubsets(T->Branch[v], PruneDefaults);
	}
    }
}



/*************************************************************************/
/*									 */
/*	Compute Coeff, used by RawExtraErrs() to adjust resubstitution	 */
/*	error rate to upper limit of the confidence level.  Coeff is	 */
/*	the square of the number of standard deviations corresponding	 */
/*	to the selected confidence level.  (Taken from Documenta Geigy	 */
/*	Scientific Tables (Sixth Edition), p185 (with modifications).)	 */
/*									 */
/*************************************************************************/


float Val[] = {  0,  0.001, 0.005, 0.01, 0.05, 0.10, 0.20, 0.40, 1.00},
      Dev[] = {4.0,  3.09,  2.58,  2.33, 1.65, 1.28, 0.84, 0.25, 0.00},
      Coeff;


void InitialiseExtraErrs()
/*   -------------------  */
{
    int i=1;

    /*  Compute and retain the coefficient value, interpolating from
	the values in Val and Dev  */

    while ( CF > Val[i] ) i++;

    Coeff = Dev[i-1] +
	      (Dev[i] - Dev[i-1]) * (CF - Val[i-1]) /(Val[i] - Val[i-1]);
    Coeff = Coeff * Coeff;
    CF = Max(CF, 1E-6);
}


/*************************************************************************/
/*									 */
/*	Calculate extra errors to correct the resubstitution error	 */
/*	rate at a leaf with N cases, E errors, predicted class C.	 */
/*	If CostWeights are used, N and E are normalised by removing	 */
/*	the effects of cost weighting and then reapplying weights to	 */
/*	the result.							 */
/*									 */
/*************************************************************************/


float ExtraErrs(CaseCount N, CaseCount E, ClassNo C)
/*    ---------  */
{
    ClassNo	EC;
    CaseCount	NormC, NormEC;

    if ( ! CostWeights )
    {
	return RawExtraErrs(N, E);
    }

    EC = 3 - C;				/* the other class */
    NormC = (N - E) / WeightMul[C];	/* normalised cases of class C */
    NormEC = E / WeightMul[EC];		/* ditto the other class */

    return WeightMul[EC] * RawExtraErrs(NormC + NormEC, NormEC);
}



float RawExtraErrs(CaseCount N, CaseCount E)
/*    ------------  */
{
    float	Val0, Pr;

    if ( E < 1E-6 )
    {
	return N * (1 - exp(log(CF) / N));
    }
    else
    if ( N > 1 && E < 0.9999 )
    {
	Val0 = N * (1 - exp(log(CF) / N));
	return Val0 + E * (RawExtraErrs(N, 1.0) - Val0);
    }
    else
    if ( E + 0.5 >= N )
    {
	return 0.67 * (N - E);
    }
    else
    {
	Pr = (E + 0.5 + Coeff/2
		+ sqrt(Coeff * ((E + 0.5) * (1 - (E + 0.5)/N) + Coeff/4)) )
	     / (N + Coeff);
	return (N * Pr - E);
    }
}



/*************************************************************************/
/*									 */
/*	If there are differential misclassification costs, the weights	 */
/*	may have been artificially adjusted.  Fix the distributions so	 */
/*	that they represent the "true" (possibly boosted) weights	 */
/*									 */
/*************************************************************************/


void RestoreDistribs(Tree T)
/*   ---------------  */
{
    DiscrValue	v;
    ClassNo	c;

    if ( T->NodeType )
    {
	ForEach(v, 1, T->Forks)
	{
	    RestoreDistribs(T->Branch[v]);
	}
    }

    if ( T->Cases >= MinLeaf )
    {
	if ( CostWeights )
	{
	    T->Cases = 0;
	    ForEach(c, 1, MaxClass)
	    {
		ClassSum[c] = (T->ClassDist[c] /= WeightMul[c]);
		T->Cases += T->ClassDist[c];
	    }
	}
	else
	{
	    ForEach(c, 1, MaxClass)
	    {
		ClassSum[c] = T->ClassDist[c];
	    }
	}

	T->Leaf   = SelectClass(1, true);
	T->Errors = T->Cases - T->ClassDist[T->Leaf];
    }
}



/*************************************************************************/
/*									 */
/*	See whether empty branches can be formed into subsets.		 */
/*	For the first trial only, and when not generating rulesets,	 */
/*	combine leaves with the same class.				 */
/*									 */
/*************************************************************************/


void CompressBranches(Tree T)
/*   ----------------  */
{
    DiscrValue	v, vv, S=0, *LocalSet;
    int		Bytes;
    Tree	Br, *OldBranch;
    ClassNo	c;
    Boolean	EmptyOnly;

    EmptyOnly = Trial || RULES;

    if ( T->NodeType )
    {
	/*  LocalSet[v] is the new branch number to which branch v belongs  */

	LocalSet = AllocZero(T->Forks+1, DiscrValue);

	ForEach(v, 1, T->Forks)
	{
	    Br = T->Branch[v];
	    CompressBranches(Br);

	    /*  Don't check if compression impossible  */

	    if ( v == 1 || T->Forks < 4 || Br->NodeType ||
		 EmptyOnly && Br->Cases >= MinLeaf )
	    {
		vv = v + 1;
	    }
	    else
	    {
		/*  Check whether some previous branch is mergeable.
		    For Trial 0, leaves are mergeable if they are
		    both empty or both non-empty and have the same class;
		    for later trials, they must both be empty  */

		for ( vv = 2 ; vv < v ; vv++ )
		{
		    if ( ! T->Branch[vv]->NodeType &&
			 ( EmptyOnly ? T->Branch[vv]->Cases < MinLeaf :
			     ( T->Branch[vv]->Cases < MinLeaf ) ==
				 ( Br->Cases < MinLeaf ) &&
			     T->Branch[vv]->Leaf == Br->Leaf ) )
		    {
			break;
		    }
		}
	    }

	    /*  If no merge was found, this becomes a new branch  */

	    LocalSet[v] = ( vv < v ? LocalSet[vv] : ++S );
	}

	if ( S < T->Forks )
	{
	    /*  Compress!  */

	    T->Subset   = Alloc(S+1, Set);
	    OldBranch   = T->Branch;
	    T->Branch	= Alloc(S+1, Tree);

	    Bytes = (MaxAttVal[T->Tested]>>3) + 1;
	    S = 0;

	    ForEach(v, 1, T->Forks)
	    {
		if ( LocalSet[v] > S )
		{
		    S++;
		    Br = T->Branch[S] = OldBranch[v];
		    if ( ! Br->ClassDist )
		    {
			Br->ClassDist = AllocZero(MaxClass+1, CaseCount);
		    }
		    T->Subset[S] = AllocZero(Bytes, Byte);

		    /*  Must include N/A even when no cases  -- otherwise
			reader gets the branches muddled  */

		    SetBit(v, T->Subset[S]);

		    ForEach(vv, v+1, T->Forks)
		    {
			if ( LocalSet[vv] == S )
			{
			    SetBit(vv, T->Subset[S]);

			    Br->Cases  += OldBranch[vv]->Cases;
			    Br->Errors += OldBranch[vv]->Errors;
			    ForEach(c, 1, MaxClass)
			    {
				Br->ClassDist[c] += OldBranch[vv]->ClassDist[c];
			    }
			}
		    }
		}
		else
		{
		    FreeTree(OldBranch[v]);
		}
	    }

	    T->NodeType = BrSubset;
	    T->Forks = S;
	    Free(OldBranch);
	}
	Free(LocalSet);
    }
}



void SetGlobalUnitWeights(int LocalFlag)
/*   --------------------  */
{
    UnitWeights = ( LocalFlag != 0 );
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Soften thresholds for continuous attributes			 */
/*	-------------------------------------------			 */
/*									 */
/*************************************************************************/




/*************************************************************************/
/*									 */
/*	Soften all thresholds for continuous attributes in tree T	 */
/*									 */
/*************************************************************************/


void SoftenThresh(Tree T)
/*   ------------  */
{
    ResubErrs(T, 0, MaxCase);

    FindBounds(T, 0, MaxCase);
}



/*************************************************************************/
/*									 */
/*	Find resubstitution errors for tree T				 */
/*									 */
/*************************************************************************/


void ResubErrs(Tree T, CaseNo Fp, CaseNo Lp)
/*   ---------  */
{
    CaseNo	i, Bp, Ep, Missing;
    CaseCount	Cases=0, KnownCases, BranchCases, MissingCases;
    double	Factor;
    DiscrValue	v;
    Boolean	PrevUnitWeights;
    Attribute	Att;

    if ( ! T->NodeType )
    {
	T->Errors = T->Cases - T->ClassDist[T->Leaf];
	return;
    }

    /*  Estimate errors for each branch  */

    Att = T->Tested;
    Missing = (Ep = Group(0, Fp, Lp, T)) - Fp + 1;

    if ( CostWeights )
    {
	MissingCases = SumNocostWeights(Fp, Ep);
	KnownCases   = SumNocostWeights(Ep+1, Lp);
    }
    else
    {
	MissingCases = CountCases(Fp, Ep);
	KnownCases   = Cases - MissingCases;
    }

    PrevUnitWeights = UnitWeights;
    if ( Missing ) UnitWeights = false;

    T->Errors = 0;
    Bp = Fp;

    ForEach(v, 1, T->Forks)
    {
	Ep = Group(v, Bp + Missing, Lp, T);

	/*  Bp -> first value in missing + remaining values
	    Ep -> last value in missing + current group  */

	BranchCases = CountCases(Bp + Missing, Ep);

	Factor = ( ! Missing ? 0 :
		   ! CostWeights ? BranchCases / KnownCases :
		   SumNocostWeights(Bp + Missing, Ep) / KnownCases );

	if ( BranchCases + Factor * MissingCases >= MinLeaf )
	{
	    if ( Missing )
	    {
		ForEach(i, Bp, Bp + Missing - 1)
		{
		    Weight(Case[i]) *= Factor;
		}
	    }

	    ResubErrs(T->Branch[v], Bp, Ep);

	    T->Errors += T->Branch[v]->Errors;

	    /*  Restore weights if changed  */

	    if ( Missing )
	    {
		for ( i = Ep ; i >= Bp ; i-- )
		{
		    if ( Unknown(Case[i], Att) )
		    {
			Weight(Case[i]) /= Factor;
			Swap(i, Ep);
			Ep--;
		    }
		}
	    }

	    Bp = Ep+1;
	}
    }

    UnitWeights = PrevUnitWeights;
}



/*************************************************************************/
/*								  	 */
/*	Calculate upper and lower bounds for each test on a continuous	 */
/*	attribute in tree T, using cases from Fp to Lp.			 */
/*								  	 */
/*	The lower bound is set so that the error rate of the GT branch	 */
/*	on the cases between the bound and the threshold is double that	 */
/*	of the correct (LE) branch; the upper bound is set similarly.	 */
/*								  	 */
/*************************************************************************/


void FindBounds(Tree T, CaseNo Fp, CaseNo Lp)
/*   --------  */
{
    int		v;
    CaseNo	i, j, Kp, Bp, Ap, Missing, SplitI;
    CaseCount	w, LEErrs, GTErrs, KnownCases, SE;
    ClassNo	RealClass;
    Attribute	Att;
    Boolean	PrevUnitWeights;
    double	Factor;

    /*  Stop when get to a leaf  */

    if ( ! T->NodeType ) return;

    Kp = Group(0, Fp, Lp, T) + 1;
    Missing = Kp - Fp;

    Att = T->Tested;
    KnownCases = CountCases(Kp, Lp);

    /*  Soften a threshold for a continuous attribute  */

    if ( T->NodeType == BrThresh )
    {
	Verbosity(1, fprintf(Of, "\nTest %s <> %g\n", AttName[Att], T->Cut))

	/*  Skip N/A values  */

	Ap = Group(1, Kp, Lp, T) + 1;

	Quicksort(Ap, Lp, Att);

	/*  Locate cut point and overall errors of the LE and GT branches  */

	SplitI = Ap;
	LEErrs = GTErrs = 0;
	ForEach(i, Ap, Lp)
	{
	    if ( CVal(Case[i], Att) <= T->Cut ) SplitI = i;
	}

	T->Mid = (CVal(Case[SplitI], Att) + CVal(Case[SplitI+1], Att)) / 2;

	/*  Consider cutoff points below and above the threshold.
	    The errors on the cases between the cutoff and the threshold
	    are computed for both the LE and GT branches.  The additional
	    errors must be less than 0.5SE and, further, the errors
	    on the "other" branch must not exceed twice the errors
	    on the "real" branch, both after Laplace adjustment  */

	SE = sqrt(T->Errors * (KnownCases - T->Errors) / (KnownCases + 1E-3))
	     * 2;

	LEErrs = GTErrs = 0;
	j = SplitI;
	for ( i = SplitI ; i > Ap ; i-- )
	{
	    RealClass = Class(Case[i]);

	    w = Weight(Case[i]);
	    GTErrs += w * ( TreeClassify(Case[i], T->Branch[3]) != RealClass );
	    LEErrs += w * ( TreeClassify(Case[i], T->Branch[2]) != RealClass );

	    if ( CVal(Case[i-1], Att) < CVal(Case[i], Att) )
	    {
		if ( GTErrs > 2 * LEErrs + 1 || GTErrs - LEErrs > 0.5 * SE )
		{
		    break;
		}

		j = i-1;
	    }
	}
	T->Lower = Min(T->Mid, CVal(Case[j], Att));

	LEErrs = GTErrs = 0;
	j = SplitI+1;
	for ( i = SplitI+1 ; i < Lp ; i++ )
	{
	    RealClass = Class(Case[i]);

	    w = Weight(Case[i]);
	    LEErrs += w * ( TreeClassify(Case[i], T->Branch[2]) != RealClass );
	    GTErrs += w * ( TreeClassify(Case[i], T->Branch[3]) != RealClass );

	    if ( CVal(Case[i], Att) < CVal(Case[i+1], Att) )
	    {
		if ( LEErrs > 2 * GTErrs + 1 || LEErrs - GTErrs > 0.5 * SE )
		{
		    break;
		}

		j = i+1;
	    }
	}
	T->Upper = Max(T->Mid, CVal(Case[j], Att));

	Verbosity(1,
	    fprintf(Of, "\tLower = %g, Upper = %g\n", T->Lower, T->Upper))
    }

    /*  Recursively scan each branch  */

    PrevUnitWeights = UnitWeights;
    if ( Missing > 0 ) UnitWeights = false;

    Bp = Fp;

    ForEach(v, 1, T->Forks)
    {
	Kp = Group(v, Bp + Missing, Lp, T);

	/*  Bp -> first value in missing + remaining values
	    Kp -> last value in missing + current group  */

	if ( Bp + Missing <= Kp &&
	     (Factor = CountCases(Bp + Missing, Kp) / KnownCases) > 1E-6 )
	{
	    if ( Missing )
	    {
		ForEach(i, Bp, Bp + Missing - 1)
		{
		    Weight(Case[i]) *= Factor;
		}
	    }

	    FindBounds(T->Branch[v], Bp, Kp);

	    /*  Restore weights if changed  */

	    if ( Missing )
	    {
		for ( i = Kp ; i >= Bp ; i-- )
		{
		    if ( Unknown(Case[i], Att) )
		    {
			Weight(Case[i]) /= Factor;
			Swap(i, Kp);
			Kp--;
		    }
		}
	    }

	    Bp = Kp+1;
	}
    }

    UnitWeights = PrevUnitWeights;
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Routines for displaying, building, saving and restoring trees	 */
/*	-------------------------------------------------------------	 */
/*									 */
/*************************************************************************/




#define	TabSize		4
#define Utility		ClassDist[0]
#define Digits(n)	((n) < 10 ? 3 : (int)(3 + log(n-1) / log(10.0)))


	/*  If lines look like getting too long while a tree is being
	    printed, subtrees are broken off and printed separately after
	    the main tree is finished	 */

int	SubTree,		/* highest subtree to be printed */
	SubSpace=0;		/* maximum subtree encountered */
Tree	*SubDef=Nil;		/* pointers to subtrees */
Boolean	LastBranch[Width];	/* whether printing last branch of subtree */



/*************************************************************************/
/*									 */
/*	Calculate the depth of nodes in a tree in Utility field		 */
/*									 */
/*************************************************************************/


void FindDepth(Tree T)
/*   ---------  */
{
    float	MaxDepth=0;
    DiscrValue	v;

    if ( T->NodeType )
    {
	ForEach(v, 1, T->Forks)
	{
	    FindDepth(T->Branch[v]);
	    if ( T->Branch[v]->Utility > MaxDepth )
	    {
		MaxDepth = T->Branch[v]->Utility;
	    }
	}
    }

    T->Utility = MaxDepth + 1;
}



/*************************************************************************/
/*									 */
/*	Display entire decision tree T					 */
/*									 */
/*************************************************************************/


void PrintTree(Tree T, String Title)
/*   ---------  */
{
    int s;

    FindDepth(T);

    SubTree=0;
    fprintf(Of, "\n%s\n", Title);
    Show(T, 0);
    fprintf(Of, "\n");

    ForEach(s, 1, SubTree)
    {
	fprintf(Of, T_Subtree, s);
	Show(SubDef[s], 0);
	fprintf(Of, "\n");
    }
}



/*************************************************************************/
/*									 */
/*	Display the tree T with offset Sh				 */
/*									 */
/*************************************************************************/


void Show(Tree T, int Sh)
/*   ---- */
{
    DiscrValue	v, MaxV, BrNo, Simplest, First;
    CaseCount	Errors=0.0;

    if ( T->NodeType )
    {
	/*  See whether separate subtree needed  */

	if ( Sh && Sh * TabSize + MaxLine(T) > Width )
	{
	    if ( ++SubTree >= SubSpace )
	    {
		SubSpace += 100;
		if ( SubDef )
		{
		    Realloc(SubDef, SubSpace, Tree);
		}
		else
		{
		    SubDef = Alloc(SubSpace, Tree);
		}
	    }

	    SubDef[SubTree] = T;
	    fprintf(Of, " [S%d]", SubTree);
	}
	else
	{
	    MaxV = T->Forks;

	    /*  Skip N/A branch if no cases  */

	    First = ( EmptyNA(T) ? 2 : 1 );
	    BrNo = First - 1;

	    /*  Print simplest branches first  */

	    while ( BrNo < MaxV )
	    {
		Simplest = First;
		ForEach(v, 2, MaxV)
		{
		    if ( T->Branch[v]->Utility < T->Branch[Simplest]->Utility ||

			 T->Branch[v]->Utility == 1 && ! T->Branch[v]->Cases )
		    {
			Simplest = v;
		    }
		}

		LastBranch[Sh+1] = ( ++BrNo == MaxV );
		ShowBranch(Sh, T, Simplest, (int)( BrNo == First ));
		T->Branch[Simplest]->Utility = 1E10;
	    }
	}
    }
    else
    {
	fprintf(Of, " %s (%.8g", ClassName[T->Leaf], P1(T->Cases));
	if ( T->Cases >= MinLeaf )
	{
	    if ( (Errors = T->Cases - T->ClassDist[T->Leaf]) >= 0.05 )
	    {
		fprintf(Of, "/%.8g", P1(Errors));
	    }
	}
	putc(')', Of);
    }
}



/*************************************************************************/
/*									 */
/*	Print a node T with offset Sh, branch value v, and continue	 */
/*									 */
/*************************************************************************/


void ShowBranch(int Sh, Tree T, DiscrValue v, DiscrValue BrNo)
/*   ----------  */
{
    DiscrValue	Pv, Last;
    Attribute	Att;
    Boolean	FirstValue;
    int		TextWidth, Skip, Values, i, Extra;
    char	CVS1[20], CVS2[20];

    Att = T->Tested;

    switch ( T->NodeType )
    {
	case BrDiscr:

	    Indent(Sh, BrNo);

	    fprintf(Of, "%s = %s:", AttName[Att], AttValName[Att][v]);

	    break;

	case BrThresh:

	    Indent(Sh, BrNo);

	    fprintf(Of, "%s", AttName[Att]);

	    if ( v == 1 )
	    {
		fprintf(Of, " = N/A:");
	    }
	    else
	    if ( T->Lower != T->Upper )
	    {
		if ( v == 2 )
		{
		    CValToStr(T->Lower, Att, CVS1);
		    CValToStr(T->Mid  , Att, CVS2);
		    fprintf(Of, " <= %s (%s):", CVS1, CVS2);
		}
		else
		{
		    CValToStr(T->Upper, Att, CVS1);
		    CValToStr(T->Mid  , Att, CVS2);
		    fprintf(Of, " >= %s (%s):", CVS1, CVS2);
		}
	    }
	    else
	    {
		CValToStr(T->Cut, Att, CVS1);
		fprintf(Of, " %s %s:", ( v == 2 ? "<=" : ">" ), CVS1);
	    }

	    break;

	case BrSubset:

	    /*  Count values at this branch  */

	    Values = Elements(Att, T->Subset[v], &Last);
	    if ( ! Values ) return;

	    Indent(Sh, BrNo);

	    if ( Values == 1 )
	    {
		fprintf(Of, "%s = %s:", AttName[Att], AttValName[Att][Last]);
		break;
	    }

	    if ( Ordered(Att) )
	    {
		/*  Find first value  */

		for ( Pv = 1 ; ! In(Pv, T->Subset[v]) ; Pv++ )
		    ;

		fprintf(Of, "%s %s [%s-%s]:", AttName[Att], T_InRange,
			AttValName[Att][Pv], AttValName[Att][Last]);
		break;
	    }

	    fprintf(Of, "%s %s {", AttName[Att], T_ElementOf);
	    FirstValue = true;
	    Skip = CharWidth(AttName[Att]) + CharWidth(T_ElementOf) + 3;
	    TextWidth = Skip + Sh * TabSize;

	    ForEach(Pv, 1, Last)
	    {
		if ( In(Pv, T->Subset[v]) )
		{
		    /*  Find number of characters after this element  */

		    if ( Pv != Last || T->Branch[v]->NodeType )
		    {
			Extra = 1;				/* for ":" */
		    }
		    else
		    {
			Extra = 2				/* for ": " */
				+ CharWidth(ClassName[T->Branch[v]->Leaf])
				+ 3				/* for " ()" */
				+ Digits(T->Cases)
				+ ( T->Errors < 0.05 ?  0 :
						1		/* for "/" */
						+ Digits(T->Errors) );
		    }

		    if ( ! FirstValue &&
			 TextWidth + CharWidth(AttValName[Att][Pv]) +
			     Extra + 1 > Width )
		    {
		  	Indent(Sh, 0);
			fprintf(Of, "%s",
				( LastBranch[Sh+1] && ! T->Branch[v]->NodeType ?
				  "    " : ":   " ));
			ForEach(i, 5, Skip) putc(' ', Of);

			TextWidth = Skip + Sh * TabSize;
			FirstValue = true;
		    }

		    fprintf(Of, "%s%c",
				AttValName[Att][Pv], Pv == Last ? '}' : ',');
		    TextWidth += CharWidth(AttValName[Att][Pv]) + 1;
		    FirstValue = false;
		}
	    }
	    putc(':', Of);
    }

    Show(T->Branch[v], Sh+1);
}



/*************************************************************************/
/*									 */
/*	Count the elements in a subset and record the last		 */
/*									 */
/*************************************************************************/


DiscrValue Elements(Attribute Att, Set S, DiscrValue *Last)
/*         --------  */
{
    DiscrValue Pv, Values=0;

    ForEach(Pv, 1, MaxAttVal[Att])
    {
	if ( In(Pv, S) )
	{
	    *Last = Pv;
	    Values++;
	}
    }

    return Values;
}



/*************************************************************************/
/*									 */
/*	Find the approximate maximum single line size for non-leaf	 */
/*	subtree T							 */
/*									 */
/*************************************************************************/


int MaxLine(Tree T)
/*  -------  */
{
    Attribute	Att;
    DiscrValue	v, vv;
    int		Ll, One, MaxLl=0;

    Att = T->Tested;

    /*  First find the max length of the line excluding tested att  */

    ForEach(v, 1, T->Forks)
    {
	switch ( T->NodeType )
	{
	    case BrThresh:
		if ( TStampVal(Att) )
		{
		    Ll = ( T->Lower != T->Upper ? 41 : 19 );
		}
		else
		if ( DateVal(Att) )
		{
		    Ll = ( T->Lower != T->Upper ? 23 : 10 );
		}
		else
		if ( TimeVal(Att) )
		{
		    Ll = ( T->Lower != T->Upper ? 19 : 8 );
		}
		else
		{
		    Ll = ( T->Lower != T->Upper ? 11 : 4 );
		}
		break;

	    case BrDiscr:
		if ( Ordered(Att) )
		{
		    vv = T->Cut;
	
		    switch ( v )
		    {
			case 1:
			    Ll = 3;
			    break;

			case 2:
			    Ll = CharWidth(AttValName[Att][2]);
			    if ( vv != 2 )
			    {
				Ll += CharWidth(AttValName[Att][vv])+1;
			    }
			    break;

			case 3:
			    Ll = CharWidth(AttValName[Att][MaxAttVal[Att]]);
			    if ( vv != MaxAttVal[Att] - 1 )
			    {
				Ll += CharWidth(AttValName[Att][vv+1])+1;
			    }
		    }
		}
		else
		{
		    Ll = CharWidth(AttValName[Att][v]) + 1;
		}
		break;

	    case BrSubset: /* difficult! */
		Ll = 0;
		ForEach(vv, 1, MaxAttVal[Att])
		{
		    if ( In(vv,T->Subset[v]) )
		    {
			One = CharWidth(AttValName[Att][vv]) + 6;
			if ( One > Ll ) Ll = One;
		    }
		}
	}

	/*  Check whether ends in leaf  */

	if ( ! T->Branch[v]->NodeType &&
	     ( v > 1 || T->Branch[v]->Cases > 0.01 ) )
	{
	    Ll += CharWidth(ClassName[T->Branch[v]->Leaf]) + 6;
	}

	if ( Ll > MaxLl ) MaxLl = Ll;
    }

    return CharWidth(AttName[Att]) + 4 + MaxLl;
}



/*************************************************************************/
/*								   	 */
/*	Indent Sh columns					  	 */
/*								  	 */
/*************************************************************************/


void Indent(int Sh, int BrNo)
/*   ------  */
{
    int	i;

    fprintf(Of, "\n");
    for ( i = 1 ; i <= Sh ; i++ )
    {
	fprintf(Of, "%s", ( i == Sh && BrNo == 1 ? ":..." :
			    LastBranch[i] ? "    " : ":   " ));
    }
}



/*************************************************************************/
/*									 */
/*	Free up space taken up by tree T				 */
/*									 */
/*************************************************************************/


void FreeTree(Tree T)
/*   --------  */
{
    DiscrValue v;

    if ( ! T ) return;

    if ( T->NodeType )
    {
	ForEach(v, 1, T->Forks)
	{
	    FreeTree(T->Branch[v]);
	}

	Free(T->Branch);

	if ( T->NodeType == BrSubset )
	{
	    FreeVector((void **) T->Subset, 1, T->Forks);
	}

    }

    Free(T->ClassDist);
    Free(T);
}



/*************************************************************************/
/*									 */
/*	Construct a leaf in a given node				 */
/*									 */
/*************************************************************************/


Tree Leaf(double *Freq, ClassNo NodeClass, CaseCount Cases, CaseCount Errors)
/*   ----  */
{
    Tree	Node;
    ClassNo	c;

    Node = AllocZero(1, TreeRec);

    Node->ClassDist = AllocZero(MaxClass+1, CaseCount);
    if ( Freq )
    {
	// Pranav: Now also including the distribution for the UNKNOWN class.
	ForEach(c, 0, MaxClass)
	//ForEach(c, 1, MaxClass)
	{
	    Node->ClassDist[c] = Freq[c];
	}
    }

    Node->NodeType	= 0;
    Node->Leaf		= NodeClass;
    Node->Cases		= Cases;
    Node->Errors	= Errors;

    return Node;
}



/*************************************************************************/
/*									 */
/*	Insert branches in a node 		         		 */
/*									 */
/*************************************************************************/


void Sprout(Tree T, DiscrValue Branches)
/*   ------  */
{
    T->Forks = Branches;
    T->Branch = AllocZero(Branches+1, Tree);
}



/*************************************************************************/
/*									 */
/*	Remove branches etc from a node					 */
/*									 */
/*************************************************************************/


void UnSprout(Tree T)
/*   --------  */
{
    DiscrValue	v;

    ForEach(v, 1, T->Forks)
    {
	FreeTree(T->Branch[v]);
    }
    Free(T->Branch);					T->Branch = Nil;

    if ( T->NodeType == BrSubset )
    {
	FreeVector((void **) T->Subset, 1, T->Forks);	T->Subset = Nil;
    }

    T->Forks = T->NodeType = 0;
}



/*************************************************************************/
/*									 */
/*	Count the non-null leaves in a tree				 */
/*									 */
/*************************************************************************/


int TreeSize(Tree T)
/*  --------  */
{
    int		Sum=0;
    DiscrValue	v;

    if ( T->NodeType )
    {
	ForEach(v, ( EmptyNA(T) ? 2 : 1 ), T->Forks)
	{
	    Sum += TreeSize(T->Branch[v]);
	}

	return Sum;
    }

    return ( T->Cases >= MinLeaf ? 1 : 0 );
}


/*************************************************************************/
/*									 */
/*	Count the total number of nodes in a tree			 */
/*									 */
/*************************************************************************/


int TotalTreeSize(Tree T)
/*  --------  */
{
    int		Sum=0;
    DiscrValue	v;

    if ( T->NodeType )
    {
	ForEach(v, ( EmptyNA(T) ? 2 : 1 ), T->Forks)
	{
	    Sum += TotalTreeSize(T->Branch[v]);
	}

	return 1 + Sum;
    }

    return ( T->Cases >= MinLeaf ? 1 : 0 );
}



/*************************************************************************/
/*									 */
/*	Count the non-null leaves in a tree that may contain		 */
/*	compressed branches via CompressBranches()			 */
/*									 */
/*************************************************************************/


int ExpandedLeafCount(Tree T)
/*  -----------------  */
{
    int		Sum=0;
    DiscrValue	v, Dummy;

    if ( ! T->NodeType )
    {
	return 1;
    }

    ForEach(v, 1, T->Forks)
    {
	if ( T->Branch[v]->Cases < MinLeaf ) continue;

	if ( T->NodeType == BrSubset && ! T->Branch[v]->NodeType )
	{
	    Sum += Elements(T->Tested, T->Subset[v], &Dummy);
	}
	else
	{
	    Sum += ExpandedLeafCount(T->Branch[v]);
	}
    }

    return Sum;
}



/*************************************************************************/
/*                                                                	 */
/*	Find the maximum depth of a tree				 */
/*                                                                	 */
/*************************************************************************/


int TreeDepth(Tree T)
/*  ---------  */
{
    DiscrValue	v;
    int		Subtree, MaxSubtree=0;

    if ( T->NodeType )
    {
	ForEach(v, 1, T->Forks)
	{
	    Subtree = TreeDepth(T->Branch[v]);
	    if ( Subtree > MaxSubtree ) MaxSubtree = Subtree;
	}
    }

    return MaxSubtree + 1;
}



/*************************************************************************/
/*									 */
/*	Return a copy of tree T						 */
/*									 */
/*************************************************************************/


Tree CopyTree(Tree T)
/*   --------  */
{
    DiscrValue	v;
    Tree	New;
    int		Bytes;

    New = Alloc(1, TreeRec);
    memcpy(New, T, sizeof(TreeRec));

    New->ClassDist = Alloc(MaxClass+1, CaseCount);
    memcpy(New->ClassDist, T->ClassDist, (MaxClass + 1) * sizeof(CaseCount));

    if ( T->NodeType == BrSubset )
    {
	Bytes = (MaxAttVal[T->Tested]>>3) + 1;

	New->Subset = Alloc(T->Forks+1, Set);
	ForEach(v, 1, T->Forks)
	{
	    New->Subset[v] = Alloc(Bytes, unsigned char);
	    memcpy(New->Subset[v], T->Subset[v], Bytes);
	}
    }

    if ( T->NodeType )
    {
	New->Branch = AllocZero(T->Forks+1, Tree);
	ForEach(v, 1, T->Forks)
	{
	    New->Branch[v] = CopyTree(T->Branch[v]);
	}
    }

    return New;
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Find a good subset of a set of rules				 */
/*	------------------------------------				 */
/*									 */
/*************************************************************************/




float	*DeltaErrs=Nil,	/* DeltaErrs[r]	 = change attributable to rule r or
					   realisable if rule r included */
	*Bits=Nil,	/* Bits[r]	 = bits to encode rule r */
	BitsErr,	/* BitsErr	 = bits to label prediction as error */
	BitsOK;		/* BitsOK	 = bits to label prediction as ok */

int	**TotVote=Nil;	/* TotVote[i][c] = case i's votes for class c */

ClassNo	*TopClass=Nil,	/* TopClass[i]	 = class with highest vote */
	*AltClass=Nil;	/* AltClass[i]	 = class with second highest vote */

Boolean	*RuleIn=Nil,	/* RuleIn[r]	 = rule r included */
	*Covered=Nil;	/* Covered[i]	 = case i covered by rule(s) */

Byte	*CovByBlock=Nil,/* holds entries for inverse of Fires */
	**CovByPtr=Nil;	/* next entry for CovBy[i] */

RuleNo	*LastCovBy=Nil; /* Last rule covering case i  */


/*************************************************************************/
/*									 */
/*	Main rule selection routine.					 */
/*	1.  Form initial theory						 */
/*      2.  Hillclimb in MDL space					 */
/*									 */
/*************************************************************************/


void SiftRules(float EstErrRate)
/*   ---------  */
{
    RuleNo	r;
    int		d, *bp;
    CRule	R;
    float	CodeLength;
    CaseNo	i;

    NotifyStage(SIFTRULES);
    Progress(-(float) NRules);

    /*  Determine inverse of Fires in CovBy, CovByPtr, CovByBlock  */

    InvertFires();

    /*  Clean up any subsets in conditions by removing values that do
	not appear in the covered cases  */

    if ( SUBSET )
    {
	assert (false);
	PruneSubsets();
    }

    Covered = Alloc(MaxCase+1, Boolean);
    RuleIn  = AllocZero(NRules+1, Boolean);

    /*  Set initial theory  */

    SetInitialTheory();

    Bits = Alloc(NRules+1, float);

    /*  Calculate the number of bits associated with attribute tests;
	this is not repeated in boosting, composite rulesets etc  */

    if ( ! BranchBits || NRules > MaxCase )
    {
	GenerateLogs(Max(MaxCase+1, Max(MaxAtt, Max(MaxClass,
			 Max(MaxDiscrVal, NRules)))));
    }

    if ( ! BranchBits )
    {
	assert (false);
	FindTestCodes();
    }

    /*  Determine rule codelengths  */

    if ( NRules >= MaxCase+1 )
    {
	Realloc(List, NRules+1, CaseNo);
    }

    ForEach(r, 1, NRules)
    {
	R = Rule[r];

	CodeLength = 0;
	ForEach(d, 1, R->Size)
	{
	    CodeLength += CondBits(R->Lhs[d]);
	}
	Bits[r] = CodeLength + LogCaseNo[R->Size] - LogFact[R->Size];
    }

    /*  Use estimated error rate to determine the bits required to
	label a theory's prediction for a case as an error or correct  */

    if ( EstErrRate > 0.5 ) EstErrRate = 0.45;

    BitsErr = - Log(EstErrRate);
    BitsOK  = - Log(1.0 - EstErrRate);


    /*  Allocate tables used in hillclimbing  */

    DeltaErrs = Alloc(NRules+1, float);
    TopClass = Alloc(MaxCase+1, ClassNo);

    AltClass = Alloc(MaxCase+1, ClassNo);
    TotVote  = Alloc(MaxCase+1, int *);

    bp = AllocZero((MaxCase+1) * (MaxClass+1), int);
    ForEach(i, 0, MaxCase)
    {
	TotVote[i] = bp;
	bp += MaxClass + 1;
    }

    /*  Now find best subset of rules  */

    HillClimb();

    /*  Determine default class and reorder rules  */

    SetDefaultClass();
    OrderRules();

    /*  Deallocate storage  */

    FreeSiftRuleData();
}



/*************************************************************************/
/*								  	 */
/*	Find inverse of Fires[][] in CovBy, CovByPtr, and CovByBlock.	 */
/*								  	 */
/*	CovBy[i] = number of rules covering case i (set by NewRule)	 */
/*								  	 */
/*	Set up CovByPtr as pointers into CovByBlock so that		 */
/*	CovByPtr[i] is the start of the compressed entry for case i	 */
/*								  	 */
/*************************************************************************/


void InvertFires()
/*   -----------  */
{
    RuleNo	r, Entry;
    int		j, Blocks, Extra;
    CaseNo	i;
    Byte	*p, *From, *To, *Next;

    CovByPtr = Alloc(MaxCase+2, Byte *);
    Extra = NRules / 128;		/* max number of filler entries */
    CovByPtr[0] = 0;
    ForEach(i, 1, MaxCase+1)
    {
	CovByPtr[i] = CovByPtr[i-1] + CovBy[i-1] + Extra;
    }

    CovByBlock = Alloc((size_t) CovByPtr[MaxCase+1], Byte);
    ForEach(i, 0, MaxCase)
    {
	CovByPtr[i] += (size_t) CovByBlock;
    }

    LastCovBy = AllocZero(MaxCase+1, RuleNo);

    /*  Add entries for each rule  */

    ForEach(r, 1, NRules)
    {
	Uncompress(Fires[r], List);
	ForEach(j, 1, List[0])
	{
	    i = List[j];

	    /*  Add compressed entry for this rule  */

	    p = CovByPtr[i];
	    Entry = r - LastCovBy[i];
	    LastCovBy[i] = r;

	    while ( Entry > 127 )
	    {
		Blocks = (Entry >> 7);
		if ( Blocks > 127 ) Blocks = 127;
		Entry -= Blocks * 128;
		*p++   = Blocks + 128;
	    }

	    *p++ = Entry;
	    CovByPtr[i] = p;
	}
    }

    Free(LastCovBy);					LastCovBy = Nil;

    /*  Reset CovByPtr entries and compact  */

    To   = CovByPtr[0];
    From = CovByPtr[0] = CovByBlock;

    ForEach(i, 1, MaxCase)
    {
	From += CovBy[i-1] + Extra;
	Next  = CovByPtr[i];
	CovByPtr[i] = To;

	for ( p = From ; p < Next ; )
	{
	    *To++ = *p++;
	}
    }

    /*  Reduce CovByBlock to size actually used  */

    From = CovByBlock;			/* current address */

    Realloc(CovByBlock, To - CovByBlock, Byte);

    if ( CovByBlock != From )
    {
	/*  CovByBlock has been moved  */

	ForEach(i, 0, MaxCase)
	{
	    CovByPtr[i] += CovByBlock - From;
	}
    }
}



/*************************************************************************/
/*								  	 */
/*	Determine code lengths for attributes and branches		 */
/*								  	 */
/*************************************************************************/


void FindTestCodes()
/*   -------------  */
{
    Attribute	Att;
    DiscrValue	v, V;
    CaseNo	i, *ValFreq;
    int		PossibleAtts=0;
    float	Sum;

    BranchBits = AllocZero(MaxAtt+1, float);
    AttValues  = AllocZero(MaxAtt+1, int);

    ForEach(Att, 1, MaxAtt)
    {
	if ( Skip(Att) || Att == ClassAtt ) continue;

	PossibleAtts++;

	if ( Ordered(Att) )
	{
	    BranchBits[Att] = 1 + 0.5 * LogCaseNo[MaxAttVal[Att] - 1];
	}
	else
	if ( (V = MaxAttVal[Att]) )
	{
	    /*  Discrete attribute  */

	    ValFreq = AllocZero(V+1, CaseNo);

	    ForEach(i, 0, MaxCase)
	    {
		assert(XDVal(Case[i],Att) >= 0 && XDVal(Case[i],Att) <= V);
		ValFreq[ XDVal(Case[i],Att) ]++;
	    }

	    Sum = 0;
	    ForEach(v, 1, V)
	    {
		if ( ValFreq[v] )
		{
		    Sum += (ValFreq[v] / (MaxCase+1.0)) *
			   (LogCaseNo[MaxCase+1] - LogCaseNo[ValFreq[v]]);
		    AttValues[Att]++;
		}
	    }
	    Free(ValFreq);

	    BranchBits[Att] = Sum;
	}
	else
	{
	    /*  Continuous attribute  */

	    BranchBits[Att] = PossibleCuts[Att] > 1 ?
			      1 + 0.5 * LogCaseNo[PossibleCuts[Att]] : 0 ;
	}
    }

    AttTestBits = LogCaseNo[PossibleAtts];
}



/*************************************************************************/
/*									 */
/*	Determine the number of bits required to encode a condition	 */
/*									 */
/*************************************************************************/


float CondBits(Condition C)
/*    --------  */
{
    Attribute	Att;
    float	Code=0;
    int		Elts=0;
    DiscrValue	v;

    Att = C->Tested;
    switch ( C->NodeType )
    {
	case BrDiscr:		/* test of discrete attribute */
	case BrThresh:		/* test of continuous attribute */

	    return AttTestBits + BranchBits[Att];

	case BrSubset:		/* subset test on discrete attribute  */

	    /* Ignore subset test form for ordered attributes  */

	    if ( Ordered(Att) )
	    {
		return AttTestBits + BranchBits[Att];
	    }

	    ForEach(v, 1, MaxAttVal[Att])
	    {
		if ( In(v, C->Subset) )
		{
		    Elts++;
		}
	    }
	    Elts = Min(Elts, AttValues[Att] - 1);  /* if values not present */
	    Code = LogFact[AttValues[Att]] -
		   (LogFact[Elts] + LogFact[AttValues[Att] - Elts]);

	    return AttTestBits + Code;
    }
}



/*************************************************************************/
/*									 */
/*	Select initial theory.  This is important, since the greedy	 */
/*	optimization procedure is very sensitive to starting with	 */
/*	a reasonable theory.						 */
/*									 */
/*	The theory is constructed class by class.  For each class,	 */
/*	rules are added in confidence order until all of the cases of	 */
/*	that class are covered.  Rules that do not improve coverage	 */
/*	are skipped.							 */
/*									 */
/*************************************************************************/


void SetInitialTheory()
/*   ----------------  */
{
    ClassNo	c;
    RuleNo	r, Active=0;

    ForEach(c, 1, MaxClass)
    {
	CoverClass(c);
    }

    /*  Remove rules that don't help coverage  */

    ForEach(r, 1, NRules)
    {
	if ( (RuleIn[r] &= 1) ) Active++;
    }
}



void CoverClass(ClassNo Target)
/*   ----------  */
{
    CaseNo	i;
    double	Remaining, FalsePos=0, NewFalsePos, NewTruePos;
    RuleNo	r, Best;
    int		j;

    memset(Covered, false, MaxCase+1);

    Remaining = ClassFreq[Target];

    while ( Remaining > FalsePos )
    {
	/*  Find most accurate unused rule from a leaf  */

	Best = 0;
	ForEach(r, 1, NRules)
	{
	    if ( Rule[r]->Rhs == Target && ! RuleIn[r] &&
		 Rule[r]->Correct >= MINITEMS )
	    {
		if ( ! Best || Rule[r]->Vote > Rule[Best]->Vote ) Best = r;
	    }
	}

	if ( ! Best ) return;

	/*  Check increased coverage  */

	NewFalsePos = NewTruePos = 0;

	Uncompress(Fires[Best], List);
	for( j = List[0] ; j ; j-- )
	{
	    i = List[j];
	    if ( ! Covered[i] )
	    {
		if ( Class(Case[i]) == Target )
		{
		    NewTruePos += Weight(Case[i]);
		}
		else
		{
		    NewFalsePos += Weight(Case[i]);
		}
	    }
	}

	/*  If coverage is not increased, set RuleIn to 2 so that
	    the rule can be removed later  */

	// Pranav: Ensures that all cases are covered
	if ( NewTruePos - NewFalsePos <= MINITEMS - Epsilon )
	//if ( NewTruePos - NewFalsePos <= MINITEMS + Epsilon )
	{
	    RuleIn[Best] = 2;
	}
	else
	{
	    Remaining -= NewTruePos;
	    FalsePos  += NewFalsePos;

	    RuleIn[Best] = true;

	    Uncompress(Fires[Best], List);
	    for( j = List[0] ; j ; j-- )
	    {
		i = List[j];
		if ( ! Covered[i] )
		{
		    Covered[i] = true;
		}
	    }
	}
    }
}



/*************************************************************************/
/*									 */
/*	Calculate total message length as				 */
/*	  THEORYFRAC * cost of transmitting theory			 */
/*	  + cost of identifying and correcting errors			 */
/*									 */
/*	The cost of identifying errors assumes that the final theory	 */
/*	will have about the same error rate as the pruned tree, so	 */
/*	is approx. the sum of the corresponding messages.		 */
/*									 */
/*	Message lengths are returned in units of 0.01			 */
/*									 */
/*************************************************************************/


int MessageLength(RuleNo NR, double RuleBits, float Errs)
/*  -------------  */
{
    return rint(100.0 *
	(THEORYFRAC * Max(0, RuleBits - LogFact[NR]) +
	 Errs * BitsErr + (MaxCase+1 - Errs) * BitsOK +
	 Errs * LogCaseNo[MaxClass-1]));
}



/*************************************************************************/
/*									 */
/*	Improve a subset of rules by adding and deleting rules.		 */
/*	MDL costs are rounded to nearest 0.01 bit			 */
/*									 */
/*************************************************************************/


void HillClimb()
/*   ---------  */
{
    RuleNo	r, RuleCount=0, OriginalCount, Toggle, LastToggle=0;
    int		OutCount;
    CaseNo	i;
    int		j;
    CaseCount	Errs;
    double	RuleBits=0;
    int		LastCost=1E9, CurrentCost, AltCost, NewCost;
    Boolean	DeleteOnly=false;

    ForEach(r, 1, NRules)
    {
	if ( RuleIn[r] )
	{
	    RuleBits += Bits[r];
	    RuleCount++;
	}
    }
    OriginalCount = RuleCount;

    InitialiseVotes();
    Verbosity(1, fprintf(Of, "\n"))

    /*  Initialise DeltaErrs[]  */

    Errs = CalculateDeltaErrs();

    /*  Add or drop rule with greatest reduction in coding cost  */

    while ( true )
    {
	CurrentCost = NewCost = MessageLength(RuleCount, RuleBits, Errs);

	Verbosity(1,
	    fprintf(Of, "\t%d rules, %.1f errs, cost=%.1f bits\n",
		   RuleCount, Errs, CurrentCost/100.0);

	    if ( ! DeleteOnly && CurrentCost > LastCost )
	    {
		fprintf(Of, "ERROR %g %g\n",
			    CurrentCost/1000.0, LastCost/100.0);
		break;
	    })

	Toggle = OutCount = 0;

	ForEach(r, 1, NRules)
	{
	    if ( r == LastToggle ) continue;

	    if ( RuleIn[r] )
	    {
		AltCost = MessageLength(RuleCount - 1,
					RuleBits - Bits[r],
					Errs + DeltaErrs[r]);
	    }
	    else
	    {
		if ( Errs < 1E-3 || DeleteOnly ) continue;

		AltCost = MessageLength(RuleCount + 1,
					RuleBits + Bits[r],
					Errs + DeltaErrs[r]);
	    }

	    Verbosity(2,
		if ( ! (OutCount++ % 5) ) fprintf(Of, "\n\t\t");
		fprintf(Of, "%d<%g=%.1f> ",
			    r, DeltaErrs[r], (AltCost - CurrentCost)/100.0))

	    if ( AltCost < NewCost ||
		 AltCost == NewCost && RuleIn[r] )
	    {
		Toggle  = r;
		NewCost = AltCost;
	    }
	}

	if ( ! DeleteOnly && NewCost > CurrentCost )
	{
	    DeleteOnly = true;
	    Verbosity(1, fprintf(Of, "(start delete mode)\n"))
	}

	Verbosity(2, fprintf(Of, "\n"))

	if ( ! Toggle || DeleteOnly && RuleCount <= OriginalCount ) break;

	Verbosity(1,
	    fprintf(Of, "\t%s rule %d/%d (errs=%.1f, cost=%.1f bits)\n",
		   ( RuleIn[Toggle] ? "Delete" : "Add" ),
		   Rule[Toggle]->TNo, Rule[Toggle]->RNo,
		   Errs + DeltaErrs[Toggle], NewCost/100.0))

	/*  Adjust vote information  */

	Uncompress(Fires[Toggle], List);
	for ( j = List[0] ; j ; j-- )
	{
	    i = List[j];

	    /*  Downdate DeltaErrs for all rules except Toggle that cover i  */

	    UpdateDeltaErrs(i, -Weight(Case[i]), Toggle);

	    if ( RuleIn[Toggle] )
	    {
		TotVote[i][Rule[Toggle]->Rhs] -= Rule[Toggle]->Vote;
	    }
	    else
	    {
		TotVote[i][Rule[Toggle]->Rhs] += Rule[Toggle]->Vote;
	    }

	    CountVotes(i);

	    /*  Update DeltaErrs for all rules except Toggle that cover i  */

	    UpdateDeltaErrs(i, Weight(Case[i]), Toggle);
	}

	/*  Update information about rules selected and current errors  */

	if ( RuleIn[Toggle] )
	{
	    RuleIn[Toggle] = false;
	    RuleBits -= Bits[Toggle];
	    RuleCount--;
	}
	else
	{
	    RuleIn[Toggle] = true;
	    RuleBits += Bits[Toggle];
	    RuleCount++;
	}

	Errs += DeltaErrs[Toggle];
	DeltaErrs[Toggle] = - DeltaErrs[Toggle];

	LastToggle = Toggle;
	LastCost   = CurrentCost;

	Progress(1.0);
    }
}



/*************************************************************************/
/*									 */
/*	Determine votes for each case from initial rules		 */
/*	Note: no vote for default class					 */
/*									 */
/*************************************************************************/


void InitialiseVotes()
/*   ---------------  */
{
    CaseNo	i;
    int		j, Vote;
    ClassNo	Rhs;
    RuleNo	r;

    /*  Adjust vote for each case covered by rule  */

    ForEach(r, 1, NRules)
    {
	if ( ! RuleIn[r] ) continue;

	Rhs  = Rule[r]->Rhs;
	Vote = Rule[r]->Vote;

	Uncompress(Fires[r], List);
	for ( j = List[0] ; j ; j-- )
	{
	    TotVote[List[j]][Rhs] += Vote;
	}
    }

    /*  Find the best and alternate class for each case  */

    ForEach(i, 0, MaxCase)
    {
	CountVotes(i);
    }
}



/*************************************************************************/
/*									 */
/*	Find the best and second-best class for each case using the	 */
/*	current values of TotVote					 */
/*									 */
/*************************************************************************/


void CountVotes(CaseNo i)
/*   ----------  */
{
    ClassNo	c, First=0, Second=0;
    int		V;

    ForEach(c, 1, MaxClass)
    {
	if ( (V = TotVote[i][c]) )
	{
	    if ( ! First || V > TotVote[i][First] )
	    {
		Second = First;
		First  = c;
	    }
	    else
	    if ( ! Second || V > TotVote[i][Second] )
	    {
		Second = c;
	    }
	}
    }

    TopClass[i] = First;
    AltClass[i] = Second;
}



/*************************************************************************/
/*									 */
/*	Adjust DeltaErrors for all rules except Toggle that cover case i */
/*									 */
/*************************************************************************/


#define Prefer(d,c1,c2) ((d) > 0 || (d) == 0 && c1 < c2)

void UpdateDeltaErrs(CaseNo i, double Delta, RuleNo Toggle)
/*   ---------------  */
{
    ClassNo	RealClass, Top, Alt, Rhs;
    RuleNo	r;
    Byte	*p;
    int		k;

    RealClass = Class(Case[i]);
    Top	= TopClass[i];
    Alt = AltClass[i];

    r = 0;
    p = CovByPtr[i];
    ForEach(k, 1, CovBy[i])
    {
	/*  Update r to next rule covering case i  */

	while ( (*p) & 128 )
	{
	    r += ((*p++) & 127) * 128;
	}
	r += *p++;

	if ( r != Toggle )
	{
	    /*  Examine effect of adding or deleting rule  */
	
	    Rhs = Rule[r]->Rhs;

	    if ( RuleIn[r] )
	    {
		if ( Rhs == Top &&
		     Prefer(TotVote[i][Alt] - (TotVote[i][Top] - Rule[r]->Vote),
			    Alt, Top) )
		{
		    DeltaErrs[r] +=
			(NCost[Alt][RealClass] - NCost[Top][RealClass]) * Delta;
		}
	    }
	    else
	    {
		if ( Rhs != Top &&
		     Prefer(TotVote[i][Rhs] + Rule[r]->Vote - TotVote[i][Top],
			    Rhs, Top) )
		{
		    DeltaErrs[r] +=
			(NCost[Rhs][RealClass] - NCost[Top][RealClass]) * Delta;
		}
	    }
	}
    }
}



/*************************************************************************/
/*									 */
/*	Calculate initial value of DeltaErrs and total errors		 */
/*									 */
/*************************************************************************/


CaseCount CalculateDeltaErrs()
/*        ------------------  */
{
    RuleNo	r;
    CaseNo	i;
    double	Errs=0;

    ForEach(i, 0, MaxCase)
    {
	Errs += Weight(Case[i]) * NCost[TopClass[i]][Class(Case[i])];
    }

    ForEach(r, 1, NRules)
    {
	DeltaErrs[r] = 0;
    }

    ForEach(i, 0, MaxCase)
    {
	UpdateDeltaErrs(i, Weight(Case[i]), 0);
    }

    return Errs;
}



/*************************************************************************/
/*									 */
/*	Remove unrepresented values from subsets			 */
/*									 */
/*************************************************************************/


void PruneSubsets()
/*   ------------  */
{
    Set		*PossibleValues;
    Attribute	Att, *Atts, Last;
    int		*Bytes, d, NAtts, j, b;
    CaseNo	i;
    CRule	R;
    RuleNo	r;

    /*  Allocate subsets for possible values  */

    Atts  = Alloc(MaxAtt+1, Attribute);
    Bytes = Alloc(MaxAtt+1, int);

    PossibleValues = AllocZero(MaxAtt+1, Set);
    ForEach(Att, 1, MaxAtt)
    {
	if ( MaxAttVal[Att] > 3 )
	{
	    Bytes[Att] = (MaxAttVal[Att]>>3)+1;
	    PossibleValues[Att] = AllocZero(Bytes[Att], Byte);
	}
    }

    /*  Check each rule in turn  */

    ForEach(r, 1, NRules)
    {
	R = Rule[r];
	NAtts = 0;

	/*  Find all subset conditions  */

	ForEach(d, 1, R->Size)
	{
	    if ( R->Lhs[d]->NodeType != BrSubset ) continue;

	    Atts[++NAtts] = Att = R->Lhs[d]->Tested;
	    ClearBits(Bytes[Att], PossibleValues[Att]);
	}

	if ( ! NAtts ) continue;	/* no subset conditions */

	/*  Scan cases covered by this rule  */

	Uncompress(Fires[r], List);
	for ( j = List[0] ; j ; j-- )
	{
	    i = List[j];

	    /*  Record values of listed attributes  */

	    ForEach(d, 1, NAtts)
	    {
		Att = Atts[d];
		SetBit(DVal(Case[i], Att), PossibleValues[Att]);
	    }
	}

	/*  Delete unrepresented values  */

	ForEach(d, 1, R->Size)
	{
	    if ( R->Lhs[d]->NodeType != BrSubset ) continue;

	    Att = R->Lhs[d]->Tested;
	    ForEach(b, 0, Bytes[Att]-1)
	    {
		R->Lhs[d]->Subset[b] &= PossibleValues[Att][b];
	    }

	    if ( Elements(Att, R->Lhs[d]->Subset, &Last) == 1 )
	    {
		R->Lhs[d]->NodeType  = BrDiscr;
		R->Lhs[d]->TestValue = Last;
		Free(R->Lhs[d]->Subset);
	    }
	}
    }

    FreeVector((void **) PossibleValues, 1, MaxAtt);
    Free(Bytes);
    Free(Atts);
}



/*************************************************************************/
/*									 */
/*	Choose the default class as the one with the maximum		 */
/*	weight of uncovered cases					 */
/*									 */
/*************************************************************************/


void SetDefaultClass()
/*   ---------------  */
{
    RuleNo	r;
    ClassNo	c;
    double	*UncoveredWeight, TotUncovered=1E-3;
    CaseNo	i, j;

    memset(Covered, false, MaxCase+1);
    UncoveredWeight = AllocZero(MaxClass+1, double);

    /*  Check which cases are covered by at least one rule  */

    ForEach(r, 1, NRules)
    {
	if ( ! RuleIn[r] ) continue;

	Uncompress(Fires[r], List);
	for ( j = List[0] ; j ; j-- )
	{
	    Covered[List[j]] = true;
	}
    }

    /*  Find weights by class of uncovered cases  */

    ForEach(i, 0, MaxCase)
    {
	if ( ! Covered[i] )
	{
	    UncoveredWeight[ Class(Case[i]) ] += Weight(Case[i]);
	    TotUncovered += Weight(Case[i]);
	}
    }

    /*  Choose new default class using rel freq and rel uncovered  */

    Verbosity(1, fprintf(Of, "\n    Weights of uncovered cases:\n"));

    ForEach(c, 1, MaxClass)
    {
	Verbosity(1, fprintf(Of, "\t%s (%.2f): %.1f\n",
			    ClassName[c], ClassFreq[c] / (MaxCase + 1.0),
			    UncoveredWeight[c]));

	ClassSum[c] = (UncoveredWeight[c] + 1) / (TotUncovered + 2.0) +
		      ClassFreq[c] / (MaxCase + 1.0);
    }

    Default = SelectClass(1, (Boolean) (MCost && ! CostWeights));

    Free(UncoveredWeight);
}



/*************************************************************************/
/*									 */
/*	Swap two rules							 */
/*									 */
/*************************************************************************/


void SwapRule(RuleNo A, RuleNo B)
/*   --------  */
{
    CRule	Hold;
    Boolean	HoldIn;

    Hold    = Rule[A];
    Rule[A] = Rule[B];
    Rule[B] = Hold;

    HoldIn    = RuleIn[A];
    RuleIn[A] = RuleIn[B];
    RuleIn[B] = HoldIn;
}



/*************************************************************************/
/*									 */
/*	Order rules by utility, least important first			 */
/*	(Called after HilClimb(), so RuleIn etc already known.)		 */
/*									 */
/*************************************************************************/


int OrderByUtility()
/*  --------------  */
{
    RuleNo	r, *Drop, NDrop=0, NewNRules=0, Toggle;
    CaseNo	i;
    int		j, OutCount;
    double	Errs=0;

    Verbosity(1, fprintf(Of, "\n    Determining rule utility\n"))

    Drop = Alloc(NRules, RuleNo);

    /*  Find the rule that has the least beneficial effect on accuracy  */

    while ( true )
    {
	Toggle = OutCount = 0;

	ForEach(r, 1, NRules)
	{
	    if ( ! RuleIn[r] ) continue;

	    Verbosity(2,
		if ( ! (OutCount++ %10 ) ) fprintf(Of, "\n\t\t");
		fprintf(Of, "%d<%g> ", r, DeltaErrs[r]))

	    if ( ! Toggle ||
		 DeltaErrs[r] < DeltaErrs[Toggle] - 1E-3 ||
		 ( DeltaErrs[r] < DeltaErrs[Toggle] + 1E-3 &&
		   Bits[r] > Bits[Toggle] ) )
	    {
		Toggle = r;
	    }
	}
	Verbosity(2, fprintf(Of, "\n"))

	if ( ! Toggle ) break;

	Verbosity(1,
	    fprintf(Of, "\tDelete rule %d/%d (errs up %.1f)\n",
		   Rule[Toggle]->TNo, Rule[Toggle]->RNo,
		   Errs + DeltaErrs[Toggle]))

	/*  Adjust vote information  */

	Uncompress(Fires[Toggle], List);
	for ( j = List[0] ; j ; j-- )
	{
	    i = List[j];

	    /*  Downdate DeltaErrs for all rules except Toggle that cover i  */

	    UpdateDeltaErrs(i, -Weight(Case[i]), Toggle);

	    TotVote[i][Rule[Toggle]->Rhs] -= Rule[Toggle]->Vote;

	    CountVotes(i);

	    /*  Update DeltaErrs for all rules except Toggle that cover i  */

	    UpdateDeltaErrs(i, Weight(Case[i]), Toggle);
	}

	Drop[NDrop++]  = Toggle;
	RuleIn[Toggle] = false;

	Errs += DeltaErrs[Toggle];
    }

    /*  Now reverse the order  */

    while ( --NDrop >= 0 )
    {
	NewNRules++;
	RuleIn[Drop[NDrop]] = true;
	SwapRule(Drop[NDrop], NewNRules);

	/*  Have to alter rule number in Drop  */
	ForEach(r, 0, NDrop-1)
	{
	    if ( Drop[r] == NewNRules ) Drop[r] = Drop[NDrop];
	}
    }
    Free(Drop);

    return NewNRules;
}




/*************************************************************************/
/*									 */
/*	Order rules by class and then by rule CF			 */
/*									 */
/*************************************************************************/


int OrderByClass()
/*  ------------  */
{
    RuleNo	r, nr, NewNRules=0;
    ClassNo	c;

    ForEach(c, 1, MaxClass)
    {
	while ( true )
	{
	    nr = 0;
	    ForEach(r, NewNRules+1, NRules)
	    {
		if ( RuleIn[r] && Rule[r]->Rhs == c &&
		     ( ! nr || Rule[r]->Vote > Rule[nr]->Vote ) )
		{
		    nr = r;
		}
	    }

	    if ( ! nr ) break;

	    NewNRules++;
	    if ( nr != NewNRules )
	    {
		SwapRule(NewNRules, nr);
	    }
	}
    }

    return NewNRules;
}



/*************************************************************************/
/*									 */
/*	Discard deleted rules and sequence and renumber those remaining. */
/*	Sort by class and then by rule CF or by utility			 */
/*									 */
/*************************************************************************/


void OrderRules()
/*   ----------  */
{
    RuleNo	r, NewNRules;

    NewNRules = ( UTILITY ? OrderByUtility() : OrderByClass() );

    ForEach(r, 1, NewNRules)
    {
	Rule[r]->RNo = r;
    }

    /*  Free discarded rules  */

    ForEach(r, NewNRules+1, NRules)
    {
	FreeRule(Rule[r]);
    }

    NRules = NewNRules;
}



/*************************************************************************/
/*									 */
/*	Tabluate logs and log factorials (to improve speed)		 */
/*									 */
/*************************************************************************/


void GenerateLogs(int MaxN)
/*   ------------  */
{
    CaseNo	i;

    if ( LogCaseNo )
    {
	Realloc(LogCaseNo, MaxN+2, double);
	Realloc(LogFact, MaxN+2, double);
    }
    else
    {
	LogCaseNo = Alloc(MaxN+2, double);
	LogFact   = Alloc(MaxN+2, double);
    }

    LogCaseNo[0] = -1E38;
    LogCaseNo[1] = 0;

    LogFact[0] = LogFact[1] = 0;

    ForEach(i, 2, MaxN+1)
    {
	LogCaseNo[i] = Log((double) i);
	LogFact[i]   = LogFact[i-1] + LogCaseNo[i];
    }
}



void FreeSiftRuleData()
/*   ----------------  */
{
    FreeUnlessNil(List);				List = Nil;
    FreeVector((void **) Fires, 1, RuleSpace-1);	Fires = Nil;
    FreeUnlessNil(CBuffer);				CBuffer = Nil;
    FreeUnlessNil(Covered);				Covered = Nil;
    FreeUnlessNil(RuleIn);				RuleIn = Nil;
    FreeUnlessNil(CovBy);				CovBy = Nil;
    FreeUnlessNil(CovByPtr);				CovByPtr = Nil;
    FreeUnlessNil(BranchBits);				BranchBits = Nil;
    FreeUnlessNil(AttValues);				AttValues = Nil;

    FreeUnlessNil(DeltaErrs);				DeltaErrs = Nil;
    FreeUnlessNil(CovByBlock);				CovByBlock = Nil;
    FreeUnlessNil(Bits);				Bits = Nil;
    FreeUnlessNil(TopClass);				TopClass = Nil;
    FreeUnlessNil(AltClass);				AltClass = Nil;
    if ( TotVote )
    {
	FreeUnlessNil(TotVote[0]);
	FreeUnlessNil(TotVote);				TotVote = Nil;
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*                                                              	 */
/*	Routines for building a rule tree for faster classification.	 */
/*	A ruletree node consists of					 */
/*	* a list of rules satisfied at this node, terminated by 0	 */
/*	* a new test							 */
/*	* subtrees for each outcome (with branch 0 dealing with those	 */
/*	  rules that do not contain the new test)			 */
/*                                                              	 */
/*************************************************************************/



Condition	*Test=Nil;	/* tests that appear in ruleset */
int		NTest,		/* number of distinct tests */
		TestSpace,	/* space allocated for tests */
		*TestOccur,	/* frequency of test occurrence in rules */
		*RuleCondOK;	/* conditions satisfied by rule */

Boolean		*TestUsed;	/* used in parent nodes */



/*************************************************************************/
/*                                                              	 */
/*    Construct ruletree for ruleset RS					 */
/*                                                              	 */
/*************************************************************************/


void ConstructRuleTree(CRuleSet RS)
/*   -----------------  */
{
    int		r, c;
    RuleNo	*All;

    Test = Alloc((TestSpace = 1000), Condition);
    NTest = 0;

    All = Alloc(RS->SNRules, RuleNo);
    ForEach(r, 1, RS->SNRules)
    {
	All[r-1] = r;

	ForEach(c, 1, RS->SRule[r]->Size)
	{
	    SetTestIndex(RS->SRule[r]->Lhs[c]);
	}
    }

    TestOccur = Alloc(NTest, int);
    TestUsed  = AllocZero(NTest, Boolean);

    RuleCondOK = AllocZero(RS->SNRules+1, int);

    RS->RT = GrowRT(All, RS->SNRules, RS->SRule);

    Free(All);
    Free(Test);
    Free(TestUsed);
    Free(TestOccur);
    Free(RuleCondOK);
}



/*************************************************************************/
/*                                                              	 */
/*	Set test number for a condition.  If no existing test matches,	 */
/*	add new test to Test[]						 */
/*                                                              	 */
/*************************************************************************/


void SetTestIndex(Condition C)
/*   ------------  */
{
    int		t;
    Condition	CC;
    Attribute	Att;

    Att = C->Tested;

    ForEach(t, 0, NTest-1)
    {
	CC = Test[t];
	if ( CC->Tested != Att || CC->NodeType != C->NodeType ) continue;

	switch ( C->NodeType )
	{
	    case BrDiscr:
		C->TestI = t;
		return;

	    case BrSubset:
		if ( ! memcmp(C->Subset, CC->Subset, (MaxAttVal[Att]>>3)+1) )
		{
		    C->TestI = t;
		    return;
		}
		break;

	    case BrThresh:
		if ( C->TestValue == 1 && CC->TestValue == 1 ||
		     ( C->TestValue != 1 && CC->TestValue != 1 &&
		       C->Cut == CC->Cut ) )
		{
		    C->TestI = t;
		    return;
		}
		break;
	}
    }

    /*  New test -- make sure have enough space  */

    if ( NTest >= TestSpace )
    {
	Realloc(Test, (TestSpace += 1000), Condition);
    }

    Test[NTest] = C;
    C->TestI = NTest++;
}



/*************************************************************************/
/*                                                              	 */
/*	Construct ruletree for rules RR					 */
/*                                                              	 */
/*************************************************************************/


RuleTree GrowRT(RuleNo *RR, int RRN, CRule *Rule)
/*       ------  */
{
    RuleTree	Node;
    RuleNo	r, *LR;
    int		FP=0, ri, TI, *Expect, LRN;
    DiscrValue	v;

    if ( ! RRN ) return Nil;

    Node = AllocZero(1, RuleTreeRec);

    /*  Record and swap to front any rules that are satisfied  */

    ForEach(ri, 0, RRN-1)
    {
	r = RR[ri];

	if ( RuleCondOK[r] == Rule[r]->Size )
	{
	    RR[ri] = RR[FP];
	    RR[FP] = r;
	    FP++;
	}
    }

    if ( FP )
    {
	Node->Fire = Alloc(FP+1, RuleNo);
	memcpy(Node->Fire, RR, FP * sizeof(RuleNo));
	Node->Fire[FP] = 0;
	RR  += FP;
	RRN -= FP;
    }
    else
    {
	Node->Fire = Nil;
    }

    if ( ! RRN ) return Node;

    /*  Choose test for this node  */

    TI = SelectTest(RR, RRN, Rule);
    TestUsed[TI] = true;

    Node->CondTest = Test[TI];

    /*  Find the desired outcome for each rule  */

    Expect = Alloc(RRN, int);
    ForEach(ri, 0, RRN-1)
    {
	Expect[ri] = DesiredOutcome(Rule[RR[ri]], TI);
    }

    /*  Now construct individual branches.  Rules that do not reference
	the selected test go down branch 0; at classification time,
	any case with an unknown outcome for the selected test also
	goes to branch 0.  */

    Node->Forks =
	( Test[TI]->NodeType == BrDiscr ? MaxAttVal[Test[TI]->Tested] :
	  Test[TI]->NodeType == BrSubset ? 1 : 3 );

    Node->Branch = Alloc(Node->Forks+1, RuleTree);

    LR = Alloc(RRN, RuleNo);
    ForEach(v, 0, Node->Forks)
    {
	/*  Extract rules with outcome v and increment conditions satisfied,
	    if relevant  */

	LRN = 0;
	ForEach(ri, 0, RRN-1)
	{
	    if ( abs(Expect[ri]) == v )
	    {
		LR[LRN++] = RR[ri];

		if ( Expect[ri] > 0 ) RuleCondOK[RR[ri]]++;
	    }
	}

	/*  LR now contains rules with outcome v  */

	Node->Branch[v] = GrowRT(LR, LRN, Rule);

	if ( v )
	{
	    /*  Restore conditions satisfied  */

	    ForEach(ri, 0, LRN-1)
	    {
		RuleCondOK[LR[ri]]--;
	    }
	}
    }

    TestUsed[TI] = false;

    /*  Free local storage  */

    Free(LR);
    Free(Expect);

    return Node;
}



/*************************************************************************/
/*                                                              	 */
/*	Check whether rule uses Test[TI].				 */
/*	Return 0 (no) or test outcome required for rule			 */
/*                                                              	 */
/*************************************************************************/


int DesiredOutcome(CRule R, int TI)
/*  --------------  */
{
    int		c;
    Boolean	ContinTest;

    ContinTest = Continuous(Test[TI]->Tested);	/* test of continuous att */

    ForEach(c, 1, R->Size)
    {
	if ( R->Lhs[c]->TestI == TI )
	{
	    return R->Lhs[c]->TestValue;
	}

	/*  If this test references the same continuous attribute but
	    with a different threshold, may be able to exploit outcome:
	      -2 means "rule can only be matched down branch 2"
	      -3 means "rule can only be matched down branch 3"  */

	if ( ContinTest && Test[TI]->Tested == R->Lhs[c]->Tested )
	{
	    switch ( R->Lhs[c]->TestValue )
	    {
		case 1:
		    return 1;

		case 2:
		    if ( R->Lhs[c]->Cut < Test[TI]->Cut ) return -2;
		    break;

		case 3:
		    if ( R->Lhs[c]->Cut > Test[TI]->Cut ) return -3;
	    }
	}
    }

    return 0;
}



/*************************************************************************/
/*                                                              	 */
/*	Select most frequently-occurring test to partition rules in RR	 */
/*                                                              	 */
/*************************************************************************/


int SelectTest(RuleNo *RR, int RRN, CRule *Rule)
/*  ----------  */
{
    int		c, cc, ri;
    RuleNo	r;

    /*  Count test occurrences  */

    ForEach(c, 0, NTest-1)
    {
	TestOccur[c] = 0;
    }

    ForEach(ri, 0, RRN-1)
    {
	r = RR[ri];

	ForEach(c, 1, Rule[r]->Size)
	{
	    TestOccur[Rule[r]->Lhs[c]->TestI]++;
	}
    }

    /*  Find most frequently-occurring test  */

    cc = -1;
    ForEach(c, 0, NTest-1)
    {
	if ( ! TestUsed[c] && ( cc < 0 || TestOccur[c] > TestOccur[cc] ) )
	{
	    cc = c;
	}
    }

    return cc;
}



/*************************************************************************/
/*                                                              	 */
/*	Free ruletree							 */
/*                                                              	 */
/*************************************************************************/


void FreeRuleTree(RuleTree RT)
/*   ------------  */
{
    int		b;

    if ( ! RT ) return;

    if ( RT->Branch )
    {
	ForEach(b, 0, RT->Forks )
	{
	    FreeRuleTree(RT->Branch[b]);
	}
	Free(RT->Branch);
    }

    /*  Don't free RT->Cond since this is just a pointer to a condition
	in one of the rules  */

    FreeUnlessNil(RT->Fire);
    Free(RT);
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*								  	 */
/*	Miscellaneous routines for rule handling		  	 */
/*	----------------------------------------		  	 */
/*								  	 */
/*************************************************************************/




/*************************************************************************/
/*								  	 */
/*	Add a new rule to the current ruleset, by updating Rule[],  	 */
/*	NRules and, if necessary, RuleSpace			  	 */
/*								  	 */
/*************************************************************************/


Boolean NewRule(Condition Cond[], int NCond, ClassNo TargetClass,
		Boolean *Deleted, CRule Existing,
		CaseCount Cover, CaseCount Correct, float Prior)
/*      -------  */
{
    int		d, dd, id, r, Size=0, Bytes;
    CaseNo	i;
    CRule	R;
    Condition	*Lhs;
    Boolean	Exclude=false;
    int		Vote;

    /*  Sort and copy the conditions if required  */

    if ( ! Existing )
    {
	ForEach(d, 1, NCond)
	{
	    if ( ! Deleted[d] ) Size++;
	}

	Lhs = Alloc(Size+1, Condition);

	/*  Sort conditions in print order  */

	ForEach(d, 1, Size)
	{
	    dd =  0;
	    ForEach(id, 1, NCond)
	    {
		if ( ! Deleted[id] && ( ! dd || Before(Cond[id], Cond[dd]) ) )
		{
		    dd = id;
		}
	    }

	    Lhs[d] = Alloc(1, CondRec);
	    memcpy(Lhs[d], Cond[dd], sizeof(CondRec));
	    if ( Lhs[d]->NodeType == BrSubset )
	    {
		Bytes = (MaxAttVal[Lhs[d]->Tested]>>3) + 1;
		Lhs[d]->Subset = Alloc(Bytes, Byte);
		memcpy(Lhs[d]->Subset, Cond[dd]->Subset, Bytes);
	    }

	    Deleted[dd] = true;
	}
    }
    else
    {
	Lhs  = Cond;
	Size = NCond;
    }

    Vote = 1000 * (Correct + 1.0) / (Cover + 2.0) + 0.5;

    /*  See if rule already exists  */

    for ( r = 1 ; ! Exclude && r <= NRules ; r++ )
    {
	if ( SameRule(r, Lhs, Size, TargetClass) )
	{
	    Verbosity(1, fprintf(Of, "\tduplicates rule %d\n", r))

	    /*  Keep the most optimistic error estimate  */

	    if ( Vote > Rule[r]->Vote )
	    {
		Rule[r]->Vote = Vote;
	    }

	    Exclude = true;
	}
    }

    if ( Exclude )
    {
	if ( ! Existing )
	{
	    ForEach(d, 1, Size)
	    {
		if ( Lhs[d]->NodeType == BrSubset ) Free(Lhs[d]->Subset);
	    }
	    FreeVector((void **) Lhs, 1, Size);
	}

	return false;
    }

    /*  Make sure there is enough room for the new rule  */

    NRules++;
    if ( NRules >= RuleSpace )
    {
	RuleSpace += 100;
	if ( RuleSpace > 100 )
	{
	    Realloc(Rule,  RuleSpace, CRule);
	    Realloc(Fires, RuleSpace, Byte *);
	    ForEach(r, RuleSpace-100, RuleSpace-1)
	    {
		Fires[r] = Nil;
	    }
	}
	else
	{
	    Rule  = Alloc(RuleSpace, CRule);
	    Fires = AllocZero(RuleSpace, Byte *);
	}
    }

    /*  Form the new rule  */

    Rule[NRules] = R = Alloc(1, RuleRec);

    R->TNo     = ( Existing ? Existing->TNo : Trial );
    R->RNo     = ( Existing ? Existing->RNo : NRules );
    R->Size    = Size;
    R->Lhs     = Lhs;
    R->Rhs     = TargetClass;
    R->Cover   = Cover;
    R->Correct = Correct;
    R->Prior   = Prior;
    R->Vote    = Vote;

    /*  Record entry in Fires and CovBy  */

    ListSort(List, 1, List[0]);
    Fires[NRules] = Compress(List);

    ForEach(i, 1, List[0])
    {
	CovBy[List[i]]++;
    }

    Verbosity(1, if ( ! Existing ) PrintRule(R))

    return true;
}



/*************************************************************************/
/*								  	 */
/*	Compress list of ascending integers.				 */
/*								  	 */
/*	The first integer occupies 4 bytes.  Each subsequent integer is	 */
/*	represented as the increment on the previous and is encoded as	 */
/*	one or more bytes b0 + b1 + .... where				 */
/*	  if byte b < 128, value is b					 */
/*	  if byte b = 128 + x, value is x * 128				 */
/*								  	 */
/*	For example, an increment 4321 (= 33 * 128 + 97) is encoded as	 */
/*	two bytes [128 + 33] [97]					 */
/*								  	 */
/*************************************************************************/


Byte *Compress(int *L)
/*    --------  */
{
    int		i, Last=0, Entry, Blocks;
    Byte	*p, *Compressed;

    /*  Copy first integer (uncompressed)  */

    memcpy(CBuffer, L, 4);
    p = CBuffer + 4;

    ForEach(i, 1, L[0])
    {
	Entry = L[i] - Last;
	Last  = L[i];

	/*  Place any necessary skip bytes  */

	while ( Entry > 127 )
	{
	    Blocks = (Entry >> 7);
	    if ( Blocks > 127 ) Blocks = 127;
	    Entry -= Blocks * 128;
	    *p++   = Blocks + 128;
	}

	*p++ = Entry;
    }

    Compressed = Alloc(p - CBuffer, Byte);
    memcpy(Compressed, CBuffer, p - CBuffer);

    return Compressed;
}



void Uncompress(Byte *CL, int *UCL)
/*   ----------  */
{
    int		i, Entry=0;
    Byte	*p;

    memcpy(UCL, CL, 4);
    p = CL + 4;

    ForEach(i, 1, UCL[0])
    {
	while ( (*p) & 128 )
	{
	    Entry += ((*p++) & 127) * 128;
	}

	Entry = UCL[i] = Entry + *p++;
    }
}



/*************************************************************************/
/*								  	 */
/*	Sort list in preparation for Compress()				 */
/*								  	 */
/*************************************************************************/


void ListSort(int *L, int Fp, int Lp)
/*   --------  */
{
    int		i, High, Middle, Thresh, Temp;

    if ( Fp < Lp )
    {
	Thresh = L[(Fp+Lp) / 2];

	/*  Divide cases into three groups:
		Fp .. Middle-1: values < Thresh
		Middle .. High: values = Thresh
		High+1 .. Lp:   values > Thresh  */

	for ( Middle = Fp ; L[Middle] < Thresh ; Middle++ )
	    ;

	for ( High = Lp ; L[High] > Thresh ; High-- )
	    ;

	for ( i = Middle ; i <= High ; )
	{
	    if ( L[i] < Thresh )
	    {
		Temp = L[Middle];
		L[Middle] = L[i];
		L[i] = Temp;
		Middle++;
		i++;
	    }
	    else
	    if ( L[i] > Thresh )
	    {
		Temp = L[High];
		L[High] = L[i];
		L[i] = Temp;
		High--;
	    }
	    else
	    {
		i++;
	    }
	}

	/*  Sort the first and third groups  */

	ListSort(L, Fp, Middle-1);
	ListSort(L, High+1, Lp);
    }
}



/*************************************************************************/
/*								  	 */
/*	Decide whether the given rule duplicates rule r		  	 */
/*								  	 */
/*************************************************************************/


Boolean SameRule(RuleNo r, Condition Cond[], int NConds, ClassNo TargetClass)
/*      --------  */
{
    int	d, i, Bytes;

    if ( Rule[r]->Size != NConds || Rule[r]->Rhs != TargetClass )
    {
	return false;
    }

    ForEach(d, 1, NConds)
    {
	if ( Rule[r]->Lhs[d]->NodeType != Cond[d]->NodeType ||
	     Rule[r]->Lhs[d]->Tested   != Cond[d]->Tested )
	{
	    return false;
	}

	switch ( Cond[d]->NodeType )
	{
	    case BrDiscr:
		if ( Rule[r]->Lhs[d]->TestValue != Cond[d]->TestValue )
		{
		    return false;
		}
		break;

	    case BrThresh:
		if ( Rule[r]->Lhs[d]->TestValue != Cond[d]->TestValue ||
		     Rule[r]->Lhs[d]->Cut != Cond[d]->Cut )
		{
		    return false;
		}
		break;

	    case BrSubset:
		Bytes = (MaxAttVal[Cond[d]->Tested]>>3) + 1;
		ForEach(i, 0, Bytes-1)
		{
		    if ( Rule[r]->Lhs[d]->Subset[i] != Cond[d]->Subset[i] )
		    {
			return false;
		    }
		}
	}
    }

    return true;
}



/*************************************************************************/
/*								  	 */
/*	Free space occupied by a rule and a ruleset			 */
/*								  	 */
/*************************************************************************/


void FreeRule(CRule R)
/*   --------  */
{
    int	d;

    ForEach(d, 1, R->Size)
    {
	if ( R->Lhs[d]->NodeType == BrSubset )
	{
	    FreeUnlessNil(R->Lhs[d]->Subset);
	}
	FreeUnlessNil(R->Lhs[d]);
    }
    FreeUnlessNil(R->Lhs);
    FreeUnlessNil(R);
}



void FreeRules(CRuleSet RS)
/*   ---------  */
{
    int	ri;

    ForEach(ri, 1, RS->SNRules)
    {
	FreeRule(RS->SRule[ri]);
    }
    Free(RS->SRule);
    FreeRuleTree(RS->RT);
    Free(RS);
}



/*************************************************************************/
/*								  	 */
/*	Print a ruleset							 */
/*								  	 */
/*************************************************************************/


void PrintRules(CRuleSet RS, String Msg)
/*   ----------  */
{
    int	r;

    fprintf(Of, "\n%s\n", Msg);

    ForEach(r, 1, RS->SNRules)
    {
	PrintRule(RS->SRule[r]);
    }
}



/*************************************************************************/
/*								  	 */
/*	Print rule R						  	 */
/*								  	 */
/*************************************************************************/


void PrintRule(CRule R)
/*   ---------  */
{
    int		d;

    fprintf(Of, T_RuleHeader);
    if ( TRIALS > 1 ) fprintf(Of, "%d/", R->TNo);
    fprintf(Of, "%d: (%.8g", R->RNo, P1(R->Cover));
    if ( R->Correct < R->Cover - 0.1 )
    {
	fprintf(Of, "/%.8g", P1(R->Cover - R->Correct));
    }
    fprintf(Of, T_RuleLift, ((R->Correct + 1) / (R->Cover + 2)) / R->Prior);

    ForEach(d, 1, R->Size)
    {
	PrintCondition(R->Lhs[d]);
    }

    fprintf(Of, "\t->  " T_class " %s  [%.3f]\n",
		ClassName[R->Rhs], R->Vote/1000.0);
}



/*************************************************************************/
/*								  	 */
/*	Print a condition C of a rule				  	 */
/*								  	 */
/*************************************************************************/


void PrintCondition(Condition C)
/*  --------------  */
{
    DiscrValue	v, pv, Last, Values;
    Boolean	First=true;
    Attribute	Att;
    int		Col, Base, Entry;
    char	CVS[20];

    v   = C->TestValue;
    Att = C->Tested;

    fprintf(Of, "\t%s", AttName[Att]);

    if ( v < 0 )
    {
	fprintf(Of, T_IsUnknown);
	return;
    }

    switch ( C->NodeType )
    {
	case BrDiscr:
	    fprintf(Of, " = %s\n", AttValName[Att][v]);
	    break;

	case BrThresh:
	    if ( v == 1 )
	    {
		fprintf(Of, " = N/A\n");
	    }
	    else
	    {
		CValToStr(C->Cut, Att, CVS);
		fprintf(Of, " %s %s\n", ( v == 2 ? "<=" : ">" ), CVS);
	    }
	    break;

	case BrSubset:
	    /*  Count values at this branch  */

	    Values = Elements(Att, C->Subset, &Last);
	    if ( Values == 1 )
	    {
		fprintf(Of, " = %s\n", AttValName[Att][Last]);
		break;
	    }

	    if ( Ordered(Att) )
	    {
		/*  Find first value  */

		for ( pv = 1 ; ! In(pv, C->Subset) ; pv++ )
		    ;

		fprintf(Of, " %s [%s-%s]\n", T_InRange,
			AttValName[Att][pv], AttValName[Att][Last]);
		break;
	    }

	    /*  Must keep track of position to break long lines  */

	    fprintf(Of, " %s {", T_ElementOf);
	    Col = Base = CharWidth(AttName[Att]) + CharWidth(T_ElementOf) + 11;

	    ForEach(pv, 1, MaxAttVal[Att])
	    {
		if ( In(pv, C->Subset) )
		{
		    Entry = CharWidth(AttValName[Att][pv]);

		    if ( First )
		    {
			First = false;
		    }
		    else
		    if ( Col + Entry + 2 >= Width )
		    {
			Col = Base;
			fprintf(Of, ",\n%*s", Col, "");
		    }
		    else
		    {
			fprintf(Of, ", ");
			Col += 2;
		    }

		    fprintf(Of, "%s", AttValName[Att][pv]);
		    Col += Entry;
		}
	    }
	    fprintf(Of, "}\n");
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Get cases from data file					 */
/*	------------------------					 */
/*									 */
/*************************************************************************/



#include "cpp_wrapper/clist.h" // Daniel
#include "cpp_wrapper/file_tools.h" // Daniel

#define Inc 2048

Boolean SuppressErrorMessages=false;
#define XError(a,b,c)	if (! SuppressErrorMessages) Error(a,b,c)

CaseNo	SampleFrom;		/* file count for sampling */


/*************************************************************************/
/*									 */
/*	Read raw cases from file with given extension.			 */
/*									 */
/*	On completion, cases are stored in array Case in the form	 */
/*	of vectors of attribute values, and MaxCase is set to the	 */
/*	number of data cases.						 */
/*									 */
/*************************************************************************/


void GetData(FILE *Df, Boolean Train, Boolean AllowUnknownClass)
/*   -------  */
{
    DataRec	DVec;
    CaseNo	CaseSpace, WantTrain, LeftTrain, WantTest, LeftTest;
    Boolean	FirstIgnore=true, SelectTrain;

    LineNo = 0;
    SuppressErrorMessages = SAMPLE && ! Train;

    /*  Don't reset case count if appending data for xval  */

    if ( Train || ! Case )
    {
	MaxCase = MaxLabel = CaseSpace = 0;
	Case = Alloc(1, DataRec);	/* for error reporting */
    }
    else
    {
	assert (false);

	CaseSpace = MaxCase + 1;
	MaxCase++;
    }

    if ( SAMPLE )
    {
	assert (false);

	if ( Train )
	{
	    SampleFrom = CountData(Df);
	    ResetKR(KRInit);		/* initialise KRandom() */
	}
	else
	{
	    ResetKR(KRInit);		/* restore  KRandom() */
	}

	WantTrain = SampleFrom * SAMPLE + 0.5;
	LeftTrain = SampleFrom;

	WantTest  = ( SAMPLE < 0.5 ? WantTrain : SampleFrom - WantTrain );
	LeftTest  = SampleFrom - WantTrain;
    }

    while ( (DVec = GetDataRec(Df, Train)) )
    {
	/*  Check whether to include if we are sampling */

	if ( SAMPLE )
	{
	    assert (false);

	    SelectTrain = KRandom() < WantTrain / (float) LeftTrain--;

	    /*  Include if
		 * Select and this is the training set
		 * ! Select and this is the test set and sub-select
	 	NB: Must use different random number generator for
		sub-selection since cannot disturb random number sequence  */

	    if ( SelectTrain )
	    {
		WantTrain--;
	    }

	    if ( SelectTrain != Train ||
		 ( ! Train && AltRandom >= WantTest / (float) LeftTest-- ) )
	    {
		FreeLastCase(DVec);
		continue;
	    }

	    if ( ! Train )
	    {
		WantTest--;
	    }
	}

	/*  Make sure there is room for another case  */

	if ( MaxCase >= CaseSpace )
	{
	    CaseSpace += Inc;
	    Realloc(Case, CaseSpace+1, DataRec);
	}

	/*  Ignore cases with unknown class  */

	if ( AllowUnknownClass || (Class(DVec) & 077777777) > 0 )
	{
	    Case[MaxCase] = DVec;
	    MaxCase++;
	}
	else
	{
	    if ( FirstIgnore && Of )
	    {
		fprintf(Of, T_IgnoreBadClass);
		FirstIgnore = false;
	    }

	    FreeLastCase(DVec);
	}
    }

    fclose(Df);
    MaxCase--;

}



/*************************************************************************/
/*									 */
/*	Read a raw case from file Df.					 */
/*									 */
/*	For each attribute, read the attribute value from the file.	 */
/*	If it is a discrete valued attribute, find the associated no.	 */
/*	of this attribute value (if the value is unknown this is 0).	 */
/*									 */
/*	Returns the DataRec of the case (i.e. the array of attribute	 */
/*	values).							 */
/*									 */
/*************************************************************************/


DataRec GetDataRec(FILE *Df, Boolean Train)
/*      ----------  */
{
    Attribute	Att;
    char	Name[1000], *EndName;
    int		Dv, Chars;
    DataRec	DVec;
    ContValue	Cv;
    Boolean	FirstValue=true;


    if ( ReadName(Df, Name, 1000, '\00') )
    {
	Case[MaxCase] = DVec = NewCase();
	ForEach(Att, 1, MaxAtt)
	{
	    if ( AttDef[Att] )
	    {
		DVec[Att] = EvaluateDef(AttDef[Att], DVec);

		if ( Continuous(Att) )
		{
		    CheckValue(DVec, Att);
		}

		if ( SomeMiss )
		{
		    SomeMiss[Att] |= Unknown(DVec, Att);
		    SomeNA[Att]   |= NotApplic(DVec, Att);
		}

		continue;
	    }

	    /*  Get the attribute value if don't already have it  */

	    if ( ! FirstValue && ! ReadName(Df, Name, 1000, '\00') )
	    {
		XError(HITEOF, AttName[Att], "");
		FreeLastCase(DVec);
		return Nil;
	    }
	    FirstValue = false;

	    if ( Exclude(Att) )
	    {
		if ( Att == LabelAtt )
		{
		    /*  Record the value as a string  */

		    SVal(DVec,Att) = StoreIVal(Name);
		}
	    }
	    else
	    if ( ! strcmp(Name, "?") )
	    {
		/*  Set marker to indicate missing value  */

		DVal(DVec, Att) = UNKNOWN;
		if ( SomeMiss ) SomeMiss[Att] = true;

		if(ClassAtt && Att == ClassAtt)
		{

		    // Pranav: Very important. Now ensuring that the Class and DVal(., ClassAtt)
		    // are consistent.
		    DVal(DVec, Att) = 0;

		    #if false
		    printf("Assigning classvalue to unknown %d\n", DVal(DVec, ClassAtt));
		    #endif
		}
	    }
	    else
	    if ( Att != ClassAtt && ! strcmp(Name, "N/A") )
	    {
		/*  Set marker to indicate not applicable  */

		DVal(DVec, Att) = NA;
		if ( SomeNA ) SomeNA[Att] = true;
	    }
	    else
	    if ( Discrete(Att) )
	    {
		/*  Discrete attribute  */

		Dv = Which(Name, AttValName[Att], 1, MaxAttVal[Att]);
		if ( ! Dv )
		{
		    if ( StatBit(Att, DISCRETE) )
		    {
			assert (false);

			if ( Train || XVAL )
			{
			    /*  Add value to list  */

			    if ( MaxAttVal[Att] >= (long) AttValName[Att][0] )
			    {
				XError(TOOMANYVALS, AttName[Att],
					 (char *) AttValName[Att][0] - 1);
				Dv = MaxAttVal[Att];
			    }
			    else
			    {
				Dv = ++MaxAttVal[Att];
				AttValName[Att][Dv]   = strdup(Name);
				AttValName[Att][Dv+1] = "<other>"; /* no free */
			    }
			    if ( Dv > MaxDiscrVal )
			    {
				MaxDiscrVal = Dv;
			    }
			}
			else
			{
			    /*  Set value to "<other>"  */

			    Dv = MaxAttVal[Att] + 1;
			}
		    }
		    else
		    {
			XError(BADATTVAL, AttName[Att], Name);
			Dv = UNKNOWN;
		    }
		}
		DVal(DVec, Att) = Dv;
	    }
	    else
	    {
		/*  Continuous value  */

		if ( TStampVal(Att) )
		{
		    CVal(DVec, Att) = Cv = TStampToMins(Name);
		    if ( Cv >= 1E9 )	/* long time in future */
		    {
			XError(BADTSTMP, AttName[Att], Name);
			DVal(DVec, Att) = UNKNOWN;
		    }
		}
		else
		if ( DateVal(Att) )
		{
		    CVal(DVec, Att) = Cv = DateToDay(Name);
		    if ( Cv < 1 )
		    {
			XError(BADDATE, AttName[Att], Name);
			DVal(DVec, Att) = UNKNOWN;
		    }
		}
		else
		if ( TimeVal(Att) )
		{
		    CVal(DVec, Att) = Cv = TimeToSecs(Name);
		    if ( Cv < 0 )
		    {
			XError(BADTIME, AttName[Att], Name);
			DVal(DVec, Att) = UNKNOWN;
		    }
		}
		else
		{
		    CVal(DVec, Att) = strtod(Name, &EndName);
		    if ( EndName == Name || *EndName != '\0' )
		    {
			XError(BADATTVAL, AttName[Att], Name);
			DVal(DVec, Att) = UNKNOWN;
		    }
		}

		CheckValue(DVec, Att);
	    }
	}

	if ( ClassAtt )
	{
	    if ( Discrete(ClassAtt) )
	    {
		Class(DVec) = XDVal(DVec, ClassAtt);
		if (Unknown (DVec, ClassAtt))
		{
		    // DVal(DVec, ClassAtt) is now already assigned to 0.
		    assert (false);
		    assert (Class(DVec) == 0);
		}
	    }
	    else
	    if ( Unknown(DVec, ClassAtt) || NotApplic(DVec, ClassAtt) )
	    {
		assert (false);
		Class(DVec) = 0;
	    }
	    else
	    {
		/*  Find appropriate segment using class thresholds  */

		Cv = CVal(DVec, ClassAtt);

		for ( Dv = 1 ; Dv < MaxClass && Cv > ClassThresh[Dv] ; Dv++ )
		    ;

		Class(DVec) = Dv;
	    }
	}
	else
	{
	    if ( ! ReadName(Df, Name, 1000, '\00') )
	    {
		XError(HITEOF, Fn, "");
		FreeLastCase(DVec);
		return Nil;
	    }

	    if ( (Class(DVec) = Dv = Which(Name, ClassName, 1, MaxClass)) == 0 )
	    {
		if ( strcmp(Name, "?") ) XError(BADCLASS, "", Name);
	    }
	}

	if ( LabelAtt &&
	     (Chars = strlen(IgnoredVals + SVal(DVec, LabelAtt))) > MaxLabel )
	{
	    MaxLabel = Chars;
	}
	return DVec;
    }
    else
    {
	return Nil;
    }
}



/*************************************************************************/
/*                                                                       */
/*      Count cases in data file					 */
/*                                                                       */
/*************************************************************************/


CaseNo CountData(FILE *Df)
/*     ---------  */
{
    char        Last=',';
    int         Count=0, Next;

    while ( true )
    {
	if ( (Next = getc(Df)) == EOF )
	{
	    if ( Last != ',' ) Count++;
	    rewind(Df);
	    return Count;
	}

	if ( Next == '|' )
	{
	    while ( (Next = getc(Df)) != '\n' )
		;
	}

	if ( Next == '\n' )
	{
	    if ( Last != ',' ) Count++;
	    Last = ',';
	}
	else
	if ( Next == '\\' )
	{
	    /*  Skip escaped character  */

	    getc(Df);
	}
	else
	if ( Next != '\t' && Next != ' ' )
	{
	    Last = Next;
	}
    }
}



/*************************************************************************/
/*									 */
/*	Store a label or ignored value in IValStore			 */
/*									 */
/*************************************************************************/


int StoreIVal(String S)
/*  ---------  */
{
    int		StartIx, Length;

    if ( (Length=strlen(S) + 1) + IValsOffset > IValsSize )
    {
	if ( IgnoredVals )
	{
	    Realloc(IgnoredVals, IValsSize += 32768, char);
	}
	else
	{
	    IValsSize   = 32768;
	    IValsOffset = 0;
	    IgnoredVals = Alloc(IValsSize, char);
	}
    }

    StartIx = IValsOffset;
    strcpy(IgnoredVals + StartIx, S);
    IValsOffset += Length;

    return StartIx;
}



/*************************************************************************/
/*									 */
/*	Free case space							 */
/*									 */
/*************************************************************************/


void FreeData()
/*   --------  */
{
    FreeCases();

    FreeUnlessNil(IgnoredVals);				IgnoredVals = Nil;
							IValsSize = 0;

    Free(Case);						Case = Nil;

    MaxCase = -1;
}



/*************************************************************************/
/*									 */
/*	Check for bad continuous value					 */
/*									 */
/*************************************************************************/


void CheckValue(DataRec DVec, Attribute Att)
/*   ----------  */
{
    ContValue	Cv;

    Cv = CVal(DVec, Att);
    if ( ! finite(Cv) )
    {
	Error(BADNUMBER, AttName[Att], "");

	CVal(DVec, Att) = UNKNOWN;
    }
}


/*************************************************************************/
/*                                                                       */
/*  Read implications from file with given name, which is constructed    */
/*  from the given filename and extension                                */
/*                                                                       */
/*************************************************************************/
void GetImplications(const char * extension) {

	if(!extension) {
		printf("Invalid extension '%s'\n", extension);
	}

	// Construct filename
	char complete_filename[strlen(FileStem) + strlen(extension) + 1]; 
	strcpy(complete_filename, FileStem);
	strcat(complete_filename, extension);

	// Read implications
	Implications = new_cmap();
	if (! read_implications(complete_filename, Implications)) {
		printf("Problem with implication file '%s'\n", complete_filename);
	}

}


/*************************************************************************/
/*                                                                       */
/*  Read intervals from file with given name, which is constructed       */
/*  from the given filename and extension                                */
/*                                                                       */
/*************************************************************************/
void GetIntervals(const char * extension) {

	if(!extension) {
		printf("Invalid extension '%s'\n", extension);
		return;
	}

	// Construct filename
	char complete_filename[strlen(FileStem) + strlen(extension) + 1]; 
	strcpy(complete_filename, FileStem);
	strcat(complete_filename, extension);

	// Read intervals
	struct clist * lower = new_clist ();
	struct clist * upper = new_clist ();
	read_intervals(complete_filename, lower, upper);

	if (lower == NULL || upper == NULL || clist_size (lower) == 0 || clist_size (upper) == 0) {
		
		//printf ("There is a problem with the intervals file '%s'\n", complete_filename);
		delete_clist (lower);
		delete_clist (upper);
		return;

	}

	// Add intervals
	IntervalsLowerBounds = clist_to_array (lower);
	IntervalsUpperBounds = clist_to_array (upper);
	delete_clist (lower);
	delete_clist (upper);
	assert (IntervalsLowerBounds != NULL && IntervalsUpperBounds != NULL);
	assert (IntervalsLowerBounds->size == IntervalsUpperBounds->size);

	// Check intervals
	int ok = 1;
	int i;
	for (i = 0; i < IntervalsLowerBounds->size; i++) {
		
		if (IntervalsLowerBounds->entries[i] < MinAtt ||
			IntervalsLowerBounds->entries[i] > MaxAtt ||
			IntervalsUpperBounds->entries[i] < MinAtt ||
			IntervalsUpperBounds->entries[i] > MaxAtt ||
			IntervalsLowerBounds->entries[i] > IntervalsUpperBounds->entries[i]) {

			ok = 0;
			break;

		}

	}
	

	// Clean up if there is a problem
	if (!ok) {

		printf ("There was something wrong with the intervals!\n\n"); 
		delete_array (IntervalsLowerBounds);
		IntervalsLowerBounds = NULL;
		delete_array (IntervalsUpperBounds);
		IntervalsUpperBounds = NULL;

	}

}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Routines to handle implicitly-defined attributes		 */
/*	------------------------------------------------		 */
/*									 */
/*************************************************************************/


#include <ctype.h>


char	*Buff;			/* buffer for input characters */
int	BuffSize, BN;		/* size and index of next character */

EltRec	*TStack;		/* expression stack model */
int	TStackSize, TSN;	/* size of stack and index of next entry */

int	DefSize, DN;		/* size of definition and next element */

Boolean PreviousError;		/* to avoid parasitic errors */

AttValue _UNK,			/* quasi-constant for unknown value */
	 _NA;			/* ditto for not applicable */


#define FailSyn(Msg)	 {DefSyntaxError(Msg); return false;}
#define FailSem(Msg)	 {DefSemanticsError(Fi, Msg, OpCode); return false;}

typedef  union  _xstack_elt
         {
            DiscrValue  _discr_val;
            ContValue   _cont_val;
            String      _string_val;
         }
	 XStackElt;

#define	cval		_cont_val
#define	sval		_string_val
#define	dval		_discr_val



/*************************************************************************/
/*									 */
/*	A definition is handled in two stages:				 */
/*	  - The definition is read (up to a line ending with a period)	 */
/*	    replacing multiple whitespace characters with one space	 */
/*	  - The definition is then read (using a recursive descent	 */
/*	    parser), building up a reverse polish expression		 */
/*	Syntax and semantics errors are flagged				 */
/*									 */
/*************************************************************************/


void ImplicitAtt(FILE *Nf)
/*   -----------  */
{
#ifdef CUBIST
    _UNK.cval = UNKNOWN;
#else
    _UNK.dval = UNKNOWN;
#endif
    _NA.dval  = NA;

    /*  Get definition as a string in Buff  */

    ReadDefinition(Nf);

    PreviousError = false;
    BN = 0;

    /*  Allocate initial stack and attribute definition  */

    TStack = Alloc(TStackSize=50, EltRec);
    TSN = 0;

    AttDef[MaxAtt] = Alloc(DefSize = 100, DefElt);
    DN = 0;

    /*  Parse Buff as an expression terminated by a period  */

    Expression();
    if ( ! Find(".") ) DefSyntaxError("'.' ending definition");

    /*  Final check -- defined attribute must not be of type String  */

    if ( ! PreviousError )
    {
	if ( DN == 1 && DefOp(AttDef[MaxAtt][0]) == OP_ATT &&
	     strcmp(AttName[MaxAtt], "case weight") )
	{
	    Error(SAMEATT, AttName[ (long) DefSVal(AttDef[MaxAtt][0]) ], Nil);
	}

	if ( TStack[0].Type == 'B' )
	{
	    /*  Defined attributes should never have a value N/A  */

	    MaxAttVal[MaxAtt] = 3;
	    AttValName[MaxAtt] = AllocZero(4, String);
	    AttValName[MaxAtt][1] = strdup("??");
	    AttValName[MaxAtt][2] = strdup("t");
	    AttValName[MaxAtt][3] = strdup("f");
	}
	else
	{
	    MaxAttVal[MaxAtt] = 0;
	}
    }

    if ( PreviousError )
    {
	DN = 0;
	SpecialStatus[MaxAtt] = EXCLUDE;
    }

    /*  Write a terminating marker  */

    DefOp(AttDef[MaxAtt][DN]) = OP_END;

    Free(Buff);
    Free(TStack);
}



/*************************************************************************/
/*									 */
/*	Read the text of a definition.  Skip comments, collapse		 */
/*	multiple whitespace characters.					 */
/*									 */
/*************************************************************************/


void ReadDefinition(FILE *f)
/*   --------------  */
{
    Boolean	LastWasPeriod=false;
    char	c;

    Buff = Alloc(BuffSize=50, char);
    BN = 0;

    while ( true )
    {
	c = InChar(f);

	if ( c == '|' ) SkipComment;

	if ( c == EOF || c == '\n' && LastWasPeriod )
	{
	    /*  The definition is complete.  Add a period if it's
		not there already and terminate the string  */

	    if ( ! LastWasPeriod ) Append('.');
	    Append(0);

	    return;
	}

	if ( Space(c) )
	{
	    Append(' ');
	}
	else
	if ( c == '\\' )
	{
	    /*  Escaped character -- bypass any special meaning  */

	    Append(InChar(f));
	}
	else
	{
	    LastWasPeriod = ( c == '.' );
	    Append(c);
	}
    }
}



/*************************************************************************/
/*									 */
/*	Append a character to Buff, resizing it if necessary		 */
/*									 */
/*************************************************************************/


void Append(char c)
/*   ------  */
{
    if ( c == ' ' && (! BN || Buff[BN-1] == ' ' ) ) return;

    if ( BN >= BuffSize )
    {
	Realloc(Buff, BuffSize += 50, char);
    }

    Buff[BN++] = c;
}



/*************************************************************************/
/*									 */
/*	Recursive descent parser with syntax error checking.		 */
/*	The reverse polish is built up by calls to Dump() and DumpOp(),	 */
/*	which also check for semantic validity.				 */
/*									 */
/*	For possible error messages, each routine also keeps track of	 */
/*	the beginning of the construct that it recognises (in Fi).	 */
/*									 */
/*************************************************************************/


Boolean Expression()
/*      ----------  */
{
    int		Fi=BN;

    if ( Buff[BN] == ' ' ) BN++;

    if ( ! Conjunct() ) FailSyn("expression");

    while ( Find("or") )
    {
	BN += 2;

	if ( ! Conjunct() ) FailSyn("expression");

	DumpOp(OP_OR, Fi);
    }

    return true;
}



Boolean Conjunct()
/*      --------  */
{
    int		Fi=BN;

    if ( ! SExpression() ) FailSyn("expression");

    while ( Find("and") )
    {
	BN += 3;

	if ( ! SExpression() ) FailSyn("expression");

	DumpOp(OP_AND, Fi);
    }

    return true;
}



String RelOps[] = {">=", "<=", "!=", "<>", ">", "<", "=", (String) 0};

Boolean SExpression()
/*      -----------  */
{
    int		o, Fi=BN;

    if ( ! AExpression() ) FailSyn("expression");

    if ( (o = FindOne(RelOps)) >= 0 )
    {
	BN += strlen(RelOps[o]);

	if ( ! AExpression() ) FailSyn("expression");

	DumpOp(( o == 0 ? OP_GE :
		 o == 1 ? OP_LE :
		 o == 4 ? OP_GT :
		 o == 5 ? OP_LT :
		 o == 2 || o == 3 ?
			( TStack[TSN-1].Type == 'S' ? OP_SNE : OP_NE ) :
			( TStack[TSN-1].Type == 'S' ? OP_SEQ : OP_EQ ) ), Fi);
    }

    return true;
}



String AddOps[] = {"+", "-", (String) 0};

Boolean AExpression()
/*      -----------  */
{
    int		o, Fi=BN;

    if ( Buff[BN] == ' ' ) BN++;

    if ( (o = FindOne(AddOps)) >= 0 )
    {
	BN += 1;
    }

    if ( ! Term() ) FailSyn("expression");

    if ( o == 1 ) DumpOp(OP_UMINUS, Fi);

    while ( (o = FindOne(AddOps)) >= 0 )
    {
	BN += 1;

	if ( ! Term() ) FailSyn("arithmetic expression");

	DumpOp((char)(OP_PLUS + o), Fi);
    }

    return true;
}



String MultOps[] = {"*", "/", "%", (String) 0};

Boolean Term()
/*      ----  */
{
    int		o, Fi=BN;

    if ( ! Factor() ) FailSyn("expression");

    while ( (o = FindOne(MultOps)) >= 0 )
    {
	BN += 1;

	if ( ! Factor() ) FailSyn("arithmetic expression");

	DumpOp((char)(OP_MULT + o), Fi);
    }

    return true;
}



Boolean Factor()
/*      ----  */
{
    int		Fi=BN;

    if ( ! Primary() ) FailSyn("value");

    while ( Find("^") )
    {
	BN += 1;

	if ( ! Primary() ) FailSyn("exponent");

	DumpOp(OP_POW, Fi);
    }

    return true;
}



Boolean Primary()
/*      -------  */
{
    if ( Atom() )
    {
	return true;
    }
    else
    if ( Find("(") )
    {
	BN++;
	if ( ! Expression() ) FailSyn("expression in parentheses");
	if ( ! Find(")") ) FailSyn("')'");
	BN++;
	return true;
    }
    else
    {
	FailSyn("attribute, value, or '('");
    }
}



String Funcs[] = {"sin", "cos", "tan", "log", "exp", "int", (String) 0};

Boolean Atom()
/*      ----  */
{
    char	*EndPtr, *Str, Date[11], Time[9];
    int		o, FirstBN, Fi=BN;
    ContValue	F;
    Attribute	Att;

    if ( Buff[BN] == ' ' ) BN++;

    if ( Buff[BN] == '"' )
    {
	FirstBN = ++BN;
	while ( Buff[BN] != '"' )
	{
	    if ( ! Buff[BN] ) FailSyn("closing '\"'");
	    BN++;
	}

	/*  Make a copy of the string without double quotes  */

	Buff[BN] = '\00';
	Str = strdup(Buff + FirstBN);

	Buff[BN++] = '"';
	Dump(OP_STR, 0, Str, Fi);
    }
    else
    if ( (Att = FindAttName()) )
    {
	BN += strlen(AttName[Att]);

	Dump(OP_ATT, 0, (String) (long) Att, Fi);
    }
    else
    if ( isdigit(Buff[BN]) )
    {
	/*  Check for date or time first  */

	if ( ( Buff[BN+4] == '/' && Buff[BN+7] == '/' ||
	       Buff[BN+4] == '-' && Buff[BN+7] == '-' )&&
	     isdigit(Buff[BN+1]) && isdigit(Buff[BN+2]) &&
		isdigit(Buff[BN+3]) &&
	     isdigit(Buff[BN+5]) && isdigit(Buff[BN+6]) &&
	     isdigit(Buff[BN+8]) && isdigit(Buff[BN+9]) )
	{
	    memcpy(Date, Buff+BN, 10);
	    Date[10] = '\00';
	    if ( (F = DateToDay(Date)) == 0 )
	    {
		Error(BADDEF1, Date, "date");
	    }

	    BN += 10;
	}
	else
	if ( Buff[BN+2] == ':' && Buff[BN+5] == ':' &&
	     isdigit(Buff[BN+1]) &&
	     isdigit(Buff[BN+3]) && isdigit(Buff[BN+4]) &&
	     isdigit(Buff[BN+6]) && isdigit(Buff[BN+7]) )
	{
	    memcpy(Time, Buff+BN, 8);
	    Time[8] = '\00';
	    if ( (F = TimeToSecs(Time)) == 0 )
	    {
		Error(BADDEF1, Time, "time");
	    }

	    BN += 8;
	}
	else
	{
	    F = strtod(Buff+BN, &EndPtr);

	    /*  Check for period after integer  */

	    if ( EndPtr > Buff+BN+1 && *(EndPtr-1) == '.' )
	    {
		EndPtr--;
	    }

	    BN = EndPtr - Buff;
	}

	Dump(OP_NUM, F, Nil, Fi);
    }
    else
    if ( (o = FindOne(Funcs)) >= 0 )
    {
	BN += 3;

	if ( ! Find("(") ) FailSyn("'(' after function name");
	BN++;

	if ( ! Expression() ) FailSyn("expression");

	if ( ! Find(")") ) FailSyn("')' after function argument");
	BN++;

	DumpOp((char)(OP_SIN + o), Fi);
    }
    else
    if ( Buff[BN] == '?' )
    {
	BN++;
	if ( TStack[TSN-1].Type == 'N' )
	{
	    Dump(OP_NUM, _UNK.cval, Nil, Fi);
	}
	else
	{
	    Dump(OP_STR, 0, Nil, Fi);
	}
    }
    else
    if ( ! memcmp(Buff+BN, "N/A", 3) )
    {
	BN += 3;
	if ( TStack[TSN-1].Type == 'N' )
	{
	    Dump(OP_NUM, _NA.cval, Nil, Fi);
	}
	else
	{
	    Dump(OP_STR, 0, strdup("N/A"), Fi);
	}
    }
    else
    {
	return false;
    }

    return true;
}



/*************************************************************************/
/*									 */
/*	Skip spaces and check for specific string			 */
/*									 */
/*************************************************************************/


Boolean Find(String S)
/*      ----  */
{
    if ( Buff[BN] == ' ' ) BN++;

    return ( ! Buff[BN] ? false : ! memcmp(Buff+BN, S, strlen(S)) );
}



/*************************************************************************/
/*									 */
/*	Find one of a zero-terminated list of alternatives		 */
/*									 */
/*************************************************************************/


int FindOne(String *Alt)
/*  -------  */
{
    int	a;

    for ( a = 0 ; Alt[a] ; a++ )
    {
	if ( Find(Alt[a]) ) return a;
    }

    return -1;
}



/*************************************************************************/
/*									 */
/*	Find an attribute name						 */
/*									 */
/*************************************************************************/


Attribute FindAttName()
/*        -----------  */
{
    Attribute	Att, LongestAtt=0;

    ForEach(Att, 1, MaxAtt-1)
    {
	if ( ! Exclude(Att) && Find(AttName[Att]) )
	{
	    if ( ! LongestAtt ||
		 strlen(AttName[Att]) > strlen(AttName[LongestAtt]) )
	    {
		LongestAtt = Att;
	    }
	}
    }

    if ( LongestAtt && ( MaxClass == 1 || ClassThresh ) &&
	 ! strcmp(ClassName[1], AttName[LongestAtt]) )
    {
	Error(BADDEF4, Nil, Nil);
    }

    return LongestAtt;
}



/*************************************************************************/
/*									 */
/*	Error message routines.  Syntax errors come from the		 */
/*	recursive descent parser, semantics errors from the routines	 */
/*	that build up the equivalent polish				 */
/*									 */
/*************************************************************************/


void DefSyntaxError(String Msg)
/*   --------------  */
{
    String	RestOfText;
    int		i=10;

    if ( ! PreviousError )
    {
	RestOfText = Buff + BN;

	/*  Abbreviate text if longer than 12 characters  */

	if ( CharWidth(RestOfText) > 12 )
	{
#ifdef UTF8
	    /*  Find beginning of UTF-8 character  */

	    for ( ; (RestOfText[i] & 0x80) ; i++)
		;
#endif
	    RestOfText[i] = RestOfText[i+1] = '.';
	}

	Error(BADDEF1, RestOfText, Msg);
	PreviousError = true;
    }
}



void DefSemanticsError(int Fi, String Msg, int OpCode)
/*   -----------------  */
{
    char	Exp[1000], XMsg[1000], Op[1000];

    if ( ! PreviousError )
    {
	/*  Abbreviate the input if necessary  */

	if ( BN - Fi > 23 )
	{
	    sprintf(Exp, "%.10s...%.10s", Buff+Fi, Buff+BN-10);
	}
	else
	{
	    sprintf(Exp, "%.*s", BN - Fi, Buff+Fi);
	}

	switch ( OpCode )
	{
	    case OP_AND:	sprintf(Op, "%s", "and"); break;
	    case OP_OR:		sprintf(Op, "%s", "or"); break;
	    case OP_SEQ:
	    case OP_EQ:		sprintf(Op, "%s", "="); break;
	    case OP_SNE:
	    case OP_NE:		sprintf(Op, "%s", "<>"); break;
	    case OP_GT:		sprintf(Op, "%s", ">"); break;
	    case OP_GE:		sprintf(Op, "%s", ">="); break;
	    case OP_LT:		sprintf(Op, "%s", "<"); break;
	    case OP_LE:		sprintf(Op, "%s", "<="); break;
	    case OP_PLUS:	sprintf(Op, "%s", "+"); break;
	    case OP_MINUS:	sprintf(Op, "%s", "-"); break;
	    case OP_UMINUS:	sprintf(Op, "%s", "unary -"); break;
	    case OP_MULT:	sprintf(Op, "%s", "*"); break;
	    case OP_DIV:	sprintf(Op, "%s", "/"); break;
	    case OP_MOD:	sprintf(Op, "%s", "%"); break;
	    case OP_POW:	sprintf(Op, "%s", "^"); break;
	    case OP_SIN:	sprintf(Op, "%s", "sin"); break;
	    case OP_COS:	sprintf(Op, "%s", "cos"); break;
	    case OP_TAN:	sprintf(Op, "%s", "tan"); break;
	    case OP_LOG:	sprintf(Op, "%s", "log"); break;
	    case OP_EXP:	sprintf(Op, "%s", "exp"); break;
	    case OP_INT:	sprintf(Op, "%s", "int");
	}

	sprintf(XMsg, "%s with '%s'", Msg, Op);
	Error(BADDEF2, Exp, XMsg);
	PreviousError = true;
    }
}



/*************************************************************************/
/*									 */
/*	Reverse polish routines.  These use a model of the stack	 */
/*	during expression evaluation to detect type conflicts etc	 */
/*									 */
/*************************************************************************/



void Dump(char OpCode, ContValue F, String S, int Fi)
/*   ----  */
{
    if ( Buff[Fi] == ' ' ) Fi++;

    if ( ! UpdateTStack(OpCode, F, S, Fi) ) return;

    /*  Make sure enough room for this element  */

    if ( DN >= DefSize-1 )
    {
	Realloc(AttDef[MaxAtt], DefSize += 100, DefElt);
    }

    DefOp(AttDef[MaxAtt][DN]) = OpCode;
    if ( OpCode == OP_ATT || OpCode == OP_STR )
    {
	DefSVal(AttDef[MaxAtt][DN]) = S;
    }
    else
    {
	DefNVal(AttDef[MaxAtt][DN]) = F;
    }

    DN++;
}



void DumpOp(char OpCode, int Fi)
/*   ------  */
{
    Dump(OpCode, 0, Nil, Fi);
}



Boolean UpdateTStack(char OpCode, ContValue F, String S, int Fi)
/*      ------------  */
{
    if ( TSN >= TStackSize )
    {
	Realloc(TStack, TStackSize += 50, EltRec);
    }

    switch ( OpCode )
    {
	case OP_ATT:
		TStack[TSN].Type = ( Continuous((long) S) ? 'N' : 'S' );
		break;

	case OP_NUM:
		TStack[TSN].Type = 'N';
		break;

	case OP_STR:
		TStack[TSN].Type = 'S';
		break;

	case OP_AND:
	case OP_OR:
		if ( TStack[TSN-2].Type != 'B' || TStack[TSN-1].Type != 'B' )
		{
		    FailSem("non-logical value");
		}
		TSN -= 2;
		break;

	case OP_EQ:
	case OP_NE:
		if ( TStack[TSN-2].Type != TStack[TSN-1].Type )
		{
		    FailSem("incompatible values");
		}
		TSN -= 2;
		TStack[TSN].Type = 'B';
		break;

	case OP_GT:
	case OP_GE:
	case OP_LT:
	case OP_LE:
		if ( TStack[TSN-2].Type != 'N' || TStack[TSN-1].Type != 'N' )
		{
		    FailSem("non-arithmetic value");
		}
		TSN -= 2;
		TStack[TSN].Type = 'B';
		break;

	case OP_SEQ:
	case OP_SNE:
		if ( TStack[TSN-2].Type != 'S' || TStack[TSN-1].Type != 'S' )
		{
		    FailSem("incompatible values");
		}
		TSN -= 2;
		TStack[TSN].Type = 'B';
		break;

	case OP_PLUS:
	case OP_MINUS:
	case OP_MULT:
	case OP_DIV:
	case OP_MOD:
	case OP_POW:
		if ( TStack[TSN-2].Type != 'N' || TStack[TSN-1].Type != 'N' )
		{
		    FailSem("non-arithmetic value");
		}
		TSN -= 2;
		break;

	case OP_UMINUS:
		if ( TStack[TSN-1].Type != 'N' )
		{
		    FailSem("non-arithmetic value");
		}
		TSN--;
		break;

	case OP_SIN:
	case OP_COS:
	case OP_TAN:
	case OP_LOG:
	case OP_EXP:
	case OP_INT:
		if ( TStack[TSN-1].Type != 'N' )
		{
		    FailSem("non-arithmetic argument");
		}
		TSN--;
    }

    TStack[TSN].Fi = Fi;
    TStack[TSN].Li = BN-1;
    TSN++;

    return true;
}



/*************************************************************************/
/*									 */
/*	Evaluate an implicit attribute for a case			 */
/*									 */
/*************************************************************************/

#define	CUnknownVal(AV)		(AV.cval==_UNK.cval)
#define	DUnknownVal(AV)		(AV.dval==_UNK.dval)
#define DUNA(a)	(DUnknownVal(XStack[a]) || NotApplicVal(XStack[a]))
#define CUNA(a)	(CUnknownVal(XStack[a]) || NotApplicVal(XStack[a]))
#define	C1(x)	(CUNA(XSN-1) ? _UNK.cval : (x))
#define	C2(x)	(CUNA(XSN-1) || CUNA(XSN-2) ? _UNK.cval : (x))
#define	CD2(x)	(CUNA(XSN-1) || CUNA(XSN-2) ? _UNK.dval : (x))
#define	D2(x)	(DUNA(XSN-1) || DUNA(XSN-2) ? _UNK.dval : (x))


AttValue EvaluateDef(Definition D, DataRec Case)
/*       -----------  */
{
    XStackElt	XStack[100];			/* allows 100-level nesting  */
    int		XSN=0, DN, bv1, bv2, Mult;
    double	cv1, cv2;
    String	sv1, sv2;
    Attribute	Att;
    DefElt	DElt;
    AttValue	ReturnVal;

    for ( DN = 0 ; ; DN++)
    {
	switch ( DefOp((DElt = D[DN])) )
	{
	    case OP_ATT:
		    Att = (long) DefSVal(DElt);

		    if ( Continuous(Att) )
		    {
			XStack[XSN++].cval = CVal(Case, Att);
		    }
		    else
		    {
			XStack[XSN++].sval =
			    ( Unknown(Case, Att) && ! NotApplic(Case, Att) ? 0 :
			      AttValName[Att][XDVal(Case, Att)] );
		    }
		    break;

	    case OP_NUM:
		    XStack[XSN++].cval = DefNVal(DElt);
		    break;

	    case OP_STR:
		    XStack[XSN++].sval = DefSVal(DElt);
		    break;

	    case OP_AND:
		    bv1 = XStack[XSN-2].dval;
		    bv2 = XStack[XSN-1].dval;
		    XStack[XSN-2].dval = ( bv1 == 3 || bv2 == 3 ? 3 :
					   D2(bv1 == 2 && bv2 == 2 ? 2 : 3) );
		    XSN--;
		    break;

	    case OP_OR:
		    bv1 = XStack[XSN-2].dval;
		    bv2 = XStack[XSN-1].dval;
		    XStack[XSN-2].dval = ( bv1 == 2 || bv2 == 2 ? 2 :
					   D2(bv1 == 2 || bv2 == 2 ? 2 : 3) );
		    XSN--;
		    break;

	    case OP_EQ:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].dval = ( cv1 == cv2 ? 2 : 3 );
		    XSN--;
		    break;

	    case OP_NE:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].dval = ( cv1 != cv2 ? 2 : 3 );
		    XSN--;
		    break;

	    case OP_GT:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].dval = CD2(cv1 > cv2 ? 2 : 3);
		    XSN--;
		    break;

	    case OP_GE:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].dval = CD2(cv1 >= cv2 ? 2 : 3);
		    XSN--;
		    break;

	    case OP_LT:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].dval = CD2(cv1 < cv2 ? 2 : 3);
		    XSN--;
		    break;

	    case OP_LE:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].dval = CD2(cv1 <= cv2 ? 2 : 3);
		    XSN--;
		    break;

	    case OP_SEQ:
		    sv1 = XStack[XSN-2].sval;
		    sv2 = XStack[XSN-1].sval;
		    XStack[XSN-2].dval =
			( ! sv1 && ! sv2 ? 2 :
			  ! sv1 || ! sv2 ? 3 :
			  ! strcmp(sv1, sv2) ? 2 : 3 );
		    XSN--;
		    break;

	    case OP_SNE:
		    sv1 = XStack[XSN-2].sval;
		    sv2 = XStack[XSN-1].sval;
		    XStack[XSN-2].dval =
			( ! sv1 && ! sv2 ? 3 :
			  ! sv1 || ! sv2 ? 2 :
			  strcmp(sv1, sv2) ? 2 : 3 );
		    XSN--;
		    break;

	    case OP_PLUS:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].cval = C2(cv1 + cv2);
		    XSN--;
		    break;

	    case OP_MINUS:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].cval = C2(cv1 - cv2);
		    XSN--;
		    break;

	    case OP_MULT:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].cval = C2(cv1 * cv2);
		    XSN--;
		    break;

	    case OP_DIV:
		    /*  Note: have to set precision of result  */

		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    if ( ! cv2 ||
			 CUnknownVal(XStack[XSN-2]) ||
			 CUnknownVal(XStack[XSN-1]) ||
			 NotApplicVal(XStack[XSN-2]) ||
			 NotApplicVal(XStack[XSN-1]) )
		    {
			XStack[XSN-2].cval = _UNK.cval;
		    }
		    else
		    {
			Mult = Denominator(cv1);
			cv1 = cv1 / cv2;
			while ( fabs(cv2) > 1 )
			{
			    Mult *= 10;
			    cv2 /= 10;
			}
			XStack[XSN-2].cval = rint(cv1 * Mult) / Mult;
		    }
		    XSN--;
		    break;

	    case OP_MOD:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].cval = C2(fmod(cv1, cv2));
		    XSN--;
		    break;

	    case OP_POW:
		    cv1 = XStack[XSN-2].cval;
		    cv2 = XStack[XSN-1].cval;
		    XStack[XSN-2].cval =
			( CUNA(XSN-1) || CUNA(XSN-2) ||
			  ( cv1 < 0 && ceil(cv2) != cv2 ) ? _UNK.cval :
			  pow(cv1, cv2) );
		    XSN--;
		    break;

	    case OP_UMINUS:
		    cv1 = XStack[XSN-1].cval;
		    XStack[XSN-1].cval = C1(-cv1);
		    break;

	    case OP_SIN:
		    cv1 = XStack[XSN-1].cval;
		    XStack[XSN-1].cval = C1(sin(cv1));
		    break;

	    case OP_COS:
		    cv1 = XStack[XSN-1].cval;
		    XStack[XSN-1].cval = C1(cos(cv1));
		    break;

	    case OP_TAN:
		    cv1 = XStack[XSN-1].cval;
		    XStack[XSN-1].cval = C1(tan(cv1));
		    break;

	    case OP_LOG:
		    cv1 = XStack[XSN-1].cval;
		    XStack[XSN-1].cval =
			( CUNA(XSN-1) || cv1 <= 0 ? _UNK.cval : log(cv1) );
		    break;

	    case OP_EXP:
		    cv1 = XStack[XSN-1].cval;
		    XStack[XSN-1].cval = C1(exp(cv1));
		    break;

	    case OP_INT:
		    cv1 = XStack[XSN-1].cval;
		    XStack[XSN-1].cval = C1(rint(cv1));
		    break;

	    case OP_END:
		    ReturnVal.cval = XStack[0].cval;	/* cval >= dval bytes */
		    return ReturnVal;
	}
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Read variable misclassification costs				 */
/*	-------------------------------------				 */
/*									 */
/*************************************************************************/




void GetMCosts(FILE *Cf)
/*   ---------  */
{
    ClassNo	Pred, Real, p, r;
    char	Name[1000];
    CaseNo	i;
    float	Val, Sum=0;

    LineNo = 0;

    /*  Read entries from cost file  */

    while ( ReadName(Cf, Name, 1000, ':') )
    {
	if ( ! (Pred = Which(Name, ClassName, 1, MaxClass)) )
	{
	    Error(BADCOSTCLASS, Name, "");
	}

	if ( ! ReadName(Cf, Name, 1000, ':') ||
	     ! (Real = Which(Name, ClassName, 1, MaxClass)) )
	{
	    Error(BADCOSTCLASS, Name, "");
	}

	if ( ! ReadName(Cf, Name, 1000, ':') ||
	     sscanf(Name, "%f", &Val) != 1 || Val < 0 )
	{
	    Error(BADCOST, "", "");
	    Val = 1;
	}

	if ( Pred > 0 && Real > 0 && Pred != Real && Val != 1 )
	{
	    /*  Have a non-trivial cost entry  */

	    if ( ! MCost )
	    {
		/*  Set up cost matrices  */

		MCost = Alloc(MaxClass+1, float *);
		ForEach(p, 1, MaxClass)
		{
		    MCost[p] = Alloc(MaxClass+1, float);
		    ForEach(r, 1, MaxClass)
		    {
			MCost[p][r] = ( p == r ? 0.0 : 1.0 );
		    }
		}
	    }

	    MCost[Pred][Real] = Val;
	}
    }
    fclose(Cf);

    /*  Don't need weights etc. for predict or interpret, or
	if not using cost weighting  */

    if ( ! (CostWeights = MaxClass == 2 && MaxCase >= 0 && MCost) )
    {
	return;
    }

    /*  Determine class frequency distribution  */

    ClassFreq = AllocZero(MaxClass+1, double);

    if ( CWtAtt )
    {
	AvCWt = 1;			/* relative weights not yet set */
	ForEach(i, 0, MaxCase)
	{
	    ClassFreq[Class(Case[i])] += RelCWt(Case[i]);
	}
    }
    else
    {
	ForEach(i, 0, MaxCase)
	{
	    ClassFreq[Class(Case[i])]++;
	}
    }

    /*  Find normalised weight multipliers  */

    WeightMul = Alloc(3, float);

    Sum = (ClassFreq[1] * MCost[2][1] + ClassFreq[2] * MCost[1][2]) /
	  (ClassFreq[1] + ClassFreq[2]);

    WeightMul[1] = MCost[2][1] / Sum;
    WeightMul[2] = MCost[1][2] / Sum;

    /*  Adjust MINITEMS to take account of case reweighting  */

    MINITEMS *= Min(WeightMul[1], WeightMul[2]);

    Free(ClassFreq);					ClassFreq = Nil;
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*								  	 */
/*	Routine for printing confusion matrices and attribute usage	 */
/*	-----------------------------------------------------------	 */
/*								  	 */
/*************************************************************************/



void PrintConfusionMatrix(CaseNo *ConfusionMat)
/*   --------------------  */
{
    int		Row, Col, Entry, EntryWidth=10000;

    /*  For more than 20 classes, use summary instead  */

    if ( MaxClass > 20 )
    {
	PrintErrorBreakdown(ConfusionMat);
	return;
    }

    /*  Find maximum entry width in chars  */

    ForEach(Row, 1, MaxClass)
    {
	ForEach(Col, 1, MaxClass)
	{
	    EntryWidth = Max(EntryWidth, ConfusionMat[Row*(MaxClass+1) + Col]);
	}
    }

    EntryWidth = floor(Log(EntryWidth + 100.0) / Log(10.0)) + 2;

    /*  Print the heading, then each row  */

    fprintf(Of, "\n\n\t");
    ForEach(Col, 1, MaxClass)
    {
	fprintf(Of, "%*s(%c)", EntryWidth-3, " ", 'a' + Col-1);
    }

    fprintf(Of, "    <-" T_classified_as "\n\t");
    ForEach(Col, 1, MaxClass)
    {
	fprintf(Of, "%*.*s", EntryWidth, EntryWidth-2, "----------");
    }
    fprintf(Of, "\n");

    ForEach(Row, 1, MaxClass)
    {
	fprintf(Of, "\t");
	ForEach(Col, 1, MaxClass)
	{
	    if ( (Entry = ConfusionMat[Row*(MaxClass+1) + Col]) )
	    {
		fprintf(Of, " %*d", EntryWidth-1, Entry);
	    }
	    else
	    {
		fprintf(Of, "%*s", EntryWidth, " ");
	    }
	}
	fprintf(Of, "    (%c): " T_class " %s\n", 'a' + Row-1, ClassName[Row]);
    }
}



void PrintErrorBreakdown(CaseNo *ConfusionMat)
/*   -------------------  */
{
    CaseNo	*TruePos, *FalsePos, *FalseNeg, Entry;
    int		Row, Col, EntryWidth=100000, ClassWidth=5;

    TruePos  = AllocZero(MaxClass+1, CaseNo);
    FalsePos = AllocZero(MaxClass+1, CaseNo);
    FalseNeg = AllocZero(MaxClass+1, CaseNo);

    ForEach(Row, 1, MaxClass)
    {
	ForEach(Col, 1, MaxClass)
	{
	    Entry = ConfusionMat[Row*(MaxClass+1) + Col];

	    if ( Col == Row )
	    {
		TruePos[Row] += Entry;
	    }
	    else
	    {
		FalseNeg[Row] += Entry;
		FalsePos[Col] += Entry;
	    }
	}

	EntryWidth = Max(EntryWidth, TruePos[Row] + FalseNeg[Row]);
	ClassWidth = Max(ClassWidth, strlen(ClassName[Row]));
    }

    EntryWidth = floor(Log(EntryWidth + 100.0) / Log(10.0)) + 2;

    /*  Print heading (tricky spacing) */

    fprintf(Of, "\n\n\t  %-*s %*s %*s %*s\n\t  %*s %*s %*s %*s\n",
		ClassWidth, "Class",
		EntryWidth, "Cases",
		EntryWidth, "False",
		EntryWidth, "False",
		ClassWidth, "",
		EntryWidth, "",
		EntryWidth, "Pos",
		EntryWidth, "Neg");
    fprintf(Of, "\t  %-*s %*s %*s %*s\n",
		ClassWidth, "-----",
		EntryWidth, "-----",
		EntryWidth, "-----",
		EntryWidth, "-----");

    ForEach(Row, 1, MaxClass)
    {
	fprintf(Of, "\t  %-*s %*d %*d %*d\n",
		ClassWidth, ClassName[Row],
		EntryWidth, TruePos[Row] + FalseNeg[Row],
		EntryWidth, FalsePos[Row],
		EntryWidth, FalseNeg[Row]);
    }

    Free(TruePos);
    Free(FalsePos);
    Free(FalseNeg);
}



void PrintUsageInfo(CaseNo *Usage)
/*   --------------  */
{
    Attribute	Att, Best;
    float	Tests;
    Boolean	First=true;

    Tests = Max(1, MaxCase+1);

    while ( true )
    {
	Best = 0;

	ForEach(Att, 1, MaxAtt)
	{
	    if ( Usage[Att] > Usage[Best] ) Best = Att;
	}

	if ( ! Best || Usage[Best] < 0.01 * Tests ) break;

	if ( First )
	{
	    fprintf(Of, T_Usage);
	    First = false;
	}

	fprintf(Of, "\t%7d%%  %s\n",
	    (int) ((100 * Usage[Best]) / Tests + 0.5), AttName[Best]);

	Usage[Best] = 0;
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Sorting utilities						 */
/*	-----------------						 */
/*									 */
/*************************************************************************/



#define SwapSRec(a,b)	{Xab=SRec[a]; SRec[a]=SRec[b]; SRec[b]=Xab;}

#define SwapSRecWithImplications(a,b)	{Xab=SRec[a]; SRec[a]=SRec[b]; SRec[b]=Xab; \
					 tmp = permutation->entries[a]; \
					 permutation->entries[a] = permutation->entries[b]; \
					 permutation->entries[b] = tmp; \
					}

#define	QuicksortSwapWithImplications(a,b) {\
		DataRec xab;\
		assert(a >= 0 && a <= MaxCase &&\
		b >= 0 && b <= MaxCase);\
		xab = Case[a]; Case[a] = Case[b]; Case[b] = xab;\
		tmp = permutation->entries[a]; \
		permutation->entries[a] = permutation->entries[b]; \
		permutation->entries[b] = tmp; \
		}
		
/*************************************************************************/
/*									 */
/*	Sort elements Fp to Lp of SRec					 */
/*									 */
/*************************************************************************/


void Cachesort(CaseNo Fp, CaseNo Lp, SortRec *SRec)
/*   ---------  */
{
    CaseNo	i, Middle, High;
    ContValue	Thresh, Val;
    SortRec	Xab;


    while ( Fp < Lp )
    {
	Thresh = SRec[(Fp+Lp) / 2].V;

	/*  Divide elements into three groups:
		Fp .. Middle-1: values < Thresh
		Middle .. High: values = Thresh
		High+1 .. Lp:   values > Thresh  */

	for ( Middle = Fp ; SRec[Middle].V < Thresh ; Middle++ )
	    ;

	for ( High = Lp ; SRec[High].V > Thresh ; High-- )
	    ;

	for ( i = Middle ; i <= High ; )
	{
	    if ( (Val = SRec[i].V) < Thresh )
	    {
		SwapSRec(Middle, i);
		Middle++;
		i++;
	    }
	    else
	    if ( Val > Thresh )
	    {
		SwapSRec(High, i);
		High--;
	    }
	    else
	    {
		i++;
	    }
	}

	/*  Sort the first group  */

	Cachesort(Fp, Middle-1, SRec);

	/*  Continue with the last group  */

	Fp = High+1;
    }
}


/*************************************************************************/
/*									 */
/*	Sort elements Fp to Lp of SRec; also record the permutation in	 */
/*	the permutation array.						 */
/*************************************************************************/


void CachesortWithImplications(CaseNo Fp, CaseNo Lp, SortRec *SRec, 
						struct array * permutation)
/*   ---------  */
{
    CaseNo	i, Middle, High;
    ContValue	Thresh, Val;
    SortRec	Xab;
    int 	tmp;

    while ( Fp < Lp )
    {
	Thresh = SRec[(Fp+Lp) / 2].V;

	/*  Divide elements into three groups:
		Fp .. Middle-1: values < Thresh
		Middle .. High: values = Thresh
		High+1 .. Lp:   values > Thresh  */

	for ( Middle = Fp ; SRec[Middle].V < Thresh ; Middle++ )
	    ;

	for ( High = Lp ; SRec[High].V > Thresh ; High-- )
	    ;

	for ( i = Middle ; i <= High ; )
	{
	    if ( (Val = SRec[i].V) < Thresh )
	    {
		SwapSRecWithImplications(Middle, i);
		Middle++;
		i++;
	    }
	    else
	    if ( Val > Thresh )
	    {
		SwapSRecWithImplications(High, i);
		High--;
	    }
	    else
	    {
		i++;
	    }
	}

	/*  Sort the first group  */

	CachesortWithImplications(Fp, Middle-1, SRec, permutation);

	/*  Continue with the last group  */

	Fp = High+1;
    }
}



/*************************************************************************/
/*									 */
/*	Sort cases from Fp to Lp on attribute Att			 */
/*									 */
/*************************************************************************/


void Quicksort(CaseNo Fp, CaseNo Lp, Attribute Att)
/*   ---------  */
{
    CaseNo	i, Middle, High;
    ContValue	Thresh, Val;

    if ( Fp < Lp )
    {
	Thresh = CVal(Case[(Fp+Lp) / 2], Att);

	/*  Divide cases into three groups:
		Fp .. Middle-1: values < Thresh
		Middle .. High: values = Thresh
		High+1 .. Lp:   values > Thresh  */

	for ( Middle = Fp ; CVal(Case[Middle], Att) < Thresh ; Middle++ )
	    ;

	for ( High = Lp ; CVal(Case[High], Att) > Thresh ; High-- )
	    ;

	for ( i = Middle ; i <= High ; )
	{
	    if ( (Val = CVal(Case[i], Att)) < Thresh )
	    {
		Swap(Middle, i);
		Middle++;
		i++;
	    }
	    else
	    if ( Val > Thresh )
	    {
		Swap(High, i);
		High--;
	    }
	    else
	    {
		i++;
	    }
	}

	/*  Sort the first and third groups  */

	Quicksort(Fp, Middle-1, Att);
	Quicksort(High+1, Lp, Att);
    }
}


/*************************************************************************/
/*									 */
/*	Sort cases from Fp to Lp on attribute Att			 */
/*									 */
/*************************************************************************/


void QuicksortWithImplications(CaseNo Fp, CaseNo Lp, Attribute Att, struct array * permutation)
/*   ---------  */
{
    CaseNo	i, Middle, High, tmp;
    ContValue	Thresh, Val;

    if ( Fp < Lp )
    {
	Thresh = CVal(Case[(Fp+Lp) / 2], Att);

	/*  Divide cases into three groups:
		Fp .. Middle-1: values < Thresh
		Middle .. High: values = Thresh
		High+1 .. Lp:   values > Thresh  */

	for ( Middle = Fp ; CVal(Case[Middle], Att) < Thresh ; Middle++ )
	    ;

	for ( High = Lp ; CVal(Case[High], Att) > Thresh ; High-- )
	    ;

	for ( i = Middle ; i <= High ; )
	{
	    if ( (Val = CVal(Case[i], Att)) < Thresh )
	    {
		QuicksortSwapWithImplications(Middle, i);
		Middle++;
		i++;
	    }
	    else
	    if ( Val > Thresh )
	    {
		QuicksortSwapWithImplications(High, i);
		High--;
	    }
	    else
	    {
		i++;
	    }
	}

	/*  Sort the first and third groups  */

	QuicksortWithImplications(Fp, Middle-1, Att, permutation);
	QuicksortWithImplications(High+1, Lp, Att, permutation);
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*		Routines that provide information on progress		 */
/*              ---------------------------------------------		 */
/*									 */
/*************************************************************************/




FILE	*Uf=0;			/* File to which update info written  */


/*************************************************************************/
/*									 */
/*	There are several stages (see messages in Progress() below)	 */
/*	Record stage and open update file if necessary			 */
/*									 */
/*************************************************************************/


void NotifyStage(int S)
/*   -----------  */
{
    Now = S;
    if ( S == 1 )
    {
	if ( ! (Uf = GetFile(".tmp", "w")) ) Error(NOFILE, "", E_ForWrite);
    }
}



/*************************************************************************/
/*									 */
/*	Print progress message.  This routine is called in two ways:	 */
/*	  *  negative Delta = measure of total effort required for stage */
/*	  *  positive Delta = increment since last call			 */
/*									 */
/*************************************************************************/


void Progress(float Delta)
/*   --------  */
{
    static float Total, Current=0;
    static int   Twentieth=0, LastStage=0;
    int		 p;
    static char *Message[]={ "",
			     "Reading training data      ",
			     "Winnowing attributes       ",
			     "Constructing decision tree ",
			     "Simplifying decision tree  ",
			     "Forming rules              ",
			     "Selecting final rules      ",
			     "Evaluating on training data",
			     "Reading test data          ",
			     "Evaluating on test data    ",
			     "Cleaning up                ",
			     "Allocating tables          ",
			     "Preparing results          " },
		Tell[]={ 0,0,0,1,1,1,1,0,0,0,0,0,0 },

		*Done=">>>>>>>>>>>>>>>>>>>>",
		*ToDo="....................";

    if ( LastStage == Now && ! Tell[Now] )
    {
	return;
    }

    LastStage = Now;

    if ( Delta <= -1 )
    {
	Total = -Delta;
	Current = 0;
	Twentieth = -1;
    }
    else
    {
	Current = Min(Total, Current + Delta);
    }

    if ( (p = rint((20.0 * Current) / Total)) != Twentieth )
    {
	Twentieth = p;
assert(p >= 0 && p <= 20);
	fprintf(Uf, "%s", Message[Now]);
	if ( Tell[Now] )
	{
	    fprintf(Uf, "  %s%s  (%d %s)",
			Done + (20 - Twentieth), ToDo + Twentieth,
			(int) (Current+0.5),
			( Now == SIFTRULES ?
			    "refinements" : "cases covered" ));
	}
	fprintf(Uf, "\n");
	fflush(Uf);
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Routines for winnowing attributes				 */
/*	---------------------------------				 */
/*									 */
/*************************************************************************/



float		*AttImp=Nil;		/* att importance */
Boolean		*Split=Nil,		/* atts used in unpruned tree */
		*Used=Nil;		/* atts used in pruned tree */


/*************************************************************************/
/*									 */
/*	Winnow attributes by constructing a tree from half the data.	 */
/*	Remove those that are never used as splits and those that	 */
/*	increase error on the remaining data, and check that the new	 */
/*	error cost does not increase					 */
/*									 */
/*************************************************************************/


void WinnowAtts()
/*   ----------  */
{
    Attribute	Att, Removed=0, Best;
    CaseNo	i, Bp, Ep;
    float	Base;
    Boolean	First=true, *Upper;
    ClassNo	c;
    extern Attribute	*DList;

    /*  Save original case order  */

    SaveCase = Alloc(MaxCase+1, DataRec);
    ForEach(i, 0, MaxCase)
    {
	SaveCase[i] = Case[i];
    }

    /*  Split data into two halves with equal class frequencies  */

    Upper = AllocZero(MaxClass+1, Boolean);

    Bp = 0;
    Ep = MaxCase;
    ForEach(i, 0, MaxCase)
    {
	c = Class(SaveCase[i]);

	if ( Upper[c] )
	{
	    Case[Ep--] = SaveCase[i];
	}
	else
	{
	    Case[Bp++] = SaveCase[i];
	}

	Upper[c] = ! Upper[c];
    }

    Free(Upper);

    /*  Use first 50% of the cases for building a winnowing tree
	and remaining 50% for measuring attribute importance  */

    AttImp = AllocZero(MaxAtt+1, float);
    Split  = AllocZero(MaxAtt+1, Boolean);
    Used   = AllocZero(MaxAtt+1, Boolean);

    Base = TrialTreeCost(true);

    /*  Remove attributes when doing so would reduce error cost  */

    ForEach(Att, 1, MaxAtt)
    {
	if ( AttImp[Att] < 0 )
	{
	    SpecialStatus[Att] ^= SKIP;
	    Removed++;
	}
    }

    /*  If any removed, rebuild tree and reinstate if error increases  */

    if ( Removed && TrialTreeCost(false) > Base )
    {
	ForEach(Att, 1, MaxAtt)
	{
	    if ( AttImp[Att] < 0 )
	    {
		AttImp[Att] = 1;
		SpecialStatus[Att] ^= SKIP;
		Verbosity(1, fprintf(Of, "  re-including %s\n", AttName[Att]))
	    }
	}

	Removed=0;
    }

    /*  Discard unused attributes  */

    ForEach(Att, 1, MaxAtt)
    {
	if ( Att != ClassAtt && ! Skip(Att) && ! Split[Att] )
	{
	    SpecialStatus[Att] ^= SKIP;
	    Removed++;
	}
    }

    /*  Print summary of winnowing  */

    if ( ! Removed )
    {
	fprintf(Of, T_NoWinnow);
    }
    else
    {
	fprintf(Of, T_AttributesWinnowed, Removed, Plural(Removed));

	/*  Print remaining attributes ordered by importance  */

	while ( true )
	{
	    Best = 0;
	    ForEach(Att, 1, MaxAtt)
	    {
		if ( AttImp[Att] >= 1 &&
		     ( ! Best || AttImp[Att] > AttImp[Best] ) )
		{
		    Best = Att;
		}
	    }
	    if ( ! Best ) break;

	    if ( First )
	    {
		fprintf(Of, T_EstImportance);
		First = false;
	    }
	    if ( AttImp[Best] >= 1.005 )
	    {
		fprintf(Of, "%7d%%  %s\n",
			    (int) ((AttImp[Best] - 1) * 100 + 0.5),
			    AttName[Best]);
	    }
	    else
	    {
		fprintf(Of, "     <1%%  %s\n", AttName[Best]);
	    }
	    AttImp[Best] = 0;
	}
    }

    if ( Removed )
    {
	Winnowed = true;

	/*  Reset DList  */

	NDList = 0;
	ForEach(Att, 1, MaxAtt)
	{
	    if ( DFreq[Att] && ! Skip(Att) )
	    {
		DList[NDList++] = Att;
	    }
	}
    }

    /*  Restore case order and clean up  */

    ForEach(i, 0, MaxCase)
    {
	Case[i] = SaveCase[i];
    }

    FreeUnlessNil(SaveCase);				SaveCase = Nil;
    FreeUnlessNil(AttImp);				AttImp = Nil;
    FreeUnlessNil(Split);				Split = Nil;
    FreeUnlessNil(Used);				Used = Nil;

    Now = 0;
}



/*************************************************************************/
/*									 */
/*	Build trial tree and check error cost on remaining data.	 */
/*	If first time, note split attributes and check effect of	 */
/*	removing every attribute					 */
/*									 */
/*************************************************************************/


float TrialTreeCost(Boolean FirstTime)
/*    -------------  */
{
    Attribute	Att;
    float	Base, Cost, SaveMINITEMS;
    CaseNo	SaveMaxCase, Cut;
    int		SaveVERBOSITY;

    Verbosity(1,
	fprintf(Of, ( FirstTime ? "\nWinnow cycle:\n" : "\nCheck:\n" )))

    /*  Build and prune trial tree  */

    SaveMaxCase   = MaxCase;
    SaveVERBOSITY = VERBOSITY;
    SaveMINITEMS  = MINITEMS;
    MINITEMS      = Max(MINITEMS / 2, 2.0);

    Cut = (MaxCase+1) / 2 - 1;

    InitialiseWeights();
    LEAFRATIO = 0;
    VERBOSITY = 0;
    MaxCase   = Cut;

    memset(Tested, 0, MaxAtt+1);		/* reset tested attributes */

    SetMinGainThresh();
	// Daniel: Remove this part of code as it conflicts with the modifications    
	assert(false);
	exit(1);
	//FormTree(0, Cut, 0, &WTree);

    if ( FirstTime )
    {
	/*  Find attributes used in unpruned tree  */

	ScanTree(WTree, Split);
    }

    Prune(WTree);

    VERBOSITY = SaveVERBOSITY;
    MaxCase   = SaveMaxCase;
    MINITEMS  = SaveMINITEMS;

    Verbosity(2,
	PrintTree(WTree, "Winnowing tree:");
	fprintf(Of, "\n  training error cost %g\n", ErrCost(WTree, 0, Cut)))

    Base = ErrCost(WTree, Cut+1, MaxCase);

    Verbosity(1,
	fprintf(Of, "  initial error cost %g\n", Base))

    if ( FirstTime )
    {
	/*  Check each attribute used in pruned tree  */

	ScanTree(WTree, Used);

	ForEach(Att, 1, MaxAtt)
	{

	    if ( ! Used[Att] )
	    {
		Verbosity(1,
		    if ( Att != ClassAtt && ! Skip(Att) )
		    {
			fprintf(Of, "  %s not used\n", AttName[Att]);
		    })

		if ( Split[Att] )
		{
		    AttImp[Att] = 1;
		}

		continue;
	    }

	    /*  Determine error cost if this attribute omitted  */

	    SpecialStatus[Att] ^= SKIP;

	    Cost = ErrCost(WTree, Cut+1, MaxCase);

	    AttImp[Att] = ( Cost < Base ? -1 : Cost / Base );
	    Verbosity(1,
		fprintf(Of, "  error cost without %s = %g%s\n",
			    AttName[Att], Cost,
			    ( Cost < Base ? " - excluded" : "" )))

	    SpecialStatus[Att] ^= SKIP;
	}
    }

    if ( WTree )
    {
	FreeTree(WTree);				WTree = Nil;
    }

    return Base;
}



/*************************************************************************/
/*									 */
/*	Determine the error rate or cost of T on cases Fp through Lp	 */
/*									 */
/*************************************************************************/


float ErrCost(Tree T, CaseNo Fp, CaseNo Lp)
/*    -------  */
{
    CaseNo	i;
    float	ErrCost=0;
    ClassNo	Pred;

    if ( MCost )
    {
	ForEach(i, Fp, Lp)
	{
	    if ( (Pred = TreeClassify(Case[i], T)) != Class(Case[i]) )
	    {
		ErrCost += MCost[Pred][Class(Case[i])];
	    }
	}
    }
    else
    {
	ForEach(i, Fp, Lp)
	{
	    if ( TreeClassify(Case[i], T) != Class(Case[i]) )
	    {
		ErrCost += 1.0;
	    }
	}
    }

    return ErrCost;
}



/*************************************************************************/
/*									 */
/*	Find attributes used in tree T					 */
/*									 */
/*************************************************************************/


void ScanTree(Tree T, Boolean *Used)
/*   --------  */
{
    DiscrValue	v;

    if ( T->NodeType )
    {
	Used[T->Tested] = true;

	ForEach(v, 1, T->Forks)
	{
	    ScanTree(T->Branch[v], Used);
	}
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*                                                              	 */
/*	Determine the class of a case from a decision tree or ruleset	 */
/*                                                              	 */
/*************************************************************************/




	/* Local data used by MarkActive and RuleClassify.
	   Note: Active is never deallocated, just grows as required */

RuleNo	*Active=Nil,	/* rules that fire while classifying case */
	NActive,	/* number ditto */
	ActiveSpace=0;	/* space allocated */



/*************************************************************************/
/*                                                              	 */
/*	Classify a case using a decision tree				 */
/*                                                              	 */
/*************************************************************************/


ClassNo TreeClassify(DataRec Case, Tree DecisionTree)
/*      ------------  */
{
    ClassNo	c;

    ForEach(c, 0, MaxClass)
    {
	ClassSum[c] = 0;
    }

    FindLeaf(Case, DecisionTree, Nil, 1.0);

    return SelectClass(1, (Boolean)(MCost != Nil));
}



/*************************************************************************/
/*                                                              	 */
/*	Classify a case using the given subtree.			 */
/*	Adjust the value ClassSum for each class			 */
/*                                                              	 */
/*************************************************************************/


void FindLeaf(DataRec Case, Tree T, Tree PT, float Fraction)
/*   --------  */
{
    DiscrValue	v, Dv;
    ClassNo	c;
    float	NewFrac, BrWt[4];

    /*  Special case for winnowing cycles  */

    if ( T->NodeType && Skip(T->Tested) )
    {
	FollowAllBranches(Case, T, Fraction);
	return;
    }

    if ( T->NodeType && Tested )
    {
	Tested[T->Tested] = true;	/* for usage */
    }

    switch ( T->NodeType )
    {
	case 0:  /* leaf */

	  LeafUpdate:

	    /*  Use parent node if effectively no cases at this node  */

	    // Pranav: Handle the case when the leaf has only data points with UNKNOWN class value.
	    // 	       Cases = 0 in this case and we want the tre to predict based on the Class value of this node.
	    //if ( T->Cases < Epsilon )
	    if ( T->Cases < 0 )
	    {
		T = PT;
	    }

	    /*  Update from all classes  */

	    ForEach(c, 1, MaxClass)
	    {
		ClassSum[c] += Fraction * T->ClassDist[c] / T->Cases;
	    }

	    return;

	case BrDiscr:  /* test of discrete attribute */

	    Dv = DVal(Case, T->Tested);	/* > MaxAttVal if unknown */

	    if ( Dv <= T->Forks )	/*  Make sure not new discrete value  */
	    {
		FindLeaf(Case, T->Branch[Dv], T, Fraction);
	    }
	    else
	    {
		FollowAllBranches(Case, T, Fraction);
	    }

	    return;

	case BrThresh:  /* test of continuous attribute */

	    if ( Unknown(Case, T->Tested) )
	    {
		FollowAllBranches(Case, T, Fraction);
	    }
	    else
	    if ( NotApplic(Case, T->Tested) )
	    {
		FindLeaf(Case, T->Branch[1], T, Fraction);
	    }
	    else
	    {
		/*  Find weights for <= and > branches, interpolating if
		    probabilistic thresholds are used  */

		BrWt[2] = Interpolate(T, CVal(Case, T->Tested));
		BrWt[3] = 1 - BrWt[2];

		ForEach(v, 2, 3)
		{
		    if ( (NewFrac = Fraction * BrWt[v]) >= 0.01 )
		    {
			FindLeaf(Case, T->Branch[v], T, NewFrac);
		    }
		}
	    }

	    return;

	case BrSubset:  /* subset test on discrete attribute  */

	    Dv = DVal(Case, T->Tested);	/* > MaxAttVal if unknown */

	    if ( Dv <= MaxAttVal[T->Tested] )
	    {
		ForEach(v, 1, T->Forks)
		{
		    if ( In(Dv, T->Subset[v]) )
		    {
			FindLeaf(Case, T->Branch[v], T, Fraction);

			return;
		    }
		}

		/* Value not found in any subset -- treat as leaf  */

		goto LeafUpdate;
	    }
	    else
	    {
		FollowAllBranches(Case, T, Fraction);
	    }
    }
}



/*************************************************************************/
/*                                                              	 */
/*	Follow all branches from a node, weighting them in proportion	 */
/*	to the number of training cases they contain			 */
/*                                                              	 */
/*************************************************************************/


void FollowAllBranches(DataRec Case, Tree T, float Fraction)
/*   -----------------  */
{
    DiscrValue	v;

    ForEach(v, 1, T->Forks)
    {
	if ( T->Branch[v]->Cases > Epsilon )
	{
	    FindLeaf(Case, T->Branch[v], T,
		     (Fraction * T->Branch[v]->Cases) / T->Cases);
	}
    }
}



/*************************************************************************/
/*                                                              	 */
/*	Classify a case using a ruleset					 */
/*                                                              	 */
/*************************************************************************/


ClassNo RuleClassify(DataRec Case, CRuleSet RS)
/*      ------------  */
{
    ClassNo	c, Best;
    float	TotWeight=0;
    int		a, u=1, d;
    CRule	R;
    RuleNo	r;

    ForEach(c, 0, MaxClass)
    {
	ClassSum[c] = 0;
	MostSpec[c] = Nil;
    }

    /*  Find active rules  */

    NActive = 0;

    if ( RS->RT )
    {
	MarkActive(RS->RT, Case);
    }
    else
    {
	ForEach(r, 1, RS->SNRules)
	{
	    R = RS->SRule[r];

	    if ( Matches(R, Case) )
	    {
		Active[NActive++] = r;
	    }
	}
    }

    /*  Must sort rules if using utility bands  */

    if ( UtilBand )
    {
	SortActive();
    }

    /*  Vote active rules  */

    ForEach(a, 0, NActive-1)
    {
	r = Active[a];
	R = RS->SRule[r];

	if ( Tested )
	{
	    ForEach(d, 1, R->Size)
	    {
		Tested[R->Lhs[d]->Tested] = true;	/* for usage */
	    }
	}
	if ( UtilBand ) CheckUtilityBand(&u, r, Class(Case), RS->SDefault);
	ClassSum[R->Rhs] += R->Vote;
	TotWeight        += 1000.0;

	/*  Check whether this is the most specific rule for this class;
	    resolve ties in favor of rule with higher vote  */

	if ( ! MostSpec[R->Rhs] ||
	     R->Cover < MostSpec[R->Rhs]->Cover ||
	     ( R->Cover == MostSpec[R->Rhs]->Cover &&
	       R->Vote > MostSpec[R->Rhs]->Vote ) )
	{
	    MostSpec[R->Rhs] = R;
	}
    }

    /*  Flush any remaining utility bands  */

    if ( UtilBand )
    {
	CheckUtilityBand(&u, RS->SNRules+1, Class(Case), RS->SDefault);
    }

    /*  Check for default and normalise ClassSum  */

    if ( ! TotWeight )
    {
	Confidence = 0.5;
	return RS->SDefault;
    }

    ForEach(c, 1, MaxClass)
    {
	ClassSum[c] /= TotWeight;
    }

    Best = SelectClass(RS->SDefault, false);

    /*  Set Confidence to the vote for the most specific rule of class Best  */

    Confidence = MostSpec[Best]->Vote / 1000.0;

    return Best;
}



/*************************************************************************/
/*                                                              	 */
/*	Determine outcome of a test on a case.				 */
/*	Return -1 if value of tested attribute is unknown		 */
/*                                                              	 */
/*************************************************************************/


int FindOutcome(DataRec Case, Condition OneCond)
/*  -----------  */
{
    DiscrValue  v, Outcome;
    Attribute	Att;

    Att = OneCond->Tested;

    /*  Determine the outcome of this test on this case  */

    switch ( OneCond->NodeType )
    {
	case BrDiscr:  /* test of discrete attribute */

	    v = XDVal(Case, Att);
	    Outcome = ( v == 0 ? -1 : v );
	    break;

	case BrThresh:  /* test of continuous attribute */

	    Outcome = ( Unknown(Case, Att) ? -1 :
			NotApplic(Case, Att) ? 1 :
			CVal(Case, Att) <= OneCond->Cut ? 2 : 3 );
	    break;

	case BrSubset:  /* subset test on discrete attribute  */

	    v = XDVal(Case, Att);
	    Outcome = ( v <= MaxAttVal[Att] && In(v, OneCond->Subset) ?
			OneCond->TestValue : 0 );
    }

    return Outcome;
}



/*************************************************************************/
/*									 */
/*	Determine whether a case satisfies a condition			 */
/*									 */
/*************************************************************************/


Boolean Satisfies(DataRec Case, Condition OneCond)
/*      ---------  */
{
    return ( FindOutcome(Case, OneCond) == OneCond->TestValue );
}



/*************************************************************************/
/*									 */
/*	Determine whether a case satisfies all conditions of a rule	 */
/*									 */
/*************************************************************************/


Boolean Matches(CRule R, DataRec Case)
/*      -------  */
{
    int d;

    ForEach(d, 1, R->Size)
    {
	if ( ! Satisfies(Case, R->Lhs[d]) )
	{
	    return false;
	}
    }

    return true;
}



/*************************************************************************/
/*									 */
/*	Make sure that Active[] has space for at least N rules		 */
/*									 */
/*************************************************************************/


void CheckActiveSpace(int N)
/*   ----------------  */
{
    if ( ActiveSpace <= N )
    {
	Realloc(Active, (ActiveSpace=N+1), RuleNo);
    }
}



/*************************************************************************/
/*									 */
/*	Use RT to enter active rules in Active[]			 */
/*									 */
/*************************************************************************/


void MarkActive(RuleTree RT, DataRec Case)
/*   ----------  */
{
    DiscrValue	v;
    int		ri;
    RuleNo	r;

    if ( ! RT ) return;

    /*  Enter any rules satisfied at this node  */

    if ( RT->Fire )
    {
	for ( ri = 0 ; (r = RT->Fire[ri]) ; ri++ )
	{
	    Active[NActive++] = r;
	}
    }

    if ( ! RT->Branch ) return;

    /*  Explore subtree for rules that include condition at this node  */

    if ( (v = FindOutcome(Case, RT->CondTest)) > 0 && v <= RT->Forks )
    {
	MarkActive(RT->Branch[v], Case);
    }

    /*  Explore default subtree for rules that do not include condition  */

    MarkActive(RT->Branch[0], Case);
}



/*************************************************************************/
/*									 */
/*	Sort active rules for utility band error rates			 */
/*									 */
/*************************************************************************/


void SortActive()
/*   ----------  */
{
    RuleNo	r;
    int		a, aa, aLow;

    ForEach(a, 0, NActive-1)
    {
	aLow = a;

	ForEach(aa, a+1, NActive-1)
	{
	    if ( Active[aa] < Active[aLow] ) aLow = aa;
	}

	r = Active[a];
	Active[a] = Active[aLow];
	Active[aLow] = r;
    }
}



/*************************************************************************/
/*									 */
/*	Update utility band error rates for all bands before rule r	 */
/*	that have not been competed yet.  Update current band.		 */
/*									 */
/*************************************************************************/


void CheckUtilityBand(int *u, RuleNo r, ClassNo Actual, ClassNo Default)
/*   ----------------  */
{
    ClassNo	c;

    while ( *u < UTILITY && r > UtilBand[*u] )
    {
	c = SelectClass(Default, false);
	if ( c != Actual )
	{
	    UtilErr[*u]++;
	    if ( MCost ) UtilCost[*u] += MCost[c][Actual];
	}

	(*u)++;
    }
}



/*************************************************************************/
/*									 */
/*	Classify a case using boosted tree or rule sequence.		 */
/*	Global variable Default must have been set prior to call	 */
/*									 */
/*	Note: boosting with costs is complicated.  With trees,		 */
/*	complete class distributions are accumulated and then a class	 */
/*	selected to minimize expected cost.  This cannot be done with	 */
/*	rulesets since a single ruleset does not give a reliable	 */
/*	class distribution; instead, the votes from all cost-adjusted	 */
/*	rulesets are combined without reference to costs.		 */
/*									 */
/*************************************************************************/


ClassNo BoostClassify(DataRec Case, int MaxTrial)
/*	-------------  */
{
    ClassNo	c, Best;
    int		t;
    float	Total=0;

    ForEach(c, 1, MaxClass)
    {
	Vote[c] = 0;
    }

    ForEach(t, 0, MaxTrial)
    {
	Best = ( RULES ? RuleClassify(Case, RuleSet[t]) :
			 TreeClassify(Case, Pruned[t]) );

	Vote[Best] += Confidence;
	Total += Confidence;

	TrialPred[t] = Best;
    }

    /*  Copy votes into ClassSum  */

    ForEach(c, 1, MaxClass)
    {
	ClassSum[c] = Vote[c] / Total;
    }

    return SelectClass(Default, false);
}



/*************************************************************************/
/*									 */
/*	Select the best class to return.  Take misclassification costs	 */
/*	into account if they are defined.				 */
/*									 */
/*************************************************************************/


ClassNo SelectClass(ClassNo Default, Boolean UseCosts)
/*      -----------  */
{
    ClassNo	c, cc, BestClass;
    float	ExpCost, BestCost=1E38, TotCost=0;

    BestClass = Default;

    if ( UseCosts )
    {
	ForEach(c, 1, MaxClass)
	{
	    ExpCost = 0;
	    ForEach(cc, 1, MaxClass)
	    {
		if ( cc == c ) continue;
		ExpCost += ClassSum[cc] * MCost[c][cc];
	    }

	    TotCost += ExpCost;

	    if ( ExpCost < BestCost )
	    {
		BestClass = c;
		BestCost  = ExpCost;
	    }
	}

	Confidence = 1 - BestCost / TotCost;
    }
    else
    {
	ForEach(c, 1, MaxClass)
	{
	    if ( ClassSum[c] > ClassSum[BestClass] ) BestClass = c;
	}

	Confidence = ClassSum[BestClass];
    }

    return BestClass;
}



/*************************************************************************/
/*								   	 */
/*	General classification routine					 */
/*								   	 */
/*************************************************************************/


ClassNo Classify(DataRec Case)
/*      --------  */
{

    return ( TRIALS > 1 ? BoostClassify(Case, TRIALS-1) :
	     RULES ?	  RuleClassify(Case, RuleSet[0]) :
			  TreeClassify(Case, Pruned[0]) );
}



/*************************************************************************/
/*								   	 */
/*	Interpolate a single value between Lower, Mid and Upper		 */
/*	(All these have the same value unless using probabilistic	 */
/*	thresholds.)							 */
/*								   	 */
/*************************************************************************/


float Interpolate(Tree T, ContValue Val)
/*    -----------  */
{
    return ( Val <= T->Lower ? 1.0 :
	     Val >= T->Upper ? 0.0 :
	     Val <= T->Mid ?
		1 - 0.5 * (Val - T->Lower) / (T->Mid - T->Lower + 1E-6) :
		0.5 - 0.5 * (Val - T->Mid) / (T->Upper - T->Mid + 1E-6) );
}



/*************************************************************************/
/*									 */
/*	Free data structures for one classifier				 */
/*									 */
/*************************************************************************/


void FreeClassifier(int Trial)
/*   --------------  */
{
    if ( Raw )
    {
	FreeTree(Raw[Trial]);				Raw[Trial] = Nil;
    }

    if ( Pruned )
    {
	FreeTree(Pruned[Trial]);			Pruned[Trial] = Nil;
    }

    if ( RULES && RuleSet && RuleSet[Trial] )
    {
	FreeRules(RuleSet[Trial]);			RuleSet[Trial] = Nil;
    }
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*								  	 */
/*	Form a set of rules from a decision tree			 */
/*	----------------------------------------			 */
/*								  	 */
/*	The cases are partitioned into sublists:			 */
/*	  * Fail0: those cases that satisfy all undeleted conditions	 */
/*	  * Fail1: those that satisfy all but one of the above		 */
/*	  * FailMany: the remaining cases				 */
/*	Lists are implemented via Succ; Succ[i] is the number of the	 */
/*	case that follows case i.					 */
/*									 */
/*************************************************************************/



double		*Errors=Nil,		/* [Condition] */
		*Total=Nil;		/* [Condition] */

float		*Pessimistic=Nil,	/* [Condition] */
		*CondCost=Nil;		/* [Condition] */

Boolean		**CondFailedBy=Nil,	/* [Condition][CaseNo] */
		*Deleted=Nil;		/* [Condition] */

Condition	*Stack=Nil;

int		MaxDepth=0,		/* depth of tree */
		NCond,
		Bestd;

ClassNo		TargetClass;

short		*NFail=Nil,		/* NFail[i] = conditions failed by i */
		*LocalNFail=Nil;	/* copy used during rule pruning */

CaseNo		Fail0,
		Fail1,
		FailMany,
		*Succ=Nil;		/* case following case i */



/*************************************************************************/
/*								  	 */
/*	Process a tree to extract a ruleset				 */
/*								  	 */
/*************************************************************************/


CRuleSet FormRules(Tree T)
    /*	 ---------  */
{
    int		i;
    CRuleSet	RS;

    NotifyStage(FORMRULES);
    Progress(-(MaxCase+1.0));

    Verbosity(2, PrintTree(T, "Pruned tree:"))

    /*  Find essential parameters and allocate storage  */

    MaxDepth = TreeDepth(T);

    Errors	 = AllocZero(MaxDepth+2, double);
    Total	 = AllocZero(MaxDepth+2, double);

    Pessimistic	 = AllocZero(MaxDepth+2, float);
    CondCost	 = AllocZero(MaxDepth+2, float);

    CondFailedBy = AllocZero(MaxDepth+2, Boolean *);
    Deleted	 = AllocZero(MaxDepth+2, Boolean);

    Stack	 = AllocZero(MaxDepth+2, Condition);

    ForEach(i, 0, MaxDepth+1)
    {
	Stack[i]	= Alloc(1, CondRec);
	CondFailedBy[i] = AllocZero(MaxCase+1, Boolean);
    }

    NFail	 = AllocZero(MaxCase+1, short);
    LocalNFail	 = AllocZero(MaxCase+1, short);

    CovBy	 = AllocZero(MaxCase+2, int);

    List	 = Alloc(MaxCase+2, CaseNo);
    Succ	 = Alloc(MaxCase+1, CaseNo);

    CBuffer	 = Alloc(4 + (MaxCase+1) + (MaxCase+1)/128, Byte);

    NRules = RuleSpace = 0;
    FindClassFreq(ClassFreq, 0, MaxCase);

    if ( ! BranchBits )
    {
	GenerateLogs(Max(MaxCase+1, Max(MaxAtt, Max(MaxClass, MaxDiscrVal))));
	FindTestCodes();
    }

    SetupNCost();

    /*  Extract and prune paths from root to leaves  */

    NCond = 0;
    Scan(T);

    Default = T->Leaf;

    /*  Deallocate storage  */

    FreeFormRuleData();

    /*  Select final rules  */

    SiftRules((T->Errors + MaxClass-1) / (MaxCase+1 + MaxClass));

    FreeVector((void **) NCost, 0, MaxClass);		NCost = Nil;

    CheckActiveSpace(NRules);

    RS = Alloc(1, RuleSetRec);

    RS->SNRules  = NRules;
    RS->SRule    = Rule;				Rule = Nil;
    RS->SDefault = Default;

    ConstructRuleTree(RS);

    return RS;
}



/*************************************************************************/
/*								  	 */
/*	Set up normalised costs.  These are all 0/1 if MCost is not	 */
/*	defined or if cost weighting is used.  Otherwise, MCost is	 */
/*	divided by an estimated average error cost, determined as	 */
/*	follows:							 */
/*									 */
/*	Assume E errors.  The expected number of cases misclassified	 */
/*	as class C is E * P(C), with the real classes distributed	 */
/*	in accordance with their priors.  This gives an expected	 */
/*	total error cost of						 */
/*	    E * sum/C { P(C) * sum/D!=C { P(D)/(1-P(C)) * M[C][D] } }	 */
/*	and dividing by E gives an expected average cost.		 */
/*									 */
/*	The above tends to be pessimistic, so we reduce it somewhat.	 */
/*								  	 */
/*	Siftrules requires a row of NCost corresponding to predicted	 */
/*	class 0 (case not covered by any rule).  All costs in this row	 */
/*	are set to 1.							 */
/*								  	 */
/*************************************************************************/


void SetupNCost()
/*   ----------  */
{
    ClassNo	Real, Pred;
    double	AvErrCost=0, ProbPred, ProbReal;

    NCost = Alloc(MaxClass+1, float *);

    ForEach(Pred, 0, MaxClass)
    {
	NCost[Pred] = Alloc(MaxClass+1, float);

	if ( ! MCost || CostWeights || Pred == 0 )
	{
	    ForEach(Real, 1, MaxClass)
	    {
		NCost[Pred][Real] = ( Pred != Real );
	    }
	}
	else
 	{
	    ProbPred = ClassFreq[Pred] / (MaxCase+1);
	    ForEach(Real, 1, MaxClass)
	    {
		NCost[Pred][Real] = MCost[Pred][Real];
		if ( Real == Pred ) continue;

		ProbReal = ClassFreq[Real] / (MaxCase+1);
		AvErrCost +=
		    ProbPred * (ProbReal / (1 - ProbPred)) * MCost[Pred][Real];
	    }
	}
    }

    if ( MCost && ! CostWeights )
    {
	AvErrCost = (AvErrCost + 1) / 2;	/* reduced average cost */
	ForEach(Real, 1, MaxClass)
	{
	    ForEach(Pred, 1, MaxClass)
	    {
		NCost[Pred][Real] /= AvErrCost;
	    }
	}
    }
}



/*************************************************************************/
/*								  	 */
/*	Extract paths from tree T and prune them to form rules		 */
/*								  	 */
/*************************************************************************/


void Scan(Tree T)
/*   ----  */
{
    DiscrValue	v, Last;
    Condition	Term;

    if ( T->NodeType )
    {
	NCond++;
	Term = Stack[NCond];

	Term->NodeType = T->NodeType;
	Term->Tested   = T->Tested;
	Term->Cut      = T->Cut;

	ForEach(v, 1, T->Forks)
	{
	    /*  Skip branches with empty leaves  */

	    if ( T->Branch[v]->Cases < MinLeaf ) continue;

	    Term->TestValue = v;

	    if ( T->NodeType == BrSubset )
	    {
		if ( Elements(T->Tested, T->Subset[v], &Last) == 1 )
		{
		    /*  Subset contains a single element  */

		    Term->NodeType  = BrDiscr;
		    Term->TestValue = Last;
		}
		else
		{
		    Term->NodeType  = BrSubset;
		    Term->Subset    = T->Subset[v];
		    Term->TestValue = 1;
		}
	    }

	    CondCost[NCond] = CondBits(Term);

	    /*  Adjust number of failed conditions  */

	    PushCondition();

	    Scan(T->Branch[v]);

	    /*  Reset number of failed conditions  */

	    PopCondition();
	}

	NCond--;
    }

    /*  Make a rule from every node of the tree other than the root  */

    if ( NCond > 0 && T->Cases >= 1 )
    {

	memcpy(LocalNFail, NFail, (MaxCase + 1) * sizeof(short));

	TargetClass = T->Leaf;
	PruneRule(Stack, T->Leaf);

	if ( ! T->NodeType ) Progress(T->Cases);
    }
}



/*************************************************************************/
/*								  	 */
/*	Update NFail when a condition is added to/removed from Stack	 */
/*								  	 */
/*************************************************************************/


void PushCondition()
/*   -------------  */
{
    int i;

    ForEach(i, 0, MaxCase)
    {
	if ( (CondFailedBy[NCond][i] = ! Satisfies(Case[i], Stack[NCond])) )
	{
	    NFail[i]++;
	}
    }
}



void PopCondition()
/*   -------------  */
{
    int i;

    ForEach(i, 0, MaxCase)
    {
	if ( CondFailedBy[NCond][i] )
	{
	    NFail[i]--;
	}
    }
}



/*************************************************************************/
/*									 */
/*	Prune the rule given by the conditions Cond, and the number of	 */
/*	conditions NCond, and add the resulting rule to the current	 */
/*	ruleset if it is sufficiently accurate				 */
/*									 */
/*************************************************************************/

#define TI(a,b)		(((a)+(b)) * Log((a)+(b)) - (a) * Log(a) - (b) * Log(b))


void PruneRule(Condition Cond[], ClassNo TargetClass)
/*   ---------  */
{
    int		d, id, Bestid, Remaining=NCond;
    double	RealTotal, RealCorrect;
    CaseNo	i, LL=0;
    float	Prior;
    double	Base, Gain, Cost=0;

    ForEach(d, 0, NCond)
    {
	Deleted[d] = false;
	Total[d]   =
	Errors[d]  = 0;

	if ( d ) Cost += CondCost[d];
    }
    Cost -= LogFact[NCond];

    Base = TI(ClassFreq[TargetClass], MaxCase+1 - ClassFreq[TargetClass]);

    /*  Initialise all fail lists  */

    Bestd = 0;
    ProcessLists();

    ForEach(d, 1, NCond)
    {
	Total[d]  += Total[0];
	Errors[d] += Errors[0];
    }

    /*  Find conditions to delete  */

    Verbosity(1, fprintf(Of, "\n  Pruning rule for %s", ClassName[TargetClass]))

    while (true )
    {
	/*  Find the condition, deleting which would most improve
	    the pessimistic accuracy of the rule.
	    Note: d = 0 means all conditions are satisfied  */

	Bestd = id = 0;

	Gain = Base - TI(Total[0]-Errors[0], Errors[0])
		    - TI(ClassFreq[TargetClass]-Total[0]+Errors[0],
			 MaxCase+1-ClassFreq[TargetClass]-Errors[0]);

	Verbosity(1,
	    fprintf(Of, "\n       Err   Used   Pess\tAbsent condition\n"))

	ForEach(d, 0, NCond)
	{
	    if ( Deleted[d] ) continue;

	    if ( Errors[d] > Total[d] ) Errors[d] = Total[d];

	    Pessimistic[d] = ( Total[d] < Epsilon ? 0.5 :
			       (Errors[d] + 1) / (Total[d] + 2.0) );

	    Verbosity(1,
		fprintf(Of, "   %7.1f%7.1f  %4.1f%%",
		       Errors[d], Total[d], 100 * Pessimistic[d]))

	    if ( ! d )
	    {
		Verbosity(1,
		    fprintf(Of, "\t<base> %.1f/%.1f bits\n", Gain, Cost))
	    }
	    else
	    {
		id++;

		Verbosity(1, PrintCondition(Cond[d]))

		/*  Bestd identifies the condition with lowest pessimistic
		    error  estimate  */

		if ( ! Bestd || Pessimistic[d] <= Pessimistic[Bestd] )
		{
		    Bestd  = d;
		    Bestid = id;
		}
	    }
	}

	if ( Remaining == 1 || ! Bestd || 
	     ( THEORYFRAC * Cost <= Gain &&
	       Pessimistic[Bestd] > Pessimistic[0] ) )
	{
	    break;
	}

	Verbosity(1, fprintf(Of, "\teliminate test %d\n", Bestid))

	Deleted[Bestd] = true;
	Remaining--;
	Cost -= CondCost[Bestd] - LogFact[Remaining+1] + LogFact[Remaining];

	ForEach(d, 1, NCond)
	{
	    if ( d != Bestd )
	    {
		Total[d]  += Total[Bestd] - Total[0];
		Errors[d] += Errors[Bestd] - Errors[0];
	    }
	}
	Total[0]  = Total[Bestd];
	Errors[0] = Errors[Bestd];

	ProcessLists();
    }

    if ( Remaining && Total[0] > 0.99 && THEORYFRAC * Cost <= Gain )
    {
	Prior = ClassFreq[TargetClass] / (MaxCase+1.0);

	/*  Find list of cases covered by this rule and adjust coverage
	    if using costs  */

	if ( ! MCost )
	{
	    RealTotal   = Total[0];
	    RealCorrect = Total[0] - Errors[0];

	    for ( i = Fail0 ; i >= 0 ; i = Succ[i] )
	    {
		List[++LL] = i;
	    }
	}
	else
	if ( CostWeights )
	{
	    /*  Adjust distributions to reverse case weighting  */

	    Prior /= WeightMul[TargetClass];

	    RealTotal = 0;
	    for ( i = Fail0 ; i >= 0 ; i = Succ[i] )
	    {
		RealTotal += Weight(Case[i]) / WeightMul[Class(Case[i])];
		List[++LL] = i;
	    }
	    RealCorrect = (Total[0] - Errors[0]) / WeightMul[TargetClass];
	}
	else
	{
	    /*  Errors have been weighted by NCost -- undo  */

	    RealTotal   = Total[0];
	    RealCorrect = 0;
	    for ( i = Fail0 ; i >= 0 ; i = Succ[i] )
	    {
		RealCorrect += Weight(Case[i]) *
			       (Class(Case[i]) == TargetClass);
		List[++LL] = i;
	    }
	}
	List[0] = LL;

	if ( (RealCorrect + 1) / ((RealTotal + 2) * Prior) >= 0.95 )
	{
	    NewRule(Cond, NCond, TargetClass, Deleted, Nil,
		    RealTotal, RealCorrect, Prior);
	}
    }
}



/*************************************************************************/
/*								  	 */
/*	Change Fail0, Fail1, and FailMany.				 */
/*	If Bestd has not been set, initialise the lists; otherwise	 */
/*	record the changes for deleting condition Bestd and reduce	 */
/*	LocalNFail for cases that do not satisfy condition Bestd	 */
/*								  	 */
/*************************************************************************/


void ProcessLists()
/*   ------------  */
{
    CaseNo	i, iNext, *Prev;
    int		d;

    if ( ! Bestd )
    {
	/*  Initialise the fail list */

	Fail0 = Fail1 = FailMany = -1;

	ForEach(i, 0, MaxCase)
	{
	    if ( ! LocalNFail[i] )
	    {
		Increment(0, i, Total, Errors);
		AddToList(&Fail0, i);
	    }
	    else
	    if ( LocalNFail[i] == 1 )
	    {
		d = SingleFail(i);
		Increment(d, i, Total, Errors);
		AddToList(&Fail1, i);
	    }
	    else
	    {
		AddToList(&FailMany, i);
	    }
	}
    }
    else
    {
	/*  Change the fail list to remove condition Bestd  */

	/*  Promote cases from Fail1 to Fail0  */

	Prev = &Fail1;

	for ( i = Fail1 ; i >= 0 ; )
	{
	    iNext = Succ[i];
	    if ( CondFailedBy[Bestd][i] )
	    {
		DeleteFromList(Prev, i);
		AddToList(&Fail0, i);
	    }
	    else
	    {
		Prev = &Succ[i];
	    }
	    i = iNext;
	}

	/*  Check cases in FailMany  */

	Prev = &FailMany;

	for ( i = FailMany ; i >= 0 ; )
	{
	    iNext = Succ[i];
	    if ( CondFailedBy[Bestd][i] && --LocalNFail[i] == 1 )
	    {
		d = SingleFail(i);
		Increment(d, i, Total, Errors);

		DeleteFromList(Prev, i);
		AddToList(&Fail1, i);
	    }
	    else
	    {
		Prev = &Succ[i];
	    }
	    i = iNext;
	}
    }
}



/*************************************************************************/
/*								  	 */
/*	Add case to list whose first case is *List			 */
/*								  	 */
/*************************************************************************/


void AddToList(CaseNo *List, CaseNo N)
/*   ---------  */
{
    Succ[N] = *List;
    *List   = N;
}



/*************************************************************************/
/*								  	 */
/*	Delete case from list where previous case is *Before		 */
/*								  	 */
/*************************************************************************/


void DeleteFromList(CaseNo *Before, CaseNo N)
/*   --------------  */
{
    *Before = Succ[N];
}



/*************************************************************************/
/*								  	 */
/*	Find single condition failed by a case				 */
/*								  	 */
/*************************************************************************/


int SingleFail(CaseNo i)
/*  ----------  */
{
    int		d;

    ForEach(d, 1, NCond)
    {
	if ( ! Deleted[d] && CondFailedBy[d][i] ) return d;
    }

    return 0;
}



/*************************************************************************/
/*								  	 */
/*	Case i covers all conditions except d; update Total and Errors	 */
/*								  	 */
/*************************************************************************/


void Increment(int d, CaseNo i, double *Total, double *Errors)
/*   ---------  */
{
    Total[d] += Weight(Case[i]);
    Errors[d]+= Weight(Case[i]) * NCost[TargetClass][Class(Case[i])];
}






/*************************************************************************/
/*								  	 */
/*	Free all data allocated for forming rules			 */
/*								  	 */
/*************************************************************************/


void FreeFormRuleData()
/*   ----------------  */
{
    if ( ! CondFailedBy ) return;

    FreeVector((void **) CondFailedBy, 0, MaxDepth+1);	CondFailedBy = Nil;
    FreeVector((void **) Stack, 0, MaxDepth+1);		Stack = Nil;
    Free(Deleted);					Deleted = Nil;
    Free(Pessimistic);					Pessimistic = Nil;
    Free(CondCost);					CondCost = Nil;
    Free(Total);					Total = Nil;
    Free(Errors);					Errors = Nil;
    Free(NFail);					NFail = Nil;
    Free(LocalNFail);					LocalNFail = Nil;
    Free(Succ);						Succ = Nil;
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Get names of classes, attributes and attribute values		 */
/*	-----------------------------------------------------		 */
/*									 */
/*************************************************************************/



#include <sys/types.h>
#include <sys/stat.h>

#define	MAXLINEBUFFER	10000
int	Delimiter;
char	LineBuffer[MAXLINEBUFFER], *LBp=LineBuffer;



/*************************************************************************/
/*									 */
/*	Read a name from file f into string s, setting Delimiter.	 */
/*									 */
/*	- Embedded periods are permitted, but periods followed by space	 */
/*	  characters act as delimiters.					 */
/*	- Embedded spaces are permitted, but multiple spaces are	 */
/*	  replaced by a single space.					 */
/*	- Any character can be escaped by '\'.				 */
/*	- The remainder of a line following '|' is ignored.		 */
/*									 */
/*	Colons are sometimes delimiters depending on ColonOpt		 */
/*									 */
/*************************************************************************/


Boolean ReadName(FILE *f, String s, int n, char ColonOpt)
/*      --------  */
{
    register char *Sp=s;
    register int  c;
    char	  Msg[2];

    /*  Skip to first non-space character  */

    while ( (c = InChar(f)) == '|' || Space(c) )
    {
	if ( c == '|' ) SkipComment;
    }

    /*  Return false if no names to read  */

    if ( c == EOF )
    {
	Delimiter = EOF;
	return false;
    }

    /*  Read in characters up to the next delimiter  */

    while ( c != ColonOpt && c != ',' && c != '\n' && c != '|' && c != EOF )
    {
	if ( --n <= 0 )
	{
	    if ( Of ) Error(LONGNAME, "", "");
	}

	if ( c == '.' )
	{
	    if ( (c = InChar(f)) == '|' || Space(c) || c == EOF ) break;
	    *Sp++ = '.';
	    continue;
	}

	if ( c == '\\' )
	{
	    c = InChar(f);
	}

	if ( Space(c) )
	{
	    *Sp++ = ' ';

	    while ( ( c = InChar(f) ) == ' ' || c == '\t' )
		;
	}
	else
	{
	    *Sp++ = c;
	    c = InChar(f);
	}
    }

    if ( c == '|' ) SkipComment;
    Delimiter = c;

    /*  Special case for ':='  */

    if ( Delimiter == ':' )
    {
	if ( *LBp == '=' )
	{
	    Delimiter = '=';
	    LBp++;
	}
    }

    /*  Strip trailing spaces  */

    while ( Sp > s && Space(*(Sp-1)) ) Sp--;

    if ( Sp == s )
    {
	Msg[0] = ( Space(c) ? '.' : c );
	Msg[1] = '\00';
	Error(MISSNAME, Fn, Msg);
    }

    *Sp++ = '\0';
    return true;
}



/*************************************************************************/
/*									 */
/*	Read names of classes, attributes and legal attribute values.	 */
/*	On completion, names are stored in:				 */
/*	  ClassName	-	class names				 */
/*	  AttName	-	attribute names				 */
/*	  AttValName	-	attribute value names			 */
/*	with:								 */
/*	  MaxAttVal	-	number of values for each attribute	 */
/*									 */
/*	Other global variables set are:					 */
/*	  MaxAtt	-	maximum attribute number		 */
/*	  MaxClass	-	maximum class number			 */
/*	  MaxDiscrVal	-	maximum discrete values for an attribute */
/*									 */
/*************************************************************************/


void GetNames(FILE *Nf)
/*   --------  */
{
    char	Buffer[1000]="", *EndBuff;
    int		AttCeiling=100, ClassCeiling=100;
    Attribute	Att;
    ClassNo	c;

    ErrMsgs = AttExIn = 0;
    LineNo  = 0;
    LBp     = LineBuffer;
    *LBp    = 0;

    MaxClass = ClassAtt = LabelAtt = CWtAtt = 0;

    /*  Get class names from names file.  This entry can be:
	- a list of discrete values separated by commas
	- the name of the discrete attribute to use as the class
	- the name of a continuous attribute followed by a colon and
	  a comma-separated list of thresholds used to segment it  */

    ClassName = AllocZero(ClassCeiling, String);
    do
    {
	ReadName(Nf, Buffer, 1000, ':');

	if ( ++MaxClass >= ClassCeiling)
	{
	    ClassCeiling += 100;
	    Realloc(ClassName, ClassCeiling, String);
	}
	ClassName[MaxClass] = strdup(Buffer);
    }
    while ( Delimiter == ',' );

    if ( Delimiter == ':' )
    {
	/*  Thresholds for continuous class attribute  */

	ClassThresh = Alloc(ClassCeiling, ContValue);
	MaxClass = 0;

	do
	{
	    ReadName(Nf, Buffer, 1000, ':');

	    if ( ++MaxClass >= ClassCeiling)
	    {
		ClassCeiling += 100;
		Realloc(ClassThresh, ClassCeiling, ContValue);
	    }

	    ClassThresh[MaxClass] = strtod(Buffer, &EndBuff);
	    if ( EndBuff == Buffer || *EndBuff != '\0' )
	    {
		Error(BADCLASSTHRESH, Buffer, Nil);
	    }
	    else
	    if ( MaxClass > 1 &&
		 ClassThresh[MaxClass] <= ClassThresh[MaxClass-1] )
	    {
		Error(LEQCLASSTHRESH, Buffer, Nil);
	    }
	}
	while ( Delimiter == ',' );
    }

    /*  Get attribute and attribute value names from names file  */

    AttName	  = AllocZero(AttCeiling, String);
    MaxAttVal	  = AllocZero(AttCeiling, DiscrValue);
    AttValName	  = AllocZero(AttCeiling, String *);
    SpecialStatus = AllocZero(AttCeiling, char);
    AttDef	  = AllocZero(AttCeiling, Definition);
    AttDefUses	  = AllocZero(AttCeiling, Attribute *);

    MaxAtt = 0;
    while ( ReadName(Nf, Buffer, 1000, ':') )
    {
	if ( Delimiter != ':' && Delimiter != '=' )
	{
	    Error(BADATTNAME, Buffer, "");
	}

	/*  Check for attributes included/excluded  */

	if ( ( *Buffer == 'a' || *Buffer == 'A' ) &&
	     ! memcmp(Buffer+1, "ttributes ", 10) &&
	     ! memcmp(Buffer+strlen(Buffer)-6, "cluded", 6) )
	{
	    AttExIn = ( ! memcmp(Buffer+strlen(Buffer)-8, "in", 2) ? 1 : -1 );
	    if ( AttExIn == 1 )
	    {
		ForEach(Att, 1, MaxAtt)
		{
		    SpecialStatus[Att] |= SKIP;
		}
	    }

	    while ( ReadName(Nf, Buffer, 1000, ':') )
	    {
		Att = Which(Buffer, AttName, 1, MaxAtt);
		if ( ! Att )
		{
		    Error(UNKNOWNATT, Buffer, Nil);
		}
		else
		if ( AttExIn == 1 )
		{
		    SpecialStatus[Att] -= SKIP;
		}
		else
		{
		    SpecialStatus[Att] |= SKIP;
		}
	    }

	    break;
	}

	if ( Which(Buffer, AttName, 1, MaxAtt) > 0 )
	{
	    Error(DUPATTNAME, Buffer, Nil);
	}

	if ( ++MaxAtt >= AttCeiling )
	{
	    AttCeiling += 100;
	    Realloc(AttName, AttCeiling, String);
	    Realloc(MaxAttVal, AttCeiling, DiscrValue);
	    Realloc(AttValName, AttCeiling, String *);
	    Realloc(SpecialStatus, AttCeiling, char);
	    Realloc(AttDef, AttCeiling, Definition);
	    Realloc(AttDefUses, AttCeiling, Attribute *);
	}

	AttName[MaxAtt]       = strdup(Buffer);
	SpecialStatus[MaxAtt] = Nil;
	AttDef[MaxAtt]        = Nil;
	MaxAttVal[MaxAtt]     = 0;
	AttDefUses[MaxAtt]    = Nil;

	if ( Delimiter == '=' )
	{
	    if ( MaxClass == 1 && ! strcmp(ClassName[1], AttName[MaxAtt]) )
	    {
		Error(BADDEF3, Nil, Nil);
	    }

	    ImplicitAtt(Nf);
	    ListAttsUsed();
	}
	else
	{
	    ExplicitAtt(Nf);
	}

	/*  Check for case weight attribute, which must be type continuous  */

	if (  ! strcmp(AttName[MaxAtt], "case weight") )
	{
	    CWtAtt = MaxAtt;

	    if ( ! Continuous(CWtAtt) )
	    {
		Error(CWTATTERR, "", "");
	    }
	}
    }

    /*  Check whether class is one of the attributes  */

    if ( MaxClass == 1 || ClassThresh )
    {
	/*  Class attribute must be present and must be either
	    a discrete attribute or a thresholded continuous attribute  */

	ClassAtt = Which(ClassName[1], AttName, 1, MaxAtt);

	if ( ClassAtt <= 0 || Exclude(ClassAtt) )
	{
	    Error(NOTARGET, ClassName[1], "");
	}
	else
	if ( ClassThresh &&
	     ( ! Continuous(ClassAtt) ||
	       StatBit(ClassAtt, DATEVAL|STIMEVAL|TSTMPVAL) ) )
	{
	    Error(BADCTARGET, ClassName[1], "");
	}
	else
	if ( ! ClassThresh &&
	     ( Continuous(ClassAtt) || StatBit(ClassAtt, DISCRETE) ) )
	{
	    Error(BADDTARGET, ClassName[1], "");
	}

	Free(ClassName[1]);

	if ( ! ClassThresh )
	{
	    Free(ClassName);
	    MaxClass  = MaxAttVal[ClassAtt];
	    ClassName = AttValName[ClassAtt];
	}
	else
	{
	    /*  Set up class names as segments of continuous target att  */

	    MaxClass++;
	    Realloc(ClassName, MaxClass+1, String);

	    sprintf(Buffer, "%s <= %g", AttName[ClassAtt], ClassThresh[1]);
	    ClassName[1] = strdup(Buffer);

	    ForEach(c, 2, MaxClass-1)
	    {
		sprintf(Buffer, "%g < %s <= %g",
			ClassThresh[c-1], AttName[ClassAtt], ClassThresh[c]);
		ClassName[c] = strdup(Buffer);
	    }

	    sprintf(Buffer, "%s > %g",
		    AttName[ClassAtt], ClassThresh[MaxClass-1]);
	    ClassName[MaxClass] = strdup(Buffer);
	}
    }

    /*  Ignore case weight attribute if it is excluded; otherwise,
	it cannot be used in models  */

    if ( CWtAtt )
    {
	if ( Skip(CWtAtt) )
	{
	    CWtAtt = 0;
	}
	else
	{
	    SpecialStatus[CWtAtt] |= SKIP;
	}
    }

    ClassName[0] = "?";

    fclose(Nf);

    if ( ErrMsgs > 0 ) Goodbye(1);

    assert (MaxClass == 2);
    assert (! strcmp(ClassName[1], "true"));
    assert (! strcmp(ClassName[2], "false"));
}



/*************************************************************************/
/*									 */
/*	Continuous or discrete attribute				 */
/*									 */
/*************************************************************************/


void ExplicitAtt(FILE *Nf)
/*   -----------  */
{
    char	Buffer[1000]="", *p;
    DiscrValue	v;
    int		ValCeiling=100, BaseYear;
    time_t	clock;

    /*  Read attribute type or first discrete value  */

    if ( ! ( ReadName(Nf, Buffer, 1000, ':') ) )
    {
	Error(EOFINATT, AttName[MaxAtt], "");
    }

    MaxAttVal[MaxAtt] = 0;

    if ( Delimiter != ',' )
    {
	/*  Typed attribute  */

	if ( ! strcmp(Buffer, "continuous") )
	{
	}
	else
	if ( ! strcmp(Buffer, "timestamp") )
	{
	    SpecialStatus[MaxAtt] = TSTMPVAL;

	    /*  Set the base date if not done already  */

	    if ( ! TSBase )
	    {
		clock = time(0);
		BaseYear = gmtime(&clock)->tm_year + 1900;
		SetTSBase(BaseYear);
	    }
	}
	else
	if ( ! strcmp(Buffer, "date") )
	{
	    SpecialStatus[MaxAtt] = DATEVAL;
	}
	else
	if ( ! strcmp(Buffer, "time") )
	{
	    SpecialStatus[MaxAtt] = STIMEVAL;
	}
	else
	if ( ! memcmp(Buffer, "discrete", 8) )
	{
	    SpecialStatus[MaxAtt] = DISCRETE;

	    /*  Read max values and reserve space  */

	    v = atoi(&Buffer[8]);
	    if ( v < 2 )
	    {
		Error(BADDISCRETE, AttName[MaxAtt], "");
	    }

	    AttValName[MaxAtt] = Alloc(v+3, String);
	    AttValName[MaxAtt][0] = (char *) (long) v+1;
	    AttValName[MaxAtt][(MaxAttVal[MaxAtt]=1)] = strdup("N/A");
	}
	else
	if ( ! strcmp(Buffer, "ignore") )
	{
	    SpecialStatus[MaxAtt] = EXCLUDE;
	}
	else
	if ( ! strcmp(Buffer, "label") )
	{
	    LabelAtt = MaxAtt;
	    SpecialStatus[MaxAtt] = EXCLUDE;
	}
	else
	{
	    /*  Cannot have only one discrete value for an attribute  */

	    Error(SINGLEATTVAL, AttName[MaxAtt], Buffer);
	}
    }
    else
    {
	/*  Discrete attribute with explicit values  */

	AttValName[MaxAtt] = AllocZero(ValCeiling, String);

	/*  Add "N/A" unless this attribute is the class  */

	if ( MaxClass > 1 || strcmp(ClassName[1], AttName[MaxAtt]) )
	{
	    AttValName[MaxAtt][(MaxAttVal[MaxAtt]=1)] = strdup("N/A");
	}
	else
	{
	    MaxAttVal[MaxAtt] = 0;
	}

	p = Buffer;

	/*  Special check for ordered attribute  */

	if ( ! memcmp(Buffer, "[ordered]", 9) )
	{
	    SpecialStatus[MaxAtt] = ORDERED;

	    for ( p = Buffer+9 ; Space(*p) ; p++ )
		;
	}

	/*  Record first real explicit value  */

	AttValName[MaxAtt][++MaxAttVal[MaxAtt]] = strdup(p);

	/*  Record remaining values  */

	do
	{
	    if ( ! ( ReadName(Nf, Buffer, 1000, ':') ) )
	    {
		Error(EOFINATT, AttName[MaxAtt], "");
	    }

	    if ( ++MaxAttVal[MaxAtt] >= ValCeiling )
	    {
		ValCeiling += 100;
		Realloc(AttValName[MaxAtt], ValCeiling, String);
	    }

	    AttValName[MaxAtt][MaxAttVal[MaxAtt]] = strdup(Buffer);
	}
	while ( Delimiter == ',' );

	/*  Cancel ordered status if <3 real values  */

	if ( Ordered(MaxAtt) && MaxAttVal[MaxAtt] <= 3 )
	{
	    SpecialStatus[MaxAtt] = 0;
	}
	if ( MaxAttVal[MaxAtt] > MaxDiscrVal ) MaxDiscrVal = MaxAttVal[MaxAtt];
    }
}



/*************************************************************************/
/*									 */
/*	Locate value Val in List[First] to List[Last]			 */
/*									 */
/*************************************************************************/


int Which(String Val, String *List, int First, int Last)
/*  -----  */
{
    int	n=First;

    while ( n <= Last && strcmp(Val, List[n]) ) n++;

    return ( n <= Last ? n : First-1 );
}



/*************************************************************************/
/*									 */
/*	Build list of attributes used in current attribute definition	 */
/*	    AttDefUses[Att][0] = number of atts used			 */
/*	    AttDefUses[Att][1..] are the atts				 */
/*									 */
/*************************************************************************/


void ListAttsUsed()
/*   ------------  */
{
    Attribute	Att;
    Boolean	*DefUses;
    Definition	D;
    int		e, NUsed=0;

    DefUses = AllocZero(MaxAtt+1, Boolean);

    D = AttDef[MaxAtt];

    for ( e = 0 ; ; e++ )
    {
	if ( DefOp(D[e]) == OP_ATT )
	{
	    Att = (Attribute) DefSVal(D[e]);
	    if ( ! DefUses[Att] )
	    {
		DefUses[Att] = true;
		NUsed++;
	    }
	}
	else
	if ( DefOp(D[e]) == OP_END )
	{
	    break;
	}
    }

    if ( NUsed )
    {
	AttDefUses[MaxAtt] = Alloc(NUsed+1, Attribute);
	AttDefUses[MaxAtt][0] = NUsed;

	NUsed=0;
	ForEach(Att, 1, MaxAtt-1)
	{
	    if ( DefUses[Att] )
	    {
		AttDefUses[MaxAtt][++NUsed] = Att;
	    }
	}
    }

    Free(DefUses);
}



/*************************************************************************/
/*									 */
/*	Free up all space allocated by GetNames()			 */
/*									 */
/*************************************************************************/


void FreeNames()
/*   ---------  */
{
    Attribute a, t;

    if ( ! AttName ) return;

    ForEach(a, 1, MaxAtt)
    {
	if ( a != ClassAtt && Discrete(a) )
	{
	    FreeVector((void **) AttValName[a], 1, MaxAttVal[a]);
	}
    }
    FreeUnlessNil(AttValName);				AttValName = Nil;
    FreeUnlessNil(MaxAttVal);				MaxAttVal = Nil;
    FreeUnlessNil(ClassThresh);				ClassThresh = Nil;
    FreeVector((void **) AttName, 1, MaxAtt);		AttName = Nil;
    FreeVector((void **) ClassName, 1, MaxClass);	ClassName = Nil;

    FreeUnlessNil(SpecialStatus);			SpecialStatus = Nil;

    /*  Definitions (if any)  */

    if ( AttDef )
    {
	ForEach(a, 1, MaxAtt)
	{
	    if ( AttDef[a] )
	    {
		for ( t = 0 ; DefOp(AttDef[a][t]) != OP_END ; t++ )
		{
		    if ( DefOp(AttDef[a][t]) == OP_STR )
		    {
			Free(DefSVal(AttDef[a][t]));
		    }
		}

		Free(AttDef[a]);
		Free(AttDefUses[a]);
	    }
	}
	Free(AttDef);					AttDef = Nil;
	Free(AttDefUses);				AttDefUses = Nil;
    }
}



/*************************************************************************/
/*									 */
/*	Read next char keeping track of line numbers			 */
/*									 */
/*************************************************************************/


int InChar(FILE *f)
/*  ------  */
{
    if ( ! *LBp )
    {
	LBp = LineBuffer;

	if ( ! fgets(LineBuffer, MAXLINEBUFFER, f) )
	{
	    LineBuffer[0] = '\00';
	    return EOF;
	}

	LineNo++;
    }
	
    return (int) *LBp++;
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Routines for saving and reading model files			 */
/*	-------------------------------------------			 */
/*									 */
/*************************************************************************/



int	Entry;

char*	Prop[]={"null",
		"att",
		"class",
		"cut",
		"conds",
		"elts",
		"entries",
		"forks",
		"freq",
		"id",
		"type",
		"low",
		"mid",
		"high",
		"result",
		"rules",
		"val",
		"lift",
		"cover",
		"ok",
		"default",
		"costs",
		"sample",
		"init"
	       };

char	PropName[20],
	*PropVal=Nil,
	*Unquoted;
int	PropValSize=0;

#define	PROPS 23

#define ERRORP		0
#define ATTP		1
#define CLASSP		2
#define CUTP		3
#define	CONDSP		4
#define ELTSP		5
#define ENTRIESP	6
#define FORKSP		7
#define FREQP		8
#define IDP		9
#define TYPEP		10
#define LOWP		11
#define MIDP		12
#define HIGHP		13
#define RESULTP		14
#define RULESP		15
#define VALP		16
#define LIFTP		17
#define COVERP		18
#define OKP		19
#define DEFAULTP	20
#define COSTSP		21
#define SAMPLEP		22
#define INITP		23


/*************************************************************************/
/*									 */
/*	Check whether file is open.  If it is not, open it and		 */
/*	read/write sampling information and discrete names		 */
/*									 */
/*************************************************************************/


void CheckFile(String Extension, Boolean Write)
/*   ---------  */
{
    static char	*LastExt="";

    if ( ! TRf || strcmp(LastExt, Extension) )
    {
	LastExt = Extension;

	if ( TRf )
	{
	    fprintf(TRf, "\n");
	    fclose(TRf);
	}

	if ( Write )
	{
	    WriteFilePrefix(Extension);
	}
	else
	{
	    ReadFilePrefix(Extension);
	}
    }
}



/*************************************************************************/
/*									 */
/*	Write information on system, sampling				 */
/*									 */
/*************************************************************************/


void WriteFilePrefix(String Extension)
/*   ---------------  */
{
    time_t	clock;
    struct tm	*now;

    if ( ! (TRf = GetFile(Extension, "w")) )
    {
	Error(NOFILE, Fn, E_ForWrite);
    }

    clock = time(0);
    now = localtime(&clock);
    now->tm_mon++;
    fprintf(TRf, "id=\"See5/C5.0 %s %d-%d%d-%d%d\"\n",
	    RELEASE,
	    now->tm_year + 1900,
	    now->tm_mon / 10, now->tm_mon % 10,
	    now->tm_mday / 10, now->tm_mday % 10);

    if ( MCost )
    {
	fprintf(TRf, "costs=\"1\"\n");
    }

    if ( SAMPLE > 0 )
    {
	fprintf(TRf, "sample=\"%g\" init=\"%d\"\n", SAMPLE, KRInit);
    }

    SaveDiscreteNames();

    fprintf(TRf, "entries=\"%d\"\n", TRIALS);
}



/*************************************************************************/
/*									 */
/*	Read header information						 */
/*									 */
/*************************************************************************/


void ReadFilePrefix(String Extension)
/*   --------------  */
{
    if ( ! (TRf = GetFile(Extension, "r")) ) Error(NOFILE, Fn, "");

    StreamIn((char *) &TRIALS, sizeof(int));
    if ( memcmp((char *) &TRIALS, "id=", 3) != 0 )
    {
	printf("\nCannot read old format classifiers\n");
	exit(1);
    }
    else
    {
	rewind(TRf);
	ReadHeader();
    }
}



/*************************************************************************/
/*									 */
/*	Save attribute values read with "discrete N"			 */
/*									 */
/*************************************************************************/


void SaveDiscreteNames()
/*   -----------------  */
{
    Attribute	Att;
    DiscrValue	v;

    ForEach(Att, 1, MaxAtt)
    {
	if ( ! StatBit(Att, DISCRETE) || MaxAttVal[Att] < 2 ) continue;

	AsciiOut("att=", AttName[Att]);
	AsciiOut(" elts=", AttValName[Att][2]); 	/* skip N/A */

	ForEach(v, 3, MaxAttVal[Att])
	{
	    AsciiOut(",", AttValName[Att][v]);
	}
	fprintf(TRf, "\n");
    }
}



/*************************************************************************/
/*									 */
/*	Save entire decision tree T in file with extension Extension	 */
/*									 */
/*************************************************************************/


void SaveTree(Tree T, String Extension)
/*   --------  */
{
    CheckFile(Extension, true);

    OutTree(T);
}



void OutTree(Tree T)
/*   -------  */
{
    DiscrValue	v, vv;
    ClassNo	c;
    Boolean	First;

    fprintf(TRf, "type=\"%d\"", T->NodeType);
    AsciiOut(" class=", ClassName[T->Leaf]);
    if ( T->Cases > 0 )
    {
	fprintf(TRf, " freq=\"%g", T->ClassDist[1]);
	ForEach(c, 2, MaxClass)
	{
	    fprintf(TRf, ",%g", T->ClassDist[c]);
	}
	fprintf(TRf, "\"");
    }

    if ( T->NodeType )
    {
	AsciiOut(" att=", AttName[T->Tested]);
	fprintf(TRf, " forks=\"%d\"", T->Forks);

	switch ( T->NodeType )
	{
	    case BrDiscr:
		break;

	    case BrThresh:
		fprintf(TRf, " cut=\"%.*g\"", PREC+1, T->Cut);
		if ( T->Upper > T->Cut )
		{
		    fprintf(TRf, " low=\"%.*g\" mid=\"%.*g\" high=\"%.*g\"",
				 PREC, T->Lower, PREC, T->Mid, PREC, T->Upper);
		}
		break;

	    case BrSubset:
		ForEach(v, 1, T->Forks)
		{
		    First=true;
		    ForEach(vv, 1, MaxAttVal[T->Tested])
		    {
			if ( In(vv, T->Subset[v]) )
			{
			    if ( First )
			    {
				AsciiOut(" elts=", AttValName[T->Tested][vv]);
				First = false;
			    }
			    else
			    {
				AsciiOut(",", AttValName[T->Tested][vv]);
			    }
			}
		    }
		    /*  Make sure have printed at least one element  */

		    if ( First ) AsciiOut(" elts=", "N/A");
		}
		break;
	}
	fprintf(TRf, "\n");

	ForEach(v, 1, T->Forks)
	{
	    OutTree(T->Branch[v]);
	}
    }
    else
    {
	fprintf(TRf, "\n");
    }
}



/*************************************************************************/
/*								  	 */
/*	Save the current ruleset in rules file				 */
/*								  	 */
/*************************************************************************/


void SaveRules(CRuleSet RS, String Extension)
/*   ---------  */
{
    int		ri, d;
    CRule	R;
    Condition	C;
    DiscrValue	v;
    Boolean	First;

    CheckFile(Extension, true);

    fprintf(TRf, "rules=\"%d\"", RS->SNRules);
    AsciiOut(" default=", ClassName[RS->SDefault]);
    fprintf(TRf, "\n");

    ForEach(ri, 1, RS->SNRules)
    {
	R = RS->SRule[ri];
	fprintf(TRf, "conds=\"%d\" cover=\"%g\" ok=\"%g\" lift=\"%g\"",
		     R->Size, R->Cover, R->Correct,
		     (R->Correct + 1) / ((R->Cover + 2) * R->Prior));
	AsciiOut(" class=", ClassName[R->Rhs]);
	fprintf(TRf, "\n");

	ForEach(d, 1, R->Size)
	{
	    C = R->Lhs[d];

	    fprintf(TRf, "type=\"%d\"", C->NodeType);
	    AsciiOut(" att=", AttName[C->Tested]);

	    switch ( C->NodeType )
	    {
		case BrDiscr:
		    AsciiOut(" val=", AttValName[C->Tested][C->TestValue]);
		    break;

		case BrThresh:
		    if ( C->TestValue == 1 )	/* N/A */
		    {
			fprintf(TRf, " val=\"N/A\"");
		    }
		    else
		    {
			fprintf(TRf, " cut=\"%.*g\" result=\"%c\"",
				     PREC+1, C->Cut,
				     ( C->TestValue == 2 ? '<' : '>' ));
		    }
		    break;

		case BrSubset:
		    First=true;
		    ForEach(v, 1, MaxAttVal[C->Tested])
		    {
			if ( In(v, C->Subset) )
			{
			    if ( First )
			    {
				AsciiOut(" elts=", AttValName[C->Tested][v]);
				First = false;
			    }
			    else
			    {
				AsciiOut(",", AttValName[C->Tested][v]);
			    }
			}
		    }
		    break;
	    }

	    fprintf(TRf, "\n");
	}
    }
}



/*************************************************************************/
/*									 */
/*	Write ASCII string with prefix, escaping any quotes		 */
/*									 */
/*************************************************************************/


void AsciiOut(String Pre, String S)
/*   --------  */
{
    fprintf(TRf, "%s\"", Pre);
    while ( *S )
    {
	if ( *S == '"' || *S == '\\' ) fputc('\\', TRf);
	fputc(*S++, TRf);
    }
    fputc('"', TRf);
}



/*************************************************************************/
/*								  	 */
/*	Read the header information (id, saved names, models)		 */
/*								  	 */
/*************************************************************************/


void ReadHeader()
/*   ---------  */
{
    Attribute	Att;
    DiscrValue	v;
    char	*p, Dummy;
    int		Year, Month, Day;
    FILE	*F;

    while ( true )
    {
	switch ( ReadProp(&Dummy) )
	{
	    case ERRORP:
		return;

	    case IDP:
		/*  Recover year run and set base date for timestamps  */

		if ( sscanf(PropVal + strlen(PropVal) - 11,
			    "%d-%d-%d\"", &Year, &Month, &Day) == 3 )
		{
		    SetTSBase(Year);
		}
		break;

	    case COSTSP:
		/*  Recover costs file used to generate model  */

		if ( (F = GetFile(".costs", "r")) )
		{
		    GetMCosts(F);
		}
		break;
	    case SAMPLEP:
		sscanf(PropVal, "\"%f\"", &SAMPLE);
		break;

	    case INITP:
		sscanf(PropVal, "\"%d\"", &KRInit);
		break;

	    case ATTP:
		Unquoted = RemoveQuotes(PropVal);
		Att = Which(Unquoted, AttName, 1, MaxAtt);
		if ( ! Att || Exclude(Att) )
		{
		    Error(MODELFILE, E_MFATT, Unquoted);
		}
		break;

	    case ELTSP:
		MaxAttVal[Att] = 1;
		AttValName[Att][1] = strdup("N/A");

		for ( p = PropVal ; *p ; )
		{
		    p = RemoveQuotes(p);
		    v = ++MaxAttVal[Att];
		    AttValName[Att][v] = strdup(p);

		    for ( p += strlen(p) ; *p != '"' ; p++ )
			;
		    p++;
		    if ( *p == ',' ) p++;
		}
		AttValName[Att][MaxAttVal[Att]+1] = "<other>";
		MaxDiscrVal = Max(MaxDiscrVal, MaxAttVal[Att]+1);
		break;

	    case ENTRIESP:
		sscanf(PropVal, "\"%d\"", &TRIALS);
		Entry = 0;
		return;
	}
    }
}



/*************************************************************************/
/*									 */
/*	Retrieve decision tree with extension Extension			 */
/*									 */
/*************************************************************************/


Tree GetTree(String Extension)
/*   -------  */
{
    CheckFile(Extension, false);

    return InTree();
}



Tree InTree()
/*   ------  */
{
    Tree	T;
    DiscrValue	v, Subset=0;
    char	Delim, *p;
    ClassNo	c;
    int		X;
    double	XD;

    T = (Tree) AllocZero(1, TreeRec);

    do
    {
	switch ( ReadProp(&Delim) )
	{
	    case ERRORP:
		return Nil;

	    case TYPEP:
		sscanf(PropVal, "\"%d\"", &X); T->NodeType = X;
		break;

	    case CLASSP:
		Unquoted = RemoveQuotes(PropVal);
		T->Leaf = Which(Unquoted, ClassName, 1, MaxClass);
		if ( ! T->Leaf ) Error(MODELFILE, E_MFCLASS, Unquoted);
		break;

	    case ATTP:
		Unquoted = RemoveQuotes(PropVal);
		T->Tested = Which(Unquoted, AttName, 1, MaxAtt);
		if ( ! T->Tested || Exclude(T->Tested) )
		{
		    Error(MODELFILE, E_MFATT, Unquoted);
		}
		break;

	    case CUTP:
		sscanf(PropVal, "\"%lf\"", &XD);	T->Cut = XD;
		T->Lower = T->Mid = T->Upper = T->Cut;
		break;

	    case LOWP:
		sscanf(PropVal, "\"%lf\"", &XD);	T->Lower = XD;
		break;

	    case MIDP:
		sscanf(PropVal, "\"%lf\"", &XD);	T->Mid = XD;
		break;

	    case HIGHP:
		sscanf(PropVal, "\"%lf\"", &XD);	T->Upper = XD;
		break;

	    case FORKSP:
		sscanf(PropVal, "\"%d\"", &T->Forks);
		break;

	    case FREQP:
		T->ClassDist = Alloc(MaxClass+1, CaseCount);
		p = PropVal+1;

		ForEach(c, 1, MaxClass)
		{
		    T->ClassDist[c] = strtod(p, &p);
		    T->Cases += T->ClassDist[c];
		    p++;
		}
		break;

	    case ELTSP:
		if ( ! Subset++ )
		{
		    T->Subset = AllocZero(T->Forks+1, Set);
		}

		T->Subset[Subset] = MakeSubset(T->Tested);
		break;
	}
    }
    while ( Delim == ' ' );

    if ( T->ClassDist )
    {
	T->Errors = T->Cases - T->ClassDist[T->Leaf];
    }
    else
    {
	T->ClassDist = Alloc(1, CaseCount);
    }

    if ( T->NodeType )
    {
	T->Branch = AllocZero(T->Forks+1, Tree);
	ForEach(v, 1, T->Forks)
	{
	    T->Branch[v] = InTree();
	}
    }

    return T;
}



/*************************************************************************/
/*									 */
/*	Retrieve ruleset with extension Extension			 */
/*	(Separate functions for ruleset, single rule, single condition)	 */
/*									 */
/*************************************************************************/


CRuleSet GetRules(String Extension)
/*	 --------  */
{
    CheckFile(Extension, false);

    return InRules();
}



CRuleSet InRules()
/*	 -------  */
{
    CRuleSet	RS;
    RuleNo	r;
    char	Delim;

    RS = Alloc(1, RuleSetRec);

    do
    {
	switch ( ReadProp(&Delim) )
	{
	    case ERRORP:
		return Nil;

	    case RULESP:
		sscanf(PropVal, "\"%d\"", &RS->SNRules);
		CheckActiveSpace(RS->SNRules);
		break;

	    case DEFAULTP:
		Unquoted = RemoveQuotes(PropVal);
		RS->SDefault = Which(Unquoted, ClassName, 1, MaxClass);
		if ( ! RS->SDefault ) Error(MODELFILE, E_MFCLASS, Unquoted);
		break;
	}
    }
    while ( Delim == ' ' );

    /*  Read each rule  */

    RS->SRule = Alloc(RS->SNRules+1, CRule);
    ForEach(r, 1, RS->SNRules)
    {
	if ( (RS->SRule[r] = InRule()) )
	{
	    RS->SRule[r]->RNo = r;
	    RS->SRule[r]->TNo = Entry;
	}
    }
    ConstructRuleTree(RS);
    Entry++;
    return RS;
}



CRule InRule()
/*    ------  */
{
    CRule	R;
    int		d;
    char	Delim;
    float	Lift;

    R = Alloc(1, RuleRec);

    do
    {
	switch ( ReadProp(&Delim) )
	{
	    case ERRORP:
		return Nil;

	    case CONDSP:
		sscanf(PropVal, "\"%d\"", &R->Size);
		break;

	    case COVERP:
		sscanf(PropVal, "\"%f\"", &R->Cover);
		break;

	    case OKP:
		sscanf(PropVal, "\"%f\"", &R->Correct);
		break;

	    case LIFTP:
		sscanf(PropVal, "\"%f\"", &Lift);
		R->Prior = (R->Correct + 1) / ((R->Cover + 2) * Lift);
		break;

	    case CLASSP:
		Unquoted = RemoveQuotes(PropVal);
		R->Rhs = Which(Unquoted, ClassName, 1, MaxClass);
		if ( ! R->Rhs ) Error(MODELFILE, E_MFCLASS, Unquoted);
		break;
	}
    }
    while ( Delim == ' ' );

    R->Lhs = Alloc(R->Size+1, Condition);
    ForEach(d, 1, R->Size)
    {
	R->Lhs[d] = InCondition();
    }

    R->Vote = 1000 * (R->Correct + 1.0) / (R->Cover + 2.0) + 0.5;

    return R;
}



Condition InCondition()
/*        -----------  */
{
    Condition	C;
    char	Delim;
    int		X;
    double	XD;

    C = Alloc(1, CondRec);

    do
    {
	switch ( ReadProp(&Delim) )
	{
	    case ERRORP:
		return Nil;

	    case TYPEP:
		sscanf(PropVal, "\"%d\"", &X); C->NodeType = X;
		break;

	    case ATTP:
		Unquoted = RemoveQuotes(PropVal);
		C->Tested = Which(Unquoted, AttName, 1, MaxAtt);
		if ( ! C->Tested || Exclude(C->Tested) )
		{
		    Error(MODELFILE, E_MFATT, Unquoted);
		}
		break;

	    case CUTP:
		sscanf(PropVal, "\"%lf\"", &XD);	C->Cut = XD;
		break;

	    case RESULTP:
		C->TestValue = ( PropVal[1] == '<' ? 2 : 3 );
		break;

	    case VALP:
		if ( Continuous(C->Tested) )
		{
		    C->TestValue = 1;
		}
		else
		{
		    Unquoted = RemoveQuotes(PropVal);
		    C->TestValue = Which(Unquoted,
					 AttValName[C->Tested],
					 1, MaxAttVal[C->Tested]);
		    if ( ! C->TestValue ) Error(MODELFILE, E_MFATTVAL, Unquoted);
		}
		break;

	    case ELTSP:
		C->Subset = MakeSubset(C->Tested);
		C->TestValue = 1;
		break;
	}
    }
    while ( Delim == ' ' );

    return C;
}



/*************************************************************************/
/*									 */
/*	ASCII reading utilities						 */
/*									 */
/*************************************************************************/


int ReadProp(char *Delim)
/*  --------  */
{
    int		c, i;
    char	*p;
    Boolean	Quote=false;

    for ( p = PropName ; (c = fgetc(TRf)) != '=' ;  )
    {
	if ( p - PropName >= 19 || c == EOF )
	{
	    Error(MODELFILE, E_MFEOF, "");
	    PropName[0] = PropVal[0] = *Delim = '\00';
	    return 0;
	}
	*p++ = c;
    }
    *p = '\00';

    for ( p = PropVal ; ((c = fgetc(TRf)) != ' ' && c != '\n') || Quote ; )
    {
	if ( c == EOF )
	{
	    Error(MODELFILE, E_MFEOF, "");
	    PropName[0] = PropVal[0] = '\00';
	    return 0;
	}

	if ( (i = p - PropVal) >= PropValSize )
	{
	    Realloc(PropVal, (PropValSize += 10000) + 3, char);
	    p = PropVal + i;
	}

	*p++ = c;
	if ( c == '\\' )
	{
	    *p++ = fgetc(TRf);
	}
	else
	if ( c == '"' )
	{
	    Quote = ! Quote;
	}
    }
    *p = '\00';
    *Delim = c;

    return Which(PropName, Prop, 1, PROPS);
}


String RemoveQuotes(String S)
/*     ------------  */
{
    char	*p, *Start;

    p = Start = S;
    
    for ( S++ ; *S != '"' ; S++ )
    {
	if ( *S == '\\' ) S++;
	*p++ = *S;
	*S = '-';
    }
    *p = '\00';

    return Start;
}



Set MakeSubset(Attribute Att)
/*  ----------  */
{
    int		Bytes, b;
    char	*p;
    Set		S;

    Bytes = (MaxAttVal[Att]>>3) + 1;
    S = AllocZero(Bytes, Byte);

    for ( p = PropVal ; *p ; )
    {
	p = RemoveQuotes(p);
	b = Which(p, AttValName[Att], 1, MaxAttVal[Att]);
	if ( ! b ) Error(MODELFILE, E_MFATTVAL, p);
	SetBit(b, S);

	for ( p += strlen(p) ; *p != '"' ; p++ )
	    ;
	p++;
	if ( *p == ',' ) p++;
    }

    return S;
}



/*************************************************************************/
/*								  	 */
/*	Character stream read for binary routines			 */
/*								  	 */
/*************************************************************************/


void StreamIn(String S, int n)
/*   --------  */
{
    while ( n-- ) *S++ = getc(TRf);
}
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Print header for all C5.0 programs				 */
/*	----------------------------------				 */
/*									 */
/*************************************************************************/



#define  NAME T_C50


void PrintHeader(String Title)
/*   -----------  */
{
    char	TitleLine[80];
    time_t	clock;
    int		Underline;

    clock = time(0);
    sprintf(TitleLine, "%s%s [%s]", NAME, Title, TX_Release(RELEASE));
    fprintf(Of, "\n%s  \t%s", TitleLine, ctime(&clock));

    Underline = CharWidth(TitleLine);
    while ( Underline-- ) putc('-', Of);
    putc('\n', Of);
}



/*************************************************************************/
/*									 */
/*	This is a specialised form of the getopt utility.		 */
/*									 */
/*************************************************************************/


String	OptArg, Option;


char ProcessOption(int Argc, char *Argv[], char *Options)
/*   -------------  */
{
    int		i;
    static int	OptNo=1;

    if ( OptNo >= Argc ) return '\00';

    if ( *(Option = Argv[OptNo++]) != '-' ) return '?';

    for ( i = 0 ; Options[i] ; i++ )
    {
	if ( Options[i] == Option[1] )
	{
	    OptArg = (char *) ( Options[i+1] != '+' ? Nil :
				Option[2] ? Option+2 :
				OptNo < Argc ? Argv[OptNo++] : "0" );
	    return Option[1];
	}
    }

    return '?';
}



/*************************************************************************/
/*									 */
/*	Protected memory allocation routines				 */
/*									 */
/*************************************************************************/



void *Pmalloc(size_t Bytes)
/*    -------  */
{
    void *p=Nil;

    if ( ! Bytes || (p = (void *) malloc(Bytes)) )
    {
	return p;
    }

    Error(NOMEM, "", "");

}



void *Prealloc(void *Present, size_t Bytes)
/*    --------  */
{
    void *p=Nil;

    if ( ! Bytes ) return Nil;

    if ( ! Present ) return Pmalloc(Bytes);

    if ( (p = (void *) realloc(Present, Bytes)) )
    {
	return p;
    }

    Error(NOMEM, "", "");

}



void *Pcalloc(size_t Number, unsigned int Size)
/*    -------  */
{
    void *p=Nil;

    if ( ! Number || (p = (void *) calloc(Number, Size)) )
    {
	return p;
    }

    Error(NOMEM, "", "");

}



void FreeVector(void **V, int First, int Last)
/*   ----------  */
{
    if ( V )
    {
	while ( First <= Last )
	{
	    FreeUnlessNil(V[First]);
	    First++;
	}

	Free(V);
    }
}



/*************************************************************************/
/*									 */
/*	Special memory allocation routines for case memory		 */
/*									 */
/*************************************************************************/

typedef struct _datablockrec	*DataBlock;

typedef	struct _datablockrec
	{
	  DataRec	Head;		/* first address */
	  int		Allocated;	/* number of cases in this block */
	  DataBlock	Prev;		/* previous data block */
	}
	DataBlockRec;

DataBlock	DataMem=Nil;
int		DataBlockSize=0;



DataRec NewCase()
/*      -------  */
{
    DataBlock	Prev;

    if ( ! DataMem || DataMem->Allocated == DataBlockSize )
    {
	DataBlockSize = Min(8192, 262144 / (MaxAtt+2) + 1);

	Prev = DataMem;
	DataMem = AllocZero(1, DataBlockRec);
	DataMem->Head = Alloc(DataBlockSize * (MaxAtt+2), AttValue);
	DataMem->Prev = Prev;
    }

    return DataMem->Head + (DataMem->Allocated++) * (MaxAtt+2) + 1;
}



void FreeCases()
/*   ---------  */
{
    DataBlock	Prev;

    while ( DataMem )
    {
	Prev = DataMem->Prev;
	Free(DataMem->Head);
	Free(DataMem);
	DataMem = Prev;
    }
}



void FreeLastCase(DataRec Case)
/*   ------------  */
{
    DataMem->Allocated--;
}



/*************************************************************************/
/*									 */
/*	Generate uniform random numbers					 */
/*									 */
/*************************************************************************/


#define	Modify(F,S)	if ( (F -= S) < 0 ) F += 1.0

int	KRFp=0, KRSp=0;

double KRandom()
/*     -------  */
{
    static double	URD[55];
    double		V1, V2;
    int			i, j;

    /*  Initialisation  */

    if ( KRFp == KRSp )
    {
	KRFp = 0;
	KRSp = 31;

	V1 = 1.0;
	V2 = 0.314159285;

	ForEach(i, 1, 55)
	{
	    URD[ j = (i * 21) % 55 ] = V1;
	    V1 = V2 - V1;
	    if ( V1 < 0 ) V1 += 1.0;
	    V2 = URD[j];
	}

	ForEach(j, 0, 5)
	{
	    ForEach(i, 0, 54)
	    {
		Modify(URD[i], URD[(i+30) % 55]);
	    }
	}
    }

    KRFp = (KRFp + 1) % 55;
    KRSp = (KRSp + 1) % 55;
    Modify(URD[KRFp], URD[KRSp]);

    return URD[KRFp];
}



void ResetKR(int KRInit)
/*   -------  */
{
    KRFp = KRSp = 0;

    KRInit += 1000;
    while ( KRInit-- )
    {
	KRandom();
    }
}



/*************************************************************************/
/*									 */
/*	Error messages							 */
/*									 */
/*************************************************************************/


void Error(int ErrNo, String S1, String S2)
/*   -----  */
{
    Boolean	Quit=false, WarningOnly=false;
    char	Buffer[10000], *Msg=Buffer;


    if ( Of ) fprintf(Of, "\n");

    if ( ErrNo == NOFILE || ErrNo == NOMEM || ErrNo == MODELFILE )
    {
	sprintf(Msg, "*** ");
    }
    else
    {
	sprintf(Msg, TX_Line(LineNo, Fn));
    }
    Msg += strlen(Buffer);

    switch ( ErrNo )
    {
	case NOFILE:
	    sprintf(Msg, E_NOFILE(Fn, S2));
	    Quit = true;
	    break;

	case BADCLASSTHRESH:
	    sprintf(Msg, E_BADCLASSTHRESH, S1);
	    break;

	case LEQCLASSTHRESH:
	    sprintf(Msg, E_LEQCLASSTHRESH, S1);
	    break;

	case BADATTNAME:
	    sprintf(Msg, E_BADATTNAME, S1);
	    break;

	case EOFINATT:
	    sprintf(Msg, E_EOFINATT, S1);
	    break;

	case SINGLEATTVAL:
	    sprintf(Msg, E_SINGLEATTVAL(S1, S2));
	    break;

	case DUPATTNAME:
	    sprintf(Msg, E_DUPATTNAME, S1);
	    break;

	case CWTATTERR:
	    sprintf(Msg, E_CWTATTERR);
	    break;

	case BADATTVAL:
	    sprintf(Msg, E_BADATTVAL(S2, S1));
	    break;

	case BADNUMBER:
	    sprintf(Msg, E_BADNUMBER(S1));
	    break;

	case BADCLASS:
	    sprintf(Msg, E_BADCLASS, S2);
	    break;

	case BADCOSTCLASS:
	    sprintf(Msg, E_BADCOSTCLASS, S1);
	    Quit = true;
	    break;

	case BADCOST:
	    sprintf(Msg, E_BADCOST, S1);
	    Quit = true;
	    break;

	case NOMEM:
	    sprintf(Msg, E_NOMEM);
	    Quit = true;
	    break;

	case TOOMANYVALS:
	    sprintf(Msg, E_TOOMANYVALS(S1, (int) (long) S2));
	    break;

	case BADDISCRETE:
	    sprintf(Msg, E_BADDISCRETE, S1);
	    break;

	case NOTARGET:
	    sprintf(Msg, E_NOTARGET, S1);
	    Quit = true;
	    break;

	case BADCTARGET:
	    sprintf(Msg, E_BADCTARGET, S1);
	    Quit = true;
	    break;

	case BADDTARGET:
	    sprintf(Msg, E_BADDTARGET, S1);
	    Quit = true;
	    break;

	case LONGNAME:
	    sprintf(Msg, E_LONGNAME);
	    Quit = true;
	    break;

	case HITEOF:
	    sprintf(Msg, E_HITEOF);
	    break;

	case MISSNAME:
	    sprintf(Msg, E_MISSNAME, S2);
	    break;

	case BADTSTMP:
	    sprintf(Msg, E_BADTSTMP(S2, S1));
	    break;

	case BADDATE:
	    sprintf(Msg, E_BADDATE(S2, S1));
	    break;

	case BADTIME:
	    sprintf(Msg, E_BADTIME(S2, S1));
	    break;

	case UNKNOWNATT:
	    sprintf(Msg, E_UNKNOWNATT, S1);
	    break;

	case BADDEF1:
	    sprintf(Msg, E_BADDEF1(AttName[MaxAtt], S1, S2));
	    break;

	case BADDEF2:
	    sprintf(Msg, E_BADDEF2(AttName[MaxAtt], S1, S2));
	    break;

	case SAMEATT:
	    sprintf(Msg, E_SAMEATT(AttName[MaxAtt], S1));
	    WarningOnly = true;
	    break;

	case BADDEF3:
	    sprintf(Msg, E_BADDEF3, AttName[MaxAtt]);
	    break;

	case BADDEF4:
	    sprintf(Msg, E_BADDEF4, AttName[MaxAtt]);
	    WarningOnly = true;
	    break;

	case MODELFILE:
	    sprintf(Msg, EX_MODELFILE(Fn));
	    sprintf(Msg, "    (%s `%s')\n", S1, S2);
	    Quit = true;
	    break;
    }

    fprintf(Of, Buffer);
	
    if ( ! WarningOnly ) ErrMsgs++;

    if ( ErrMsgs == 10 )
    {
	fprintf(Of,  T_ErrorLimit);
	MaxCase--;
	Quit = true;
    }

    if ( Quit && Of )
    {
	Goodbye(1);
    }
}



/*************************************************************************/
/*                                                                       */
/*      Generate the label for a case                                    */
/*                                                                       */
/*************************************************************************/

char	LabelBuffer[1000];


String CaseLabel(CaseNo N)
/*     ---------  */
{
    String      p;

    if ( LabelAtt && (p = IgnoredVals + SVal(Case[N], LabelAtt)) )
	;
    else
    {
	sprintf(LabelBuffer, "#%d", N+1);
	p = LabelBuffer;
    }

    return p;
}



/*************************************************************************/
/*									 */
/*	Open file with given extension for read/write			 */
/*									 */
/*************************************************************************/


FILE *GetFile(String Extension, String RW)
/*    --------  */
{
    strcpy(Fn, FileStem);
    strcat(Fn, Extension);
    return fopen(Fn, RW);
}



/*************************************************************************/
/*									 */
/*	Determine total elapsed time so far.				 */
/*									 */
/*************************************************************************/


#include <sys/time.h>

double  ExecTime()
/*      --------  */
{
    struct timeval	TV;
    struct timezone	TZ={0,0};

    gettimeofday(&TV, &TZ);
    return TV.tv_sec + TV.tv_usec / 1000000.0;
}



/*************************************************************************/
/*									 */
/*	Determine precision of floating value				 */
/*									 */
/*************************************************************************/


int Denominator(ContValue Val)
/*  -----------  */
{
    double	RoundErr, Accuracy;
    int		Mult;

    Accuracy = fabs(Val) * 1E-6;	/* approximate */
    Val = modf(Val, &RoundErr);

    for ( Mult = 100000 ; Mult >= 1 ; Mult /= 10 )
    {
	RoundErr = fabs(rint(Val * Mult) / Mult - Val);
	if ( RoundErr > 2 * Accuracy )
	{
	    return Mult * 10;
	}
    }

    return 1;
}



/*************************************************************************/
/*									 */
/*	Routines to process date (Algorithm due to Gauss?)		 */
/*									 */
/*************************************************************************/


int GetInt(String S, int N)
/*  ------  */
{
    int	Result=0;

    while ( N-- )
    {
	if ( ! isdigit(*S) ) return 0;

	Result = Result * 10 + (*S++ - '0');
    }

    return Result;
}


int DateToDay(String DS)	/*  Day 1 is 0000/03/01  */
/*  ---------  */
{
    int Year, Month, Day;

    if ( strlen(DS) != 10 ) return 0;

    Year  = GetInt(DS, 4);
    Month = GetInt(DS+5, 2);
    Day   = GetInt(DS+8, 2);

    if ( ! ( DS[4] == '/' && DS[7] == '/' || DS[4] == '-' && DS[7] == '-' ) ||
	 Year < 0 || Month < 1 || Day < 1 ||
	 Month > 12 ||
	 Day > 31 ||
	 Day > 30 &&
	    ( Month == 4 || Month == 6 || Month == 9 || Month == 11 ) ||
	 Month == 2 &&
	    ( Day > 29 ||
	      Day > 28 && ( Year % 4 != 0 ||
			    Year % 100 == 0 && Year % 400 != 0 ) ) )
    {
	return 0;
    }

    if ( (Month -= 2) <= 0 )
    {
	Month += 12;
	Year -= 1;
    }

    return Year * 365 + Year / 4 - Year / 100 + Year / 400
	   + 367 * Month / 12
	   + Day - 30;
}



void DayToDate(int Day, String Date)
/*   ---------  */
{
    int Year, Month, OrigDay=Day;

    if ( Day <= 0 )
    {
	strcpy(Date, "?");
	return;
    }

    Year = (Day - 1) / 365.2425L;  /*  Year = completed years  */
    Day -= Year * 365 + Year / 4 - Year / 100 + Year / 400;

    if ( Day < 1 )
    {
	Year--;
	Day = OrigDay - (Year * 365 + Year / 4 - Year / 100 + Year / 400);
    }
    else
    if ( Day > 366 ||
	 Day == 366 &&
	 ( (Year+1) % 4 != 0 || (Year+1) % 100 == 0 && (Year+1) % 400 != 0 ) )
    {
	Year++;
	Day = OrigDay - (Year * 365 + Year / 4 - Year / 100 + Year / 400);
    }

    Month = (Day + 30) * 12 / 367;
    Day -= 367 * Month / 12 - 30;
    if ( Day < 1 )
    {
	Month = 11;
	Day = 31;
    }

    Month += 2;
    if ( Month > 12 )
    {
	Month -= 12;
	Year++;
    }

    sprintf(Date, "%d/%d%d/%d%d", Year, Month/10, Month % 10, Day/10, Day % 10);
}



/*************************************************************************/
/*									 */
/*	Routines to process clock time and timestamps			 */
/*									 */
/*************************************************************************/


int TimeToSecs(String TS)
/*  ----------  */
{
    int Hour, Mins, Secs;

    if ( strlen(TS) != 8 ) return -1;

    Hour = GetInt(TS, 2);
    Mins = GetInt(TS+3, 2);
    Secs = GetInt(TS+6, 2);

    if ( TS[2] != ':' || TS[5] != ':' ||
	 Hour >= 24 || Mins >= 60 || Secs >= 60 )
    {
	return -1;
    }

    return Hour * 3600 + Mins * 60 + Secs;
}



void SecsToTime(int Secs, String Time)
/*   ----------  */
{
    int Hour, Mins;

    Hour = Secs / 3600;
    Mins = (Secs % 3600) / 60;
    Secs = Secs % 60;

    sprintf(Time, "%d%d:%d%d:%d%d",
		  Hour / 10, Hour % 10,
		  Mins / 10, Mins % 10,
		  Secs / 10, Secs % 10);
}



void SetTSBase(int y)
/*   ---------  */
{
    y -= 15;
    TSBase = y * 365 + y / 4 - y / 100 + y / 400 + (367 * 4) / 12 + 1 - 30;
}



int TStampToMins(String TS)
/*  ------------  */
{
    int		Day, Sec, i;

    /*  Check for reasonable length and space between date and time  */

    if ( strlen(TS) < 19 || ! Space(TS[10]) ) return (1 << 30);

    /*  Read date part  */

    TS[10] = '\00';
    Day = DateToDay(TS);
    TS[10] = ' ';

    /*  Skip one or more spaces  */

    for ( i = 11 ; TS[i] && Space(TS[i]) ; i++ )
	;

    /*  Read time part  */

    Sec = TimeToSecs(TS+i);

    /*  Return a long time in the future if there is an error  */

    return ( Day < 1 || Sec < 0 ? (1 << 30) :
	     (Day - TSBase) * 1440 + (Sec + 30) / 60 );
}



/*************************************************************************/
/*									 */
/*	Convert a continuous value to a string.		DS must be	 */
/*	large enough to hold any value (e.g. a date, time, ...)		 */
/*									 */
/*************************************************************************/


void CValToStr(ContValue CV, Attribute Att, String DS)
/*   ---------  */
{
    int		Mins;

    if ( TStampVal(Att) )
    {
	DayToDate(floor(CV / 1440) + TSBase, DS);
	DS[10] = ' ';
	Mins = rint(CV) - floor(CV / 1440) * 1440;
	SecsToTime(Mins * 60, DS+11);
    }
    else
    if ( DateVal(Att) )
    {
	DayToDate(CV, DS);
    }
    else
    if ( TimeVal(Att) )
    {
	SecsToTime(CV, DS);
    }
    else
    {
	sprintf(DS, "%.*g", PREC, CV);
    }
}



/*************************************************************************/
/*									 */
/*	Check parameter value						 */
/*									 */
/*************************************************************************/


void Check(float Val, float Low, float High)
/*   -----  */
{
    if ( Val < Low || Val > High )
    {
	fprintf(Of, TX_IllegalValue(Val, Low, High));
	exit(1);
    }
}





/*************************************************************************/
/*									 */
/*	Deallocate all dynamic storage					 */
/*									 */
/*************************************************************************/


void Cleanup()
/*   -------  */
{
    int		t, r;

    extern DataRec	*Blocked;
    extern Tree		*SubDef;
    extern RuleNo	*Active;
    extern float	*AttImp;
    extern char		*PropVal;
    extern Boolean	*Split, *Used;
    extern FILE		*Uf;

    NotifyStage(CLEANUP);

    CheckClose(Uf);					Uf = Nil;
    CheckClose(TRf);					TRf = Nil;

    /*  Boost voting (construct.c)  */

    FreeUnlessNil(BVoteBlock);				BVoteBlock = Nil;

    /*  Stuff from attribute winnowing  */

    FreeUnlessNil(SaveCase);				SaveCase = Nil;
    FreeUnlessNil(AttImp);				AttImp = Nil;
    FreeUnlessNil(Split);				Split = Nil;
    FreeUnlessNil(Used);				Used = Nil;

    FreeUnlessNil(PropVal);				PropVal = Nil;
							PropValSize = 0;

    if ( RULES )
    {
	FreeFormRuleData();
	FreeSiftRuleData();
    }

    /*  May have interrupted a winnowing tree  */

    if ( WINNOW && WTree )
    {
	FreeTree(WTree);				WTree = Nil;
    }

    FreeUnlessNil(Blocked);				Blocked = Nil;

    FreeData();

    if ( MCost )
    {
	FreeVector((void **) MCost, 1, MaxClass);	MCost = Nil;
	FreeUnlessNil(WeightMul);			WeightMul = Nil;
    }

    ForEach(t, 0, MaxTree)
    {
	FreeClassifier(t);
    }

    if ( RULES )
    {
	/*  May be incomplete ruleset in Rule[]  */

	if ( Rule )
	{
	    ForEach(r, 1, NRules)
	    {
		FreeRule(Rule[r]);
	    }
	    Free(Rule);					Rule = Nil;
	}						

	FreeUnlessNil(RuleSet);				RuleSet = Nil;
	FreeUnlessNil(LogCaseNo);			LogCaseNo = Nil;
	FreeUnlessNil(LogFact);				LogFact = Nil;
    }

    FreeTreeData();

    FreeUnlessNil(Active);				Active = Nil;
							ActiveSpace = 0;

    FreeUnlessNil(UtilErr);				UtilErr = Nil;
    FreeUnlessNil(UtilBand);				UtilBand = Nil;
    FreeUnlessNil(UtilCost);				UtilCost = Nil;

    FreeUnlessNil(SomeMiss);				SomeMiss = Nil;
    FreeUnlessNil(SomeNA);				SomeNA = Nil;

    FreeNames();

    FreeUnlessNil(SubDef);				SubDef = Nil;
							SubSpace = 0;
    MaxCase = -1;

    NotifyStage(0);
}



#ifdef UTF8
///////////////////////////////////////////////////////////////////////////
//									 //
//	Routines for Unicode/UTF-8 processing				 //
//	-------------------------------------				 //
//									 //
///////////////////////////////////////////////////////////////////////////

#include <wchar.h>



/*************************************************************************/
/*									 */
/*	Determine the total character width of a UTF-8 string		 */
/*									 */
/*************************************************************************/


int UTF8CharWidth(unsigned char *U)
/*  -------------  */
{
    int		CWidth=0, Mask, This;
    wchar_t	Unicode;

    while ( *U )
    {
	Unicode = *U;

	if ( *U < 0x7F )
	{
	    /*  ASCII character  */

	    CWidth++;
	    U++;
	}
	else
	{
	    /*  Discard header bits  */

	    Mask = 0x80;
	    while ( Unicode & Mask )
	    {
		Unicode ^= Mask;
		Mask = Mask >> 1;
	    }

	    while ( ((*(++U)) & 0xc0) == 0x80 )
	    {
		Unicode = (Unicode << 6) | (*U & 0x3f);
	    }

	    if ( (This = wcwidth(Unicode)) > 0 ) CWidth += This;
	}
    }

    return CWidth;
}



////////////////////////////////////////////////////////////////////////////////
//	Public domain code to determine the width of a Unicode character      //
////////////////////////////////////////////////////////////////////////////////


/*
 * This is an implementation of wcwidth() and wcswidth() as defined in
 * "The Single UNIX Specification, Version 2, The Open Group, 1997"
 * <http://www.UNIX-systems.org/online.html>
 *
 * Markus Kuhn -- 2000-02-08 -- public domain
 */

//#include <wchar.h>

/* These functions define the column width of an ISO 10646 character
 * as follows:
 *
 *    - The null character (U+0000) has a column width of 0.
 *
 *    - Other C0/C1 control characters and DEL will lead to a return
 *      value of -1.
 *
 *    - Non-spacing and enclosing combining characters (general
 *      category code Mn or Me in the Unicode database) have a
 *      column width of 0.
 *
 *    - Spacing characters in the East Asian Wide (W) or East Asian
 *      FullWidth (F) category as defined in Unicode Technical
 *      Report #11 have a column width of 2.
 *
 *    - All remaining characters (including all printable
 *      ISO 8859-1 and WGL4 characters, Unicode control characters,
 *      etc.) have a column width of 1.
 *
 * This implementation assumes that wchar_t characters are encoded
 * in ISO 10646.
 */

int wcwidth(wchar_t ucs)
{
  /* sorted list of non-overlapping intervals of non-spacing characters */
  static const struct interval {
    unsigned short first;
    unsigned short last;
  } combining[] = {
    { 0x0300, 0x034E }, { 0x0360, 0x0362 }, { 0x0483, 0x0486 },
    { 0x0488, 0x0489 }, { 0x0591, 0x05A1 }, { 0x05A3, 0x05B9 },
    { 0x05BB, 0x05BD }, { 0x05BF, 0x05BF }, { 0x05C1, 0x05C2 },
    { 0x05C4, 0x05C4 }, { 0x064B, 0x0655 }, { 0x0670, 0x0670 },
    { 0x06D6, 0x06E4 }, { 0x06E7, 0x06E8 }, { 0x06EA, 0x06ED },
    { 0x0711, 0x0711 }, { 0x0730, 0x074A }, { 0x07A6, 0x07B0 },
    { 0x0901, 0x0902 }, { 0x093C, 0x093C }, { 0x0941, 0x0948 },
    { 0x094D, 0x094D }, { 0x0951, 0x0954 }, { 0x0962, 0x0963 },
    { 0x0981, 0x0981 }, { 0x09BC, 0x09BC }, { 0x09C1, 0x09C4 },
    { 0x09CD, 0x09CD }, { 0x09E2, 0x09E3 }, { 0x0A02, 0x0A02 },
    { 0x0A3C, 0x0A3C }, { 0x0A41, 0x0A42 }, { 0x0A47, 0x0A48 },
    { 0x0A4B, 0x0A4D }, { 0x0A70, 0x0A71 }, { 0x0A81, 0x0A82 },
    { 0x0ABC, 0x0ABC }, { 0x0AC1, 0x0AC5 }, { 0x0AC7, 0x0AC8 },
    { 0x0ACD, 0x0ACD }, { 0x0B01, 0x0B01 }, { 0x0B3C, 0x0B3C },
    { 0x0B3F, 0x0B3F }, { 0x0B41, 0x0B43 }, { 0x0B4D, 0x0B4D },
    { 0x0B56, 0x0B56 }, { 0x0B82, 0x0B82 }, { 0x0BC0, 0x0BC0 },
    { 0x0BCD, 0x0BCD }, { 0x0C3E, 0x0C40 }, { 0x0C46, 0x0C48 },
    { 0x0C4A, 0x0C4D }, { 0x0C55, 0x0C56 }, { 0x0CBF, 0x0CBF },
    { 0x0CC6, 0x0CC6 }, { 0x0CCC, 0x0CCD }, { 0x0D41, 0x0D43 },
    { 0x0D4D, 0x0D4D }, { 0x0DCA, 0x0DCA }, { 0x0DD2, 0x0DD4 },
    { 0x0DD6, 0x0DD6 }, { 0x0E31, 0x0E31 }, { 0x0E34, 0x0E3A },
    { 0x0E47, 0x0E4E }, { 0x0EB1, 0x0EB1 }, { 0x0EB4, 0x0EB9 },
    { 0x0EBB, 0x0EBC }, { 0x0EC8, 0x0ECD }, { 0x0F18, 0x0F19 },
    { 0x0F35, 0x0F35 }, { 0x0F37, 0x0F37 }, { 0x0F39, 0x0F39 },
    { 0x0F71, 0x0F7E }, { 0x0F80, 0x0F84 }, { 0x0F86, 0x0F87 },
    { 0x0F90, 0x0F97 }, { 0x0F99, 0x0FBC }, { 0x0FC6, 0x0FC6 },
    { 0x102D, 0x1030 }, { 0x1032, 0x1032 }, { 0x1036, 0x1037 },
    { 0x1039, 0x1039 }, { 0x1058, 0x1059 }, { 0x17B7, 0x17BD },
    { 0x17C6, 0x17C6 }, { 0x17C9, 0x17D3 }, { 0x18A9, 0x18A9 },
    { 0x20D0, 0x20E3 }, { 0x302A, 0x302F }, { 0x3099, 0x309A },
    { 0xFB1E, 0xFB1E }, { 0xFE20, 0xFE23 }
  };
  int min = 0;
  int max = sizeof(combining) / sizeof(struct interval) - 1;
  int mid;

  /* test for 8-bit control characters */
  if (ucs == 0)
    return 0;
  if (ucs < 32 || (ucs >= 0x7f && ucs < 0xa0))
    return -1;

  /* first quick check for Latin-1 etc. characters */
  if (ucs < combining[0].first)
    return 1;

  /* binary search in table of non-spacing characters */
  while (max >= min) {
    mid = (min + max) / 2;
    if (combining[mid].last < ucs)
      min = mid + 1;
    else if (combining[mid].first > ucs)
      max = mid - 1;
    else if (combining[mid].first <= ucs && combining[mid].last >= ucs)
      return 0;
  }

  /* if we arrive here, ucs is not a combining or C0/C1 control character */

  /* fast test for majority of non-wide scripts */
  if (ucs < 0x1100)
    return 1;

  return 1 +
    ((ucs >= 0x1100 && ucs <= 0x115f) || /* Hangul Jamo */
     (ucs >= 0x2e80 && ucs <= 0xa4cf && (ucs & ~0x0011) != 0x300a &&
      ucs != 0x303f) ||                  /* CJK ... Yi */
     (ucs >= 0xac00 && ucs <= 0xd7a3) || /* Hangul Syllables */
     (ucs >= 0xf900 && ucs <= 0xfaff) || /* CJK Compatibility Ideographs */
     (ucs >= 0xfe30 && ucs <= 0xfe6f) || /* CJK Compatibility Forms */
     (ucs >= 0xff00 && ucs <= 0xff5f) || /* Fullwidth Forms */
     (ucs >= 0xffe0 && ucs <= 0xffe6));
}


int wcswidth(const wchar_t *pwcs, size_t n)
{
  int w, width = 0;

  for (;*pwcs && n-- > 0; pwcs++)
    if ((w = wcwidth(*pwcs)) < 0)
      return -1;
    else
      width += w;

  return width;
}
#endif
/*************************************************************************/
/*									 */
/*  Copyright 2010 Rulequest Research Pty Ltd.				 */
/*									 */
/*  This file is part of C5.0 GPL Edition, a single-threaded version	 */
/*  of C5.0 release 2.07.						 */
/*									 */
/*  C5.0 GPL Edition is free software: you can redistribute it and/or	 */
/*  modify it under the terms of the GNU General Public License as	 */
/*  published by the Free Software Foundation, either version 3 of the	 */
/*  License, or (at your option) any later version.			 */
/*									 */
/*  C5.0 GPL Edition is distributed in the hope that it will be useful,	 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of	 */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	 */
/*  General Public License for more details.				 */
/*									 */
/*  You should have received a copy of the GNU General Public License	 */
/*  (gpl.txt) along with C5.0 GPL Edition.  If not, see 		 */
/*									 */
/*      <http://www.gnu.org/licenses/>.					 */
/*									 */
/*************************************************************************/



/*************************************************************************/
/*									 */
/*	Carry out crossvalidation trials				 */
/*	--------------------------------				 */
/*									 */
/*************************************************************************/



DataRec	*Blocked=Nil;
float	**Result=Nil;	/* Result[f][0] = tree/ruleset size
				    [1] = tree/ruleset errors
				    [2] = tree/ruleset cost  */



/*************************************************************************/
/*									 */
/*	Outer function (differs from xval script)			 */
/*									 */
/*************************************************************************/


void CrossVal()
/*   --------  */
{
    CaseNo	i, Size, Start=0, Next, SaveMaxCase;
    int		f, SmallTestBlocks, t, SaveTRIALS;
    ClassNo	c;
    static CaseNo *ConfusionMat=Nil;
    static int    SaveFOLDS=0;

    /*  Check for left-overs after interrupt  */

    if ( Result )
    {
	FreeVector((void **) Result, 0, SaveFOLDS-1);
	Free(ConfusionMat);
    }

    if ( FOLDS > MaxCase+1 )
    {
	fprintf(Of, T_FoldsReduced);
	FOLDS = MaxCase+1;
    }

    Result	 = AllocZero((SaveFOLDS = FOLDS), float *);
    Blocked	 = Alloc(MaxCase+1, DataRec);
    ConfusionMat = AllocZero((MaxClass+1)*(MaxClass+1), CaseNo);

    Prepare();

    SaveMaxCase = MaxCase;
    SaveTRIALS  = TRIALS;

    /*  First test blocks may be smaller than the others  */

    SmallTestBlocks = FOLDS - ((MaxCase+1) % FOLDS);
    Size = (MaxCase + 1) / FOLDS;

    ForEach(f, 0, FOLDS-1)
    {
	fprintf(Of, "\n\n[ " T_Fold " %d ]\n", f+1);
	Result[f] = AllocZero(3, float);

	if ( f == SmallTestBlocks ) Size++;
	MaxCase = SaveMaxCase - Size;

	ForEach(i, 0, MaxCase)
	{
	    Case[i] = Blocked[Start];
	    Start = (Start + 1) % (SaveMaxCase + 1);
	}

	ConstructClassifiers();

	/*  Check size (if appropriate) and errors  */

	if ( TRIALS == 1 )
	{
	    Result[f][0] = ( RULES ? RuleSet[0]->SNRules :
				     TreeSize(Pruned[0]) );
	    Next = Start;
	    ForEach(i, 0, Size-1)
	    {
		Case[i] = Blocked[Next];
		c = ( RULES ? RuleClassify(Blocked[Next], RuleSet[0]) :
			      TreeClassify(Blocked[Next], Pruned[0]) );
		if ( c != Class(Blocked[Next]) )
		{
		    Result[f][1] += 1.0;
		    if ( MCost )
		    {
			Result[f][2] += MCost[c][Class(Blocked[Next])];
		    }
		}

		/*  Add to confusion matrix for target classifier  */

		ConfusionMat[ Class(Blocked[Next])*(MaxClass+1)+c ]++;

		Next = (Next + 1) % (SaveMaxCase + 1);
	    }
	}
	else
	{
	    Result[f][0] = -1;
	    Next = Start;
	    Default = ( RULES ? RuleSet[0]->SDefault : Pruned[0]->Leaf );
	    ForEach(i, 0, Size-1)
	    {
		Case[i] = Blocked[Next];
		c = BoostClassify(Blocked[Next], TRIALS-1);
		if ( c != Class(Blocked[Next]) )
		{
		    Result[f][1] += 1.0;
		    if ( MCost )
		    {
			Result[f][2] += MCost[c][Class(Blocked[Next])];
		    }
		}

		/*  Add to confusion matrix for target classifier  */

		ConfusionMat[ Class(Blocked[Next])*(MaxClass+1)+c ]++;

		Next = (Next + 1) % (SaveMaxCase + 1);
	    }
	}

	Result[f][1] = (100.0 * Result[f][1]) / Size;
	Result[f][2] /= Size;

	fprintf(Of, T_EvalHoldOut, Size);
	MaxCase = Size-1;
	Evaluate(0);

	/*  Free space used by classifiers  */

	ForEach(t, 0, MaxTree)
	{
	    FreeClassifier(t);
	}
	MaxTree = -1;

	TRIALS = SaveTRIALS;
    }

    /*  Print summary of crossvalidation  */

    MaxCase = SaveMaxCase;

    Summary();
    PrintConfusionMatrix(ConfusionMat);

    /*  Free local storage  */

    ForEach(i, 0, MaxCase)
    {
	Case[i] = Blocked[i];
    }

    FreeVector((void **) Result, 0, FOLDS-1);		Result = Nil;
    Free(Blocked);					Blocked = Nil;
    Free(ConfusionMat);					ConfusionMat = Nil;
}



/*************************************************************************/
/*                                                                       */
/*      Prepare data for crossvalidation (similar to xval-prep.c)	 */
/*                                                                       */
/*************************************************************************/


void Prepare()
/*   -------  */
{
    CaseNo	i, First=0, Last, *Temp, Hold, Next=0;
    ClassNo	Group;

    Temp = Alloc(MaxCase+1, CaseNo);
    ForEach(i, 0, MaxCase)
    {
	Temp[i] = i;
    }

    Shuffle(Temp);

    /*  Sort into class groups  */

    while ( First <= MaxCase )
    {
	Last = First;
	Group = Class(Case[Temp[First]]);

	ForEach(i, First+1, MaxCase)
	{
	    if ( Class(Case[Temp[i]]) == Group )
	    {
		Last++;
		Hold = Temp[Last];
		Temp[Last] = Temp[i];
		Temp[i] = Hold;
	    }
	}

	First = Last+1;
    }

    /*  Organize into stratified blocks  */

    ForEach(First, 0, FOLDS-1)
    {
	for ( i = First ; i <= MaxCase ; i += FOLDS )
	{
	    Blocked[Next++] = Case[Temp[i]];
	}
    }

    Free(Temp);
}



/*************************************************************************/
/*                                                                       */
/*      Shuffle the data cases                                           */
/*                                                                       */
/*************************************************************************/


void Shuffle(int *Vec)
/*   -------  */
{
    int	This=0, Alt, Left=MaxCase+1, Hold;

    ResetKR(KRInit);

    while ( Left )
    {
	Alt = This + (Left--) * KRandom();

	Hold 	    = Vec[This];
	Vec[This++] = Vec[Alt];
	Vec[Alt]    = Hold;
    }
}



/*************************************************************************/
/*									 */
/*	Summarise a crossvalidation					 */
/*									 */
/*************************************************************************/


char
     *FoldHead[] = { F_Fold, F_UFold, "" };

void Summary()
/*   -------  */
{
    int		i, f, t;
    Boolean	PrintSize=true;
    float	Sum[3], SumSq[3];
    extern char	*StdP[], *StdPC[], *Extra[], *ExtraC[];

    for ( i = 0 ; i < 3 ; i++ )
    {
	Sum[i] = SumSq[i] = 0;
    }

    ForEach(f, 0, FOLDS-1)
    {
	if ( Result[f][0] < 1 ) PrintSize = false;
    }

    fprintf(Of, "\n\n[ " T_Summary " ]\n\n");

    ForEach(t, 0, 2)
    {
	fprintf(Of, "%s", FoldHead[t]);
	putc('\t', Of);
	if ( RULES )
	{
	    fprintf(Of, "%s", ( MCost ? ExtraC[t] : Extra[t] ));
	}
	else
	{
	    fprintf(Of, "%s", ( MCost ? StdPC[t] : StdP[t] ));
	}
	putc('\n', Of);
    }
    putc('\n', Of);

    ForEach(f, 0, FOLDS-1)
    {
	fprintf(Of, "%4d\t", f+1);

	if ( PrintSize )
	{
	    fprintf(Of, " %5g", Result[f][0]);
	}
	else
	{
	    fprintf(Of, "     *");
	}
	fprintf(Of, " %10.1f%%", Result[f][1]);

	if ( MCost )
	{
	    fprintf(Of, "%7.2f", Result[f][2]);
	}
	fprintf(Of, "\n");

	for ( i = 0 ; i < 3 ; i++ )
	{
	    Sum[i] += Result[f][i];
	    SumSq[i] += Result[f][i] * Result[f][i];
	}
    }

    fprintf(Of, "\n  " T_Mean "\t");

    if ( ! PrintSize )
    {
	fprintf(Of, "      ");
    }
    else
    {
	fprintf(Of, "%6.1f", Sum[0] / FOLDS);
    }

    fprintf(Of, " %10.1f%%", Sum[1] / FOLDS);

    if ( MCost )
    {
	fprintf(Of, "%7.2f", Sum[2] / FOLDS);
    }

    fprintf(Of, "\n  " T_SE "\t");

    if ( ! PrintSize )
    {
	fprintf(Of, "      ");
    }
    else
    {
	fprintf(Of, "%6.1f", SE(Sum[0], SumSq[0], FOLDS));
    }

    fprintf(Of, " %10.1f%%", SE(Sum[1], SumSq[1], FOLDS));

    if ( MCost )
    {
	fprintf(Of, "%7.2f", SE(Sum[2], SumSq[2], FOLDS));
    }
    fprintf(Of, "\n");
}



float SE(float sum, float sumsq, int no)
/*    --  */
{
    float mean;

    mean = sum / no;

    return sqrt( ((sumsq - no * mean * mean) / (no - 1)) / no );
}

int verify (Tree tree, DataRec * cases, struct cmap * implications, int maxCase) {

	/*
	 * 1) Verify all that all cases are classified correctly
	 */
	int i;
	int total_cases_checked = 0;
	int total_positive_cases_checked = 0;
	int total_negative_cases_checked = 0;
	int classified_cases_checked = 0;
	ForEach (i, 0, maxCase) {

		// Check if defined
		//if (!Unknown (cases[i], ClassAtt)) {
		if (DVal(cases[i], ClassAtt) > 0) {

			assert (DVal(cases[i], ClassAtt) == Class(cases[i]));
			classified_cases_checked += 1;

			if (DVal(cases[i], ClassAtt) == 1) {
				total_positive_cases_checked += 1;
			} else if (DVal(cases[i], ClassAtt) == 2) {
				total_negative_cases_checked += 1;
			}

			if (TreeClassify (cases[i], tree) != DVal(cases[i], ClassAtt)) {

				printf ("Tree classification of case %d does not match table classification!\n", i);
				return 0;

			}

		}

	}
	total_cases_checked = i;

	/*
	 * 2) Verify whether implications are satisfied
	 */
	int ok = 1;
	int total_implications_checked = 0;
	int truetrue_implications_checked = 0;
	int falsetrue_implications_checked = 0;
	int falsefalse_implications_checked = 0;

	// Check every key
	struct array * keys = cmap_get_keys (implications);
	for (i=0; i<keys->size; i++) {

		struct array * values = cmap_get (implications, keys->entries[i]);

		int j;
		for (j=0; j<values->size; j++) {

			// Get classification of lhs and rhs
			ClassNo lhs_class = TreeClassify (cases[keys->entries[i]], tree);
			ClassNo rhs_class = TreeClassify (cases[values->entries[j]], tree);

			total_implications_checked += 1;

			if (lhs_class == 1 && rhs_class == 1) {
				truetrue_implications_checked += 1;
			} else if (lhs_class == 2 && rhs_class == 1) {
				falsetrue_implications_checked += 1;
			} else if (lhs_class == 2 && rhs_class == 2) {
				falsefalse_implications_checked += 1;
			}


			if (!(lhs_class != 1 || rhs_class == 1)) {

				printf ("Implications %d -> %d is not satisfied!\n", keys->entries[i], values->entries[j]);
				ok = 0;
				break;

			}

		}

		delete_array (values);

		if (!ok) {
			break;
		}

	}

	delete_array (keys);

	// Output
	if (ok) {
		printf ("Tree passed %d total data checks (%d positive, %d negative; %d in total)\n", total_cases_checked, total_positive_cases_checked, total_negative_cases_checked, classified_cases_checked);	
		printf ("Tree passed %d total implication checks (%d 1->1; %d 2->1; %d 2->2)\n", total_implications_checked, truetrue_implications_checked, falsetrue_implications_checked, falsefalse_implications_checked);
	

    		printf("filename: %s\n", FileStem);
		
		FILE *F = GetFile(".out", "w+");
		assert(F != NULL);
		fprintf(F, "%d\n", truetrue_implications_checked);
		fprintf(F, "%d\n", falsetrue_implications_checked);
		fprintf(F, "%d\n", falsefalse_implications_checked);
		fclose(F);
	}

	return ok;

}


int are_equal (DataRec * c1, DataRec * c2, int max_case, int max_attribute) {

	int i;
	ForEach (i, 0, max_case) {
		int j;
		ForEach (j, 0, max_attribute) {
			if (memcmp (&c1[i][j], &c2[i][j], sizeof (AttValue)) != 0) {
				return 0;
			}
		}
	}

	return 1;

}
