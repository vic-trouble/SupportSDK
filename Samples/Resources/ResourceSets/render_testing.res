set_name = "render_testing"

mesh = {
	resource_name = "SimpleBox"
	path = "..\..\Resources\Models\Box.obj"
	vertices_usage = "static"
	indices_usage = "static"
}

texture = {
	resource_name = "nano_arm_diffuse_tex"
	path = "..\..\Resources\Textures\nanosuit\arm_dif.png"
}

texture = {
	resource_name = "nano_arm_specular_tex"
	path = "..\..\Resources\Textures\nanosuit\arm_showroom_spec.png"
}

texture = {
	resource_name = "nano_body_diffuse_tex"
	path = "..\..\Resources\Textures\nanosuit\body_dif.png"
}

texture = {
	resource_name = "nano_body_specular_tex"
	path = "..\..\Resources\Textures\nanosuit\body_showroom_spec.png"
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
	resource_name = "nano_helmet_diffuse_tex"
	path = "..\..\Resources\Textures\nanosuit\helmet_dif.png"
}

texture = {
	resource_name = "nano_helmet_specular_tex"
	path = "..\..\Resources\Textures\nanosuit\helmet_showroom_spec.png"
}

texture = {
	resource_name = "nano_glass_diffuse_tex"
	path = "..\..\Resources\Textures\nanosuit\glass_dif.png"
}

texture = {
	resource_name = "nano_glass_specular_tex"
	path = "..\..\Resources\Textures\nanosuit\glass_ddn.png"
}

texture = {
	resource_name = "nano_leg_diffuse_tex"
	path = "..\..\Resources\Textures\nanosuit\leg_dif.png"
}

texture = {
	resource_name = "nano_leg_specular_tex"
	path = "..\..\Resources\Textures\nanosuit\leg_showroom_spec.png"
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

material = {
	resource_name = "Sample_material"
	path = "..\..\Resources\Materials\sample.material"
}

material = {
	resource_name = "Arms_material"
	path = "..\..\Resources\Materials\nanosuit\arms.material"
}

material = {
	resource_name = "Body_material"
	path = "..\..\Resources\Materials\nanosuit\body.material"
}

material = {
	resource_name = "Hands_material"
	path = "..\..\Resources\Materials\nanosuit\hands.material"
}

material = {
	resource_name = "Helmet_material"
	path = "..\..\Resources\Materials\nanosuit\helmet.material"
}

material = {
	resource_name = "Legs_material"
	path = "..\..\Resources\Materials\nanosuit\legs.material"
}

material = {
	resource_name = "Visor_material"
	path = "..\..\Resources\Materials\nanosuit\visor.material"
}

model = {
	resource_name = "Nanosuit"
	path = "..\..\Resources\Models\nanosuit\nanosuit.obj"
	description_path = "..\..\Resources\Models\nanosuit\nanosuit.model"
	vertices_usage = "static"
	indices_usage = "static"
}