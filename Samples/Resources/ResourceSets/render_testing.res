set_name = "render_testing"
mesh = {
	resource_name = "SimpleBox"
	path = "..\..\Resources\Models\Box.obj"
	vertices_usage = "static"
	indices_usage = "static"
}

mesh = {
	resource_name = "Nanosuit"
	path = "..\..\Resources\Models\nanosuit\nanosuit.obj"
	vertices_usage = "static"
	indices_usage = "static"
}

texture = {
	resource_name = "diffuse_tex"
	path = "..\..\Resources\Textures\img_test.png"
}

texture = {
	resource_name = "nano_hand_diffuse_tex"
	path = "..\..\Resources\Textures\nanosuit\hand_dif.png"
}

texture = {
	resource_name = "nano_hand_specular_tex"
	path = "..\..\Resources\Textures\nanosuit\hand_showroom_spec.png"
}

texture = {
	resource_name = "box_diffuse"
	path = "..\..\Resources\Textures\Box texture.png"
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

shader_program = {
	resource_name = "SuitShader"
	shader = {
		type = "vertex"
		path = "..\..\Resources\Shaders\Suit.vertexshader"
	}
	shader = {
		type = "fragment"
		path = "..\..\Resources\Shaders\Suit.fragmentshader"
	}
}

material = {
	resource_name = "Sample_material"
	path = "..\..\Resources\Materials\sample.material"
}

material = {
	resource_name = "Hand_material"
	path = "..\..\Resources\Materials\nanosuit\hand.material"
}