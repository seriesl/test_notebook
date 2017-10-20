import numpy as np
import ctypes as ct

from bokeh.io import  output_file, show
from bokeh.plotting import figure 

c_integration = ct.CDLL("./integration.so")

def fcn_brusselator(u, f):
    f[0] = 1. + u[0]*u[0]*u[1] - 4.*u[0]
    f[1] = - u[0]*u[0]*u[1] + 3.*u[0]

def fcn_brusselator_radau(n, t, u, f, rpar, ipar):
    fcn_brusselator(u, f)

def solout(nr, told, t, y, cont, lrc, n, rpar, ipar, irtrn):
    #import ipdb; ipdb.set_trace()
    print(t.contents.value, y.contents.value)

def forward_euler_from_c(tini, tend, nt, neq, uini, fcn):

    func_type = ct.CFUNCTYPE(None, ct.POINTER(ct.c_double), ct.POINTER(ct.c_double))
    c_forward_euler = c_integration.forward_euler
    c_forward_euler.argtypes = [ct.c_double, ct.c_double, ct.c_int, ct.c_int,
                                np.ctypeslib.ndpointer(dtype = np.float64), 
                                np.ctypeslib.ndpointer(dtype = np.float64), 
                                func_type]
    c_forward_euler.restype = None

    callable_fcn = func_type(fcn)

    usol = np.zeros(neq)
    c_forward_euler(tini, tend, nt, neq, uini, usol, callable_fcn)

    return usol

def forward_euler_from_fortran(tini, tend, nt, neq, uini, fcn):

    func_type = ct.CFUNCTYPE(None, ct.POINTER(ct.c_double), ct.POINTER(ct.c_double))
    f_forward_euler = c_integration.fortran_forward_euler
    f_forward_euler.argtypes = [ct.c_double, ct.c_double, ct.c_int, ct.c_int,
                                np.ctypeslib.ndpointer(dtype = np.float64), 
                                np.ctypeslib.ndpointer(dtype = np.float64), 
                                func_type]
    f_forward_euler.restype = None

    callable_fcn = func_type(fcn)

    usol = np.zeros(neq)
    f_forward_euler(tini, tend, nt, neq, uini, usol, callable_fcn)

    return usol

def radau5(tini, tend, nt, neq, uini, fcn, solout):

    func_type = ct.CFUNCTYPE(None, ct.POINTER(ct.c_int), ct.POINTER(ct.c_double), ct.POINTER(ct.c_double),
                             ct.POINTER(ct.c_double), ct.POINTER(ct.c_double), ct.POINTER(ct.c_int))
    solout_type = ct.CFUNCTYPE(None, ct.POINTER(ct.c_int), ct.POINTER(ct.c_double), ct.POINTER(ct.c_double),
                               ct.POINTER(ct.c_double), ct.POINTER(ct.c_double), ct.POINTER(ct.c_int),
                               ct.POINTER(ct.c_int), ct.POINTER(ct.c_double), ct.POINTER(ct.c_int), ct.POINTER(ct.c_int))

    f_radau5 = c_integration.radau5_integrator
    f_radau5.argtypes = [ct.c_double, ct.c_double, ct.c_int, ct.c_int,
                                np.ctypeslib.ndpointer(dtype = np.float64), 
                                np.ctypeslib.ndpointer(dtype = np.float64), 
                                func_type]
    f_radau5.restype = None

    callable_fcn = func_type(fcn)
    callable_solout = solout_type(solout)

    usol = np.zeros(neq)
    f_radau5(tini, tend, nt, neq, uini, usol, callable_fcn, callable_solout)

    return usol

def main(): 

    tini = 0.
    tend = 20.
    nt = 1001
    neq = 2
    yini = np.array([1.5, 3.0])

    print("call C function")
    usol = forward_euler_from_c(tini, tend, nt, neq, yini, fcn_brusselator)
    print(usol[0])
    print(usol[1])

    print("call fortran function")
    usol = forward_euler_from_fortran(tini, tend, nt, neq, yini, fcn_brusselator)
    print(usol[0])
    print(usol[1])

    print("call radau5 function")
    usol = radau5(tini, tend, nt, neq, yini, fcn_brusselator_radau, solout)
    print(usol[0])
    print(usol[1])
    #tsol = np.linspace(tini, tend, nt)
    #dt = (tend - tini) / (nt-1)
    #yn = yini
    #ysol = [yini]
    #for i in range(0,nt-1):
    #    ysol.append(forward_euler(tsol[i], tsol[i+1], 2, neq, ysol[i], fcn_brusselator))
    #ysol = np.array(ysol)
    #y1 = ysol[:, 0] 
    #y2 = ysol[:, 1]

    #fig_sol = figure(x_range=(tini, tend), plot_height=300, plot_width=900) 
    #plt_sol_y1 = fig_sol.x(tsol, y1) 
    #plt_sol_y2 = fig_sol.x(tsol, y2, color="Green") 
    #            
    #output_file("brusselator.html", title="Brusselator")  
    #show(fig_sol) 

if __name__ == "__main__":
    main()
