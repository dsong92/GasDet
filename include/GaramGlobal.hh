#ifndef GaramGlobal_h
#define GaramGlobal_h 1

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#ifdef WIN32
#include <io.h>
#endif // WIN32

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"
#include "G4Types.hh"


using namespace std;
using namespace CLHEP;

namespace garam {
    typedef enum _beamDistribution {
        GAUSSIAN,
        WATERBAG4D,
        WATERBAG6D
    } BeamDistribution;

    typedef enum _dirError {
        NAME_ERR,
        PERM_ERR
    } DirError;


    typedef struct _vec3d {
        double x;
        double y;
        double z;
    } vec3d;

    vector<string> &ssplit(const string& s,
        char delim,
        vector<string>& elems);

    vector<string> ssplit(const string &s, char delim);

    int str_to_int(const string s);

    double str_to_double(const string s);

    template <typename t>
    string to_str(const t number)
    {
        stringstream ss;
        ss << number;
        return ss.str();
    }

    double abs_max(const double a, const double b);

    void RootStyleInit();

    string date_time();

    string date_time_sec();

    const char* date_time(char* buff, const int buffsize);

    const char* date_time_sec(char* buff, const int buffsize);

#ifdef WIN32
#ifdef __cplusplus
    extern "C"
    {
#endif
        typedef struct _dirent
        {
            char *d_name;
        } dirent;
        typedef struct _DIR
        {
            long                handle; /* -1 for failed rewind */
            struct _finddata_t  info;
            dirent    result; /* d_name null iff first time */
            char                *name;  /* null-terminated char string */
        } DIR;

        /*���� */
        typedef struct
        {
            short   year;    /* �� */
            char   mon;    /* �� */
            char   day;    /* �� */
        } DC_DATE_T;
        /* �ð� */
        typedef struct
        {
            char   hour;    /* �� */
            char   min;    /* �� */
            char   sec;    /* �� */
        } DC_TIME_T;

        /* ���� ��Ʈ�� ���� */
        typedef struct
        {
            unsigned short  name_size;  /* ���ϸ��� �ݳ��ϴ� ������ ũ�� */
            unsigned short  *Ufname;  /* ���ϸ��� �ݳ��ϴ� ������ ���� �ּ� */
            /* ���⿡�� �Ʒ��� T_MFIL_FILINFO �� ���� */
            unsigned short  attr;   /* �Ӽ� */
            DC_TIME_T   mod_time;  /* ���� �ð� */
            DC_DATE_T   mod_date;  /* ���� ���� */
            unsigned long  file_size;  /* ���� ������ */
            void*    ext_pra;  /* Ȯ�� ����(�Ϻ����� ����) */
        } DC_ETYINFO_T;
        /* ���丮 ��Ʈ�� ���� */
        typedef struct
        {
            int  Uname_size;    /* ��Ī�� ���� ������ */
            unsigned short *Uname;     /* ��Ī */
            long st_mode;    /* �Ӽ� */
        } DC_T_DIRENT;

#define FULL_FILENAME_SIZE  512  // path�� ���Ե� file name size

        //   typedef struct DIR DIR;

        DIR           *opendir(const char *);
        int           closedir(DIR *);
        dirent    *readdir(DIR *);
        void          rewinddir(DIR *);
        /*
          Copyright Kevlin Henney, 1997, 2003. All rights reserved.
          Permission to use, copy, modify, and distribute this software and its
          documentation for any purpose is hereby granted without fee, provided
          that this copyright and permissions notice appear in all copies and
          derivatives.

          This software is supplied "as is" without express or implied warranty.
          But that said, if there are any problems please get in touch.
        */
#ifdef __cplusplus
    }
#endif
#endif // WIN32
    const char* mkdir_p(const char* dirname);

    const double mc2 = 931.494028;
    const double c = 299.792458;
    const double m = 0.0103642688;

    inline double gamma(const double w0)
    {
        // w0: kinetic energy [MeV/u]
        // return : ratio [no unit]
        return (1.0 + abs(w0) / mc2);
    }

    inline double gammaT(const double w0, const double rest_energy)
    {
        // w0          : kinetic energy [MeV]
        // rest_energy : rest energy [MeV]
        // return      : ratio [no unit]
        return (1.0 + abs(w0) / rest_energy);
    }

    inline double beta(const double w0)
    {
        // w0: kinetic energy [MeV/u]
        // return : ratio [no unit]
        const double g = gamma(w0);
        return sqrt(1.0 - 1.0 / (g*g));
    }

    inline double betaT(const double w0, const double rest_energy)
    {
        // w0          : kinetic energy [MeV]
        // rest_energy : rest energy [MeV]
        // return      : ratio [no unit]
        const double g = gammaT(w0, rest_energy);
        return sqrt(1.0 - 1.0 / (g*g));
    }

    inline double beta_gamma(const double w0)
    {
        // w0: kinetic energy [MeV/u]
        // return : ratio [no unit]
        const double g = gamma(w0);
        return sqrt(g*g - 1.0);
    }

    inline double beta_gammaT(const double w0, const double rest_energy)
    {
        // w0          : kinetic energy [MeV]
        // rest_energy : rest energy [MeV]
        // return      : ratio [no unit]
        const double g = gammaT(w0, rest_energy);
        return sqrt(g*g - 1.0);
    }

    inline double brho(const double w0, const double q_per_a)
    {
        // w0: kinetic energy [MeV/u]
        // q_per_a : q/a [no unit], for example 1/3, 1/2, 1 etc...
        // return : magnetic_field * length = [T][E][Q^-1][L^-1], clhep unit system
        return beta_gamma(w0) * m * c / q_per_a;
    }

    inline double brhoT(
        const double w0,
        const double rest_energy,
        const double charge_number)
    {
        // w0            : kinetic energy [MeV]
        // rest_energy   : rest energy [MeV]
        // charge_number : charge number [INTEGER]
        // return : magnetic_field * length = [T][E][Q^-1][L^-1], clhep unit system
        return beta_gammaT(w0, rest_energy) * rest_energy * c / charge_number;
    }

    inline double dpop(const double w0, const double dwow0)
    {
        // w0: kinetic energy [MeV/u]
        // dwow0: kinetic energy ratio |w - w0| / w0 [arb. unit]
        // return : ratio [no unit]
        const double g = gamma(w0);
        return dwow0 * g / (g + 1.0);
    }

    inline double dpopT(
        const double w0,
        const double rest_energy,
        const double dwow0)
    {
        // w0          : kinetic energy [MeV]
        // rest_energy : rest energy [MeV]
        // dwow0       : kinetic energy ratio |w - w0| / w0 [arb. unit]
        // return      : ratio [no unit]
        const double g = gammaT(w0, rest_energy);
        return dwow0 * g / (g + 1.0);
    }

    inline double dwow(const double w0, const double dpop0)
    {
        // w0: kinetic energy [MeV/u]
        // dpop0: kinetic energy ratio |p - p0| / p0 [arb. unit]
        // return : ratio [no unit]
        const double g = gamma(w0);
        return dpop0 * (g + 1.0) / g;
    }

    inline double dwowT(
        const double w0,
        const double rest_energy,
        const double dpop0)
    {
        // w0: kinetic energy [MeV/u]
        // dpop0: kinetic energy ratio |p - p0| / p0 [arb. unit]
        // return : ratio [no unit]
        const double g = gammaT(w0, rest_energy);
        return dpop0 * (g + 1.0) / g;
    }

    double c12energy_by_graphiteT(const double T);

    double graphiteT_by_c12energy(const double W);



    class particle_io
    {
    private:
        int pid;
        //int Q;
        //int Z;
        //int A;
        double x;
        double xp;
        double y;
        double yp;
        double z;
        double zp;
        double w;

    public:
        particle_io() {};
        ~particle_io() {};

        particle_io(
            int particle_id,
            //int charge_number,
            //int atomic_number,
            //int mass_number,
            double xx,
            double xxp,
            double yy,
            double yyp,
            double zz,
            double zzp,
            double kinetic_energy)
        {
            pid = particle_id;
            //Q = charge_number;
            //Z = atomic_number;
            //A = mass_number;

            x = xx;
            xp = xxp;
            y = yy;
            yp = yyp;
            z = zz;
            zp = zzp;
            w = kinetic_energy;
        };

        particle_io(const particle_io& r)
        {
            pid = r.pid;
            //Z = r.Z;
            //Q = r.Q;
            //A = r.A;
            x = r.x;
            xp = r.xp;
            y = r.y;
            yp = r.yp;
            z = r.z;
            zp = r.zp;
            w = r.w;
        };

        const particle_io& operator=(const particle_io& r)
        {
            pid = r.pid;
            //Z = r.Z;
            //Q = r.Q;
            //A = r.A;
            x = r.x;
            xp = r.xp;
            y = r.y;
            yp = r.yp;
            z = r.z;
            zp = r.zp;
            w = r.w;
            return *this;
        };

        void print()
        {
            std::cout << std::setprecision(5);
            if (pid)
            {
                std::cout
                    << "pid=" << pid
                    << "(" << get_name() << ")";
            }
            else
            {
                //std::cout
                    //<< "Q=" << Q
                    //<< ", A=" << A
                    //<< ", Z=" << Z;
            }
            std::cout
                << "x=" << x
                << "mm, xp=" << xp
                << " , y=" << y
                << "mm, yp=" << yp
                << " , z=" << w
                << "mm, zp=" << yp
                << "w=" << w
                << "MeV" << std::endl;
        };


        int get_pid() const { return pid; };
        //int get_Q() const { return Q; };
        //int get_Z() const { return Z; };
        //int get_A() const { return A; };
        double get_x() const { return x; };
        double get_xp() const { return xp; };
        double get_y() const { return y; };
        double get_yp() const { return yp; };
        double get_z() const { return z; };
        double get_zp() const { return zp; };
        double get_w() const { return w; };

        std::string get_name() const
        {
            std::string name;
            if (pid)
            {
                switch (pid)
                {
                case 11:
                    name = "e-";
                    break;
                case 13:
                    name = "muon-";
                    break;
                case 15:
                    name = "tau-";
                    break;
                case 24:
                    name = "W+boson";
                    break;
                case 22:
                    name = "photon";
                    break;
                case 2212:
                    name = "proton";
                    break;
                case 2112:
                    name = "neutron";
                    break;
                default:
                    name = "not found";
                }
            }
            else
            {
                name = "ion";
            }
            return name;
        };

        void set_pid(int ipid) { pid = ipid; };
        //void set_Q(int iq) { Q = iq; };
        //void set_Z(int iz) { Z = iz; };
        //void set_A(int ia) { A = ia; };
        void set_x(double ix) { x = ix; };
        void set_xp(double ixp) { xp = ixp; };
        void set_y(double iy) { y = iy; };
        void set_yp(double iyp) { yp = iyp; };
        void set_z(double iz) { z = iz; };
        void set_zp(double izp) { zp = izp; };
        void set_w(double iw) { w = iw; };
        bool is_ion() const { return !(pid); };

    };
}


#endif
