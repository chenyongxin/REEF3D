/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2020 Hans Bihs

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
--------------------------------------------------------------------*/

#include"6DOF_f.h"
#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"
#include"fieldint.h"

void sixdof_f::ray_cast_io_ycorr(lexer *p, fdm *a, ghostcell *pgc, int ts, int te)
{
	double ys,ye,zs,ze;
	double Px,Py,Pz;
	double Qx,Qy,Qz;
	double Rx,Ry,Rz;
	double Ax,Ay,Az;
	double Bx,By,Bz;
	double Cx,Cy,Cz;
	double PQx,PQy,PQz;
	double PAx,PAy,PAz;
	double PBx,PBy,PBz;
	double PCx,PCy,PCz;
	double Mx,My,Mz;
	int is,ie,js,je,ks,ke;
	int ir,insidecheck;
	double u,v,w;
	double denom;
	double psi = 1.0e-8*p->DXM;

    ALOOP
	{
	cutl(i,j,k)=0;
	cutr(i,j,k)=0;
	}

	for(n=ts; n<te; ++n)
	{
	Ax = tri_x[n][0]-p->originx;
	Ay = tri_y[n][0]-p->originy;
	Az = tri_z[n][0]-p->originz;
		
	Bx = tri_x[n][1]-p->originx;
	By = tri_y[n][1]-p->originy;
	Bz = tri_z[n][1]-p->originz;
		
	Cx = tri_x[n][2]-p->originx;
	Cy = tri_y[n][2]-p->originy;
	Cz = tri_z[n][2]-p->originz;
	
	
	xs = MIN3(Ax,Bx,Cx);
	xe = MAX3(Ax,Bx,Cx);
	
	zs = MIN3(Az,Bz,Cz);
	ze = MAX3(Az,Bz,Cz);	
	
	is = p->posf_i(xs+p->originx);
	ie = p->posf_i(xe+p->originx);
	
	ks = p->posf_k(zs+p->originz);
	ke = p->posf_k(ze+p->originz);
    
	xs = MIN3(Ax,Bx,Cx) - epsi*p->XP[is +marge-1];
	xe = MAX3(Ax,Bx,Cx) + epsi*p->XP[ie +marge+1];
	
	zs = MIN3(Az,Bz,Cz) - epsi*p->ZP[ks +marge-1];
	ze = MAX3(Az,Bz,Cz) + epsi*p->ZP[ke +marge+1];
	
	
	is = p->posf_i(xs+p->originx);
	ie = p->posf_i(xe+p->originx);
	
	ks = p->posf_k(zs+p->originz);
	ke = p->posf_k(ze+p->originz);

	
	is = MAX(is,0);
	ie = MIN(ie,p->knox);
	
	ks = MAX(ks,0);
	ke = MIN(ke,p->knoz);
	
	
		for(i=is;i<ie;i++)
		for(k=ks;k<ke;k++)
		{
		Px = p->XP[IP]+psi-p->originx;
		Py = p->global_ymin-10.0*p->DXM ;
		Pz = p->ZP[KP]+psi-p->originz;
		
		Qx = p->XP[IP]+psi-p->originx;
		Qy = p->global_ymax+10.0*p->DXM ;
		Qz = p->ZP[KP]+psi-p->originz;
		
		
		PQx = Qx-Px;
		PQy = Qy-Py;
		PQz = Qz-Pz;
		
		PAx = Ax-Px;
		PAy = Ay-Py;
		PAz = Az-Pz;
		
		PBx = Bx-Px;
		PBy = By-Py;
		PBz = Bz-Pz;
		
		PCx = Cx-Px;
		PCy = Cy-Py;
		PCz = Cz-Pz;
		
		// uvw
		Mx = PQy*Pz - PQz*Py;
		My = PQz*Px - PQx*Pz;
		Mz = PQx*Py - PQy*Px;

		
		u = PQx*(Cy*Bz - Cz*By) + PQy*(Cz*Bx - Cx*Bz) + PQz*(Cx*By - Cy*Bx)
		  + Mx*(Cx-Bx) + My*(Cy-By) + Mz*(Cz-Bz);
		  
		v = PQx*(Ay*Cz - Az*Cy) + PQy*(Az*Cx - Ax*Cz) + PQz*(Ax*Cy - Ay*Cx)
		  + Mx*(Ax-Cx) + My*(Ay-Cy) + Mz*(Az-Cz);
		  
		w = PQx*(By*Az - Bz*Ay) + PQy*(Bz*Ax - Bx*Az) + PQz*(Bx*Ay - By*Ax)
		  + Mx*(Bx-Ax) + My*(By-Ay) + Mz*(Bz-Az);
		
		
		int check=1;
		if(u==0.0 && v==0.0 && w==0.0)
		check = 0;
		
			if((u>=0.0 && v>=0.0 && w>=0.0) || (u<0.0 && v<0.0 && w<0.0) && check==1)
			{
			denom = 1.0/(u+v+w);
			u *= denom;
			v *= denom;
			w *= denom;
			
			Ry = u*Ay + v*By + w*Cy;
			
			Ry+=p->originy;
            
			for(j=0;j<p->knoy;++j)
            {
				if(p->YP[JP]-p->originy<Ry)
				cutr(i,j,k) += 1;
				
				if(p->YP[JP]-p->originy>=Ry)
				cutl(i,j,k) += 1;
            }
            
			}
		}
	}
    
    
    count=0;
	ALOOP
	if(a->fb(i,j,k)>0)
	++count;
    
    cout<<"Number of active cells after fb_ray_io_y_corr: "<<count<<endl;

}