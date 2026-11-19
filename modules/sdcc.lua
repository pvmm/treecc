local function trim(value)
  return value:match("^%s*(.-)%s*$")
end

local function configured_sdcc_path()
  local home = os.getenv("HOME")
  if not home or home == "" then
    return nil
  end

  local config = io.open(home .. "/.config/treecc", "r")
  if not config then
    return nil
  end

  local in_sdcc_section = false
  local sdcc_path
  for line in config:lines() do
    line = trim(line)
    if line:match("^%[.-%]$") then
      in_sdcc_section = line == "[sdcc]"
    elseif in_sdcc_section and not line:match("^[#;]") then
      local key, value = line:match("^([%w_]+)%s*=%s*(.-)%s*$")
      if key == "path" then
        value = value:gsub('^"(.*)"$', "%1")
        value = value:gsub("^'(.*)'$", "%1")
        if value ~= "" then
          sdcc_path = value
        end
      end
    end
  end
  config:close()
  return sdcc_path
end

local function shell_quote(value)
  return "'" .. value:gsub("'", "'\"'\"'") .. "'"
end

local function is_working_sdcc(path)
  local command = io.popen(shell_quote(path .. "/sdcc") .. " --version >/dev/null 2>&1", "r")
  if not command then
    return false
  end
  local succeeded = command:close()
  return succeeded == true
end

local function bootstrap()
  local configured_path = configured_sdcc_path()
  if configured_path and is_working_sdcc(configured_path) then
    return
  end

  local command = io.popen("command -v sdcc 2>/dev/null", "r")
  if not command then
    error("could not check for SDCC in PATH")
  end

  local executable = command:read("*l")
  local succeeded = command:close()
  if not executable or executable == "" or not succeeded then
    if configured_path then
      error("SDCC configured at '" .. configured_path ..
        "' is not a working compiler, and 'sdcc' was not found in PATH")
    end
    error("SDCC compiler 'sdcc' was not found in PATH")
  end
end

bootstrap()

return {
  source = function(command)
    local context_id = command.args[1] and command.args[1].value
    local context = command.args[2]
    if not context_id or not context or context.type ~= "context" then
      error("sdcc::source requires an id and context")
    end
    print("sdcc::source: " .. context_id)

    local function process(node)
      if node.type == "function_call" and node.name == "filename" then
        local filename = node.args[1] and node.args[1].value
        if not filename then
          error("filename command requires a string")
        end
        print("source filename: " .. filename)
      end
      for _, child in ipairs(node.children) do
        process(child)
      end
    end

    process(context)
  end,

  ["sdcc::function"] = function(command)
    local name = command.args[1] and command.args[1].value
    if not name then
      error("sdcc::function requires an id")
    end
    print("sdcc::function: " .. name)
  end,

  ["sdcc::append-to"] = function()
    print("sdcc::append-to: recognized")
  end,

  ["sdcc::clean"] = function()
    print("sdcc::clean: recognized")
  end,

  ["sdcc::compile"] = function()
    print("sdcc::compile: recognized")
  end,

  ["sdcc::ihx2rom"] = function()
    print("sdcc::ihx2rom: recognized")
  end,

  ["sdcc::link"] = function()
    print("sdcc::link: recognized")
  end,

  ["sdcc::openmsx"] = function()
    print("sdcc::openmsx: recognized")
  end,

  ["sdcc::optional"] = function()
    print("sdcc::optional: recognized")
  end,
}
