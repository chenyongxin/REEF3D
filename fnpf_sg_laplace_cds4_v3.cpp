/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2018 Hans Bihs

This file is part of REEF3D.

REEF3D is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
--------------------------------------------------------------------*/

#include"fnpf_sg_laplace_cds4_v3.h"
#include"lexer.h"
#include"fdm_fnpf.h"
#include"solver.h"
#include"ghostcell.h"

fnpf_sg_laplace_cds4_v3::fnpf_sg_laplace_cds4_v3() 
{
}

fnpf_sg_laplace_cds4_v3::~fnpf_sg_laplace_cds4_v3()
{
}

void fnpf_sg_laplace_cds4_v3::start(lexer* p, fdm_fnpf *c, ghostcell *pgc, solver *psolv, fnpf_sg_fsfbc *pf, double *f)
{
    // see p. 1130-1132
    
    double sigxyz2;
    double ab,abb,abbb,denom;
    double fbxm,fbxp,fbym,fbyp;
    double distfac;
	n=0;
    
    // 4th-order
	n=0;
    LOOP
    {
        sigxyz2 = pow(p->sigx[FIJK],2.0) + pow(p->sigy[FIJK],2.0) + pow(p->sigz[FIJK],2.0);
        
        c->M.p[n] = 2.5/(p->DXN[IP]*p->DXN[IP])*p->x_dir 
                  + 2.5/(p->DYN[JP]*p->DYN[JP])*p->y_dir 
                  + sigxyz2*2.5/(p->DZN[KP]*p->DZN[KP])*p->z_dir; 
        
        
        c->M.n[n] = -(4.0/3.0)/(p->DXN[IP]*p->DXN[IP])*p->x_dir; 
        c->M.s[n] = -(4.0/3.0)/(p->DXN[IP]*p->DXN[IP])*p->x_dir;

        c->M.w[n] = -(4.0/3.0)/(p->DYN[JP]*p->DYN[JP])*p->y_dir; 
        c->M.e[n] = -(4.0/3.0)/(p->DYN[JP]*p->DYN[JP])*p->y_dir; 

        c->M.t[n] = -(sigxyz2*(4.0/3.0)/(p->DZN[KP]*p->DZN[KP])  + p->sigxx[FIJK]/(p->DZN[KP]+p->DZN[KM1]))*p->z_dir;
        c->M.b[n] = -(sigxyz2*(4.0/3.0)/(p->DZN[KP]*p->DZN[KP]) - p->sigxx[FIJK]/(p->DZN[KP]+p->DZN[KM1]))*p->z_dir;
        
        c->M.tt[n] = sigxyz2*(1.0/12.0)/(p->DZN[KP]*p->DZN[KP])*p->z_dir; 
        c->M.bb[n] = sigxyz2*(1.0/12.0)/(p->DZN[KP]*p->DZN[KP])*p->z_dir; 
        
        if(k==0 || k==p->knoz-1)
        {
        c->M.t[n] = -(sigxyz2*(3.0/3.0)/(p->DZN[KP]*p->DZN[KP])  + p->sigxx[FIJK]/(p->DZN[KP]+p->DZN[KM1]))*p->z_dir;
        c->M.b[n] = -(sigxyz2*(3.0/3.0)/(p->DZN[KP]*p->DZN[KP]) - p->sigxx[FIJK]/(p->DZN[KP]+p->DZN[KM1]))*p->z_dir;
        
        c->M.tt[n] = 0.0; 
        c->M.bb[n] = 0.0; 
        
        
        c->M.p[n] = 2.5/(p->DXN[IP]*p->DXN[IP])*p->x_dir 
                  + 2.5/(p->DYN[JP]*p->DYN[JP])*p->y_dir 
                  + sigxyz2*2.0/(p->DZN[KP]*p->DZN[KP])*p->z_dir; 
        }
        
        
        
        c->M.nn[n] = (1.0/12.0)/(p->DXN[IP]*p->DXN[IP])*p->x_dir;
        c->M.ss[n] = (1.0/12.0)/(p->DXN[IP]*p->DXN[IP])*p->x_dir;
        
        c->M.ww[n] = (1.0/12.0)/(p->DYN[JP]*p->DYN[JP])*p->y_dir;
        c->M.ee[n] = (1.0/12.0)/(p->DYN[JP]*p->DYN[JP])*p->y_dir;
         
        
        
            
        c->rhsvec.V[n] = 2.0*p->sigx[IJK]*(f[FIp1JKp1] - f[FIm1JKp1] - f[FIp1JKm1] + f[FIm1JKm1])
                        /((p->DXN[IP]+p->DXN[IM1])*(p->DZN[KP]+p->DZN[KM1]))*p->x_dir
                        
                        +2.0*p->sigy[IJK]*(f[FIJp1Kp1] - f[FIJm1Kp1] - f[FIJp1Km1] + f[FIJm1Km1])
                        /((p->DYN[JP]+p->DYN[JM1])*(p->DZN[KP]+p->DZN[KM1]))*p->y_dir;
          /*             
        c->rhsvec.V[n] +=   (f[FIp3JK]*(1.0/X1) + f[FIm3JK]*(1.0/X4))*p->x_dir
                        +  (f[FIJp3K]*(1.0/Y1) + f[FIJm3K]*(1.0/Y4))*p->y_dir
                        +  sigxyz2*(f[FIJKp3]*(1.0/Z1) + f[FIJKm3]*(1.0/Z4))*p->z_dir;*/
        ++n;
        }
    
    
     n=0;
	LOOP
    if(p->flag7[FIJK]>0)
	{
        
    /*
            if(p->flag7[FIm1JK]<0)
            {
            c->rhsvec.V[n] -= c->M.s[n]*f[FIJK];
            c->M.s[n] = 0.0;
            }
            
            if(p->flag7[FIp1JK]<0)
            {
            c->rhsvec.V[n] -= c->M.n[n]*f[FIJK];
            c->M.n[n] = 0.0;
            }*/
            
            if(p->flag7[FIm1JK]<0)
            {
            c->M.p[n] += c->M.s[n];
            c->M.s[n] = 0.0;
            }
            
            if(p->flag7[FIp1JK]<0)
            {
            c->M.p[n] += c->M.n[n];
            c->M.n[n] = 0.0;
            }
            
            if(p->flag7[FIJm1K]<0)
            {
            c->M.p[n] += c->M.e[n];
            c->M.e[n] = 0.0;
            }
            
            if(p->flag7[FIJp1K]<0)
            {
            c->M.p[n] += c->M.w[n];
            c->M.w[n] = 0.0;
            }
            
            if(p->flag7[FIJKp2]<0)
            {
            c->rhsvec.V[n] -= c->M.t[n]*f[FIJKp1];
            c->M.t[n] = 0.0;
            }
            
            
           //--
           /*
            if(p->flag7[FIm2JK]<0)
            {
            c->rhsvec.V[n] -= c->M.ss[n]*f[FIJK];
            c->M.ss[n] = 0.0;
            }
            
            if(p->flag7[FIp2JK]<0)
            {
            c->rhsvec.V[n] -= c->M.nn[n]*f[FIJK];
            c->M.nn[n] = 0.0;
            }
            */
            
            if(p->flag7[FIm2JK]<0)
            {
            c->M.p[n] += c->M.ss[n];
            c->M.ss[n] = 0.0;
            }
            
            if(p->flag7[FIp2JK]<0)
            {
            c->M.p[n] += c->M.nn[n];
            c->M.nn[n] = 0.0;
            }
            
            if(p->flag7[FIJm2K]<0)
            {
            c->M.p[n] += c->M.ee[n];
            c->M.ee[n] = 0.0;
            }
            
            if(p->flag7[FIJp2K]<0)
            {
            c->M.p[n] += c->M.ww[n];
            c->M.ww[n] = 0.0;
            }

            if(p->flag7[FIJKp3]<0)
            {
            c->rhsvec.V[n] -= c->M.tt[n]*f[FIJKp3];
            c->M.tt[n] = 0.0;
            }
            
            
    
        // KBEDBC
        if(p->flag7[FIJKm1]<0 || p->flag7[FIJKm2]<0 )
        {
            ab  = c->M.b[n];
            abb = c->M.bb[n];
            
            // ------
            if(p->flag7[FIJKm2]<0 && p->flag7[FIJKm1]>0)
            {
            // bb
            denom = p->sigz[FIJK] + c->Bx(i,j)*p->sigx[FIJK] + c->By(i,j)*p->sigy[FIJK];
            
            c->M.n[n] += abb*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.s[n] += -abb*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.e[n] += abb*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -abb*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.p[n] += abb;
            c->M.bb[n] = 0.0;
            }
            
            
            // ------
            if(p->flag7[FIJKm2]<0 && p->flag7[FIJKm1]<0)
            {
            denom = p->sigz[FIJK] + c->Bx(i,j)*p->sigx[FIJK] + c->By(i,j)*p->sigy[FIJK];
            
            // b
            fbxp = f[FIp1JKp1] + (2.0*p->DZP[KP]*(c->Bx(i+1,j)*((c->Fi[FIp2JK]-c->Fi[FIJK])/(p->DXP[IP] + p->DXP[IM1]))
                                                 +c->By(i+1,j)*((c->Fi[FIp1Jp1K]-c->Fi[FIp1Jm1K])/(p->DYP[JP] + p->DYP[JM1]))))
                                /(p->sigz[FIp1JK] + c->Bx(i+1,j)*p->sigx[FIp1JK] + c->By(i+1,j)*p->sigy[FIp1JK]);
            
            fbxm = f[FIm1JKp1] + (2.0*p->DZP[KP]*(c->Bx(i-1,j)*((c->Fi[FIJK]-c->Fi[FIm2JK])/(p->DXP[IP] + p->DXP[IM1]))
                                                 +c->By(i-1,j)*((c->Fi[FIm1Jp1K]-c->Fi[FIm1Jm1K])/(p->DYP[JP] + p->DYP[JM1]))))
                                /(p->sigz[FIm1JK] + c->Bx(i-1,j)*p->sigx[FIm1JK] + c->By(i-1,j)*p->sigy[FIm1JK]);
            
            fbyp = f[FIJp1Kp1] + (2.0*p->DZP[KP]*(c->Bx(i,j+1)*((c->Fi[FIp1Jp1K]-c->Fi[FIm1Jp1K])/(p->DXP[IP] + p->DXP[IM1]))
                                                 +c->By(i,j+1)*((c->Fi[FIJp2K]-c->Fi[FIJK])/(p->DYP[JP] + p->DYP[JM1]))))
                                /(p->sigz[FIJp1K] + c->Bx(i,j+1)*p->sigx[FIJp1K] + c->By(i,j+1)*p->sigy[FIJp1K]);
            
            fbym = f[FIJm1Kp1] + (2.0*p->DZP[KP]*(c->Bx(i,j-1)*((c->Fi[FIp1Jm1K]-c->Fi[FIm1Jm1K])/(p->DXP[IP] + p->DXP[IM1]))
                                                 +c->By(i,j-1)*((c->Fi[FIJK]-c->Fi[FIJm2K])/(p->DYP[JP] + p->DYP[JM1]))))
                                /(p->sigz[FIJm1K] + c->Bx(i,j-1)*p->sigx[FIJm1K] + c->By(i,j-1)*p->sigy[FIJm1K]);
                                                 
            
            //--
            c->rhsvec.V[n] -=  2.0*p->sigx[FIJK]*(f[FIp1JKp1] - f[FIm1JKp1] - f[FIp1JKm1] + f[FIm1JKm1])
                    /((p->DXN[IP]+p->DXN[IM1])*(p->DZN[KP]+p->DZN[KM1]))*p->x_dir
                    
                    + 2.0*p->sigy[FIJK]*(f[FIJp1Kp1] - f[FIJm1Kp1] - f[FIJp1Km1] + f[FIJm1Km1])
                    /((p->DYN[JP]+p->DYN[JM1])*(p->DZN[KP]+p->DZN[KM1]))*p->y_dir;
                    
            c->rhsvec.V[n] +=  2.0*p->sigx[FIJK]*(f[FIp1JKp1] - f[FIm1JKp1] - fbxp + fbxm)
                    /((p->DXN[IP]+p->DXN[IM1])*(p->DZN[KP]+p->DZN[KM1]))*p->x_dir
                    
                    + 2.0*p->sigy[FIJK]*(f[FIJp1Kp1] - f[FIJm1Kp1] - fbyp + fbym)
                    /((p->DYN[JP]+p->DYN[JM1])*(p->DZN[KP]+p->DZN[KM1]))*p->y_dir;
                    
        
            
            c->M.n[n] += ab*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));     
            c->M.s[n] += -ab*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));    
            c->M.e[n] += ab*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -ab*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));  
            c->M.t[n] += ab;
            c->M.b[n] = 0.0;

            // bb 
            distfac = 2.0;
            /*
            c->M.n[n] += abb*distfac*(p->DZN[KP]+p->DZN[KP1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.s[n] += -abb*distfac*(p->DZN[KP]+p->DZN[KP1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.e[n] += abb*distfac*(p->DZN[KP]+p->DZN[KP1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -abb*distfac*(p->DZN[KP]+p->DZN[KP1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.tt[n] += abb;
            c->M.bb[n] = 0.0;*/
            }
            

        }
	++n;
	}
    
    
    double starttime=pgc->timer();
    psolv->startF(p,pgc,f,c->rhsvec,c->M,10,250,p->N44);
    double endtime=pgc->timer();
    
    p->poissoniter=p->solveriter;
    p->poissontime=endtime-starttime;
	if(p->mpirank==0 && innercounter==p->N50-1 && (p->count%p->P12==0))
	cout<<"Fi_iter: "<<p->solveriter<<"  Fi_time: "<<setprecision(3)<<p->poissontime<<endl;
}




/*
n=0;
	LOOP
	{
        if(p->flag7[FIJK]>0)
        {
    
            if(p->flag7[FIm1JK]<0)
            {
            c->rhsvec.V[n] -= c->M.s[n]*f[FIJK];
            c->M.s[n] = 0.0;
            }
            
            if(p->flag7[FIp1JK]<0)
            {
            c->rhsvec.V[n] -= c->M.n[n]*f[FIJK];
            c->M.n[n] = 0.0;
            }
            
            if(p->flag7[FIJm1K]<0)
            {
            c->rhsvec.V[n] -= c->M.e[n]*f[FIJK];
            c->M.e[n] = 0.0;
            }
            
            if(p->flag7[FIJp1K]<0)
            {
            c->rhsvec.V[n] -= c->M.w[n]*f[FIJK];
            c->M.w[n] = 0.0;
            }

            if(p->flag7[FIJKp2]<0 && p->flag7[FIJKp1]>0)
            {
            c->rhsvec.V[n] -= c->M.t[n]*f[FIJKp1];
            c->M.t[n] = 0.0;
            }
            
            
            //--
            if(p->flag7[FIm2JK]<0)
            {
            c->rhsvec.V[n] -= c->M.ss[n]*f[FIJK];
            c->M.ss[n] = 0.0;
            }
            
            if(p->flag7[FIp2JK]<0)
            {
            c->rhsvec.V[n] -= c->M.nn[n]*f[FIJK];
            c->M.nn[n] = 0.0;
            }
            
            if(p->flag7[FIJm2K]<0)
            {
            c->rhsvec.V[n] -= c->M.ee[n]*f[FIJK];
            c->M.ee[n] = 0.0;
            }
            
            if(p->flag7[FIJp2K]<0)
            {
            c->rhsvec.V[n] -= c->M.ww[n]*f[FIJK];
            c->M.ww[n] = 0.0;
            }
            
            if(p->flag7[FIJKp3]<0)
            {
            c->rhsvec.V[n] -= c->M.tt[n]*f[FIJKp2];
            c->M.tt[n] = 0.0;
            }
            
            //-------
            if(p->flag7[FIm3JK]<0)
            {
            c->rhsvec.V[n] -= c->M.sss[n]*f[FIJK];
            c->M.sss[n] = 0.0;
            }
            
            if(p->flag7[FIp3JK]<0)
            {
            c->rhsvec.V[n] -= c->M.nnn[n]*f[FIJK];
            c->M.nnn[n] = 0.0;
            }
            
            if(p->flag7[FIJm3K]<0)
            {
            c->rhsvec.V[n] -= c->M.eee[n]*f[FIJK];
            c->M.eee[n] = 0.0;
            }
            
            if(p->flag7[FIJp3K]<0)
            {
            c->rhsvec.V[n] -= c->M.www[n]*f[FIJK];
            c->M.www[n] = 0.0;
            }
            
            if(p->flag7[FIJKp4]<0)
            {
            c->rhsvec.V[n] -= c->M.ttt[n]*f[FIJKp3];
            c->M.ttt[n] = 0.0;
            }
            
            
        // KBEDBC
        if(p->flag7[FIJKm1]<0 || p->flag7[FIJKm2]<0  || p->flag7[FIJKm3]<0 )
        {
            sigxyz2 = pow(p->sigx[FIJK],2.0) + pow(p->sigy[FIJK],2.0) + pow(p->sigz[FIJK],2.0);
            
            Z0 = -0.5*p->ZN[KP2] + 13.0*p->ZN[KP1] - 13.0*p->ZN[KM1] + 0.5*p->ZN[KM2];
            Z1 = (-p->ZN[KP3] + 27.0*p->ZN[KP2] -27.0*p->ZN[KP1] + p->ZN[KP])*Z0;
            Z2 = (-p->ZN[KP2] + 27.0*p->ZN[KP1] -27.0*p->ZN[KP] + p->ZN[KM1])*Z0;
            Z3 = (-p->ZN[KP1] + 27.0*p->ZN[KP] -27.0*p->ZN[KM1] + p->ZN[KM2])*Z0;
            Z4 = (-p->ZN[KP] + 27.0*p->ZN[KM1] -27.0*p->ZN[KM2] + p->ZN[KM3])*Z0;
            
            // ------
            if(p->flag7[FIJKm3]<0 && p->flag7[FIJKm2]>0 && p->flag7[FIJKm1]>0)
            {
            // bbb
            abbb = -(1.0/Z4)*p->z_dir;
            denom = p->sigz[FIJK] + c->Bx(i,j)*p->sigx[FIJK] + c->By(i,j)*p->sigy[FIJK];
            
            c->M.n[n] += abb*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.s[n] += -abb*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.e[n] += abb*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -abb*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.b[n] += abbb;
            c->M.bbb[n] = 0.0;
            }
            
            // ------
            if(p->flag7[FIJKm3]<0 && p->flag7[FIJKm2]<0 && p->flag7[FIJKm1]>0)
            {
            // bb
            abb = sigxyz2*(27.0/Z3 + 27.0/Z4)*p->z_dir;
            denom = p->sigz[FIJK] + c->Bx(i,j)*p->sigx[FIJK] + c->By(i,j)*p->sigy[FIJK];
            
            c->M.n[n] += abb*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.s[n] += -abb*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.e[n] += abb*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -abb*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.p[n] += abb;
            c->M.bb[n] = 0.0;
            
            // bbb
            abbb = -(1.0/Z4)*p->z_dir;
            denom = p->sigz[FIJK] + c->Bx(i,j)*p->sigx[FIJK] + c->By(i,j)*p->sigy[FIJK];
            
            c->M.n[n] += abb*2.0*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.s[n] += -abb*2.0*(p->DZN[KP]+p->DZN[KM1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.e[n] += abb*2.0*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -abb*2.0*(p->DZN[KP]+p->DZN[KM1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.t[n] += abbb;
            c->M.bbb[n] = 0.0;
            }
            
            
            // ------
            if(p->flag7[FIJKm3]<0 && p->flag7[FIJKm2]<0 && p->flag7[FIJKm1]<0)
            {
            // b
            ab = - (sigxyz2*(27.0/Z2 + 729.0/Z3 + 27.0/Z4) - p->sigxx[FIJK]/(p->DZN[KP]+p->DZN[KM1]));
            denom = p->sigz[FIJK] + c->Bx(i,j)*p->sigx[FIJK] + c->By(i,j)*p->sigy[FIJK];
            
            fbxp = f[FIp1JKp1] + (2.0*p->DZP[KP]*(c->Bx(i+1,j)*((c->Fi[FIp2JK]-c->Fi[FIJK])/(p->DXP[IP] + p->DXP[IM1]))
                                                 +c->By(i+1,j)*((c->Fi[FIp1Jp1K]-c->Fi[FIp1Jm1K])/(p->DYP[JP] + p->DYP[JM1]))))
                                /(p->sigz[FIp1JK] + c->Bx(i+1,j)*p->sigx[FIp1JK] + c->By(i+1,j)*p->sigy[FIp1JK]);
            
            fbxm = f[FIm1JKp1] + (2.0*p->DZP[KP]*(c->Bx(i-1,j)*((c->Fi[FIJK]-c->Fi[FIm2JK])/(p->DXP[IP] + p->DXP[IM1]))
                                                 +c->By(i-1,j)*((c->Fi[FIm1Jp1K]-c->Fi[FIm1Jm1K])/(p->DYP[JP] + p->DYP[JM1]))))
                                /(p->sigz[FIm1JK] + c->Bx(i-1,j)*p->sigx[FIm1JK] + c->By(i-1,j)*p->sigy[FIm1JK]);
            
            fbyp = f[FIJp1Kp1] + (2.0*p->DZP[KP]*(c->Bx(i,j+1)*((c->Fi[FIp1Jp1K]-c->Fi[FIm1Jp1K])/(p->DXP[IP] + p->DXP[IM1]))
                                                 +c->By(i,j+1)*((c->Fi[FIJp2K]-c->Fi[FIJK])/(p->DYP[JP] + p->DYP[JM1]))))
                                /(p->sigz[FIJp1K] + c->Bx(i,j+1)*p->sigx[FIJp1K] + c->By(i,j+1)*p->sigy[FIJp1K]);
            
            fbym = f[FIJm1Kp1] + (2.0*p->DZP[KP]*(c->Bx(i,j-1)*((c->Fi[FIp1Jm1K]-c->Fi[FIm1Jm1K])/(p->DXP[IP] + p->DXP[IM1]))
                                                 +c->By(i,j-1)*((c->Fi[FIJK]-c->Fi[FIJm2K])/(p->DYP[JP] + p->DYP[JM1]))))
                                /(p->sigz[FIJm1K] + c->Bx(i,j-1)*p->sigx[FIJm1K] + c->By(i,j-1)*p->sigy[FIJm1K]);
                                                 
            
            //--
            c->rhsvec.V[n] -=  2.0*p->sigx[FIJK]*(f[FIp1JKp1] - f[FIm1JKp1] - f[FIp1JKm1] + f[FIm1JKm1])
                    /((p->DXN[IP]+p->DXN[IM1])*(p->DZN[KP]+p->DZN[KM1]))*p->x_dir
                    
                    + 2.0*p->sigy[FIJK]*(f[FIJp1Kp1] - f[FIJm1Kp1] - f[FIJp1Km1] + f[FIJm1Km1])
                    /((p->DYN[JP]+p->DYN[JM1])*(p->DZN[KP]+p->DZN[KM1]))*p->y_dir;
                    
            c->rhsvec.V[n] +=  2.0*p->sigx[FIJK]*(f[FIp1JKp1] - f[FIm1JKp1] - fbxp + fbxm)
                    /((p->DXN[IP]+p->DXN[IM1])*(p->DZN[KP]+p->DZN[KM1]))*p->x_dir
                    
                    + 2.0*p->sigy[FIJK]*(f[FIJp1Kp1] - f[FIJm1Kp1] - fbyp + fbym)
                    /((p->DYN[JP]+p->DYN[JM1])*(p->DZN[KP]+p->DZN[KM1]))*p->y_dir;
                    
        
            
            c->M.n[n] += ab*2.0*p->DZN[KP]*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));     
            c->M.s[n] += -ab*2.0*p->DZN[KP]*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));    
            c->M.e[n] += ab*2.0*p->DZN[KP]*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -ab*2.0*p->DZN[KP]*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));  
            c->M.t[n] += ab;
            c->M.b[n] = 0.0;

            // bb
            abb = sigxyz2*(27.0/Z3 + 27.0/Z4)*p->z_dir;
            denom = p->sigz[FIJK] + c->Bx(i,j)*p->sigx[FIJK] + c->By(i,j)*p->sigy[FIJK];
            
            distfac = 2.0;
            
            c->M.n[n] += abb*distfac*(p->DZN[KP]+p->DZN[KP1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.s[n] += -abb*distfac*(p->DZN[KP]+p->DZN[KP1])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.e[n] += abb*distfac*(p->DZN[KP]+p->DZN[KP1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -abb*distfac*(p->DZN[KP]+p->DZN[KP1])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.tt[n] += abb;
            c->M.bb[n] = 0.0;
            
            
            // bbb
            abbb = -(1.0/Z4)*p->z_dir;
            denom = p->sigz[FIJK] + c->Bx(i,j)*p->sigx[FIJK] + c->By(i,j)*p->sigy[FIJK];
            
            c->M.n[n] += abb*2.0*(p->DZN[KP]+p->DZN[KP1]+p->DZN[KP2])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.s[n] += -abb*2.0*(p->DZN[KP]+p->DZN[KP1]+p->DZN[KP2])*c->Bx(i,j)/(denom*(p->DXP[IP] + p->DXP[IM1]));
            c->M.e[n] += abb*2.0*(p->DZN[KP]+p->DZN[KP1]+p->DZN[KP2])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.w[n] += -abb*2.0*(p->DZN[KP]+p->DZN[KP1]+p->DZN[KP2])*c->By(i,j)/(denom*(p->DYP[JP] + p->DYP[JM1]));
            c->M.ttt[n] += abbb;
            c->M.bbb[n] = 0.0;
            }
        }
        }
	++n;
	}*/