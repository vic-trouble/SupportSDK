set_name = "ui_testing"
shader_program = {
	resource_name = "TextShader"
	shader = {
		type = "vertex"
		path = "..\..\Resources\Shaders\TextShader.vertexshader"
	}
	shader = {
		type = "fragment"
		path = "..\..\Resources\Shaders\TextShader.fragmentshader"
	}
}

font = {
	resource_name = "Arial_en"
	path = "..\..\Resources\Fonts\arial.ttf"
	codes = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
	pixel_width = 0
	pixel_height = 48
}

font = {
	resource_name = "Arial_ru"
	path = "..\..\Resources\Fonts\arial.ttf"
	codes = L" !\"#$%&'()*+,-./0123456789:;<=>?@АБВГДАЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЬЭЮЯ[\\]^_`абвгдежзийклмнопрстуфхцчшщъыьэюя{|}~"
	pixel_width = 0
	pixel_height = 48
}