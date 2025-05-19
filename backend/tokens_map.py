# tokens_map.py
# Python mapping of token codes defined in tokens.h

TOKEN_MAP = {
    # --- Keywords ---
    100: "INT",
    101: "LONG",
    102: "LONG_LONG",
    103: "SHORT",
    104: "SIGNED",
    105: "UNSIGNED",
    106: "FOR",
    107: "BREAK",
    108: "CONTINUE",
    109: "RETURN",
    110: "CHAR",
    111: "IF",
    112: "ELSE",

    # --- Operators ---
    200: "DECREMENT",
    201: "INCREMENT",
    202: "PTR_SELECT",
    203: "LOGICAL_AND",
    204: "LOGICAL_OR",
    205: "LS_THAN_EQ",
    206: "GR_THAN_EQ",
    207: "EQ",
    208: "NOT_EQ",
    209: "ASSIGN",
    210: "MINUS",
    211: "PLUS",
    212: "STAR",
    213: "MODULO",
    214: "LS_THAN",
    215: "GR_THAN",

    # --- Special Symbols ---
    300: "DELIMITER",
    301: "OPEN_BRACES",
    302: "CLOSE_BRACES",
    303: "COMMA",
    304: "OPEN_PAR",
    305: "CLOSE_PAR",
    306: "OPEN_SQ_BRKT",
    307: "CLOSE_SQ_BRKT",
    308: "FW_SLASH",

    # --- Constants ---
    400: "HEX_CONSTANT",
    401: "DEC_CONSTANT",
    402: "HEADER_FILE",
    403: "STRING",

    # --- Identifier ---
    500: "IDENTIFIER"
}
