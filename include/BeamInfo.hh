#ifndef BEAMINFO_HH_
#define BEAMINFO_HH_

#include "globals.hh"

class BeamInfo {
public:
    BeamInfo();
    BeamInfo(const BeamInfo& bi);
    BeamInfo(G4double ix, G4double iemitx, G4double iy, G4double iemity);
    BeamInfo(G4double ialpx, G4double ibetx, G4double iemitx,
        G4double ialpy, G4double ibety, G4double iemity);
    ~BeamInfo();

    inline BeamInfo& operator = (const BeamInfo& bi);

    G4double X() const;
    G4double Xp() const;
    G4double Y() const;
    G4double Yp() const;
    G4double Z() const;
    G4double Zp() const;
    G4double Alpx() const;
    G4double Betx() const;
    G4double Emitx() const;
    G4double Alpy() const;
    G4double Bety() const;
    G4double Emity() const;
    G4double MeanEnergy() const;
    G4double EsRatio() const;
    G4double MsRatio() const;
    G4double Slopx() const;
    G4double Slopy() const;
    G4double SetShift() const;

    void SetXXe(G4double ix, G4double iemitx);
    void SetYYe(G4double iy, G4double iemity);
    void SetXabe(G4double ialpx, G4double ibetx, G4double iemitx);
    void SetYabe(G4double ialpy, G4double ibety, G4double iemity);
    void SetEnergy(G4double ie_mean);
    void SetEsRatio(G4double ies_ratio);
    void SetMsRatio(G4double ies_ratio);

    G4bool is_normal_beam() const;
    G4bool is_pencil_beam() const;

    G4String printout() const;


private:
    G4double x;  // mm
    G4double xp; // mrad
    G4double y;
    G4double yp;
    G4double z;
    G4double zp;
    G4double alpx;
    G4double betx;
    G4double emitx;
    G4double alpy;
    G4double bety;
    G4double emity;
    G4double e_mean;
    G4double ms_ratio;
    G4double es_ratio;
    G4double slopx;
    G4double slopy;

    G4double shift;

    void make_pencil_beam_x();
    void make_pencil_beam_y();


};


//==============================================================================
inline BeamInfo& BeamInfo::operator = (const BeamInfo& bi)
{
    x = bi.x;
    xp = bi.xp;
    y = bi.y;
    yp = bi.yp;
    z = bi.z;
    zp = bi.zp;
    alpx = bi.alpx;
    betx = bi.betx;
    emitx = bi.emitx;
    alpy = bi.alpy;
    bety = bi.bety;
    emity = bi.emity;
    e_mean = bi.e_mean;
    ms_ratio = bi.ms_ratio;
    es_ratio = bi.es_ratio;
    slopx = bi.slopx;
    slopy = bi.slopy;
    shift = bi.shift;
    return *this;
}

#endif // RMSBEAMINFO_HH_
