#ifndef FPActionInitialization_h
#define FPActionInitialization_h

#include "G4VUserActionInitialization.hh"

namespace FP{
    class ActionInitialization: public G4VUserActionInitialization{
        public:
        ActionInitialization() = default;
        ~ActionInitialization() override = default;
        void Build() const override;
        void BuildForMaster() const override;
    }
}

#endif