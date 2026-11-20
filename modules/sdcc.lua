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

local functions = {}
local variables = {}
local attributes = {
  [".OPTIONAL"] = true,
  [".SIZE"] = true,
}
local current_function

local function interpolate(value)
  return (value:gsub("%$[%w_]+", function(variable)
    local replacement = variables[variable]
    if replacement == nil then
      error("undefined variable `" .. variable .. "`")
    end
    return replacement
  end))
end

local function process_context(node)
  if node.type == "string" and node.value then
    node.value = interpolate(node.value)
  end

  if node.type == "function_call" then
    for _, argument in ipairs(node.args) do
      process_context(argument)
    end

    if node.name == "variable" then
      local variable = node.args[1]
      local value = node.args[2]
      if not variable or variable.type ~= "variable" or not variable.value or
          not value or not value.value then
        error("variable requires a variable name and a value")
      end
      variables[variable.value] = tostring(value.value)
    elseif node.name == "debug" then
      local message = node.args[1]
      if not message or message.type ~= "string" or not message.value then
        error("debug requires a string")
      end
      io.stderr:write("debug: ", message.value, "\n")
    end
    return
  end

  for _, child in ipairs(node.children) do
    process_context(child)
  end
end

local function collect_attributes(node, function_object)
  if node.type == "function_call" and node.name == "attributes" then
    local list = node.args[1]
    if not list or list.type ~= "list" then
      error("attributes requires a list")
    end

    function_object.current_attributes = {}
    for _, argument in ipairs(list.children) do
      local attribute = argument.children[1]
      if not attribute or not attribute.value then
        error("attributes entries must be names")
      end
      if not attributes[attribute.value] then
        error("unsupported SDCC attribute `" .. attribute.value .. "`")
      end
      table.insert(function_object.current_attributes, attribute.value)
    end

    print("sdcc::current_attributes: [" ..
      table.concat(function_object.current_attributes, ", ") .. "]")
  end

  for _, child in ipairs(node.children) do
    collect_attributes(child, function_object)
  end
end

local function extract_contents(blob_text, delimiters)
  if #delimiters == 0 then
    error("contents requires delimiters to be specified first")
  end

  local opening = delimiters[1]
  local closing = delimiters[2] or delimiters[1]
  if opening == "" or closing == "" then
    error("contents delimiters must not be empty")
  end

  local opening_start, opening_end = blob_text:find(opening, 1, true)
  if not opening_start then
    error("contents blob does not contain its opening delimiter")
  end
  local closing_start, closing_end = blob_text:find(closing, opening_end + 1, true)
  if not closing_start then
    error("contents blob does not contain its closing delimiter")
  end

  local value = blob_text:sub(opening_end + 1, closing_start - 1)
  value = value:gsub("^\r?\n", "", 1)
  value = value:gsub("\r?\n$", "", 1)
  return value
end

local function append_asm_source(function_object, path)
  if not path then
    error("sdcc::append-to requires a path")
  end
  table.insert(function_object.append_to, path)
  print("sdcc::append-to: received " .. path)
end

local function collect_asm_sources(node, function_object)
  if node.type == "function_call" and node.name == "append-to" then
    local path = node.args[1] and node.args[1].value
    append_asm_source(function_object, path)
  end
  for _, child in ipairs(node.children) do
    collect_asm_sources(child, function_object)
  end
end

local function collect_delimiters(node, function_object)
  if node.type == "function_call" and node.name == "delimiters" then
    local list = node.args[1]
    if not list or list.type ~= "list" then
      error("delimiters requires a list")
    end

    local received_delimiters = {}
    for _, argument in ipairs(list.children) do
      local value = argument.children[1]
      if not value or value.type ~= "string" then
        error("delimiters entries must be strings")
      end
      table.insert(received_delimiters, value.value)
    end
    function_object.delimiters = received_delimiters
    print("sdcc::delimiters: received [" ..
      table.concat(function_object.delimiters, ", ") .. "]")
  end

  for _, child in ipairs(node.children) do
    collect_delimiters(child, function_object)
  end
end

local function collect_contents(node, function_object)
  if node.type == "blob_property" then
    local blob
    for _, child in ipairs(node.children) do
      if child.type == "blob" then
        blob = child
        break
      end
    end
    if not blob then
      error("contents property is missing its blob")
    end
    function_object.contents =
      extract_contents(blob.text, function_object.delimiters)
    print("sdcc::contents: received " .. function_object.contents)
  end

  for _, child in ipairs(node.children) do
    collect_contents(child, function_object)
  end
end

local function write_asm_source(function_object, path)
  local directories_created, directory_error =
    treecc.create_parent_directories(path)
  if not directories_created then
    error("could not create parent directories for `" .. path .. "`: " ..
      directory_error)
  end

  local output, open_error = io.open(path, "ab")
  if not output then
    error("could not open `" .. path .. "` for appending: " .. open_error)
  end
  local written, write_error = output:write(function_object.contents)
  if not written then
    output:close()
    error("could not write to `" .. path .. "`: " .. write_error)
  end
  local closed, close_error = output:close()
  if not closed then
    error("could not finish writing `" .. path .. "`: " .. close_error)
  end
end

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
    local function_object = {
      name = name,
      append_to = {},
      current_attributes = {},
      delimiters = {},
      contents = "",
    }
    local context = command.args[2]
    if context then
      process_context(context)
      collect_asm_sources(context, function_object)
      collect_attributes(context, function_object)
      collect_delimiters(context, function_object)
      collect_contents(context, function_object)
      for _, path in ipairs(function_object.append_to) do
        write_asm_source(function_object, path)
      end
    end
    table.insert(functions, function_object)
    current_function = function_object
    local names = {}
    for _, registered_function in ipairs(functions) do
      table.insert(names, registered_function.name)
    end
    print("sdcc::function: registered " .. name)
    print("sdcc::functions: registered [" .. table.concat(names, ", ") .. "]")
  end,

  ["sdcc::append-to"] = function(command)
    local path = command.args[1] and command.args[1].value
    if not current_function then
      error("sdcc::append-to must be inside sdcc::function")
    end
    if not path then
      error("sdcc::append-to requires a path")
    end
    append_asm_source(current_function, path)
    write_asm_source(current_function, path)
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
