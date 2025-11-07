add_rules("mode.debug", "mode.release")
set_languages("c99")

target("binary_tree")
	set_kind("binary")
	add_files("binary_tree.c")
target_end()

target("quad_tree")
	set_kind("binary")
	add_files("binary_tree.c")
target_end()

target("oc_tree")
	set_kind("binary")
	add_files("oc_tree.c")
target_end()
