#include<stdio.h>
#include "integration.h"

void forward_euler(double tini, double tend, int nt, int neq, double *uini, double *u, func fcn) 
{ 
  double f[neq]; 
  double dt;
  int it, ieq; 

  dt = (tend-tini) / (nt-1); 
 
  // initialisation 
  for (ieq=0; ieq<neq; ++ieq) u[ieq] = uini[ieq]; 

  //printf("%f ", tini);  
  //for (ieq=0; ieq<neq; ++ieq) printf("%f ", u[ieq]);
  //printf("\n");

  // forward euler interation 
  for (it=0; it<nt-1; ++it) 
  {
    fcn(u, f); 
    for (ieq=0; ieq<neq; ++ieq) u[ieq] = u[ieq] + dt*f[ieq]; 
    //printf("%f ", tini+(it+1)*dt); 
    //for (ieq=0; ieq<neq; ++ieq) printf("%f  ", u[ieq]);
    //printf("\n");
  } 
}

//void fortran_forward_euler(double tini, double tend, int nt, int neq, double *uini, double *u, func fcn)
//{
//
//  forward_euler_integration(&tini, &tend, &nt, &neq, uini, u, fcn);
//
//}

//void radau5_integrator(double tini, double tend, int nt, int neq, double *uini, double *u, func_radau fcn)
void radau5_integrator(double tini, double tend, int nt, int neq, double *uini, double *u, func_radau fcn, func_solout solout)
{
  double t, dt;

  //  required tolerance
  double rtol = 1.e-8;
  double atol=1.0*rtol;
  int itol=0;

  // jacobian is computed internally by finite differences
  int ijac=0;
  // jacobian is a full matrix
  int mljac=neq;
  int mujac;

  // mass matrix (assumed to be the identity matrix)
  int imas=0;
  int mlmas;
  int mumas;

  // output routine is used during integration
  int iout=1;

  int ljac=neq;
  int le=neq;
  int lmas=0;
  int lwork = neq*(ljac+lmas+3*le+12)+20;
  double work[lwork];
  int liwork = 3*neq+20;
  int iwork[liwork];

  double rpar;
  int ipar;
  int idid;

  int i, ieq;

  // init values
  t=tini;
  dt=0.;
  for (ieq=0; ieq<neq; ++ieq) u[ieq] = uini[ieq];

  for(i=0; i<20; i++)
  {
    iwork[i]=0;
    work[i]=0.0;
  }

  // directly calling fortran
  radau5(&neq, fcn, &t, u, &tend, &dt,
         &rtol,&atol,&itol,
         jac,&ijac,&mljac,&mujac,
         mas,&imas,&mlmas,&mumas,
         solout,&iout,
         work,&lwork,iwork,&liwork,
         &rpar,&ipar,&idid);

  // print statistics 
  printf("  Radau5 informations : \n");
  printf("  Tolerance used = %e\n", rtol);
  printf("  Nb. of computed step = %d\n", iwork[15]);
  printf("  Nb. of accepted step = %d\n", iwork[16]);
  printf("  Nb. of rejected step = %d\n", iwork[17]);
}

void jac(int *n, double *x, double *y, double *dfy,
         int *ldfy, double *rpar, double *ipar)
{
}

void mas(int *n,double *am, int *lmas,int *rpar, int *ipar)
{
}

//void solout(int *nr, double *xold, double *x, double *y,
//            double *cont, int *lrc, int *n,
//            double *rpar, int *ipar, int *irtrn)
//{
//  //FILE *fsol;
//  //fsol = fopen("sol_radau5.dat", "a");
//  //printf(fsol, "%e %e %e %e\n", *x, y[0], y[1], y[2]);
//  //fclose(fsol);
//  printf("%e %e %e\n", *x, y[0], y[1]);
//}
