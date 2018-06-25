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


#include "defns.i"
#include "extern.i"

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
