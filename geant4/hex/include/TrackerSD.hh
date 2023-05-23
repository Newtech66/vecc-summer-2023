#ifndef HEXTRACKERSD
#define HEXTRACKERSD

#include "G4VSensitiveDetector.hh"

#include "TrackerHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

namespace Hex
{

/// Tracker sensitive detector class
///
/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with non zero
/// energy deposit.

class TrackerSD : public G4VSensitiveDetector
{
  public:
    TrackerSD(const G4String& name,
                const G4String& hitsCollectionName);
    ~TrackerSD() override = default;

    // methods from base class
    void   Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

  private:
    TrackerHitsCollection* fHitsCollection = nullptr;
};

}

#endif //HEXTRACKERSD
