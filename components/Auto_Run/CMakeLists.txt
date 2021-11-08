idf_component_register(SRCS "Auto_Run.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "interrupts""servo""BrewStates""HeaterRelay""PumpRelay""HeaterPWM")
