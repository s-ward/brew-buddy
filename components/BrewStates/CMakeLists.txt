idf_component_register(SRCS "BrewStates.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "interrupts""Instant_Heat""servo""HeaterPID" "Auto_Run_Setup" "EquipConfig" "ActiveRecipe" "PumpRelay" "HeaterRelay""Auto_Run""HeaterPWM") 
