extends Node2D

var gameId = -1;

func Start():
	gameId = GDGameWrapper.Create(1, 1, 5*60000, "../log.csv")
	print_debug("Initialized: ", gameId)
	pass
	
func Step():
	GDGameWrapper.Step(gameId)

func _on_start_button_pressed():
	get_node("../StartButton").disabled = true
	get_node("../StepButton").disabled = false
	get_node("../CarbsButton").disabled = false
	get_node("../InsulinButton").disabled = false
	get_node("../BasalHighButton").disabled = false
	get_node("../BasalLowButton").disabled = false
	get_node("../TerminateButton").disabled = false
	Start()
	get_node("../Label").text = "Simulation started"

func _on_step_button_pressed():
	Step()
	get_node("../Label").text = str(GDGameWrapper.Get_IG(gameId))

func _on_carbs_button_pressed():
	GDGameWrapper.ScheduleCarbohydratesIntake(gameId, 30.0, 0.0)

func _on_insulin_button_pressed():
	GDGameWrapper.ScheduleInsulinBolus(gameId, 5.0, 0.0)

func _on_basal_high_button_pressed():
	GDGameWrapper.ScheduleInsulinBasalRate(gameId, 3.0, 0.0)

func _on_basal_low_button_pressed():
	GDGameWrapper.ScheduleInsulinBasalRate(gameId, 0.0, 0.0)

func _on_terminate_button_pressed():
	GDGameWrapper.Terminate(gameId)
	get_node("../StartButton").disabled = false
	get_node("../StepButton").disabled = true
	get_node("../CarbsButton").disabled = true
	get_node("../InsulinButton").disabled = true
	get_node("../BasalHighButton").disabled = true
	get_node("../BasalLowButton").disabled = true
	get_node("../TerminateButton").disabled = true
