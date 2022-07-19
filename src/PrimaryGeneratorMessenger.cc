#include "PrimaryGeneratorMessenger.hh"
#include "BeamInfo.hh"

#include "G4UIdirectory.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"

#include "Randomize.hh"
#include "GaramGlobal.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* iga)
    :ga(iga)
{
    pts = new std::vector<garam::particle_io>();

    beam = new G4UIdirectory("/beam/");
    beam->SetGuidance("set paramters of beam");

    beam_pdist = new G4UIcmdWithAString("/beam/pdist", this);
    beam_pdist->SetGuidance("particle dist d2 file");

    //beam_shift = new G4UIcmdWithADouble("/beam/pos/shift", this);
    beam_shift = new G4UIcmdWithADoubleAndUnit("/beam/pos/shift", this); // 0711
    beam_shift->SetDefaultUnit("um");
    beam_shift->SetUnitCandidates("um mm cm m");
    beam_shift->SetGuidance("beam_shift");

    //beam_height = new G4UIcmdWithADouble("/beam/pos/up", this);
    beam_height = new G4UIcmdWithADoubleAndUnit("/beam/pos/up", this); // 0711
    beam_height->SetDefaultUnit("mm");
    beam_height->SetUnitCandidates("um mm cm m");
    beam_height->SetGuidance("beam_from_electrode");

    beam_gauss = new G4UIdirectory("/beam/gauss/");
    beam_gauss->SetGuidance("one sigma value of x, xp  y, yp");

    beam_gauss_xe = new G4UIcmdWithAString("/beam/gauss/xe", this);
    beam_gauss_xe->SetGuidance("x_sigma, xp_sigma");
    beam_gauss_xe->AvailableForStates(G4State_PreInit);

    beam_gauss_ye = new G4UIcmdWithAString("/beam/gauss/ye", this);
    beam_gauss_ye->SetGuidance("y_sigma, yp_sigma");
    beam_gauss_ye->AvailableForStates(G4State_PreInit);

    beam_gauss_xabe = new G4UIcmdWith3Vector("/beam/gauss/xabe", this);
    beam_gauss_xabe->SetGuidance("set twiss parameters of X (alpha, beta, rms_emit[pi mm mrad])");
    beam_gauss_xabe->SetParameterName("Alpha", "Beta", "rmsEmittance", true);
    beam_gauss_xabe->SetRange("Beta>0 && rmsEmittance>0");
    beam_gauss_xabe->AvailableForStates(G4State_PreInit);

    beam_gauss_yabe = new G4UIcmdWith3Vector("/beam/gauss/yabe", this);
    beam_gauss_yabe->SetGuidance("set twiss parameters of Y (alpha, beta, rms_emit[pi mm mrad])");
    beam_gauss_yabe->SetParameterName("Alpha", "Beta", "rmsEmittance", true);
    beam_gauss_yabe->SetRange("Beta>0 && rmsEmittance>0");
    beam_gauss_yabe->AvailableForStates(G4State_PreInit);

    beam_energy = new G4UIcmdWithADoubleAndUnit("/beam/energy", this);
    beam_energy->SetGuidance("mean energy [MeV/u]");
    beam_energy->SetParameterName("energy", false);
    beam_energy->SetDefaultUnit("MeV");
    beam_energy->SetUnitCandidates("eV KeV MeV GeV TeV");
    beam_energy->AvailableForStates(G4State_PreInit);

    beam_espread = new G4UIcmdWithADouble("/beam/e_spread", this);
    beam_espread->SetGuidance("Energy Spread as %");
    beam_espread->SetParameterName("espread", false);
    beam_espread->AvailableForStates(G4State_PreInit);

    beam_mspread = new G4UIcmdWithADouble("/beam/m_spread", this);
    beam_mspread->SetGuidance("Momentum spread as %");
    beam_mspread->SetParameterName("mspread", false);
    beam_mspread->AvailableForStates(G4State_PreInit);

    beam_rseed = new G4UIcmdWithAnInteger("/beam/rseed", this);
    beam_rseed->SetGuidance("set random seed");
    beam_rseed->SetParameterName("rseed", false);
    beam_rseed->AvailableForStates(G4State_PreInit);

    pgm = new G4UIdirectory("/pgm/");

    pgm_book = new G4UIcmdWithAnInteger("/pgm/book", this);
    pgm_book->SetParameterName("book", false);
    pgm_book->AvailableForStates(G4State_PreInit);
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
    delete beam;
    delete beam_pdist;
    delete beam_gauss;
    delete beam_shift; // 0711
    delete beam_height; //0711
    delete beam_gauss_xe;
    delete beam_gauss_ye;
    delete beam_gauss_xabe;
    delete beam_gauss_yabe;
    delete beam_energy;
    delete beam_espread;
    delete beam_mspread;
    delete beam_rseed;
    delete pgm;
    delete pgm_book;
    if (pts) delete pts;
}

G4double PrimaryGeneratorMessenger::GetBeamShift() const { return tmp_shift; }
G4double PrimaryGeneratorMessenger::GetBeamHeight() const { return tmp_height; }


std::vector<garam::particle_io>* PrimaryGeneratorMessenger::GetPdist() const
{
    return pts;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* icmd, G4String istr)
{
    if (icmd == beam_pdist)
    {
        //G4String filename = istr;
        double px, py, pz;
        char dummy[1024];
        G4int n_particle = 0;
        garam::particle_io p;

        std::ifstream fi(istr.c_str(), std::ios::in | std::ios::binary);
        try
        {
            fi.read(dummy, 1024);
        }
        catch (int i)
        {
            std::cerr << "d2 reading error : cannot read header!" << std::endl;
        }

        while (fi.read((char*)&p, sizeof(garam::particle_io)))
        {
            n_particle += 1;
            pts->push_back(p);
            //G4cout << p.get_x() << G4endl;
            
        }
        fi.close();
        std::cout << n_particle << " of particle(s) is(are) loaded" << std::endl;
    }
    if (icmd == beam_shift)
    {
        tmp_shift = beam_shift->GetNewDoubleValue(istr);
        //G4cout << "*** test : " << beam_shift->GetNewDoubleValue(istr) << G4endl;
    }
    if (icmd == beam_height)
    {
        tmp_height = beam_height->GetNewDoubleValue(istr);
        //G4cout << "*** test : " << beam_shift->GetNewDoubleValue(istr) << G4endl;
    }
    /*
    if (icmd == beam_gauss_xe)
    {
        std::vector<std::string> xxp = garam::ssplit(istr, ' ');
        binfo.SetXXe(garam::str_to_double(xxp[0]), garam::str_to_double(xxp[1]));
    }
    
    if (icmd == beam_gauss_ye)
    {
        std::vector<std::string> yyp = garam::ssplit(istr, ' ');
        binfo.SetYYe(garam::str_to_double(yyp[0]), garam::str_to_double(yyp[1]));
    }

    if (icmd == beam_gauss_xabe)
    {
        std::vector<std::string> xabe = garam::ssplit(istr, ' ');
        binfo.SetXabe(garam::str_to_double(xabe[0]),
            garam::str_to_double(xabe[1]),
            garam::str_to_double(xabe[2]));
    }
    
    if (icmd == beam_gauss_yabe)
    {
        std::vector<std::string> yabe = garam::ssplit(istr, ' ');
        binfo.SetYabe(garam::str_to_double(yabe[0]),
            garam::str_to_double(yabe[1]),
            garam::str_to_double(yabe[2]));
    }
    */
    if (icmd == beam_energy)
    {
        binfo.SetEnergy(beam_energy->GetNewDoubleValue(istr));
    }
    
    if (icmd == beam_espread)
    {
        binfo.SetEsRatio(beam_espread->GetNewDoubleValue(istr));
    }

    if (icmd == beam_mspread)
    {
        binfo.SetMsRatio(beam_mspread->GetNewDoubleValue(istr));
    }

    if (icmd == beam_rseed)
    {
        CLHEP::HepRandom::setTheSeed(beam_rseed->GetNewIntValue(istr));
    }
    
    if (icmd == pgm_book)
    {
        ga->BookHisto();
    }
}

const BeamInfo& PrimaryGeneratorMessenger::GetBeamInfo()
{
    return binfo;
}

