local function find_rule(context, name)
  for _, child in ipairs(context.children) do
    if child.type == "function_call" and child.name == name then
      local rule_context = child.args[1]
      if not rule_context or rule_context.type ~= "context" then
        error("action rule `" .. name .. "` requires a context")
      end
      return rule_context
    end
  end
  return nil
end

local function execute_rule(context, name, seen)
  if seen[name] then
    error("recursive action rule inclusion: " .. name)
  end
  local rule_context = find_rule(context, name)
  if not rule_context then
    error("unknown action rule `" .. name .. "`")
  end
  seen[name] = true
  for _, command in ipairs(rule_context.children) do
    if command.type == "function_call" then
      if command.name == "include" then
        local included = command.args[1] and command.args[1].value
        if not included then
          error("include requires a rule name")
        end
        execute_rule(context, included, seen)
      else
        local ok = treecc.call_handler(command.name, command)
        if not ok then
          error("action command `" .. command.name .. "` failed")
        end
      end
    end
  end
end

return {
  actions = function(command)
    local context = command.args[1]
    if not context or context.type ~= "context" then
      error("actions requires a context")
    end
    local rule = treecc.rule()
    if not rule or rule == "" then
      rule = "default"
    end
    print("actions: running rule " .. rule)
    execute_rule(context, rule, {})
  end,
}
