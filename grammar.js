module.exports = grammar({
  name: "treecc",

  externals: ($) => [
    $._blob_start,
    $._blob_text,
    $._blob_end,
  ],

  extras: ($) => [/[ \t\r]+/, $.comment],

  rules: {
    source_file: ($) => seq(repeat($._newline), repeat(seq($._statement, repeat($._newline)))),

    _statement: ($) =>
      choice(
        $.function_definition,
        $.blob_property,
        $.mapping,
        $.function_call,
        $.trim_contents,
        $.contents_interpolation,
      ),

    trim_contents: ($) =>
      seq("trim-contents", choice($.identifier, $.string)),

    contents_interpolation: ($) =>
      seq("contents-interpolation", choice($.identifier, $.string)),

    function_definition: ($) =>
      seq(
        optional("infix"),
        "fun",
        field("name", $.identifier),
        $.parameter_declarations,
        field("return_type", $.identifier),
        $.blob,
      ),

    parameter_declarations: ($) =>
      choice(
        seq("(", ")"),
        seq("(", "void", ")"),
        seq(
          "(",
          $.parameter_declaration,
          repeat(seq(";", $.parameter_declaration)),
          ")",
        ),
      ),

    parameter_declaration: ($) => seq($.identifier, $.identifier),

    blob_property: ($) => seq("contents", optional(":"), $.blob),

    blob: ($) => seq($._blob_start, optional($._blob_text), $._blob_end),

    mapping: ($) => seq($.argument, ":", $.argument),

    function_call: ($) =>
      prec.right(
        seq(
        field("function", $.identifier),
        repeat(
          choice(
            $.argument,
            seq(",", $.argument),
            seq(",", repeat1($._newline), $.argument),
          ),
        ),
        optional(","),
        ),
      ),

    argument: ($) =>
      choice(
        $.special_string,
        $.string,
        $.variable,
        $.number,
        $.tuple,
        $.list,
        $.context,
        $.identifier,
      ),

    tuple: ($) =>
      seq(
        "(",
        optional(seq($.argument, repeat(seq(optional(","), $.argument)))),
        ")",
      ),

    list: ($) =>
      seq(
        "[",
        repeat(
          choice(
            seq(optional(","), $.argument),
            seq(",", repeat1($._newline), $.argument),
          ),
        ),
        optional(","),
        "]",
      ),

    context: ($) =>
      seq(
        "{",
        repeat($._newline),
        repeat(seq($._context_statement, repeat(choice($._newline, ";")))),
        "}",
      ),

    _context_statement: ($) =>
      choice(
        $._statement,
        $._non_identifier_argument,
      ),

    _non_identifier_argument: ($) =>
      choice(
        $.special_string,
        $.string,
        $.variable,
        $.number,
        $.tuple,
        $.list,
        $.context,
      ),

    special_string: ($) => token(/"""[\s\S]*?"""/),

    string: ($) => token(/"([^"\\\n]|\\.)*"/),

    variable: ($) =>
      token(prec(1, /\$\$|\$[^\s()\x5B\x5D{};,"]+/)),

    number: ($) => token(/[+-]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?/),

    identifier: ($) =>
      token(
        prec(
          -1,
          /[^\s()\x5B\x5D{};,:"]+(?:::[^\s()\x5B\x5D{};,:"]+)*/,
        ),
      ),

    comment: ($) => token(seq("//", /[^\n]*/)),

    _newline: ($) => /\n/,
  },
});
