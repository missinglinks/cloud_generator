extends Control

func _process(delta: float) -> void:
	$ColorRect/VBoxContainer/FPSLabel.text = "FPS: " + str(Performance.get_monitor(Performance.TIME_FPS))
	$ColorRect/VBoxContainer/VerticesLabel.text = "Vertices: " + str(Performance.get_monitor(Performance.RENDER_VERTICES_IN_FRAME))
