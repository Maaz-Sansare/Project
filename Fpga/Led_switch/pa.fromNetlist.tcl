
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

create_project -name Led_switch -dir "E:/everything_git/Projects/Project/Fpga/Led_switch/planAhead_run_3" -part xc3s50atq144-4
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "E:/everything_git/Projects/Project/Fpga/Led_switch/Led_blink.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {E:/everything_git/Projects/Project/Fpga/Led_switch} }
set_property target_constrs_file "Led_blink.ucf" [current_fileset -constrset]
add_files [list {Led_blink.ucf}] -fileset [get_property constrset [current_run]]
link_design
