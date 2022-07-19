#include "BeamInfo.hh"
#include "globals.hh"
#include "GaramGlobal.hh"

BeamInfo::BeamInfo()
    :x(0.), y(0.), z(0.),
    alpx(0.), betx(0.), emitx(0.),
    alpy(0.), bety(0.), emity(0.),
    slopx(0.), slopy(0.),
    e_mean(0.), ms_ratio(0.), es_ratio(0.)
{}

BeamInfo::BeamInfo(const BeamInfo& bi)
    : x(bi.x),
    xp(bi.xp),
    y(bi.y),
    yp(bi.yp),
    z(bi.z),
    zp(bi.zp),
    alpx(bi.alpx),
    betx(bi.betx),
    emitx(bi.emitx),
    alpy(bi.alpy),
    bety(bi.bety),
    emity(bi.emity),
    e_mean(bi.e_mean),
    ms_ratio(bi.ms_ratio),
    es_ratio(bi.es_ratio),
    slopx(bi.slopx),
    slopy(bi.slopy)
{
}

BeamInfo::BeamInfo(G4double ix, G4double iemitx, G4double iy, G4double iemity)
    : x(ix), emitx(iemitx),
    y(iy), emity(iemity),
    alpx(0.), alpy(0.), slopx(0.), slopy(0.),
    e_mean(0.), ms_ratio(0.), es_ratio(0.)
{
    xp = emitx / x;
    yp = emity / y;
    betx = x * x / emitx;
    bety = y * y / emity;
}

BeamInfo::BeamInfo(G4double ialpx, G4double ibetx, G4double iemitx,
    G4double ialpy, G4double ibety, G4double iemity)
    :alpx(ialpx), betx(ibetx), emitx(iemitx),
    alpy(ialpy), bety(ibety), emity(iemity),
    e_mean(0.), ms_ratio(0.), es_ratio(0.)
{
    x = sqrt(betx * emitx);
    xp = sqrt((alpx * alpx + 1.0) / betx * emitx);
    y = sqrt(bety * emity);
    yp = sqrt((alpy * alpy + 1.0) / bety * emity);
    slopx = -alpx / betx;
    slopy = -alpy / bety;
}

BeamInfo::~BeamInfo() {}

G4double BeamInfo::X() const { return x; }
G4double BeamInfo::Xp() const { return xp; }
G4double BeamInfo::Y() const { return y; }
G4double BeamInfo::Yp() const { return yp; }
G4double BeamInfo::Z() const { return z; }
G4double BeamInfo::Zp() const { return zp; }
G4double BeamInfo::Alpx() const { return alpx; }
G4double BeamInfo::Betx() const { return betx; }
G4double BeamInfo::Emitx() const { return emitx; }
G4double BeamInfo::Alpy() const { return alpy; }
G4double BeamInfo::Bety() const { return bety; }
G4double BeamInfo::Emity() const { return emity; }
G4double BeamInfo::MeanEnergy() const { return e_mean; }
G4double BeamInfo::EsRatio() const { return es_ratio; }
G4double BeamInfo::MsRatio() const { return ms_ratio; }
G4double BeamInfo::Slopx() const { return slopx; }
G4double BeamInfo::Slopy() const { return slopy; }

G4double BeamInfo::SetShift() const { return shift; }



void BeamInfo::SetXXe(G4double ix, G4double iemitx)
{
    x = ix;
    emitx = iemitx;
    if ((x == 0.) || (emitx == 0.))
    {
        make_pencil_beam_x();
    }
    else
    {
        xp = emitx / x;
        betx = x * x / emitx;
    }
}

void BeamInfo::SetYYe(G4double iy, G4double iemity)
{
    y = iy;
    emity = iemity;
    if ((y == 0.) || (emity == 0.))
    {
        make_pencil_beam_y();
    }
    else
    {
        yp = emity / y;
        bety = y * y / emity;
    }
}

void BeamInfo::SetXabe(G4double ialpx, G4double ibetx, G4double iemitx)
{
    alpx = ialpx;
    betx = ibetx;
    emitx = iemitx;
    if ((emitx == 0.) || (betx == 0.)) // pencil beam configuration
    {
        make_pencil_beam_x();
    }
    else
    {
        x = sqrt(betx * emitx);
        xp = sqrt((alpx * alpx + 1.0) / betx * emitx);
        slopx = -alpx / betx;
    }
}

void BeamInfo::SetYabe(G4double ialpy, G4double ibety, G4double iemity)
{
    alpy = ialpy;
    bety = ibety;
    emity = iemity;
    if ((emity == 0.) || (bety == 0.))
    {
        make_pencil_beam_y();
    }
    else
    {
        y = sqrt(bety * emity);
        yp = sqrt((alpy * alpy + 1.0) / bety * emity);
        slopy = -alpy / bety;
    }
}
void BeamInfo::SetEnergy(G4double ie_mean)
{
    e_mean = ie_mean;
    if (es_ratio != 0.)
        es_ratio = garam::dwow(e_mean, ms_ratio);
    else if (ms_ratio != 0.)
        ms_ratio = garam::dpop(e_mean, es_ratio);
}

void BeamInfo::SetEsRatio(G4double ies_ratio)
{
    es_ratio = ies_ratio;
    if (e_mean != 0.)
        ms_ratio = garam::dpop(e_mean, es_ratio);
}

void BeamInfo::SetMsRatio(G4double ims_ratio)
{
    ms_ratio = ims_ratio;
    if (e_mean != 0.)
        es_ratio = garam::dwow(e_mean, ms_ratio);
}

G4bool BeamInfo::is_normal_beam() const
{
    return ((e_mean != 0.) &&
        (betx != 0.) &&
        (emitx != 0.) &&
        (bety != 0.) &&
        (emity != 0.));
}

G4bool BeamInfo::is_pencil_beam() const
{
    return ((e_mean != 0.) &&
        (emitx == 0.) &&
        (emity == 0.));
}

void BeamInfo::make_pencil_beam_x()
{
    x = 0.;
    xp = 0.;
    alpx = 0.;
    betx = 0.;
    emitx = 0.;
    slopx = 0.;
}

void BeamInfo::make_pencil_beam_y()
{
    y = 0.;
    yp = 0.;
    alpy = 0.;
    bety = 0.;
    emity = 0.;
    slopy = 0.;
}

G4String BeamInfo::printout() const
{
    G4String st = "BeamInfo = { ";
    return st 
        + "x(" + garam::to_str(x) + "), "
        + "xp(" + garam::to_str(xp) + "), "
        + "y(" + garam::to_str(y) + "), "
        + "yp(" + garam::to_str(yp) + "), "
        + "alpx(" + garam::to_str(alpx) + "), "
        + "betx(" + garam::to_str(betx) + "), "
        + "emitx(" + garam::to_str(emitx) + "), "
        + "alpy(" + garam::to_str(alpy) + "), "
        + "bety(" + garam::to_str(bety) + "), "
        + "emity(" + garam::to_str(emity) + "), "
        + "e_mean(" + garam::to_str(e_mean) + "), "
        + "ms_ratio(" + garam::to_str(ms_ratio) + "), "
        + "es_ratio(" + garam::to_str(es_ratio) + "), "
        + "slopx(" + garam::to_str(slopx) + "), "
        + "slopy(" + garam::to_str(slopy) + ") } \n";
}

