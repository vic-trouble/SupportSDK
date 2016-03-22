set_name = "render_testing"
mesh = {
	resource_name = "SimpleBox"
	path = "..\..\Resources\Models\Box.obj"
	vertices_usage = "static"
	indices_usage = "static"
}

texture = {
	resource_name = "diffuse_tex"
	path = "..\..\Resources\Textures\img_test.png"
}

texture = {
	resource_name = "box_diffuse"
	path = "..\..\Resources\Textures\box_diffuse.png"
}

shader_program = {
	resource_name = "SimpleShader"
	shader = {
		type = "vertex"
		path = "..\..\Resources\Shaders\Sample.vertexshader"
	}
	shader = {
		type = "fragment"
		path = "..\..\Resources\Shaders\Sample.fragmentshader"
	}
}

material = {
	resource_name = "Sample_material"
	path = "..\..\Resources\Materials\sample.material"
}