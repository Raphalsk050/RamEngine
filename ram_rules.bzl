MATC_DEFAULT_TOOL = Label("//tools:matc_tool")

def ram_material(name, src, matc_tool = MATC_DEFAULT_TOOL):
    filamat_file = "compiled_materials/{}.filamat".format(name)
    mat_file = src

    native.genrule(
        name = "simple_material_compile",
        srcs = ["simple_material.mat"],
        outs = ["compiled_materials/simple_material.filamat"],
        cmd = """
               mkdir -p compiled_materials
               echo "Compilando material: $(location simple_material.mat) -> $(location compiled_materials/simple_material.filamat)"
               $(location //tools:matc_tool) -o $(location compiled_materials/simple_material.filamat) $(location simple_material.mat)
               """,
        tools = ["//tools:matc_tool"],  # Supondo que matc esteja definido em //tools
    )

    native.cc_library(
        name = name,
        srcs = [],
        hdrs = [],
        data = [filamat_file],  # Inclui o .filamat como data
        includes = ["compiled_materials"],  # Permite caminhos relativos para includes
        visibility = ["//visibility:public"],
    )
