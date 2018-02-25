#include "seahorn/seahorn.h"

//This example is adapted from StInG 
void main()
{
	int I = unknown();
	int Sa = unknown();
	int Ea = unknown();
	int Ma = unknown();
	int Sb = unknown();
	int Eb = unknown();
	int Mb = unknown();

	if (! (I>=1)) return;
	Sa=0;
	Ea=0;
	Ma=0;
	Sb=0;
	Eb=0;
	Mb=0;

	while(unknown())
	{
		if (unknown())
		{
			if (! (Eb >=1)) return;
			Eb = Eb -1;
			Mb = Mb +1;
		}
		else
		{
			if (unknown())
			{
				if (! (Ea >=1)) return;
				Ea = Ea -1;
				Ma = Ma +1;
			}
			else
			{
				if (unknown())
				{
					if (! (Sa>=1)) return;
					Sa=Sa-1;
					I=I+Sb+Eb+Mb;
					Sb=0;
					Eb=1;
					Mb=0;

				}
				else
				{
					if (unknown())
					{
						if (! (Sb>=1)) return;
						I=I+Sb+Eb+Mb;
						Sb=0;
						Eb=1;
						Mb=0;
					}
					else
					{
						if (unknown())
						{

							if (! (Sb>=1)) return;
							Sb=Sb-1;
							I=I+Sa+Ea+Ma;
							Sa=0;
							Ea=1;
							Ma=0;

						}
						else
						{
							if (unknown())
							{
								if (! (Sa>=1)) return;
								I=I+Sa+Ea+Ma;
								Sa=0;
								Ea=1;
								Ma=0;
							}
							else
							{
								if (unknown())
								{
									if (! (Sa>=1)) return;
									Sa=Sa-1;
									Sb=Sb+Eb+Mb+1;
									Eb=0;
									Mb=0;
								}
								else
								{
									if (unknown())
									{
										if (! (I>=1)) return;
										I=I-1;
										Sb=Sb+Eb+Mb+1;
										Eb=0;
										Mb=0;
									}
									else
									{
										if (unknown())
										{
											if (! (I >= 1)) return;
											I = I -1;
											Sa = Sa + Ea + Ma + 1;
											Ea = 0;
											Ma =0;
										}
										else
										{
											if (! (Sb >= 1)) return;
											Sb = Sb-1;
											Sa = Ea+Ma+1;
											Ea = 0;
											Ma = 0;

										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	sassert (Ea + Ma <= 1);
	sassert (Eb + Mb <= 1);
	sassert (I >= 0);
	sassert (Sa >= 0);
	sassert (Ma  >= 0);
	sassert (Ea  >= 0);
	sassert (Sb >= 0);
	sassert (Mb  >= 0);
	sassert (Eb  >= 0);
	sassert (I + Sa + Ea + Ma + Sb + Eb + Mb >= 1);
}

