#!/usr/bin/env python3

import argparse
from pathlib import Path
from faker import Faker
import random


def parse_args():
    parser = argparse.ArgumentParser(
        description="Generate test documents and dictionary for DDC."
    )
    parser.add_argument(
        "--nfiles", type=int, default=8, help="Number of documents to generate."
    )
    parser.add_argument(
        "--keywords",
        type=str,
        default="lorem ipsum dolor amet elit sed do eiusmod tempor incididunt",
        help="Space-separated list of dictionary keywords.",
    )
    parser.add_argument(
        "--output-dir",
        type=str,
        default="example",
        help="Output root directory (default: ./example).",
    )
    parser.add_argument(
        "--sizes",
        type=int,
        nargs="*",
        default=[512],
        help="List of document sizes in KB (cycled if shorter).",
    )
    return parser.parse_args()


def main():
    args = parse_args()

    faker = Faker()
    dict_words = args.keywords.strip().split()

    output_dir = Path(args.output_dir)
    input_dir = output_dir / "input"
    dict_path = output_dir / "dict.txt"

    input_dir.mkdir(parents=True, exist_ok=True)

    with dict_path.open("w") as f:
        for word in dict_words:
            f.write(f"{word}\n")

    for i in range(args.nfiles):
        size_kb = args.sizes[i % len(args.sizes)]
        doc_path = input_dir / f"doc{i+1}.txt"
        with doc_path.open("w") as f:
            while f.tell() < size_kb * 1024:
                sentence = faker.text(max_nb_chars=200)
                if random.random() < 0.3:
                    sentence += " " + random.choice(dict_words)
                f.write(sentence + "\n")

    print(f"Generated {args.nfiles} files in {input_dir}")
    print(f"Dictionary saved to {dict_path}")


if __name__ == "__main__":
    main()
