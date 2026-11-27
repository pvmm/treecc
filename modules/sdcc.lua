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

local function print_command(command)
  io.stderr:write(command .. "\n")
end

local function is_working_sdcc(path)
  local command = io.popen(shell_quote(path .. "/sdcc") .. " --version >/dev/null 2>&1", "r")
  if not command then
    return false
  end
  local succeeded = command:close()
  return succeeded == true
end

local sdcc_executable
local sdcc_target

local fallback_sdcc_directory = "/usr/libexec/sdcc"

local function bootstrap()
  local configured_path = configured_sdcc_path()
  if configured_path and is_working_sdcc(configured_path) then
    sdcc_executable = configured_path .. "/sdcc"
    return
  end

  local command = io.popen("command -v sdcc 2>/dev/null", "r")
  if not command then
    error("could not check for SDCC in PATH")
  end

  local executable = command:read("*l")
  local succeeded = command:close()
  if executable and executable ~= "" and succeeded then
    sdcc_executable = executable
    return
  end

  if is_working_sdcc(fallback_sdcc_directory) then
    sdcc_executable = fallback_sdcc_directory .. "/sdcc"
    return
  end

  if configured_path then
    error("SDCC configured at '" .. configured_path ..
      "' is not a working compiler, 'sdcc' was not found in PATH, and " ..
      fallback_sdcc_directory .. "/sdcc is unavailable")
  end
  error("SDCC compiler 'sdcc' was not found in PATH and " ..
    fallback_sdcc_directory .. "/sdcc is unavailable")
end

bootstrap()

local functions = {}
local sources = {}
local link_options = {}
local variables = {}
local used_symbols = {}
local used_symbol_languages = {}
local translations = {}
local translations_asm = {}
local has_optional_function = false
local attributes = {
  [".OPTIONAL"] = true,
  [".SIZE"] = true,
}
local current_function

local function interpolate(value, preserve_undefined)
  local result = {}
  local cursor = 1
  while cursor <= #value do
    local dollar = value:find("$", cursor, true)
    if not dollar then
      table.insert(result, value:sub(cursor))
      break
    end
    table.insert(result, value:sub(cursor, dollar - 1))

    if value:sub(dollar + 1, dollar + 1) == "$" then
      table.insert(result, "$$")
      cursor = dollar + 2
    else
      local braced = value:sub(dollar + 1, dollar + 1) == "{"
      local name_start = dollar + (braced and 2 or 1)
      local name_end = name_start
      while name_end <= #value and value:sub(name_end, name_end):match("[%w_:]") do
        name_end = name_end + 1
      end
      local name = value:sub(name_start, name_end - 1)
      local close_brace = braced and value:sub(name_end, name_end) == "}"
      if name ~= "" and (not braced or close_brace) then
        local variable_name = name:gsub(":+$", "")
        local trailing_colons = name:sub(#variable_name + 1)
        local replacement = variables["$" .. variable_name]
        if replacement == nil then
          if not preserve_undefined then
            error("undefined variable `$" .. variable_name .. "`")
          end
          table.insert(result, value:sub(dollar, name_end - 1))
          if close_brace then
            table.insert(result, "}")
          end
        else
          table.insert(result, replacement .. trailing_colons)
        end
        cursor = name_end + (close_brace and 1 or 0)
      else
        table.insert(result, "$")
        cursor = dollar + 1
      end
    end
  end
  return table.concat(result)
end

local function process_context(node, scope_id)
  if node.type == "string" and node.value then
    node.value = interpolate(node.value, true)
  end

  if node.type == "contents_include" then
    for _, child in ipairs(node.children) do
      process_context(child, scope_id)
    end
    return
  end

  if node.type == "function_call" then
    for _, argument in ipairs(node.args) do
      process_context(argument, scope_id)
    end

    if node.name == "variable" then
      local variable = node.args[1]
      local value = node.args[2]
      if not variable or variable.type ~= "variable" or not variable.value or
          not value or not value.value then
        error("variable requires a variable name and a value")
      end
      variables[variable.value] = tostring(value.value)
      if scope_id then
        local local_name = variable.value:gsub("^$", "")
        variables["$" .. scope_id .. "::" .. local_name:gsub("^%$", "")] =
          tostring(value.value)
      end
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
    process_context(child, scope_id)
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
  return value
end

local function read_contents_file(path)
  if not path or path == "" then
    error("contents .INCLUDE requires a file name")
  end

  local resolved = path
  local recipe_path = treecc.path and treecc.path() or ""
  if recipe_path ~= "" then
    local directory = recipe_path:match("^(.*[/\\])")
    if directory then
      resolved = directory .. path
    end
  end

  local file, open_error = io.open(resolved, "r")
  if not file then
    error("could not open contents include `" .. path .. "`: " ..
      tostring(open_error))
  end
  local contents = file:read("*a")
  local closed, close_error = file:close()
  if not contents then
    error("could not read contents include `" .. path .. "`")
  end
  if not closed then
    error("could not close contents include `" .. path .. "`: " ..
      tostring(close_error))
  end
  return contents
end

local function read_contents_value(node, owner)
  if node.type == "blob" then
    return extract_contents(node.text, owner.delimiters)
  end
  if node.type == "contents_include" then
    local path = node.children[1]
    if not path or path.type ~= "string" or not path.value then
      error("contents .INCLUDE requires a string file name")
    end
    return read_contents_file(path.value)
  end
  return nil
end

local function is_c_function(function_object)
  for _, path in ipairs(function_object.append_to) do
    if path:lower():match("%.c$") then
      return true
    end
  end
  return false
end

local function append_asm_source(function_object, path)
  if not path then
    error("sdcc::append-to requires a path")
  end
  table.insert(function_object.append_to, path)
  function_object.language = is_c_function(function_object) and "c" or "asm"
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

local function collect_signature(node, function_object)
  if node.type == "function_call" and node.name == "signature" then
    local signature = node.args[1] and node.args[1].value
    if not signature then
      error("signature requires a string")
    end
    function_object.signature = signature
    print("sdcc::signature: " .. signature)
  end
  for _, child in ipairs(node.children) do
    collect_signature(child, function_object)
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

local function collect_trim_contents(node, owner)
  if node.type == "trim_contents" then
    local option = node.children[1] and node.children[1].value
    if option ~= "left" and option ~= "right" and option ~= "both" then
      error("trim-contents requires left, right, or both")
    end
    owner.trim_contents = option
    print("sdcc::trim-contents: " .. option)
  end

  for _, child in ipairs(node.children) do
    collect_trim_contents(child, owner)
  end
end

local function collect_contents_interpolation(node, owner)
  if node.type == "contents_interpolation" then
    local option = node.children[1] and node.children[1].value
    if option ~= "true" and option ~= "false" then
      error("contents-interpolation requires true or false")
    end
    owner.contents_interpolation = option == "true"
    print("sdcc::contents-interpolation: " .. option)
  end

  for _, child in ipairs(node.children) do
    collect_contents_interpolation(child, owner)
  end
end

local function trim_contents(value, option)
  if option == "left" or option == "both" then
    value = value:gsub("^%s+", "")
  end
  if option == "right" or option == "both" then
    value = value:gsub("%s+$", "")
  end
  return value
end

local function finalize_contents(owner)
  if owner.contents_interpolation then
    owner.contents = interpolate(owner.contents, true)
  end
  if owner.trim_contents then
    owner.contents = trim_contents(owner.contents, owner.trim_contents)
  end
end

local function collect_contents(node, function_object)
  if node.type == "blob_property" then
    local contents
    for _, child in ipairs(node.children) do
      contents = read_contents_value(child, function_object)
      if contents ~= nil then
        break
      end
    end
    if contents == nil then
      error("contents property is missing its blob or .INCLUDE")
    end
    function_object.contents = contents
    function_object.has_contents = true
    finalize_contents(function_object)
    local is_optional = false
    for _, attribute in ipairs(function_object.current_attributes) do
      if attribute == ".OPTIONAL" then
        is_optional = true
        local contents = function_object.contents
        if contents ~= "" and contents:sub(-1) ~= "\n" then
          contents = contents .. "\n"
        end
        local open_guard, close_guard
        if function_object.language == "c" then
          open_guard = "#ifdef _USED_" .. function_object.name .. "_FUNCTION\n"
          close_guard = "#endif\n"
        else
          open_guard = ".ifdef _USED_" .. function_object.name .. "_FUNCTION\n"
          close_guard = ".endif\n"
          contents = ".globl " .. function_object.name .. "\n" .. contents
        end
        function_object.contents = open_guard .. contents .. close_guard
        break
      end
    end
    if not is_optional and function_object.language ~= "c" then
      function_object.contents =
        ".globl " .. function_object.name .. "\n" .. function_object.contents
    end
    local include_lines = function_object.language == "c"
      and "#include \"out/defs.h\"\n"
      or ".include \"out/defs.asm\"\n"
    if is_optional then
      include_lines = include_lines .. (function_object.language == "c"
        and "#include \"out/translations.h\"\n"
        or ".include \"out/translations.asm\"\n")
    end
    function_object.contents = include_lines .. function_object.contents
    print("sdcc::contents: received " .. function_object.contents)
  end

  for _, child in ipairs(node.children) do
    collect_contents(child, function_object)
  end
end

local function collect_source_metadata(node, source_object)
  if node.type == "function_call" then
    if node.name == "filename" then
      source_object.filename = node.args[1] and node.args[1].value
      if not source_object.filename then
        error("filename command requires a string")
      end
      print("source filename: " .. source_object.filename)
    elseif node.name == "include-dirs" or node.name == "library-dirs" then
      local list = node.args[1]
      if not list or list.type ~= "list" then
        error(node.name .. " requires a list")
      end
      local values = {}
      for _, argument in ipairs(list.children) do
        local value = argument.children[1]
        if not value or not value.value then
          error(node.name .. " entries must be values")
        end
        table.insert(values, value.value)
      end
      if node.name == "include-dirs" then
        source_object.include_dirs = values
      else
        source_object.library_dirs = values
      end
      print("sdcc::source " .. node.name .. ": [" ..
        table.concat(values, ", ") .. "]")
    elseif node.name == "delimiters" then
      local list = node.args[1]
      if not list or list.type ~= "list" then
        error("delimiters requires a list")
      end
      local values = {}
      for _, argument in ipairs(list.children) do
        local value = argument.children[1]
        if not value or value.type ~= "string" then
          error("delimiters entries must be strings")
        end
        table.insert(values, value.value)
      end
      source_object.delimiters = values
      print("sdcc::delimiters: received [" ..
        table.concat(source_object.delimiters, ", ") .. "]")
    end
  elseif node.type == "blob_property" then
    local contents
    for _, child in ipairs(node.children) do
      contents = read_contents_value(child, source_object)
      if contents ~= nil then
        break
      end
    end
    if contents == nil then
      error("contents property is missing its blob or .INCLUDE")
    end
    source_object.contents = contents
    source_object.has_contents = true
    finalize_contents(source_object)
    print("sdcc::source contents: received " .. source_object.contents)
  end

  for _, child in ipairs(node.children) do
    collect_source_metadata(child, source_object)
  end
end

local function copy_variables()
  local copied = {}
  for name, value in pairs(variables) do
    copied[name] = value
  end
  return copied
end

local function restore_variables(saved)
  for name in pairs(variables) do
    variables[name] = nil
  end
  for name, value in pairs(saved) do
    variables[name] = value
  end
end

local function write_translations_header()
  local path = "out/translations.h"
  local directories_created, directory_error =
    treecc.create_parent_directories(path)
  if not directories_created then
    error("could not create parent directories for `" .. path .. "`: " ..
      directory_error)
  end
  local output, open_error = io.open(path, "wb")
  if not output then
    error("could not open `" .. path .. "` for writing: " .. open_error)
  end
  local content = "#pragma once\n"
  if #translations > 0 then
    content = content .. table.concat(translations, "\n") .. "\n"
  end
  local written, write_error = output:write(content)
  if not written then
    output:close()
    error("could not write to `" .. path .. "`: " .. write_error)
  end
  local closed, close_error = output:close()
  if not closed then
    error("could not finish writing `" .. path .. "`: " .. close_error)
  end
  print("sdcc::translations: " .. path)
end

local function write_translations_assembly()
  local path = "out/translations.asm"
  local directories_created, directory_error =
    treecc.create_parent_directories(path)
  if not directories_created then
    error("could not create parent directories for `" .. path .. "`: " ..
      directory_error)
  end
  local output, open_error = io.open(path, "wb")
  if not output then
    error("could not open `" .. path .. "` for writing: " .. open_error)
  end
  local content = ".ifndef _TRANSLATIONS_ASM_INCLUDED\n" ..
    ".define _TRANSLATIONS_ASM_INCLUDED /1/\n"
  if #translations_asm > 0 then
    content = content .. table.concat(translations_asm, "\n") .. "\n"
  end
  content = content .. ".endif\n"
  local written, write_error = output:write(content)
  if not written then
    output:close()
    error("could not write to `" .. path .. "`: " .. write_error)
  end
  local closed, close_error = output:close()
  if not closed then
    error("could not finish writing `" .. path .. "`: " .. close_error)
  end
  print("sdcc::translations: " .. path)
end

local function write_source_file(source_object)
  if not source_object.filename then
    error("sdcc::source requires a filename")
  end
  local directories_created, directory_error =
    treecc.create_parent_directories(source_object.filename)
  if not directories_created then
    error("could not create parent directories for `" ..
      source_object.filename .. "`: " .. directory_error)
  end
  local signatures = {}
  for _, function_object in ipairs(functions) do
    if function_object.signature then
      table.insert(signatures, function_object.signature)
    end
  end
  local content = "#include \"out/translations.h\"\n"
  if #signatures > 0 then
    content = content .. table.concat(signatures, "\n") .. "\n"
  end
  content = content .. source_object.contents
  local output, open_error = io.open(source_object.filename, "wb")
  if not output then
    error("could not open `" .. source_object.filename ..
      "` for writing: " .. open_error)
  end
  local written, write_error = output:write(content)
  if not written then
    output:close()
    error("could not write to `" .. source_object.filename ..
      "`: " .. write_error)
  end
  local closed, close_error = output:close()
  if not closed then
    error("could not finish writing `" .. source_object.filename ..
      "`: " .. close_error)
  end
  print("sdcc::source file: created " .. source_object.filename)
end

local function source_basename(filename)
  local base = filename:match("([^/\\]+)$") or filename
  return base:match("^(.*)%.[^.]+$") or base
end

local function compile_source(source_object)
  local out_dir = "out"
  local base = source_basename(source_object.filename)
  local rel_path = out_dir .. "/" .. base .. ".rel"
  local directories_created, directory_error =
    treecc.create_parent_directories(rel_path)
  if not directories_created then
    error("could not create parent directories for `" .. rel_path .. "`: " ..
      directory_error)
  end

  local command = shell_quote(sdcc_executable) .. " -c"
  if sdcc_target then
    command = command .. " -m" .. sdcc_target
  end
  for _, include_dir in ipairs(source_object.include_dirs) do
    command = command .. " -I" .. shell_quote(include_dir)
  end
  command = command .. " -o " .. shell_quote(rel_path) ..
    " " .. shell_quote(source_object.filename)

  print_command(command)
  local handle = io.popen(command .. " 2>&1", "r")
  if not handle then
    error("could not execute SDCC compiler")
  end
  local output = handle:read("*a") or ""
  local ok, reason, status = handle:close()
  if ok ~= true then
    local exit_code = status or reason or "unknown"
    error("SDCC compilation failed for `" .. source_object.filename ..
      "` (exit " .. tostring(exit_code) .. "): " .. output)
  end
  print("sdcc::source compiled: " .. rel_path)
  return out_dir .. "/" .. base .. ".lst"
end

local function write_used_defs(lst_path)
  local content = ""
  local input = io.open(lst_path, "r")
  if input then
    content = input:read("*a") or ""
    input:close()
  end

  for _, function_object in ipairs(functions) do
    local symbol = "_USED_" .. function_object.name
    -- include "_USED_<function>_FUNCTION" for each used function
    local macro = "_USED_" .. function_object.name .. "_FUNCTION"
    if not used_symbols[macro] and content:find(symbol, 1, true) then
      used_symbols[macro] = true
      used_symbol_languages[macro] = function_object.language
      table.insert(used_symbols, macro)
    end
  end

  local function write_defs_file(path, language, header, footer, format_line)
    local symbols = {}
    for _, symbol in ipairs(used_symbols) do
      if used_symbol_languages[symbol] == language then
        table.insert(symbols, symbol)
      end
    end
    local directories_created, directory_error =
      treecc.create_parent_directories(path)
    if not directories_created then
      error("could not create parent directories for `" .. path .. "`: " ..
        directory_error)
    end
    local output, open_error = io.open(path, "wb")
    if not output then
      error("could not open `" .. path .. "` for writing: " .. open_error)
    end
    if header ~= "" then
      local written, write_error = output:write(header)
      if not written then
        output:close()
        error("could not write to `" .. path .. "`: " .. write_error)
      end
    end
    for _, symbol in ipairs(symbols) do
      local written, write_error = output:write(format_line(symbol) .. "\n")
      if not written then
        output:close()
        error("could not write to `" .. path .. "`: " .. write_error)
      end
    end
    if footer ~= "" then
      local written, write_error = output:write(footer)
      if not written then
        output:close()
        error("could not write to `" .. path .. "`: " .. write_error)
      end
    end
    local closed, close_error = output:close()
    if not closed then
      error("could not finish writing `" .. path .. "`: " .. close_error)
    end
    print("sdcc::defs: " .. path .. " [" .. table.concat(symbols, ", ") .. "]")
  end

  write_defs_file("out/defs.h", "c", "#pragma once\n", "", function(symbol)
    return "#define " .. symbol .. " 1"
  end)
  write_defs_file("out/defs.asm", "asm",
    ".ifndef _DEFS_ASM_INCLUDED\n.define _DEFS_ASM_INCLUDED /1/\n",
    ".endif\n",
    function(symbol)
      return ".define " .. symbol .. " /1/"
    end)
end

local function assembler_executable()
  local prefix = sdcc_executable:match("^(.*[/\\])") or ""
  local name = "sdasz80"
  if sdcc_target then
    local target = sdcc_target:lower()
    if target:match("^z80") or target:match("^z180") or
       target:match("^ez80") or target:match("^gb") then
      name = "sdasz80"
    else
      name = "sdas" .. target
    end
  end
  return prefix .. name
end

local function run_external(command, description)
  print_command(command)
  local handle = io.popen(command .. " 2>&1", "r")
  if not handle then
    error("could not execute " .. description)
  end
  local output = handle:read("*a") or ""
  local ok, reason, status = handle:close()
  if ok ~= true then
    local exit_code = status or reason or "unknown"
    error(description .. " failed (exit " .. tostring(exit_code) .. "): " ..
      output)
  end
end

local function link_source(parameter)
  if not parameter or parameter == "" then
    error("sdcc::link requires a source id or .rel file")
  end

  local rel_path
  local base
  if parameter:lower():match("%.rel$") then
    rel_path = parameter:match("[/\\]") and parameter or ("out/" .. parameter)
    base = source_basename(parameter)
  else
    local source_object
    for _, registered in ipairs(sources) do
      if registered.id == parameter then
        source_object = registered
        break
      end
    end
    if not source_object then
      error("sdcc::link: unknown sdcc::source `" .. parameter .. "`")
    end
    base = source_basename(source_object.filename)
    rel_path = "out/" .. base .. ".rel"
  end

  local ihx_path = "out/" .. base .. ".ihx"
  local directories_created, directory_error =
    treecc.create_parent_directories(ihx_path)
  if not directories_created then
    error("could not create parent directories for `" .. ihx_path .. "`: " ..
      directory_error)
  end

  local include_dirs = {}
  local seen = {}
  for _, source_object in ipairs(sources) do
    for _, include_dir in ipairs(source_object.include_dirs) do
      if not seen[include_dir] then
        seen[include_dir] = true
        table.insert(include_dirs, include_dir)
      end
    end
  end

  local command = shell_quote(sdcc_executable)
  if sdcc_target then
    command = command .. " -m" .. sdcc_target
  end
  for _, option in ipairs(link_options) do
    command = command .. " " .. shell_quote(option)
  end
  for _, include_dir in ipairs(include_dirs) do
    command = command .. " -I" .. shell_quote(include_dir)
  end
  command = command .. " -o " .. shell_quote(ihx_path)

  local inputs = { rel_path }
  for _, function_object in ipairs(functions) do
    for _, path in ipairs(function_object.append_to) do
      if path:lower():match("%.c$") then
        local function_base = source_basename(path)
        local function_rel = "out/" .. function_base .. "_c.rel"
        local created, create_error =
          treecc.create_parent_directories(function_rel)
        if not created then
          error("could not create parent directories for `" .. function_rel ..
            "`: " .. create_error)
        end
        local compile_command = shell_quote(sdcc_executable) .. " -c"
        if sdcc_target then
          compile_command = compile_command .. " -m" .. sdcc_target
        end
        for _, include_dir in ipairs(include_dirs) do
          compile_command = compile_command .. " -I" .. shell_quote(include_dir)
        end
        compile_command = compile_command .. " -o " ..
          shell_quote(function_rel) .. " " .. shell_quote(path)
        run_external(compile_command,
          "SDCC compilation of `" .. path .. "`")
        table.insert(inputs, function_rel)
      elseif path:lower():match("%.asm$") then
        local function_base = source_basename(path)
        local function_rel = "out/" .. function_base .. ".rel"
        local created, create_error = treecc.create_parent_directories(function_rel)
        if not created then
          error("could not create parent directories for `" .. function_rel ..
            "`: " .. create_error)
        end
        run_external(shell_quote(assembler_executable()) .. " -o " ..
          shell_quote(function_rel) .. " " .. shell_quote(path),
          "SDCC assembly of `" .. path .. "`")
        table.insert(inputs, function_rel)
      else
        table.insert(inputs, path)
      end
    end
  end
  for _, input in ipairs(inputs) do
    command = command .. " " .. shell_quote(input)
  end

  run_external(command, "SDCC linking of `" .. rel_path .. "`")
  print("sdcc::linked: " .. ihx_path)
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
    local source_object = {
      id = context_id,
      filename = nil,
      include_dirs = {},
      library_dirs = {},
      delimiters = {},
      trim_contents = nil,
      contents_interpolation = true,
      contents = "",
      has_contents = false,
    }
    local saved_variables = copy_variables()
    local succeeded, failure = pcall(function()
      variables["$sdcc::source"] = context_id
      process_context(context, context_id)
      collect_trim_contents(context, source_object)
      collect_contents_interpolation(context, source_object)
      collect_source_metadata(context, source_object)
      write_source_file(source_object)
    end)
    restore_variables(saved_variables)
    if not succeeded then
      error(failure, 0)
    end
    table.insert(sources, source_object)
    print("sdcc::source: " .. context_id)
    print("sdcc::sources: registered [" ..
      table.concat((function()
        local ids = {}
        for _, source in ipairs(sources) do
          table.insert(ids, source.id)
        end
        return ids
      end)(), ", ") .. "]")
  end,

  ["sdcc::function"] = function(command)
    local name = command.args[1] and command.args[1].value
    if not name then
      error("sdcc::function requires an id")
    end
    local function_object = {
      name = name,
      language = "asm",
      append_to = {},
      current_attributes = {},
      delimiters = {},
      trim_contents = nil,
      contents_interpolation = true,
      contents = "",
      has_contents = false,
      signature = nil,
    }
    local context = command.args[2]
    if not context or context.type ~= "context" then
      error("sdcc::function requires a context with contents")
    end
    local variable_name = "$sdcc::function"
    local previous_value = variables[variable_name]
    variables[variable_name] = name
    local succeeded, failure = pcall(function()
      process_context(context)
      collect_signature(context, function_object)
      collect_asm_sources(context, function_object)
      collect_attributes(context, function_object)
      collect_delimiters(context, function_object)
      collect_trim_contents(context, function_object)
      collect_contents_interpolation(context, function_object)
      collect_contents(context, function_object)
      if not function_object.has_contents then
        error("sdcc::function requires contents")
      end
      for _, path in ipairs(function_object.append_to) do
        write_asm_source(function_object, path)
      end
    end)
    variables[variable_name] = previous_value
    if not succeeded then
      error(failure, 0)
    end
    table.insert(functions, function_object)
    table.insert(translations,
      "#define " .. name .. "(...) _USED_" .. name .. "(__VA_ARGS__)")
    for _, attribute in ipairs(function_object.current_attributes) do
      if attribute == ".OPTIONAL" then
        has_optional_function = true
        table.insert(translations_asm,
          ".define " .. name .. " /__USED_" .. name .. "/")
        break
      end
    end
    write_translations_header()
    if has_optional_function then
      write_translations_assembly()
    end
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

  ["sdcc::target"] = function(command)
    local target = command.args[1] and command.args[1].value
    if not target then
      error("sdcc::target requires a target")
    end
    sdcc_target = target
    print("sdcc::target: " .. target)
  end,

  ["sdcc::clean"] = function()
    print("sdcc::clean: recognized")
  end,

  ["sdcc::compile"] = function(command)
    local source_id = command.args[1] and command.args[1].value
    if not source_id then
      error("sdcc::compile requires a source id")
    end
    local source_object
    for _, registered in ipairs(sources) do
      if registered.id == source_id then
        source_object = registered
        break
      end
    end
    if not source_object then
      error("sdcc::compile: unknown sdcc::source `" .. source_id .. "`")
    end
    local lst_path = compile_source(source_object)
    write_used_defs(lst_path)
    print("sdcc::compile: " .. source_id)
  end,

  ["sdcc::ihx2rom"] = function()
    print("sdcc::ihx2rom: recognized")
  end,

  ["sdcc::link_opt"] = function(command)
    local list = command.args[1]
    if not list or list.type ~= "list" then
      error("sdcc::link_opt requires a list")
    end
    local values = {}
    for _, argument in ipairs(list.children) do
      local value = argument.children[1]
      if not value or not value.value then
        error("sdcc::link_opt entries must be values")
      end
      table.insert(values, value.value)
    end
    for _, value in ipairs(values) do
      table.insert(link_options, value)
    end
    print("sdcc::link_opt: [" .. table.concat(values, ", ") .. "]")
  end,

  ["sdcc::link"] = function(command)
    local parameter = command.args[1] and command.args[1].value
    link_source(parameter)
  end,

  ["sdcc::openmsx"] = function()
    print("sdcc::openmsx: recognized")
  end,

  ["sdcc::optional"] = function()
    print("sdcc::optional: recognized")
  end,
}
