#!/usr/bin/env python3
"""Convert legacy ARM XML records to JSON sidecars without changing the originals."""

import argparse
import json
import os
from pathlib import Path
import tempfile
import xml.etree.ElementTree as ET


INTEGER_FIELDS = {"CAMERA", "PAIN_SCORE", "ENDRECORD", "major", "minor"}
FLOAT_FIELDS = {
    "POS_X", "POS_Y", "POS_Z", "XAXIS_X", "XAXIS_Y", "XAXIS_Z",
    "YAXIS_X", "YAXIS_Y", "YAXIS_Z", "ZAXIS_X", "ZAXIS_Y", "ZAXIS_Z",
    "SCALE",
}


def convert(element):
    children = list(element)
    if not children:
        value = (element.text or "").strip()
        if element.tag in INTEGER_FIELDS:
            try:
                return int(value)
            except ValueError:
                pass
        if element.tag in FLOAT_FIELDS or element.tag == "ZAXIS_y":
            try:
                return float(value)
            except ValueError:
                pass
        return value

    result = dict(element.attrib)
    for child in children:
        key = "ZAXIS_Y" if child.tag == "ZAXIS_y" else child.tag
        value = convert(child)
        if key not in result:
            result[key] = value
        elif isinstance(result[key], list):
            result[key].append(value)
        else:
            result[key] = [result[key], value]
    return result


def migrate(path):
    destination = path.with_suffix(".json")
    if destination.exists():
        print(f"Skipping existing {destination}")
        return
    root = ET.parse(path).getroot()
    data = {root.tag: convert(root)}
    temporary = None
    try:
        with tempfile.NamedTemporaryFile("w", dir=destination.parent, suffix=".tmp",
                                     delete=False, encoding="utf-8") as output:
            temporary = Path(output.name)
            json.dump(data, output, indent=2, ensure_ascii=False)
            output.write("\n")
        os.replace(temporary, destination)
    finally:
        if temporary is not None and temporary.exists():
            temporary.unlink()
    print(f"Created {destination}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("paths", nargs="+", type=Path,
                        help="Legacy .xml files or directories to convert recursively")
    args = parser.parse_args()
    for source in args.paths:
        files = sorted(source.rglob("*.xml")) if source.is_dir() else [source]
        for path in files:
            if path.suffix.lower() != ".xml":
                parser.error(f"Expected an .xml file: {path}")
            try:
                migrate(path)
            except (OSError, ET.ParseError) as error:
                parser.exit(1, f"Could not convert {path}: {error}\n")


if __name__ == "__main__":
    main()
