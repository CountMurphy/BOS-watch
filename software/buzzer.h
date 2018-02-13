void PlayTheme();

tpm_general_config_t tmpConfig = {
    .isDBGMode = false,
    .isGlobalTimeBase = false,
    .isTriggerMode = false,
    .isStopCountOnOveflow = false,
    .isCountReloadOnTrig = false,
    .triggerSource = kTpmTrigSel0,
};
tpm_pwm_param_t param = {
    .mode = kTpmEdgeAlignedPWM,
    .edgeMode = kTpmHighTrue,
    .uDutyCyclePercent = 50,
};

