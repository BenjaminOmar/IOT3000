import re
import argparse
from pathlib import Path

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("path")
    parser.add_argument("-n", "--name", default="s")
    parser.add_argument("-o", "--oneline", action="store_true")
    args = parser.parse_args()
    print(build_code_lines(args.path, args.name, args.oneline))

def build_code_lines(filepath: Path, name: str, oneline: bool) -> str:
    with open(filepath) as f:
        lines = f.read().splitlines()
    if oneline:
        value = ''.join(map(_format_value, lines))
        return format_line(name, value, True)
    return "\n".join(
        [format_line(name, _format_value(lines[0]), True)]
        + [
            format_line(name, _format_value(line))
            for line in lines[1:]
        ]
    )

def format_line(name: str, value: str, is_first: bool = False) -> str:
    return f"{name} {'+' if not is_first else ''}= \"{value}\";"

def _format_value(text: str) -> str:
    expressions = [
        (r"[\r\t]", ""),
        (r"^\s{2,}", ""),
        (r"\"", "'")
    ]
    for exp, replace in expressions:
        text = re.sub(exp, replace, text)
    return text

if __name__ == "__main__":
    main()