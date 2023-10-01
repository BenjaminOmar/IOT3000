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
        return f"{name} = {''.join(map(format_line, lines))};"
    return "\n".join(
        [f"{name} = \"{format_line(lines[0])}\";"]
        + [
            f"{name} += \"{format_line(line)}\";"
            for line in lines[1:]
        ]
    )

def format_line(text: str) -> str:
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