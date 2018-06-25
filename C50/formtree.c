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


#include "defns.i"
#include "extern.i"

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



