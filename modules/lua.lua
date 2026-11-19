return {
  greet = function(command)
    local name = command.args[1] and command.args[1].value or "(no argument)"
    print("Lua handled " .. command.name .. ": " .. name)
  end,
}
