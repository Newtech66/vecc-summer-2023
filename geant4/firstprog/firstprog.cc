#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4UIManager.hh"

using namespace FP;

int main(int arg,char* argv[])
{
    auto runmanager = G4RunManagerFactory::CreateRunManager();
    runmanager->SetUserInitialization(new DetectorConstruction);
    runmanager->SetUserInitialization(new PhysicsList);
    runmanager->SetUserInitialization(new ActionInitialization);
    runmanager->Initialize();
    G4UImanager* UImanager = G4UImanager::GetUIPointer();
    /*UImanager->ApplyCommand("/run/verbose 1");
    UImanager->ApplyCommand("/event/verbose 1");
    UImanager->ApplyCommand("/tracking/verbose 1");
    int numOfEvent = 3;
    runmanager->BeamOn(numOfEvent);*/
    if(argc==1) {
        G4UIExecutive* ui = new G4UIExecutive(argc,argv);
        UImanager->ApplyCommand("control/execute/ init.mac");
        ui->SessionStart();
        delete ui;
    }
    else {
        G4String command = "control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    delete runmanager;
    return 0;
}
