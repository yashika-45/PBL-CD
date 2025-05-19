# parse_lexical_output.py

from tokens_map import TOKEN_MAP

def parse_output_file(filepath):
    with open(filepath, 'r') as file:
        lines = file.readlines()

    print(f"{'Lexeme':<20} | {'Token Code':<10} | Token Name")
    print("-" * 55)

    for line in lines:
        # Only process lines with token info, skip symbol/constant table sections
        if ":" not in line or "Symbol table" in line or "Constants Table" in line:
            continue

        # Example line: "\tint                            : 100"
        parts = line.strip().split(":")
        if len(parts) == 2:
            lexeme = parts[0].strip()
            try:
                code = int(parts[1].strip())
                token_name = TOKEN_MAP.get(code, "UNKNOWN")
                print(f"{lexeme:<20} | {code:<10} | {token_name}")
            except ValueError:
                continue

if __name__ == "__main__":
    parse_output_file("output.txt")
