#include "DetectorConstruction.hh"
//#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "QBBC.hh"
#include "G4StepLimiterPhysics.hh"
#include "globals.hh"

class G4VModularPhysicsList;
class G4VisManager;

using namespace Hex;

int main(int argc,char* argv[])
{
    //this creates the run manager
    auto runmanager = G4RunManagerFactory::CreateRunManager();

    //initialise the detector geometry
    runmanager->SetUserInitialization(new DetectorConstruction);

    //initialise the physics list
    //runmanager->SetUserInitialization(new PhysicsList);
    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    physicsList->SetVerboseLevel(0);
    runmanager->SetUserInitialization(physicsList);

    //initialise user actions (primary generation,run,step,track,etc.)
    runmanager->SetUserInitialization(new ActionInitialization);

    //set stuff up
    runmanager->Initialize();
    
    //set up vis manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    //get ui
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    /*UImanager->ApplyCommand("/run/verbose 1");
    UImanager->ApplyCommand("/event/verbose 1");
    UImanager->ApplyCommand("/tracking/verbose 1");
    int numOfEvent = 5;
    runmanager->BeamOn(numOfEvent);*/
    if(argc==1) {
        //if no arguments passed, run init.mac and start interactive session
        G4UIExecutive* ui = new G4UIExecutive(argc,argv);
        UImanager->ApplyCommand("/control/execute init.mac");
        ui->SessionStart();
        delete ui;
    }
    else {
        //otherwise run passed macro
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    delete visManager;
    delete runmanager;
    return 0;
}
