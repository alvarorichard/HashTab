add_rules("mode.debug", "mode.release")

-- Defina as ferramentas de compilação que deseja usar (GCC e Clang)
set_toolchains("gcc", "clang")

target("main")
    set_kind("binary")
    add_files("main.c", "hash_table.c")
    add_links("m")  -- Adiciona a biblioteca matemática (-lm)
