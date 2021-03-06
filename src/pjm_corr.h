/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2021 Hans Bihs

This file is part of REEF3D.

REEF3D is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
Author: Hans Bihs
--------------------------------------------------------------------*/

#include"pressure.h"
#include"increment.h"
#include"field4.h"

class heat;
class concentration;
class density;

using namespace std;

#ifndef PJM_CORR_H_
#define PJM_CORR_H_

class pjm_corr : public pressure, public increment
{

public:

	pjm_corr(lexer* p, fdm *a, heat*&, concentration*&);
	virtual ~pjm_corr();

	virtual void start(fdm*,lexer* p, poisson*, solver*, ghostcell*, ioflow*, field&, field&, field&,double);
	virtual void rhs(lexer*,fdm*,ghostcell*,field&,field&,field&,double);
	virtual void vel_setup(lexer*,fdm*,ghostcell*,field&,field&,field&,double);
    virtual void presscorr(lexer*p,fdm *a,field&,field&,field&,field&, double);
	virtual void ucorr(lexer*p,fdm*,field&,double);
	virtual void vcorr(lexer*p,fdm*,field&,double);
	virtual void wcorr(lexer*p,fdm*,field&,double);
	virtual void upgrad(lexer*,fdm*);
	virtual void vpgrad(lexer*,fdm*);
	virtual void ptimesave(lexer*,fdm*,ghostcell*);

	virtual void wpgrad(lexer*,fdm*);
	virtual void fillapu(lexer*,fdm*);
	virtual void fillapv(lexer*,fdm*);
	virtual void fillapw(lexer*,fdm*);
	virtual void pressure_norm(lexer*,fdm*,ghostcell*);
    
    
    field4 pcorr;

private:
	double starttime,endtime;
	int count, gcval_press;
	int gcval_u, gcval_v, gcval_w;
	
	void debug(lexer*,fdm*);
    
    density *pd;
};



#endif

