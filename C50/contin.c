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

#include "defns.i"
#include "extern.i"

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
