register history : History

val init_history : unit -> unit 
function init_history () -> unit = {
    history.instCount = 0;
    history.cycleCount = 0;
    history.stalledCycleCount = 0;

    history.predictedBranch = 0;
    history.unpredictedBranch = 0;

    history.dataHazardCount = 0;
    history.controlHazardCount = 0;
    history.memoryHazardCount = 0; 
}


val printStatistics : unit -> unit 
function printStatistics() = {

  avgCpi = dec_str(tdiv_int(history.cycleCount, history.instCount));
  avgCpi_dot = dec_str(tdiv_int(1000 * (history.cycleCount % history.instCount), history.instCount));

  temp = history.predictedBranch + history.unpredictedBranch;
  bpa = dec_str(tdiv_int(100 * history.predictedBranch, temp));
  bpa_dot = dec_str(tdiv_int(1000 * ((100 * history.predictedBranch) % temp), temp));

  print("------------ STATISTICS -----------");
  print_int("Number of Instructions: ", history.instCount);
  print_int("Number of Cycles: ", history.cycleCount);
  print("Avg Cycles per Instrcution: " ^ avgCpi ^ "." ^ avgCpi_dot);
  print_int("Number of predictedBranch: ", history.predictedBranch);
  print_int("Number of unpredictedBranch: ", history.unpredictedBranch);
  print("Branch Perdiction Accuacy(Strategy: BPB): " ^ bpa ^ "." ^ bpa_dot ^ "%");
  print_int("Number of Control Hazards: ", history.controlHazardCount);
  print_int("Number of Data Hazards: ", history.dataHazardCount);
  print_int("Number of Memory Hazards: ", history.memoryHazardCount);
  print("-----------------------------------");
}



