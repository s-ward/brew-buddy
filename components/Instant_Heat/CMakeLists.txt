idf_component_register(SRCS "Instant_Heat.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "HeaterRelay""PumpRelay""EquipConfig""HeaterPWM""BrewStates""servo")
