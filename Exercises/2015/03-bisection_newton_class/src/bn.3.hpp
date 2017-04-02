#ifndef BN_HPP
#define BN_HPP 1

#include <algorithm>
#include <ostream>

namespace rootfinding {
    typedef double real;
    typedef real (*fctptr)(real);

    typedef enum checkT {INCREMENT, RESIDUAL};

    struct bisection {
        bisection(real tol, int maxit, checkT check)
            : m_tol(tol), m_maxit(maxit), m_check(check) {}

        real& tol() { return m_tol; }
        int& maxit() { return m_maxit; }

        const real& tol() const { return m_tol; }
        const int& maxit() const{ return m_maxit; }
        const int& nit() const{ return m_nit; }
        
        real apply(real a, real b, fctptr f){
            real u = f(a);
            real l = b - a; // Interval length
            real r;         // Residual
            real c;         // Middle point

            m_nit = 0;
            for(int k = 1; k <= m_maxit; k++) {
                m_nit++;
                l *= 0.5;
                c = a + l;
                real r = f(c);                

                if(converged(fabs(l), fabs(r))) break;

                ((u > 0. && r < 0.) || (u < 0. && r > 0.)) ? (b = c) : (a = c, u = r);
            }

            return c;
        }
    private:
        bool inline converged(real increment, real residual) {
            return (m_check == INCREMENT && increment < m_tol) ||
                (m_check == RESIDUAL && residual < m_tol);
        }

        real m_tol;
        int m_maxit;
        int m_nit;
        checkT m_check;
    };

    struct newton {
        newton(real tol, int maxit) 
            : m_tol(tol), m_maxit(maxit),
              m_check(INCREMENT) {}

        real& tol() { return m_tol; }
        int& maxit() { return m_maxit; }

        const real& tol() const { return m_tol; }
        const int& maxit() const{ return m_maxit; }
        const int& nit() const{ return m_nit; }
        
        real apply(real xp, fctptr f, fctptr df){
            real v = f(xp);
            real xnew;

            m_nit = 0;
            for(int k = 1; k <= m_maxit; k++) {
                m_nit++;
                double derv = df(xp);
                if(!derv) {
                    std::cerr << "ERROR: Division by 0 occourred in newton::apply()" 
                              << std::endl;
                    exit(1);
                }

                xnew = xp - v / derv;
                v = f(xnew);
                if(converged(fabs(xnew - xp), fabs(v))) break;
                xp = xnew;
            }
            return xnew;
        }
    private:
        bool inline converged(real increment, real residual) {
            return (m_check == INCREMENT && increment < m_tol) ||
                (m_check == RESIDUAL && residual < m_tol);
        }

        real m_tol;
        int m_maxit;
        int m_nit;
        real m_check;
    };

    struct robust {
        typedef bisection coarseT;
        typedef newton fineT;

        robust(real tol, int maxit,
               checkT check = INCREMENT,
               real cfratio = 1.E4) :
            m_tol(tol),
            m_maxit(maxit),
            m_check(check),
            m_cfratio(cfratio),
            m_coarse(cfratio * tol, maxit, check),
            m_fine(tol, maxit) {}

        real apply(real a, real b, fctptr f, fctptr df){
            return m_fine.apply( m_coarse.apply(a, b, f), f, df );
        }

        const coarseT& coarse() const { return m_coarse; }
        const fineT& fine() const { return m_fine; }

        friend std::ostream& operator<<(std::ostream&, robust&);
    private:
        real m_tol;
        int m_maxit;
        checkT m_check;
        coarseT m_coarse;
        fineT m_fine;
        real m_cfratio;
    };

    std::ostream& operator<<(std::ostream& ostr, robust& r) {
        ostr << "* Robust Method *" << std::endl;
        ostr << "Tolerance           :" << '\t' << r.m_tol 
             << std::endl;
        ostr << "Max # of iterations :" << '\t' << r.m_maxit
             << std::endl;
        ostr << "Convergence check   :" << '\t'
             << ((r.m_check == INCREMENT) ? "increment" : "residual")
             << std::endl;
        ostr << "# of iterations (C) :" << '\t' << r.coarse().nit() 
                  << std::endl;
        ostr << "# of iterations (F) :" << '\t' << r.fine().nit()
                  << std::endl;
        ostr << "C-to-F tol ratio    :" << '\t' << r.m_cfratio
                  << std::flush;
    }
}
#endif
