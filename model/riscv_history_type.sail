struct History = {
    instCount : int,
    cycleCount : int,
    stalledCycleCount : int,

    predictedBranch : int, // Number of branch that is predicted successfully
    unpredictedBranch : int, // Number of branch that is not predicted
                                // successfully

    dataHazardCount : int,
    controlHazardCount : int,
    memoryHazardCount : int,

    //std::vector<std::string> instRecord;
    //std::vector<std::string> regRecord;

    //std::string memoryDump;
  } 
