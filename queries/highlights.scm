(function_definition "fun" @keyword)
(function_definition "infix" @keyword)
(include "include" @keyword)
(identifier) @variable
(function_call function: (identifier) @function)
(parameter_declaration (identifier) @type)
(string) @string
(special_string) @string
(variable) @variable
(number) @number
(comment) @comment
