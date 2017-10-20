#ifndef INTEGRATION_H
#define INTEGRATION_H

typedef void(*func)(double*, double*);
typedef void(*func_radau)(int*, double*, double*, double*, double*, int*);

typedef void(*func_solout)(int *, double*, double*, double*, double*, int*, int*, double*, int*, int*);

void forward_euler(double tini, double tend, int nt, int neq, double *uini, double *usol, func fcn);

//void fortran_forward_euler(double tini, double tend, int nt, int neq, double *uini, double *u, func fcn);

//void forward_euler_integration(double *tini, double *tend, int *nt, int *neq, double *uini, double *u, func fcn);

void radau5_integrator(double tini, double tend, int nt, int neq, double *uini, double *u, func_radau fcn, func_solout solout);

//void radau5(int *n, func_radau fcn, double *x, double *y, double *xend, double *h,
//            double *rtol, double *atol, int *itol,
//            void jac(int*, double*, double*, double*, int*, double*, double*),
//            int *ijac, int *mljac, int *mujac,
//            void mas(int *n,double *am, int *lmas,int *rpar, int *ipar),
//            int *imas, int *mlmas, int *mumas,
//            void solout(int*,double*,double*,double*,double*,int*,int*,double*,int*,int*),
//            int *iout,
//            double *work, int *lwork,int *iwork, int *liwork,
//            double *rpar, int *ipar, int *idid);
void radau5(int *n, func_radau fcn, double *x, double *y, double *xend, double *h,
            double *rtol, double *atol, int *itol,
            void jac(int*, double*, double*, double*, int*, double*, double*),
            int *ijac, int *mljac, int *mujac,
            void mas(int *n,double *am, int *lmas,int *rpar, int *ipar),
            int *imas, int *mlmas, int *mumas,
            func_solout solout,
            int *iout,
            double *work, int *lwork,int *iwork, int *liwork,
            double *rpar, int *ipar, int *idid);

void jac(int *n, double *x, double *y, double *dfy,
         int *ldfy, double *rpar, double *ipar);

void mas(int *n,double *am, int *lmas,int *rpar, int *ipar);


//void solout(int *nr, double *xold, double *x, double *y,
//            double *cont, int *lrc, int *n,
//            double *rpar, int *ipar, int *irtrn);


#endif
